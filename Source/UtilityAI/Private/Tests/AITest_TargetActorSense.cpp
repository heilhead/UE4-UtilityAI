#include "Tests/AITest_TargetActorSense.h"
#include "Tests/AITestStatics.h"
#include "Perception/AIPerceptionComponent.h"

float
UAITest_TargetActorSense::ComputeScore(const FAIEvaluationContext& Context,
                                       const FAITarget& Target) const
{
    const auto* ActorPerception = UAITestStatics::GetAITargetPerceptionInfo(Context.Perception, Target);
    if (!ActorPerception) {
        return 0.f;
    }

    switch (TestType) {
    case EAISenseTestType::ActiveStimulusOfSense:
        return ActorPerception->IsSenseActive(UAISense::GetSenseID(SenseClass)) ? 1.f : 0.f;
    case EAISenseTestType::KnownStimulusOfSense:
        return ActorPerception->HasKnownStimulusOfSense(UAISense::GetSenseID(SenseClass)) ? 1.f : 0.f;
    case EAISenseTestType::AnyActiveStimulus:
        return ActorPerception->HasAnyCurrentStimulus() ? 1.f : 0.f;
    case EAISenseTestType::AnyKnownStimulus:
        return ActorPerception->HasAnyKnownStimulus() ? 1.f : 0.f;
    }

    return 0.f;
}
