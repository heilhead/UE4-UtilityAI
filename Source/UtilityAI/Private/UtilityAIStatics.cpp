#include "UtilityAIStatics.h"
#include "AIController.h"
#include "DataProviders/AIDataProvider.h"
#include "GameFramework/CharacterMovementComponent.h"

void
UUtilityAIStatics::SetPawnUseControllerDesiredRotation(APawn* Pawn, bool bUseControllerDesiredRotation)
{
    auto* MoveComp = Cast<UCharacterMovementComponent>(Pawn->GetMovementComponent());
    if (IsValid(MoveComp)) {
        MoveComp->bUseControllerDesiredRotation = bUseControllerDesiredRotation;
    }
}

void
UUtilityAIStatics::SetPawnOrientRotationToMovement(APawn* Pawn, bool bOrientRotationToMovement)
{
    auto* MoveComp = Cast<UCharacterMovementComponent>(Pawn->GetMovementComponent());
    if (IsValid(MoveComp)) {
        MoveComp->bOrientRotationToMovement = bOrientRotationToMovement;
    }
}

float
UUtilityAIStatics::GetAIDataProviderFloatValue(const AAIController* Controller,
                                               const FAIDataProviderFloatValue& Binding)
{
    Binding.BindData(Controller, 0);
    return Binding.GetValue();
}

float
UUtilityAIStatics::GetAIDataProviderFloatValue_AnyThread(const AAIController* Controller,
                                                         const FAIDataProviderFloatValue& Binding)
{
    // Create a copy and bind data to the copy, so we don't compete with other threads.
    const FAIDataProviderFloatValue BindingCopy = Binding;
    return GetAIDataProviderFloatValue(Controller, BindingCopy);
}
