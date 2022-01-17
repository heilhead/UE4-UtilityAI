#include "EQS/EnvQueryContext_FocusActor.h"
#include "AIController.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "EnvironmentQuery/Items/EnvQueryItemType_Actor.h"
#include "UtilityAILog.h"

void
UEnvQueryContext_FocusActor::ProvideContext(FEnvQueryInstance& QueryInstance,
                                            FEnvQueryContextData& ContextData) const
{
    const AActor* Result = nullptr;

    if (const auto* OwnerPawn = Cast<APawn>(QueryInstance.Owner.Get())) {
        if (const auto* PawnController = Cast<AAIController>(OwnerPawn->GetController())) {
            Result = PawnController->GetFocusActor();
        } else {
            UE_LOG(LogUAI, Warning, TEXT("%s Query instance owner's controller is not `AAIController`"),
                   TEXT(__FUNCTION__));
        }
    } else {
        UE_LOG(LogUAI, Warning, TEXT("%s Query instance owner is not `APawn`"), TEXT(__FUNCTION__));
    }

    UEnvQueryItemType_Actor::SetContextHelper(ContextData, Result);
}
