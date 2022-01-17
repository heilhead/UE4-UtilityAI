#include "DataProviders/AIDataProvider_BlackboardValue.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Bool.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Float.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Int.h"

void
UAIDataProvider_BlackboardValue::BindData(const UObject& Owner, int32 RequestId)
{
    const auto* Controller = Cast<AAIController>(&Owner);
    if (!Controller) {
        return;
    }

    const auto* Blackboard = Controller->GetBlackboardComponent();
    if (!Blackboard) {
        return;
    }

    const auto KeyID = Blackboard->GetKeyID(KeyName);
    if (KeyID == FBlackboard::InvalidKey) {
        return;
    }

    FloatValue = Blackboard->GetValue<UBlackboardKeyType_Float>(KeyID);
    IntValue = Blackboard->GetValue<UBlackboardKeyType_Int>(KeyID);
    BoolValue = Blackboard->GetValue<UBlackboardKeyType_Bool>(KeyID);
}

FString
UAIDataProvider_BlackboardValue::ToString(FName PropName) const
{
    return FString::Printf(TEXT("BlackboardValue.%s"), *KeyName.ToString());
}
