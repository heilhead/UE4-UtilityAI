#pragma once

#include "BrainComponent.h"
#include "UtilityAITargetSource.h"
#include "UtilityAITask.h"
#include "UtilityAIComponent.generated.h"

class IUtilityAITargetSource;
class UUtilityAISubsystem;
class UUtilityAI;
struct FAITaskExecutionContext;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class UTILITYAI_API UUtilityAIComponent : public UBrainComponent {
    GENERATED_BODY()

public:
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAITaskActivatedSignature, UUtilityAITask*, Task);

    DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FAITaskFinishedSignature, UUtilityAITask*, Task,
                                                 EAITaskTerminationReason, Reason);

    UUtilityAIComponent();

public:
    UPROPERTY(Transient, BlueprintReadOnly)
    FAITaskActivatedSignature OnTaskActivated;

    UPROPERTY(Transient, BlueprintReadOnly)
    FAITaskFinishedSignature OnTaskFinished;

protected:
    UPROPERTY(EditDefaultsOnly, Category = "AI")
    UUtilityAI* AI;

    UPROPERTY(Transient, BlueprintReadOnly, Category = "AI")
    UUtilityAITask* ActiveTask;

    UPROPERTY(Transient)
    TWeakObjectPtr<UUtilityAISubsystem> UAISystem;

private:
    UPROPERTY(Transient)
    TArray<UUtilityAITask*> TaskInstances;

    UPROPERTY(Transient)
    UObject* TargetSource;

    float SavedTickInterval{0.f};
    bool bRunning{false};
    bool bPaused{false};
    bool bUpdateRequired{false};

public:
    virtual void
    BeginPlay() override;

    virtual void
    EndPlay(const EEndPlayReason::Type EndPlayReason) override;

    virtual void
    TickComponent(float DeltaTime, ELevelTick TickType,
                  FActorComponentTickFunction* ThisTickFunction) override;

    virtual void
    StartLogic() override;

    virtual void
    RestartLogic() override;

    virtual void
    StopLogic(const FString& Reason) override;

    virtual void
    PauseLogic(const FString& Reason) override;

    virtual EAILogicResuming::Type
    ResumeLogic(const FString& Reason) override;

    virtual bool
    IsRunning() const override;

    virtual bool
    IsPaused() const override;

    virtual void
    Cleanup() override;

    virtual void
    HandleMessage(const FAIMessage& Message) override;

    FORCEINLINE UUtilityAITask*
    GetTaskInstance(const int32 Index)
    {
        return TaskInstances[Index];
    }

    void
    ExecuteTask(const FAITaskExecutionContext& Context, UUtilityAITask* NewTaskInstance);

    void
    EndActiveTask();

    FORCEINLINE UUtilityAI*
    GetAI() const
    {
        return AI;
    }

    FORCEINLINE UUtilityAITask*
    GetActiveTask() const
    {
        return ActiveTask;
    }

    UFUNCTION(BlueprintCallable)
    void
    SetTargetSource(TScriptInterface<IUtilityAITargetSource> Source);

    FORCEINLINE IUtilityAITargetSource*
    GetTargetSource() const
    {
        return CastChecked<IUtilityAITargetSource>(TargetSource);
    }

    FORCEINLINE bool
    IsUpdateRequired() const
    {
        return bUpdateRequired;
    }

    FORCEINLINE void
    SetUpdateRequired(bool bInUpdateRequired)
    {
        bUpdateRequired = bInUpdateRequired;
    }

    FORCEINLINE const FAITarget&
    GetTaskTarget() const
    {
        return ActiveTask->GetTarget();
    }

private:
    void
    StartLogicInternal();

    void
    StopLogicInternal();

    void
    EnableUpdates();

    void
    DisableUpdates();

    void
    ResetTask();

    void
    ScheduleUpdate();

    UFUNCTION()
    void
    TaskFinishedDelegate(UUtilityAITask* Task, EAITaskTerminationReason Reason);
};
