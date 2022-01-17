#pragma once

#include "UtilityAITest.h"
#include "AITest_TimeElapsed.generated.h"

UCLASS(meta=(DisplayName="Time Elapsed"))
class UTILITYAI_API UAITest_TimeElapsed : public UAITest {
    GENERATED_BODY()

public:
    UPROPERTY(EditDefaultsOnly, Category = "Parameters")
    FName BlackboardKeyName;

public:
    virtual float
    ComputeScore(const FAIEvaluationContext& Context, const FAITarget& Target) const override;
};
