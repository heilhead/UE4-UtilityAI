#pragma once

#include "GameplayEffectTypes.h"
#include "UtilityAITypes.h"
#include "UtilityAITask.generated.h"

class UUtilityAIDecision;
class UAIPerceptionComponent;
class UAITargetBase;
class UBlackboardComponent;
class UGameplayEffect;
class UAITask;
class UUtilityAIComponent;
class AAIController;

UENUM(BlueprintType)
enum class EAITaskTerminationReason : uint8 { Finished, Interrupted };

UENUM(BlueprintType)
enum class EAITaskInterruptionPolicy : uint8 {
    /* The task will not be interrupted. Effectively locks the brain component until the task is finished. */
    Never,

    /* Any tasks that score higher can interrupt the execution of this task. */
    Always,

    /* Only tasks with higher scoring factor can interrupt the execution of this task. */
    HigherPriority,
};

USTRUCT(BlueprintType)
struct UTILITYAI_API FAITaskExecutionContext {
    GENERATED_BODY()

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    FAITarget Target;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    float ActivationTime{0.f};

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    float Score{0.f};
};

UCLASS(Abstract, Blueprintable)
class UTILITYAI_API UUtilityAITask : public UObject {
    GENERATED_BODY()

public:
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FAITaskFinishedSignature, UUtilityAITask*, Task,
                                                 EAITaskTerminationReason, Reason);

    UUtilityAITask();

public:
    UPROPERTY(Transient, BlueprintReadOnly)
    FAITaskFinishedSignature OnFinished;

protected:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Task")
    float Cooldown{0.f};

    /* Sets AI update interval during this task. Equivalent to tick interval. -1.0 means no change. */
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AI")
    float AIUpdateInterval{-1.f};

    /* Specifies which tasks can interrupt the execution of this task. */
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AI")
    EAITaskInterruptionPolicy InterruptionPolicy{EAITaskInterruptionPolicy::Never};

private:
    FTickerDelegate TickDelegate;
    FDelegateHandle TickDelegateHandle;

    UPROPERTY(Transient)
    UWorld* World;

    UPROPERTY(Transient)
    UUtilityAIDecision* Decision;

    UPROPERTY(Transient)
    AAIController* Controller;

    UPROPERTY(Transient)
    UUtilityAIComponent* Brain;

    UPROPERTY(Transient)
    UAIPerceptionComponent* Perception;

    UPROPERTY(Transient)
    UBlackboardComponent* Blackboard;

    UPROPERTY(Transient)
    FAITaskExecutionContext ExecutionContext;

    TArray<TWeakObjectPtr<UAITask>, TInlineAllocator<4>> SpawnedTasks;

    bool bIsActive{false};

    float NextActivationTime{0.f};

public:
    void
    InitTask(AAIController* Controller, UUtilityAIDecision* Decision);

    UFUNCTION(BlueprintPure, Category = "Utility AI|Tasks")
    AAIController*
    GetController() const;

    UFUNCTION(BlueprintPure, Category = "Utility AI|Tasks")
    UUtilityAIComponent*
    GetBrainComponent() const;

    UFUNCTION(BlueprintPure, Category = "Utility AI|Tasks")
    UAIPerceptionComponent*
    GetPerceptionComponent() const;

    UFUNCTION(BlueprintPure, Category = "Utility AI|Tasks")
    UBlackboardComponent*
    GetBlackboardComponent() const;

    UFUNCTION(BlueprintPure, Category = "Utility AI|Tasks")
    APawn*
    GetPawn() const;

    UFUNCTION(BlueprintCallable, Category = "Utility AI|Tasks")
    void
    SetTickEnabled(bool bEnabled);

    UFUNCTION(BlueprintImplementableEvent, Category = "Utility AI|Tasks",
        meta = (ScriptName = "OnTick", DisplayName = "OnTick"))
    void
    BP_OnTick(const float DeltaTime);

    UFUNCTION(BlueprintImplementableEvent, Category = "Utility AI|Tasks",
        meta = (ScriptName = "OnActivateTask", DisplayName = "OnActivateTask"))
    void
    BP_OnActivateTask(const FAITaskExecutionContext& Context);

    UFUNCTION(BlueprintImplementableEvent, Category = "Utility AI|Tasks",
        meta = (ScriptName = "OnEndTask", DisplayName = "OnEndTask"))
    void
    BP_OnEndTask(const EAITaskTerminationReason Reason);

    UFUNCTION(BlueprintCallable, Category = "Utility AI|Tasks",
        meta = (ScriptName = "EndTask", DisplayName = "EndTask"))
    void
    BP_EndTask();

    virtual void
    ActivateTask(const FAITaskExecutionContext& Context);

    virtual void
    EndTask(const EAITaskTerminationReason Reason = EAITaskTerminationReason::Finished);

    UFUNCTION(BlueprintPure, Category = "Utility AI|Tasks")
    bool
    HasEnded() const;

    UFUNCTION(BlueprintPure, Category = "Utility AI|Tasks")
    bool
    IsInterruptible() const;

    bool
    CanBeInterruptedBy(const UUtilityAITask* NewTask, const FAITarget& NewTarget) const;

    UFUNCTION(BlueprintPure, Category = "Utility AI|Tasks")
    const FAITaskExecutionContext&
    GetExecutionContext() const;

    UFUNCTION(BlueprintPure, Category = "Utility AI|Tasks")
    const FAITarget&
    GetTarget() const;

    FORCEINLINE void
    ExternalEndTask()
    {
        if (bIsActive) {
            EndTask(EAITaskTerminationReason::Interrupted);
        }
    }

    FORCEINLINE bool
    IsEnabled() const
    {
        return !IsOnCooldown();
    }

    UFUNCTION(BlueprintPure, Category = "Utility AI|Tasks")
    bool
    IsOnCooldown() const;

    UFUNCTION(BlueprintCallable, Category = "Utility AI|Tasks")
    void
    BeginCooldown();

    UFUNCTION(BlueprintCallable, Category = "Utility AI|Tasks")
    void
    ResetCooldown();

    UFUNCTION(BlueprintPure, Category = "Utility AI|Tasks")
    float
    GetAIUpdateInterval() const;

    /* Register the task to be automatically cleaned up */
    UFUNCTION(BlueprintCallable, Category = "Utility AI|Tasks")
    void
    RegisterTaskForCleanup(UAITask* Task);

    virtual UWorld*
    GetWorld() const override;

    FORCEINLINE float
    GetActivationScore() const
    {
        return ExecutionContext.Score;
    }

    UFUNCTION(BlueprintPure, Category = "Utility AI|Tasks")
    const AActor*
    GetTargetActor() const;

    UFUNCTION(BlueprintPure, Category = "Utility AI|Tasks")
    FVector
    GetTargetLocation() const;

    UFUNCTION(BlueprintPure, Category = "Utility AI|Tasks")
    FVector
    GetTargetActorLocation() const;

protected:
    bool
    TickInternal(const float DeltaTime);

    virtual void
    Tick(const float DeltaTime);

private:
    void
    CleanupTasks();
};
