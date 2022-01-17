#pragma once

#include "UtilityAIStatics.h"
#include "UtilityAITypes.h"
#include "DataProviders/AIDataProvider.h"
#include "UtilityAITest.generated.h"

USTRUCT()
struct UTILITYAI_API FAIScoreNormalizationParams {
    GENERATED_BODY()

    UPROPERTY(EditDefaultsOnly, Category = "Score")
    FAIDataProviderFloatValue ScoreClampMin;

    UPROPERTY(EditDefaultsOnly, Category = "Score")
    FAIDataProviderFloatValue ScoreClampMax;

    UPROPERTY(EditDefaultsOnly, Category = "Score")
    FAIResponseCurve ResponseCurve;

    FAIScoreNormalizationParams()
    {
        ScoreClampMin.DefaultValue = 0.f;
        ScoreClampMax.DefaultValue = 1.f;
    }
};

UCLASS(Abstract, EditInlineNew)
class UTILITYAI_API UAITest : public UObject {
    GENERATED_BODY()

public:
    UPROPERTY(EditDefaultsOnly, Category = "Score", meta = (ShowOnlyInnerProperties))
    FAIScoreNormalizationParams NormalizationParams;

public:
    virtual float
    ComputeScore(const FAIEvaluationContext& Context, const FAITarget& Target) const
    {
        return 0.f;
    }

    float
    ComputeNormalizedScore(const FAIEvaluationContext& Context, const FAITarget& Target) const;
};

UCLASS(Blueprintable, Abstract)
class UTILITYAI_API UAITest_BlueprintBase : public UAITest {
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintNativeEvent, Category = "Utility AI",
        meta = (ScriptName = "ComputeScore", DisplayName = "ComputeScore"))
    float
    BP_ComputeScore(const FAIEvaluationContext& Context, const FAITarget& Target) const;

    float
    BP_ComputeScore_Implementation(const FAIEvaluationContext& Context, const FAITarget& Target)
    {
        return 0.f;
    }

    virtual float
    ComputeScore(const FAIEvaluationContext& Context, const FAITarget& Target) const override final
    {
        return BP_ComputeScore(Context, Target);
    }
};

FORCEINLINE float
UAITest::ComputeNormalizedScore(const FAIEvaluationContext& Context, const FAITarget& Target) const
{
    const float ClampMin = UUtilityAIStatics::GetAIDataProviderFloatValue_AnyThread(
        Context.Controller, NormalizationParams.ScoreClampMin);

    float ClampMax = UUtilityAIStatics::GetAIDataProviderFloatValue_AnyThread(
        Context.Controller, NormalizationParams.ScoreClampMax);

    if (ClampMax < ClampMin) {
        // Invalid clamping range. Best to return 0.f here to avoid confusion later on.
        return 0.f;
    }

    if (ClampMax == ClampMin) {
        // Generally, they shouldn't be equal, but for constant equation we may only care about the min value.
        // This helps avoid NaN when normalizing.
        ClampMax += KINDA_SMALL_NUMBER;
    }

    const float ValueSpan = ClampMax - ClampMin;
    const float ClampedScore = FMath::Clamp(ComputeScore(Context, Target), ClampMin, ClampMax);
    const float NormalizedScore = (ClampedScore - ClampMin) / ValueSpan;

    return NormalizationParams.ResponseCurve.ComputeValue(NormalizedScore);
}
