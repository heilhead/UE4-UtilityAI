#include "Tests/AITest_AgentHealthNormalized.h"
#include "AbilitySystemGlobals.h"
#include "AbilitySystemComponent.h"
#include "AIController.h"

float
UAITest_AgentHealthNormalized::ComputeScore(const FAIEvaluationContext& Context,
                                            const FAITarget& Target) const
{
    const auto* SourceActor = Context.Controller->GetPawn();
    const auto* ASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(SourceActor);
    if (ASC) {
        const float CurrentHealth = ASC->GetNumericAttribute(CurrentHealthAttribute);
        const float MaxHealth = ASC->GetNumericAttribute(MaxHealthAttribute);

        return CurrentHealth / MaxHealth;
    }

    return 0.f;
}
