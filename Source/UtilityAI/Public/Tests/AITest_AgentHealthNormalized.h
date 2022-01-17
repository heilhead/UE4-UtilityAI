#pragma once

#include "AttributeSet.h"
#include "UtilityAITest.h"
#include "AITest_AgentHealthNormalized.generated.h"

UCLASS(meta=(DisplayName="Agent Health Normalized"))
class UTILITYAI_API UAITest_AgentHealthNormalized : public UAITest {
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, Category = "Parameters")
    FGameplayAttribute CurrentHealthAttribute;

    UPROPERTY(EditAnywhere, Category = "Parameters")
    FGameplayAttribute MaxHealthAttribute;

    virtual float
    ComputeScore(const FAIEvaluationContext& Context, const FAITarget& Target) const override;
};
