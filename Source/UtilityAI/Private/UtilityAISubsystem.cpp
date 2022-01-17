#include "UtilityAISubsystem.h"
#include "AIController.h"
#include "UtilityAI.h"
#include "Components/UtilityAIComponent.h"
#include "UtilityAITask.h"
#include "UtilityAILog.h"

DECLARE_CYCLE_STAT(TEXT("Evaluation Total [GT]"), STAT_AgentEvaluationTotal, STATGROUP_UtilityAI);
DECLARE_CYCLE_STAT(TEXT("Task Update Total [GT]"), STAT_AgentTaskUpdateTotal, STATGROUP_UtilityAI);

void
UUtilityAISubsystem::Tick(float DeltaTime)
{
    CurrentTime = GetWorld()->GetTimeSeconds();

    {
        SCOPE_CYCLE_COUNTER(STAT_AgentEvaluationTotal);

        FScopeLock Lock(&ScheduledAgentsSync);

        UpdatedAgents.Reset(ScheduledAgents.Num());

        ParallelFor(ScheduledAgents.Num(), [=](const int32 Index) {
            UpdateAgent_Concurrent(ScheduledAgents[Index]);
        });

        ScheduledAgents.Reset();
    }

    {
        SCOPE_CYCLE_COUNTER(STAT_AgentTaskUpdateTotal);

        for (const auto& UpdateData : UpdatedAgents) {
            auto* Brain = UpdateData.Brain;
            auto* Controller = CastChecked<AAIController>(Brain->GetOwner());

            FAITaskExecutionContext ExecutionContext;
            ExecutionContext.ActivationTime = CurrentTime;
            ExecutionContext.Target = UpdateData.DecisionResult.Target;
            ExecutionContext.Score = UpdateData.DecisionResult.Score;

            Brain->ExecuteTask(ExecutionContext, UpdateData.DecisionResult.TaskInstance);
        }
    }
}

TStatId
UUtilityAISubsystem::GetStatId() const
{
    RETURN_QUICK_DECLARE_CYCLE_STAT(UUtilityAISubsystem, STATGROUP_Tickables);
}

bool
UUtilityAISubsystem::DoesSupportWorldType(EWorldType::Type WorldType) const
{
    return WorldType == EWorldType::Game || WorldType == EWorldType::PIE;
}

void
UUtilityAISubsystem::UpdateAgent_Concurrent(UUtilityAIComponent* Brain)
{
    const bool bShouldUpdate = IsValid(Brain) && Brain->IsUpdateRequired();
    if (!bShouldUpdate) {
        return;
    }

    Brain->SetUpdateRequired(false);

    auto* TargetSource = Brain->GetTargetSource();
    if (!TargetSource) {
        UE_LOG(LogUAI, Warning, TEXT("%s Evaluation attempted without a valid target source: %s"),
               TEXT(__FUNCTION__), *GetNameSafe(Brain->GetAIOwner()));
        return;
    }

    auto* Controller = Brain->GetAIOwner();
    const auto* AI = Brain->GetAI();

    if (!AI) {
        UE_LOG(LogUAI, Warning,
               TEXT("%s Attempted to register component without a valid `UtilityAI` instance: %s"),
               TEXT(__FUNCTION__), *GetNameSafe(Controller));
        return;
    }

    TargetSource->UpdateTargetList();

    FAIEvaluationContext Context;
    Context.Controller = Controller;
    Context.Brain = Brain;
    Context.Perception = Controller->GetAIPerceptionComponent();
    Context.Pawn = Controller->GetPawn();
    Context.Blackboard = Brain->GetBlackboardComponent();
    Context.CurrentTime = CurrentTime;

    const auto& TargetList = TargetSource->GetTargetList();
    const auto Result = AI->Evaluate(Context, TargetList);

    const auto* NewTask = Result.TaskInstance;
    if (!NewTask) {
        return;
    }

    auto* ActiveTask = Brain->GetActiveTask();
    const bool bHasActiveTask = ActiveTask != nullptr;
    const bool bIsHigherScore = !bHasActiveTask || ActiveTask->GetActivationScore() < Result.Score;

    if (!bHasActiveTask || bIsHigherScore && ActiveTask->CanBeInterruptedBy(NewTask, Result.Target)) {
        FAIUpdateData* UpdateData;

        {
            FScopeLock UpdateLock(&UpdatedAgentsSync);

            // There will be no reallocation, since we have allocated the maximum possible number of updates.
            UpdateData = new(UpdatedAgents) FAIUpdateData();
        }

        UpdateData->Brain = Brain;
        UpdateData->DecisionResult = Result;
    }
}
