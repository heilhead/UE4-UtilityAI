#include "AITasks/AITask_ActivateGameplayAbility.h"
#include "AbilitySystemGlobals.h"
#include "AbilitySystemComponent.h"
#include "AIController.h"
#include "GameplayAbilitySpec.h"

UAITask_ActivateGameplayAbility::UAITask_ActivateGameplayAbility(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{
    bTickingTask = true;
}

UAITask_ActivateGameplayAbility*
UAITask_ActivateGameplayAbility::ActivateGameplayAbilityByClass(AAIController* Controller,
                                                                TSubclassOf<UGameplayAbility> Ability,
                                                                bool bLockAILogic)
{
    auto* AbilityClass = *Ability;
    if (!AbilityClass) {
        return nullptr;
    }

    auto* ASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(Controller->GetPawn());
    if (!ASC) {
        return nullptr;
    }

    FGameplayAbilitySpecHandle SpecHandle;

    for (const auto& Spec : ASC->GetActivatableAbilities()) {
        if (Spec.Ability && Spec.Ability->GetClass() == AbilityClass) {
            SpecHandle = Spec.Handle;
        }
    }

    if (!SpecHandle.IsValid()) {
        return nullptr;
    }

    return ActivateGameplayAbilityByHandle(Controller, ASC, SpecHandle, bLockAILogic);
}

UAITask_ActivateGameplayAbility*
UAITask_ActivateGameplayAbility::ActivateGameplayAbilityByTag(AAIController* Controller,
                                                              const FGameplayTag Tag, bool bLockAILogic)
{
    if (!Tag.IsValid()) {
        return nullptr;
    }

    auto* ASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(Controller->GetPawn());
    if (!ASC) {
        return nullptr;
    }

    FGameplayAbilitySpecHandle SpecHandle;

    for (const auto& Spec : ASC->GetActivatableAbilities()) {
        if (Spec.Ability && Spec.Ability->AbilityTags.HasTag(Tag)) {
            SpecHandle = Spec.Handle;
        }
    }

    if (!SpecHandle.IsValid()) {
        return nullptr;
    }

    return ActivateGameplayAbilityByHandle(Controller, ASC, SpecHandle, bLockAILogic);
}

UAITask_ActivateGameplayAbility*
UAITask_ActivateGameplayAbility::ActivateGameplayAbilityByHandle(AAIController* Controller,
                                                                 UAbilitySystemComponent* ASC,
                                                                 const FGameplayAbilitySpecHandle Handle,
                                                                 bool bLockAILogic)
{
    UAITask_ActivateGameplayAbility* MyTask = NewAITask<UAITask_ActivateGameplayAbility>(
        *Controller, EAITaskPriority::High);
    if (MyTask) {
        FRequestSpec Spec;
        Spec.ActiveAbilityHandle = Handle;
        Spec.ASC = ASC;

        MyTask->RequestSpec = Spec;

        if (bLockAILogic) {
            MyTask->RequestAILogicLocking();
        }
    }

    return MyTask;
}

void
UAITask_ActivateGameplayAbility::Activate()
{
    Super::Activate();

    auto* ASC = RequestSpec.ASC.Get();
    if (!ASC) {
        return HandleActivationFailed();
    }

    const bool bSuccess = ASC->TryActivateAbility(RequestSpec.ActiveAbilityHandle);
    if (!bSuccess) {
        return HandleActivationFailed();
    }
}

void
UAITask_ActivateGameplayAbility::OnDestroy(bool bInOwnerFinished)
{
    // In case the task was aborted externally, we want to end the gameplay ability too.
    if (auto* ASC = RequestSpec.ASC.Get()) {
        if (const auto* Spec = ASC->FindAbilitySpecFromHandle(RequestSpec.ActiveAbilityHandle)) {
            if (Spec->IsActive()) {
                ASC->CancelAbility(Spec->Ability);
            }
        }
    }

    Super::OnDestroy(bInOwnerFinished);
}

void
UAITask_ActivateGameplayAbility::TickTask(float DeltaTime)
{
    Super::TickTask(DeltaTime);

    auto* ASC = RequestSpec.ASC.Get();
    if (!ASC) {
        return HandleAbilityFinished();
    }

    const auto* Spec = ASC->FindAbilitySpecFromHandle(RequestSpec.ActiveAbilityHandle);
    if (!Spec || !Spec->IsActive()) {
        return HandleAbilityFinished();
    }
}
