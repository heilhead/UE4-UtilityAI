#include "DataProviders/AIDataProvider_GameplayAttribute.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "AIController.h"

void
UAIDataProvider_GameplayAttribute::BindData(const UObject& Owner, int32 RequestId)
{
    if (const auto* Controller = Cast<AAIController>(&Owner)) {
        const auto* ASC =
            UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(Controller->GetPawn());
        if (ASC) {
            FloatValue = ASC->GetNumericAttribute(Attribute);
            IntValue = static_cast<int32>(FloatValue);
            BoolValue = FloatValue != 0.f;
        }
    }
}

FString
UAIDataProvider_GameplayAttribute::ToString(FName PropName) const
{
    return FString::Printf(TEXT("GameplayAttribute.%s"), *Attribute.GetName());
}
