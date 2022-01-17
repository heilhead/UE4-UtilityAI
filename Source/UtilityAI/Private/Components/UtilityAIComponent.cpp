#include "Components/UtilityAIComponent.h"
#include "AIController.h"
#include "UtilityAI.h"
#include "UtilityAISubsystem.h"
#include "UtilityAITask.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "UtilityAITargetSource.h"
#include "Perception/AIPerceptionComponent.h"

UUtilityAIComponent::UUtilityAIComponent()
{
    PrimaryComponentTick.bCanEverTick = true;
    PrimaryComponentTick.bStartWithTickEnabled = false;
}

void
UUtilityAIComponent::BeginPlay()
{
    Super::BeginPlay();

    UAISystem = GetWorld()->GetSubsystem<UUtilityAISubsystem>();
    check(UAISystem.Get());

    if (AI) {
        auto* Controller = Cast<AAIController>(GetOwner());
        check(Controller);

        const auto& Decisions = AI->DecisionMaker->Decisions;
        TaskInstances.Empty(Decisions.Num());

        for (auto* Decision : Decisions) {
            auto* Instance = NewObject<UUtilityAITask>(this, Decision->TaskClass);
            Instance->InitTask(Controller, Decision);
            Instance->OnFinished.AddDynamic(this, &ThisClass::TaskFinishedDelegate);
            TaskInstances.Add(Instance);
        }
    }
}

void
UUtilityAIComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    StopLogicInternal();
    Super::EndPlay(EndPlayReason);
}

void
UUtilityAIComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                   FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    if (bRunning && !bPaused) {
        ScheduleUpdate();
    }
}

void
UUtilityAIComponent::StartLogic()
{
    StartLogicInternal();
}

void
UUtilityAIComponent::RestartLogic()
{
    StopLogicInternal();
    StartLogicInternal();
}

void
UUtilityAIComponent::StopLogic(const FString& Reason)
{
    StopLogicInternal();
}

void
UUtilityAIComponent::PauseLogic(const FString& Reason)
{
    bPaused = true;
    DisableUpdates();
}

EAILogicResuming::Type
UUtilityAIComponent::ResumeLogic(const FString& Reason)
{
    bPaused = false;
    const auto Result = Super::ResumeLogic(Reason);
    if (Result == EAILogicResuming::Continue) {
        EnableUpdates();
    }

    return Result;
}

bool
UUtilityAIComponent::IsRunning() const
{
    return bRunning;
}

bool
UUtilityAIComponent::IsPaused() const
{
    return bPaused;
}

void
UUtilityAIComponent::Cleanup()
{
}

void
UUtilityAIComponent::HandleMessage(const FAIMessage& Message)
{
    Super::HandleMessage(Message);
}

void
UUtilityAIComponent::SetTargetSource(TScriptInterface<IUtilityAITargetSource> Source)
{
    TargetSource = Source.GetObject();
}

void
UUtilityAIComponent::ExecuteTask(const FAITaskExecutionContext& Context, UUtilityAITask* NewTaskInstance)
{
    if (ActiveTask) {
        EndActiveTask();
    }

    SavedTickInterval = GetComponentTickInterval();

    const float NewTickInterval = NewTaskInstance->GetAIUpdateInterval();
    if (NewTickInterval >= 0.f) {
        SetComponentTickInterval(NewTickInterval);
    }

    const bool bShouldTick = NewTaskInstance->IsInterruptible();
    if (!bShouldTick) {
        PauseLogic("Active task requires logic lock");
    }

    ActiveTask = NewTaskInstance;
    ActiveTask->ActivateTask(Context);
    OnTaskActivated.Broadcast(ActiveTask);
}

FORCEINLINE void
UUtilityAIComponent::EndActiveTask()
{
    check(ActiveTask);
    ActiveTask->ExternalEndTask();
    ResetTask();
}

FORCEINLINE void
UUtilityAIComponent::StartLogicInternal()
{
    if (bRunning) {
        return;
    }

    bRunning = true;

    if (auto* Src = GetTargetSource()) {
        Src->StartTargetProcessing();
    }

    EnableUpdates();
}

FORCEINLINE void
UUtilityAIComponent::StopLogicInternal()
{
    if (!bRunning) {
        return;
    }

    bRunning = false;

    if (auto* Src = GetTargetSource()) {
        Src->StopTargetProcessing();
    }

    if (ActiveTask) {
        EndActiveTask();
    }

    DisableUpdates();
}

FORCEINLINE void
UUtilityAIComponent::EnableUpdates()
{
    SetComponentTickEnabled(true);
}

FORCEINLINE void
UUtilityAIComponent::DisableUpdates()
{
    SetComponentTickEnabled(false);
    SetUpdateRequired(false);
}

FORCEINLINE void
UUtilityAIComponent::ResetTask()
{
    ActiveTask = nullptr;

    // Restore saved tick interval.
    SetComponentTickInterval(SavedTickInterval);

    if (IsPaused()) {
        ResumeLogic("Active task ended");
    }
}

FORCEINLINE void
UUtilityAIComponent::ScheduleUpdate()
{
    if (!IsUpdateRequired()) {
        UAISystem->ScheduleAgent(this);
        SetUpdateRequired(true);
    }
}

void
UUtilityAIComponent::TaskFinishedDelegate(UUtilityAITask* Task, EAITaskTerminationReason Reason)
{
    check(Task == ActiveTask);
    ResetTask();
    ScheduleUpdate();
    OnTaskFinished.Broadcast(Task, Reason);
}
