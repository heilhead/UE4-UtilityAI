#include "EQS/EnvQueryContext_FocalPoint.h"
#include "AIController.h"
#include "AITypes.h"
#include "EnvironmentQuery/Items/EnvQueryItemType_Point.h"
#include "UtilityAILog.h"

void
UEnvQueryContext_FocalPoint::ProvideContext(FEnvQueryInstance& QueryInstance,
                                            FEnvQueryContextData& ContextData) const
{
    FVector ResultingLocation = FAISystem::InvalidLocation;

    if (const auto* OwnerPawn = Cast<APawn>(QueryInstance.Owner.Get())) {
        if (const auto* PawnController = Cast<AAIController>(OwnerPawn->GetController())) {
            ResultingLocation = PawnController->GetFocalPoint();
        } else {
            UE_LOG(LogUAI, Warning, TEXT("%s Query instance owner's controller is not `AAIController`"),
                   TEXT(__FUNCTION__));
        }
    } else {
        UE_LOG(LogUAI, Warning, TEXT("%s Query instance owner is not `APawn`"), TEXT(__FUNCTION__));
    }

    UEnvQueryItemType_Point::SetContextHelper(ContextData, ResultingLocation);
}
