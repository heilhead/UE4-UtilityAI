#pragma once

#include "UtilityAITest.h"
#include "AITest_TargetActorLineOfSight.generated.h"

UENUM(BlueprintType)
enum class EAIVisibilityTraceShape : uint8 { Line, Sphere };

UCLASS(meta=(DisplayName="Target Actor Line Of Sight"))
class UTILITYAI_API UAITest_TargetActorLineOfSight : public UAITest {
    GENERATED_BODY()

public:
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Parameters")
    EAIVisibilityTraceShape TraceShape = EAIVisibilityTraceShape::Line;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Parameters",
        meta = (EditCondition = "TraceShape == EAIVisibilityTraceShape::Sphere", EditConditionHides))
    float TraceSphereRadius = 10.f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Parameters")
    TEnumAsByte<ECollisionChannel> CollisionChannel;

public:
    virtual float
    ComputeScore(const FAIEvaluationContext& Context, const FAITarget& Target) const override;
};
