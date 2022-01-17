#include "UtilityAIScoreEvaluator.h"
#include "AIController.h"

void
UUtilityAIScoreEvaluator::PostLoad()
{
    Super::PostLoad();

    CompensationFactor = 1.f - 1.f / Tests.Num();
}

float
UUtilityAIScoreEvaluator::ComputeScore(const FAIEvaluationContext& Context, const float Min,
                                       const float ScoringFactor, const FAITarget& Target) const
{
    float NormalizedScore = 1.f;
    float FinalScore = ScoringFactor;

    for (const auto* Test : Tests) {
        if (FinalScore <= Min) {
            break;
        }

        NormalizedScore *= Test->ComputeNormalizedScore(Context, Target);
        FinalScore = ComputeCompensatedScore(NormalizedScore) * ScoringFactor;
    }

    return FinalScore;
}

FORCEINLINE float
UUtilityAIScoreEvaluator::ComputeCompensatedScore(const float NormalizedScore) const
{
    const float CompensationValue = (1.f - NormalizedScore) * CompensationFactor;
    return NormalizedScore + CompensationValue * NormalizedScore;
}
