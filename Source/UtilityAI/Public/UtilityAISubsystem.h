#pragma once

#include "UtilityAIDecisionMaker.h"
#include "UtilityAITypes.h"
#include "UtilityAISubsystem.generated.h"

class AAIController;
class UUtilityAIComponent;

UCLASS()
class UTILITYAI_API UUtilityAISubsystem final : public UTickableWorldSubsystem {
    GENERATED_BODY()

    struct FAIUpdateData {
        UUtilityAIComponent* Brain;
        FAIEvaluationResult DecisionResult;
    };

    TArray<FAIUpdateData> UpdatedAgents;
    FCriticalSection UpdatedAgentsSync;

    UPROPERTY(Transient)
    TArray<UUtilityAIComponent*> ScheduledAgents;
    FCriticalSection ScheduledAgentsSync;

    float CurrentTime{0.f};

public:
    virtual void
    Tick(float DeltaTime) override;

    virtual TStatId
    GetStatId() const override;

    FORCEINLINE void
    ScheduleAgent(UUtilityAIComponent* Brain)
    {
        FScopeLock Lock(&ScheduledAgentsSync);
        ScheduledAgents.Add(Brain);
    }

    virtual bool
    DoesSupportWorldType(EWorldType::Type WorldType) const override;

private:
    void
    UpdateAgent_Concurrent(UUtilityAIComponent* Brain);
};
