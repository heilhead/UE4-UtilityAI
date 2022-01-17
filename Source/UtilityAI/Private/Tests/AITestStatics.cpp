#include "Tests/AITestStatics.h"
#include "UtilityAITypes.h"
#include "Perception/AIPerceptionComponent.h"

const FActorPerceptionInfo*
UAITestStatics::GetAITargetPerceptionInfo(const UAIPerceptionComponent* Perception, const FAITarget& Target)
{
    const auto* TargetActor = Target.GetActor();
    if (!TargetActor) {
        return nullptr;
    }

    if (!Perception) {
        return nullptr;
    }

    return Perception->GetActorInfo(*TargetActor);
}
