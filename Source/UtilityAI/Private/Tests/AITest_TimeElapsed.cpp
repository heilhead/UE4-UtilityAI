#include "Tests/AITest_TimeElapsed.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Float.h"

float
UAITest_TimeElapsed::ComputeScore(const FAIEvaluationContext& Context, const FAITarget& Target) const
{
    const FBlackboard::FKey KeyID = Context.Blackboard->GetKeyID(BlackboardKeyName);

    if (KeyID == FBlackboard::InvalidKey) {
        return 0.f;
    }

    if (const auto* Blackboard = Context.Blackboard) {
        const float Timestamp = Blackboard->GetValue<UBlackboardKeyType_Float>(KeyID);
        return Context.CurrentTime - Timestamp;
    }

    return 0.f;
}
