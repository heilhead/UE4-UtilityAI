#include "UtilityAI.h"
#include "UtilityAITypes.h"

FAIEvaluationResult
UUtilityAI::Evaluate(const FAIEvaluationContext& Context, const TArray<FAITarget>& TargetList) const
{
    return DecisionMaker->Evaluate(Context, TargetList);
}
