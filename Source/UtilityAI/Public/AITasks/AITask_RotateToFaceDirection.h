#pragma once

#include "Tasks/AITask.h"
#include "AITask_RotateToFaceDirection.generated.h"

UCLASS()
class UTILITYAI_API UAITask_RotateToFaceDirection : public UAITask {
    GENERATED_BODY()

public:
    UAITask_RotateToFaceDirection(const FObjectInitializer& ObjectInitializer);

protected:
    DECLARE_DYNAMIC_MULTICAST_DELEGATE(FTaskFinishedSignature);

    UPROPERTY(BlueprintAssignable)
    FTaskFinishedSignature OnFinished;

    UPROPERTY(Transient)
    AActor* FocusActor;

    UPROPERTY(Transient)
    FVector FocusLocation;

    float Precision = 2.f;
    float PrecisionDot;

public:
    UFUNCTION(BlueprintCallable, Category = "AI|Tasks",
        meta = (AdvancedDisplay = "bLockAILogic", DefaultToSelf = "Controller", BlueprintInternalUseOnly =
            "TRUE"))
    static UAITask_RotateToFaceDirection*
    RotateToFaceDirection(AAIController* Controller, AActor* Actor = nullptr,
                          const FVector Location = FVector::ZeroVector, const float PrecisionDegrees = 5.f,
                          bool bLockAILogic = false);

protected:
    virtual void
    Activate() override;

    virtual void
    TickTask(float DeltaTime) override;
};
