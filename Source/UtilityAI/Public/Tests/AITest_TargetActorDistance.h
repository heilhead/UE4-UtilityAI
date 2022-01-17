#pragma once

#include "UtilityAITest.h"
#include "AITest_TargetActorDistance.generated.h"

UCLASS(meta=(DisplayName="Target Actor Distance"))
class UTILITYAI_API UAITest_TargetActorDistance : public UAITest {
    GENERATED_BODY()

public:
    virtual float
    ComputeScore(const FAIEvaluationContext& Context, const FAITarget& Target) const override;
};
