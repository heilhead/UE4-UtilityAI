#pragma once

#include "UtilityAITest.h"
#include "AITest_TargetActorSense.generated.h"

class UAISense;

UENUM(BlueprintType)
enum class EAISenseTestType : uint8 {
    ActiveStimulusOfSense,
    KnownStimulusOfSense,
    AnyActiveStimulus,
    AnyKnownStimulus,
};

UCLASS(meta=(DisplayName="Target Actor Sense"))
class UTILITYAI_API UAITest_TargetActorSense : public UAITest {
    GENERATED_BODY()

public:
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Parameters")
    EAISenseTestType TestType{EAISenseTestType::ActiveStimulusOfSense};

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Parameters",
        meta=(EditCondition=
            "TestType == EAISenseTestType::ActiveStimulusOfSense || TestType == EAISenseTestType::KnownStimulusOfSense"
            , EditConditionHides
        ))
    TSubclassOf<UAISense> SenseClass;

public:
    virtual float
    ComputeScore(const FAIEvaluationContext& Context, const FAITarget& Target) const override;
};
