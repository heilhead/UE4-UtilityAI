#pragma once

#include "Tasks/AITask.h"
#include "GameplayAbilitySpec.h"
#include "GameplayTagContainer.h"
#include "AITask_ActivateGameplayAbility.generated.h"

class UGameplayAbility;

UCLASS()
class UTILITYAI_API UAITask_ActivateGameplayAbility : public UAITask {
    GENERATED_BODY()

public:
    UAITask_ActivateGameplayAbility(const FObjectInitializer& ObjectInitializer);

protected:
    DECLARE_DYNAMIC_MULTICAST_DELEGATE(FTaskFinishedSignature);

    struct FRequestSpec {
        TWeakObjectPtr<UAbilitySystemComponent> ASC{nullptr};
        FGameplayAbilitySpecHandle ActiveAbilityHandle{};

        FRequestSpec() = default;
    };

    FRequestSpec RequestSpec;

    UPROPERTY(BlueprintAssignable)
    FTaskFinishedSignature OnAbilityFinished;

    UPROPERTY(BlueprintAssignable)
    FTaskFinishedSignature OnActivationFailed;

public:
    UFUNCTION(BlueprintCallable, Category = "AI|Tasks",
        meta = (AdvancedDisplay = "bLockAILogic", DefaultToSelf = "Controller", BlueprintInternalUseOnly =
            "TRUE"))
    static UAITask_ActivateGameplayAbility*
    ActivateGameplayAbilityByClass(AAIController* Controller, TSubclassOf<UGameplayAbility> Ability,
                                   bool bLockAILogic = false);

    UFUNCTION(BlueprintCallable, Category = "AI|Tasks",
        meta = (AdvancedDisplay = "bLockAILogic", DefaultToSelf = "Controller", BlueprintInternalUseOnly =
            "TRUE"))
    static UAITask_ActivateGameplayAbility*
    ActivateGameplayAbilityByTag(AAIController* Controller, const FGameplayTag Tag,
                                 bool bLockAILogic = false);

    UFUNCTION(BlueprintPure)
    FGameplayAbilitySpecHandle
    GetAbilitySpecHandle() const
    {
        return RequestSpec.ActiveAbilityHandle;
    }

protected:
    static UAITask_ActivateGameplayAbility*
    ActivateGameplayAbilityByHandle(AAIController* Controller, UAbilitySystemComponent* ASC,
                                    const FGameplayAbilitySpecHandle Handle, bool bLockAILogic);

    virtual void
    Activate() override;

    virtual void
    OnDestroy(bool bInOwnerFinished) override;

    virtual void
    TickTask(float DeltaTime) override;

    FORCEINLINE void
    HandleActivationFailed()
    {
        // TODO: Log reason?
        EndTask();
        return OnActivationFailed.Broadcast();
    }

    FORCEINLINE void
    HandleAbilityFinished()
    {
        EndTask();
        return OnAbilityFinished.Broadcast();
    }
};
