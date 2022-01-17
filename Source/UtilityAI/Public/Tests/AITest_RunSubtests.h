#pragma once

#include "UtilityAITest.h"
#include "AITest_RunSubtests.generated.h"

UENUM()
enum class EAISubtestScoreFunction : uint8 {
    Highest,
    Lowest,
    Average
};

UCLASS(meta=(DisplayName="Run Subtests"))
class UTILITYAI_API UAITest_RunSubtests : public UAITest {
    GENERATED_BODY()

public:
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Tests")
    EAISubtestScoreFunction ScoreFunction{EAISubtestScoreFunction::Highest};

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Instanced, Category = "Tests")
    TArray<UAITest*> Tests;

public:
    virtual float
    ComputeScore(const FAIEvaluationContext& Context, const FAITarget& Target) const override;
};
