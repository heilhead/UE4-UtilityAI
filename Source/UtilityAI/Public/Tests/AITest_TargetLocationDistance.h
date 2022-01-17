#pragma once

#include "UtilityAITest.h"
#include "AITest_TargetLocationDistance.generated.h"

UCLASS(meta=(DisplayName="Target Location Distance"))
class UTILITYAI_API UAITest_TargetLocationDistance : public UAITest {
    GENERATED_BODY()

public:
    virtual float
    ComputeScore(const FAIEvaluationContext& Context, const FAITarget& Target) const override;
};
