#include "Tests/AITest_RunSubtests.h"

namespace {
    FORCEINLINE float
    ComputeHighestScore(const TArray<UAITest*>& Tests, const FAIEvaluationContext& Context,
                        const FAITarget& Target)
    {
        float Result = -FLT_MAX;

        for (const auto* Test : Tests) {
            const float Score = Test->ComputeNormalizedScore(Context, Target);
            if (Score > Result) {
                Result = Score;
            }
        }

        return Result;
    }

    FORCEINLINE float
    ComputeLowestScore(const TArray<UAITest*>& Tests, const FAIEvaluationContext& Context,
                       const FAITarget& Target)
    {
        float Result = FLT_MAX;

        for (const auto* Test : Tests) {
            const float Score = Test->ComputeNormalizedScore(Context, Target);
            if (Score < Result) {
                Result = Score;
            }
        }

        return Result;
    }

    FORCEINLINE float
    ComputeAverageScore(const TArray<UAITest*>& Tests, const FAIEvaluationContext& Context,
                        const FAITarget& Target)
    {
        float Result = 0.f;

        for (const auto* Test : Tests) {
            Result += Test->ComputeNormalizedScore(Context, Target);
        }

        return Result / Tests.Num();
    }
}

float
UAITest_RunSubtests::ComputeScore(const FAIEvaluationContext& Context, const FAITarget& Target) const
{
    if (!Tests.Num()) {
        return 0.f;
    }

    switch (ScoreFunction) {
    case EAISubtestScoreFunction::Highest:
        return ComputeHighestScore(Tests, Context, Target);
    case EAISubtestScoreFunction::Lowest:
        return ComputeLowestScore(Tests, Context, Target);
    case EAISubtestScoreFunction::Average:
        return ComputeAverageScore(Tests, Context, Target);
    default:
        checkNoEntry();
        return 0.f;
    }
}
