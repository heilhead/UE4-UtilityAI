#pragma once

#include "UtilityAIDecisionMaker.h"
#include "UtilityAI.generated.h"

class AAIController;

UCLASS()
class UTILITYAI_API UUtilityAI : public UDataAsset {
    GENERATED_BODY()

public:
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    UUtilityAIDecisionMaker* DecisionMaker;

public:
    FAIEvaluationResult
    Evaluate(const FAIEvaluationContext& Context, const TArray<FAITarget>& TargetList) const;
};
