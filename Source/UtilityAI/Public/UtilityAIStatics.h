#pragma once

#include "UtilityAIStatics.generated.h"

class AAIController;
struct FAIDataProviderFloatValue;

UCLASS(Abstract)
class UTILITYAI_API UUtilityAIStatics : public UBlueprintFunctionLibrary {
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable, Category = "Utility AI")
    static void
    SetPawnUseControllerDesiredRotation(APawn* Pawn, bool bUseControllerDesiredRotation);

    UFUNCTION(BlueprintCallable, Category = "Utility AI")
    static void
    SetPawnOrientRotationToMovement(APawn* Pawn, bool bOrientRotationToMovement);

    static float
    GetAIDataProviderFloatValue(const AAIController* Controller, const FAIDataProviderFloatValue& Binding);

    static float
    GetAIDataProviderFloatValue_AnyThread(const AAIController* Controller,
                                          const FAIDataProviderFloatValue& Binding);
};
