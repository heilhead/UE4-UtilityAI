#include "Tests/AITest_TargetActorAffiliation.h"
#include "AIController.h"

float
UAITest_TargetActorAffiliation::ComputeScore(const FAIEvaluationContext& Context, const FAITarget& Target) const
{
    const auto* TargetActor = Target.GetActor();

    if (!TargetActor) {
        return 0.f;
    }

    const auto Attitude = Context.Controller->GetTeamAttitudeTowards(*TargetActor);

    switch (Attitude) {
    case ETeamAttitude::Friendly:
        return bAcceptFriendly ? 1.f : 0.f;
    case ETeamAttitude::Hostile:
        return bAcceptHostile ? 1.f : 0.f;
    case ETeamAttitude::Neutral:
        return bAcceptNeutral ? 1.f : 0.f;
    }

    return 0.f;
}
