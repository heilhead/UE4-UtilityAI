#pragma once

#include "UtilityAIScoreEvaluator.h"
#include "DataProviders/AIDataProvider.h"
#include "UtilityAIDecisionMaker.generated.h"

struct FAIInitializationContext;
struct FAIEvaluationContext;
class UUtilityAIScoreEvaluator;
class UUtilityAITask;

USTRUCT(BlueprintType)
struct UTILITYAI_API FAIEvaluationResult {
    GENERATED_BODY()

    UPROPERTY(Transient, BlueprintReadOnly)
    UUtilityAITask* TaskInstance{nullptr};

    UPROPERTY(Transient, BlueprintReadOnly)
    FAITarget Target;

    UPROPERTY(Transient, BlueprintReadOnly)
    float Score{0.f};

    FAIEvaluationResult() = default;

    FAIEvaluationResult(UUtilityAITask* InTaskInstance, const float InScore, const FAITarget& InTarget);
};

UCLASS(BlueprintType, EditInlineNew)
class UTILITYAI_API UUtilityAIDecision : public UDataAsset {
    GENERATED_BODY()

public:
    UUtilityAIDecision();

public:
    UPROPERTY(EditDefaultsOnly)
    FAIDataProviderFloatValue ScoringFactor;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    TSubclassOf<UUtilityAITask> TaskClass;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Instanced)
    UUtilityAIScoreEvaluator* Evaluator;
};

UCLASS(BlueprintType)
class UTILITYAI_API UUtilityAIDecisionMaker : public UDataAsset {
    GENERATED_BODY()

public:
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Instanced)
    TArray<UUtilityAIDecision*> Decisions;

public:
    virtual void
    PostLoad() override;

    FAIEvaluationResult
    Evaluate(const FAIEvaluationContext& Context, const TArray<FAITarget>& TargetList) const;
};
