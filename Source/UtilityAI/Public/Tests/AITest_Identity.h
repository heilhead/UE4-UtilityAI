#pragma once

#include "UtilityAITest.h"
#include "AITest_Identity.generated.h"

UCLASS(meta=(DisplayName="Identity"))
class UTILITYAI_API UAITest_Identity : public UAITest {
    GENERATED_BODY()

public:
    UAITest_Identity();

public:
    UPROPERTY(EditDefaultsOnly, Category = "Parameters")
    FAIDataProviderFloatValue ReturnValue;

public:
    virtual float
    ComputeScore(const FAIEvaluationContext& Context, const FAITarget& Target) const override;
};
