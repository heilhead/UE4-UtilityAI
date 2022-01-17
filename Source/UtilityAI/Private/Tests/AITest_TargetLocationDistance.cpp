#include "Tests/AITest_TargetLocationDistance.h"

float
UAITest_TargetLocationDistance::ComputeScore(const FAIEvaluationContext& Context, const FAITarget& Target) const
{
    const auto* OwnerPawn = Context.Pawn;
    const FVector OwnerLocation = OwnerPawn->GetActorLocation();

    return FVector::Distance(OwnerLocation, Target.GetLocation());
}
