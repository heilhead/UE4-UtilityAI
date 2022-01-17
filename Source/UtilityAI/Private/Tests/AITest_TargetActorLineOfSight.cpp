#include "Tests/AITest_TargetActorLineOfSight.h"
#include "AIController.h"

float
UAITest_TargetActorLineOfSight::ComputeScore(const FAIEvaluationContext& Context,
                                             const FAITarget& Target) const
{
    const auto* OwnerPawn = Context.Pawn;
    const auto* TargetActor = Target.GetActor();

    if (!TargetActor) {
        return 0.f;
    }

    FCollisionQueryParams CollisionQueryParams(FName(TEXT("AITargetActorLineOfSight")),
                                               SCENE_QUERY_STAT_ONLY(AITargetActorLineOfSight), false,
                                               nullptr);
    CollisionQueryParams.bReturnPhysicalMaterial = false;
    CollisionQueryParams.bReturnFaceIndex = false;
    CollisionQueryParams.bSkipNarrowPhase = true;
    CollisionQueryParams.bFindInitialOverlaps = true;

    CollisionQueryParams.AddIgnoredActor(OwnerPawn);
    CollisionQueryParams.AddIgnoredActor(TargetActor);

    const FCollisionShape CollisionShape = TraceShape == EAIVisibilityTraceShape::Sphere
                                               ? FCollisionShape::MakeSphere(TraceSphereRadius)
                                               : FCollisionShape::MakeSphere(KINDA_SMALL_NUMBER);

    FHitResult OutHitResult;

    const UWorld* World = Context.Controller->GetWorld();

    const FVector OwnerLocation = OwnerPawn->GetActorLocation();
    const FVector FocusLocation = TargetActor->GetActorLocation();

    const auto bHit =
        World->SweepSingleByChannel(OutHitResult, OwnerLocation, FocusLocation, FQuat::Identity,
                                    CollisionChannel, CollisionShape, CollisionQueryParams);

    return bHit ? 0.f : 1.f;
}
