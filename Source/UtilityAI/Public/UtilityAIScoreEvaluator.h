#pragma once

#include "UtilityAITest.h"
#include "UtilityAIScoreEvaluator.generated.h"

UCLASS(BlueprintType, EditInlineNew, DefaultToInstanced)
class UTILITYAI_API UUtilityAIScoreEvaluator : public UDataAsset {
    GENERATED_BODY()

public:
    UPROPERTY(EditDefaultsOnly, Category = "Tests")
    bool bRequiresTarget = true;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Instanced, Category = "Tests")
    TArray<UAITest*> Tests;

    float CompensationFactor = 0.f;

public:
    virtual void
    PostLoad() override;

    float
    ComputeScore(const FAIEvaluationContext& Context, const float Min, const float ScoringFactor,
                 const FAITarget& Target) const;

private:
    float
    ComputeCompensatedScore(float NormalizedScore) const;
};
