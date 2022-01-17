#pragma once

#include "UtilityAITest.h"
#include "AITest_TargetActorAffiliation.generated.h"

UCLASS(meta=(DisplayName="Target Actor Affiliation"))
class UTILITYAI_API UAITest_TargetActorAffiliation : public UAITest {
    GENERATED_BODY()

public:
    UPROPERTY(EditDefaultsOnly, Category = "Parameters")
    bool bAcceptFriendly = true;

    UPROPERTY(EditDefaultsOnly, Category = "Parameters")
    bool bAcceptHostile = true;

    UPROPERTY(EditDefaultsOnly, Category = "Parameters")
    bool bAcceptNeutral = true;

public:
    virtual float
    ComputeScore(const FAIEvaluationContext& Context, const FAITarget& Target) const override;
};
