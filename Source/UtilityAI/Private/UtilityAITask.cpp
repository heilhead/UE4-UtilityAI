#include "UtilityAITask.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "AIController.h"
#include "UtilityAIDecisionMaker.h"
#include "UtilityAIStatics.h"
#include "Components/UtilityAIComponent.h"
#include "Tasks/AITask.h"

UUtilityAITask::UUtilityAITask()
{
    TickDelegate = FTickerDelegate::CreateUObject(this, &ThisClass::TickInternal);
}

void
UUtilityAITask::InitTask(AAIController* InController, UUtilityAIDecision* InDecision)
{
    Controller = InController;
    World = InController->GetWorld();
    Decision = InDecision;
    Brain = Cast<UUtilityAIComponent>(InController->GetBrainComponent());
    Blackboard = Brain->GetBlackboardComponent();
    Perception = InController->GetAIPerceptionComponent();
}

AAIController*
UUtilityAITask::GetController() const
{
    return Controller;
}

UUtilityAIComponent*
UUtilityAITask::GetBrainComponent() const
{
    return Brain;
}

UAIPerceptionComponent*
UUtilityAITask::GetPerceptionComponent() const
{
    return Perception;
}

UBlackboardComponent*
UUtilityAITask::GetBlackboardComponent() const
{
    return Blackboard;
}

APawn*
UUtilityAITask::GetPawn() const
{
    return Controller->GetPawn();
}

void
UUtilityAITask::SetTickEnabled(bool bEnabled)
{
    if (TickDelegateHandle.IsValid() == bEnabled) {
        return;
    }

    if (bEnabled) {
        TickDelegateHandle = FTicker::GetCoreTicker().AddTicker(TickDelegate);
    } else {
        FTicker::GetCoreTicker().RemoveTicker(TickDelegateHandle);
        TickDelegateHandle.Reset();
    }
}

void
UUtilityAITask::BP_EndTask()
{
    EndTask(EAITaskTerminationReason::Finished);
}

void
UUtilityAITask::ActivateTask(const FAITaskExecutionContext& Context)
{
    check(!bIsActive);
    bIsActive = true;
    ExecutionContext = Context;
    BP_OnActivateTask(ExecutionContext);
}

void
UUtilityAITask::EndTask(const EAITaskTerminationReason Reason)
{
    check(bIsActive);
    bIsActive = false;
    BP_OnEndTask(Reason);
    CleanupTasks();
    OnFinished.Broadcast(this, Reason);

    // Let the old target be garbage collected.
    ExecutionContext.Target = FAITarget();
}

bool
UUtilityAITask::HasEnded() const
{
    return !bIsActive;
}

bool
UUtilityAITask::IsInterruptible() const
{
    return InterruptionPolicy != EAITaskInterruptionPolicy::Never;
}

bool
UUtilityAITask::CanBeInterruptedBy(const UUtilityAITask* NewTask, const FAITarget& NewTarget) const
{
    if (!NewTask) {
        return false;
    }

    switch (InterruptionPolicy) {
    case EAITaskInterruptionPolicy::Never:
        return false;

    case EAITaskInterruptionPolicy::Always:
        // Allow the interruption as long as it's either a different task, or a different target. Otherwise it makes no sense.
        return GetClass() != NewTask->GetClass() || ExecutionContext.Target != NewTarget;

    case EAITaskInterruptionPolicy::HigherPriority: {
        if (GetClass() == NewTask->GetClass()) {
            return false;
        }

        const float CurrentPriority = UUtilityAIStatics::GetAIDataProviderFloatValue_AnyThread(
            Controller, Decision->ScoringFactor);
        const float NewPriority = UUtilityAIStatics::GetAIDataProviderFloatValue_AnyThread(
            Controller, NewTask->Decision->ScoringFactor);

        return NewPriority > CurrentPriority;
    }

    default:
        checkNoEntry();
        return false;
    }
}

const FAITaskExecutionContext&
UUtilityAITask::GetExecutionContext() const
{
    return ExecutionContext;
}

const FAITarget&
UUtilityAITask::GetTarget() const
{
    return ExecutionContext.Target;
}

bool
UUtilityAITask::IsOnCooldown() const
{
    return World->GetTimeSeconds() < NextActivationTime;
}

void
UUtilityAITask::BeginCooldown()
{
    NextActivationTime = World->GetTimeSeconds() + Cooldown;
}

void
UUtilityAITask::ResetCooldown()
{
    NextActivationTime = World->GetTimeSeconds();
}

float
UUtilityAITask::GetAIUpdateInterval() const
{
    return AIUpdateInterval;
}

void
UUtilityAITask::RegisterTaskForCleanup(UAITask* Task)
{
    SpawnedTasks.Add(Task);
}

UWorld*
UUtilityAITask::GetWorld() const
{
    return World;
}

bool
UUtilityAITask::TickInternal(const float DeltaTime)
{
    Tick(DeltaTime);
    return true;
}

void
UUtilityAITask::Tick(const float DeltaTime)
{
    BP_OnTick(DeltaTime);
}

const AActor*
UUtilityAITask::GetTargetActor() const
{
    return ExecutionContext.Target.GetActor();
}

FVector
UUtilityAITask::GetTargetLocation() const
{
    return ExecutionContext.Target.GetLocation();
}

FVector
UUtilityAITask::GetTargetActorLocation() const
{
    if (auto* Actor = ExecutionContext.Target.GetActor()) {
        return Actor->GetActorLocation();
    } else {
        return FVector(FLT_MAX);
    }
}

FORCEINLINE void
UUtilityAITask::CleanupTasks()
{
    for (auto& TaskPtr : SpawnedTasks) {
        auto* Task = TaskPtr.Get();
        if (IsValid(Task) && Task->IsActive()) {
            Task->ExternalCancel();
        }
    }

    SpawnedTasks.Reset();
}
