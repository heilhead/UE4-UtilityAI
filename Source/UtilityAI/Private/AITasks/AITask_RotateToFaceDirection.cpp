#include "AITasks/AITask_RotateToFaceDirection.h"
#include "AIController.h"

FORCEINLINE float
CalculateAngleDifferenceDot(const FVector& VectorA, const FVector& VectorB)
{
    return VectorA.IsNearlyZero() || VectorB.IsNearlyZero() ? 1.f : VectorA.CosineAngle2D(VectorB);
}

UAITask_RotateToFaceDirection::UAITask_RotateToFaceDirection(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{
    bTickingTask = true;
}

UAITask_RotateToFaceDirection*
UAITask_RotateToFaceDirection::RotateToFaceDirection(AAIController* Controller, AActor* Actor,
                                                     const FVector Location, const float PrecisionDegrees,
                                                     bool bLockAILogic)
{
    UAITask_RotateToFaceDirection* MyTask = NewAITask<UAITask_RotateToFaceDirection>(
        *Controller, EAITaskPriority::High);
    if (MyTask) {
        MyTask->Precision = PrecisionDegrees;
        MyTask->FocusActor = Actor;
        MyTask->FocusLocation = Location;

        if (bLockAILogic) {
            MyTask->RequestAILogicLocking();
        }
    }

    return MyTask;
}

void
UAITask_RotateToFaceDirection::Activate()
{
    Super::Activate();

    PrecisionDot = FMath::Cos(FMath::DegreesToRadians(Precision));

    auto* Controller = GetAIController();
    if (FocusActor) {
        Controller->SetFocus(FocusActor);
    } else {
        Controller->SetFocalPoint(FocusLocation);
    }
}

void
UAITask_RotateToFaceDirection::TickTask(float DeltaTime)
{
    Super::TickTask(DeltaTime);

    const auto* Controller = GetAIController();
    const auto* Pawn = Controller->GetPawn();
    const FVector PawnDirection = Pawn->GetActorForwardVector();
    const FVector FocalPoint = Controller->GetFocalPointForPriority(EAIFocusPriority::Gameplay);

    const float AngleDiff = CalculateAngleDifferenceDot(PawnDirection, FocalPoint - Pawn->GetActorLocation());

    if (AngleDiff >= PrecisionDot) {
        EndTask();
        OnFinished.Broadcast();
    }
}
