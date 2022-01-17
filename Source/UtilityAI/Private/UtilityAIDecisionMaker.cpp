#include "UtilityAIDecisionMaker.h"
#include "AIController.h"
#include "Components/UtilityAIComponent.h"
#include "UtilityAIScoreEvaluator.h"
#include "UtilityAIStatics.h"
#include "UtilityAITask.h"

DECLARE_CYCLE_STAT(TEXT("Decision Maker Evaluation [TG]"), STAT_DecisionMakerEvaluation, STATGROUP_UtilityAI);

struct FAIEvaluationInternalData {
    const UUtilityAIDecision* Decision{nullptr};
    const FAIEvaluationContext* Context{nullptr};
    UUtilityAITask* CurrentTaskInstance{nullptr};
    UUtilityAITask* WinningTaskInstance{nullptr};
    FAITarget WinningTarget;
    float WinningScore{0.f};
    float ScoringFactor{0.f};
};

FAIEvaluationResult::FAIEvaluationResult(UUtilityAITask* InTaskInstance, const float InScore,
                                         const FAITarget& InTarget)
    : TaskInstance{InTaskInstance}, Target{InTarget}, Score{InScore}
{
}

UUtilityAIDecision::UUtilityAIDecision()
{
    ScoringFactor.DefaultValue = 1.f;
}

FORCEINLINE void
EvaluateSingle(FAIEvaluationInternalData& EvalData, const FAITarget& Target)
{
    const float Score =
        EvalData.Decision->Evaluator->ComputeScore(*EvalData.Context, EvalData.WinningScore,
                                                   EvalData.ScoringFactor, Target);
    if (Score > EvalData.WinningScore) {
        EvalData.WinningScore = Score;
        EvalData.WinningTaskInstance = EvalData.CurrentTaskInstance;
        EvalData.WinningTarget = Target;
    }
}

FORCEINLINE void
EvaluateTargetList(FAIEvaluationInternalData& EvalData, const TArray<FAITarget>& TargetList)
{
    for (const auto& Target : TargetList) {
        EvaluateSingle(EvalData, Target);
    }
}

void
UUtilityAIDecisionMaker::PostLoad()
{
    Super::PostLoad();
}

FAIEvaluationResult
UUtilityAIDecisionMaker::Evaluate(const FAIEvaluationContext& Context,
                                  const TArray<FAITarget>& TargetList) const
{
    SCOPE_CYCLE_COUNTER(STAT_DecisionMakerEvaluation);

    FAIEvaluationInternalData EvalData;

    auto* Brain = Context.Brain;

    for (int32 Idx = 0, Num = Decisions.Num(); Idx < Num; Idx++) {
        const auto* Decision = Decisions[Idx];
        const float ScoringFactor = UUtilityAIStatics::GetAIDataProviderFloatValue_AnyThread(
            Context.Controller, Decision->ScoringFactor);
        if (ScoringFactor < EvalData.WinningScore) {
            continue;
        }

        auto* CurrentTaskInstance = Brain->GetTaskInstance(Idx);
        if (!CurrentTaskInstance->IsEnabled()) {
            continue;
        }

        EvalData.CurrentTaskInstance = CurrentTaskInstance;
        EvalData.Decision = Decision;
        EvalData.Context = &Context;
        EvalData.ScoringFactor = ScoringFactor;

        if (Decision->Evaluator->bRequiresTarget) {
            EvaluateTargetList(EvalData, TargetList);
        } else {
            EvaluateSingle(EvalData, FAITarget::InvalidTarget);
        }
    }

    return FAIEvaluationResult(EvalData.WinningTaskInstance, EvalData.WinningScore, EvalData.WinningTarget);
}
