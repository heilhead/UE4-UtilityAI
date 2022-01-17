#pragma once

#include "UtilityAITypes.generated.h"

class UAIPerceptionComponent;
class UBlackboardComponent;
class UUtilityAIComponent;
class AAIController;

DECLARE_STATS_GROUP(TEXT("UtilityAI"), STATGROUP_UtilityAI, STATCAT_Advanced);

USTRUCT(BlueprintType)
struct UTILITYAI_API FAIEvaluationContext {
    GENERATED_BODY()

    UPROPERTY(Transient, BlueprintReadOnly)
    AAIController* Controller{nullptr};

    UPROPERTY(Transient, BlueprintReadOnly)
    UUtilityAIComponent* Brain{nullptr};

    UPROPERTY(Transient, BlueprintReadOnly)
    UAIPerceptionComponent* Perception{nullptr};

    UPROPERTY(Transient, BlueprintReadOnly)
    APawn* Pawn{nullptr};

    UPROPERTY(Transient, BlueprintReadOnly)
    UBlackboardComponent* Blackboard{nullptr};

    UPROPERTY(Transient, BlueprintReadOnly)
    float CurrentTime{0.f};

    // TODO: Decision parameters, like decision type (e.g. skill, emote)?
};

UENUM()
enum class EAIResponseCurveType : uint8 {
    Linear,
    Polynomial,
    Logistic,
    Logit,
    Normal,
    Sine,
};

USTRUCT()
struct UTILITYAI_API FAIResponseCurve {
    GENERATED_BODY()

    UPROPERTY(EditDefaultsOnly)
    EAIResponseCurveType Type{EAIResponseCurveType::Linear};

    UPROPERTY(EditDefaultsOnly)
    float Slope = 1.f;

    UPROPERTY(EditDefaultsOnly)
    float Exponent = 0.f;

    UPROPERTY(EditDefaultsOnly)
    float XShift = 0.f;

    UPROPERTY(EditDefaultsOnly)
    float YShift = 0.f;

    float
    ComputeValue(float NormalizedInputValue) const;
};

UENUM()
enum class EAITargetType { None, Actor, Location };

USTRUCT(BlueprintType)
struct UTILITYAI_API FAITarget {
    GENERATED_BODY()

    friend bool
    operator==(const FAITarget& A, const FAITarget& B);

    static FAITarget InvalidTarget;

protected:
    UPROPERTY(Transient, BlueprintReadOnly)
    AActor* Actor{nullptr};

    UPROPERTY(Transient, BlueprintReadOnly)
    FVector Location{FLT_MAX};

    void* UserData{nullptr};

public:
    FAITarget() = default;

    explicit
    FAITarget(const FVector& InLocation)
        : Location(InLocation)
    {
    }

    explicit
    FAITarget(AActor* InActor)
        : Actor(InActor)
    {
    }

    FORCEINLINE void
    SetActor(AActor* InActor)
    {
        Actor = InActor;
    }

    FORCEINLINE const AActor*
    GetActor() const
    {
        return Actor;
    }

    FORCEINLINE AActor*
    GetActor()
    {
        return Actor;
    }

    FORCEINLINE void
    SetLocation(const FVector& InLocation)
    {
        Location = InLocation;
    }

    FORCEINLINE FVector
    GetLocation() const
    {
        return Location;
    }

    FORCEINLINE void
    SetUserData(void* InUserData)
    {
        UserData = InUserData;
    }

    FORCEINLINE void*
    GetUserData() const
    {
        return UserData;
    }

    FORCEINLINE void
    Reset()
    {
        Actor = nullptr;
    }
};

FORCEINLINE bool
operator==(const FAITarget& A, const FAITarget& B)
{
    return A.Actor == B.Actor && A.Location == B.Location && A.UserData == B.UserData;
}

FORCEINLINE bool
operator!=(const FAITarget& A, const FAITarget& B)
{
    return !(A == B);
}
