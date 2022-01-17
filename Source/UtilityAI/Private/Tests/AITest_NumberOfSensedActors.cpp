#include "Tests/AITest_NumberOfSensedActors.h"
#include "AIController.h"
#include "Perception/AIPerceptionComponent.h"

float
UAITest_NumberOfSensedActors::ComputeScore(const FAIEvaluationContext& Context, const FAITarget& Target) const
{
    const auto* Perception = Context.Perception;
    if (!Perception) {
        return 0.f;
    }

    const auto* Controller = Context.Controller;
    const auto SenseID = UAISense::GetSenseID(SenseClass);

    float Result = 0.f;

    for (auto DataIt = Perception->GetPerceptualDataConstIterator(); DataIt; ++DataIt) {
        const FActorPerceptionInfo& ActorPerceptionInfo = DataIt->Value;
        if ((SenseID.IsValid() && !ActorPerceptionInfo.HasKnownStimulusOfSense(SenseID)) ||
            !ActorPerceptionInfo.HasAnyKnownStimulus()) {
            continue;
        }

        const auto* TargetActor = ActorPerceptionInfo.Target.Get();
        if (!IsValid(TargetActor)) {
            continue;
        }

        const auto Attitude = Controller->GetTeamAttitudeTowards(*TargetActor);
        if ((Attitude == ETeamAttitude::Friendly && bAcceptFriendly) ||
            (Attitude == ETeamAttitude::Hostile && bAcceptHostile) ||
            (Attitude == ETeamAttitude::Neutral && bAcceptNeutral)) {
            Result += 1.f;
        }
    }

    return Result;
}
