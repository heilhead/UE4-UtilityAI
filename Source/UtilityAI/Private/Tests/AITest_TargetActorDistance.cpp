#include "Tests/AITest_TargetActorDistance.h"

float
UAITest_TargetActorDistance::ComputeScore(const FAIEvaluationContext& Context,
                                          const FAITarget& Target) const
{
    const auto* OwnerPawn = Context.Pawn;
    const auto* TargetActor = Target.GetActor();

    if (!TargetActor) {
        return FLT_MAX;
    }

    const FVector OwnerLocation = OwnerPawn->GetActorLocation();
    const FVector FocusLocation = TargetActor->GetActorLocation();

    return FVector::Distance(OwnerLocation, FocusLocation);
}
