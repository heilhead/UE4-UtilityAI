#pragma once

#include "UtilityAITest.h"
#include "AITest_NumberOfSensedActors.generated.h"

class UAISense;

UCLASS(meta=(DisplayName="Number Of Sensed Actors"))
class UTILITYAI_API UAITest_NumberOfSensedActors : public UAITest {
    GENERATED_BODY()

public:
    UPROPERTY(EditDefaultsOnly, Category = "Parameters")
    bool bAcceptFriendly = true;

    UPROPERTY(EditDefaultsOnly, Category = "Parameters")
    bool bAcceptHostile = true;

    UPROPERTY(EditDefaultsOnly, Category = "Parameters")
    bool bAcceptNeutral = true;

    UPROPERTY(EditDefaultsOnly, Category = "Parameters")
    bool bFilterBySense = false;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Parameters",
        meta=(EditCondition="bFilterBySense == true", EditConditionHides))
    TSubclassOf<UAISense> SenseClass;

public:
    virtual float
    ComputeScore(const FAIEvaluationContext& Context, const FAITarget& Target) const override;
};
