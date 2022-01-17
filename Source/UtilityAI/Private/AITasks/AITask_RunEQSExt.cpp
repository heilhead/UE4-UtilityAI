#include "AITasks/AITask_RunEQSExt.h"
#include "AIController.h"
#include "BrainComponent.h"
#include "EnvironmentQuery/EnvQueryManager.h"
#include "EnvironmentQuery/Items/EnvQueryItemType_ActorBase.h"

bool
UEnvQueryInstanceBlueprintWrapperExt::GetBestResultLocation(FVector& ResultLocation)
{
    if (QueryResult.IsValid() && (QueryResult->GetRawStatus() == EEnvQueryStatus::Success)) {
        ResultLocation = QueryResult->GetItemAsLocation(0);
        return true;
    }

    return false;
}

bool
UEnvQueryInstanceBlueprintWrapperExt::GetBestResultActor(AActor*& ResultActor)
{
    if (QueryResult.IsValid() &&
        (QueryResult->GetRawStatus() == EEnvQueryStatus::Success) &&
        (ItemType.Get() != nullptr) &&
        ItemType->IsChildOf(UEnvQueryItemType_ActorBase::StaticClass())) {
        ResultActor = QueryResult->GetItemAsActor(0);
        return true;
    }

    return false;
}

UAITask_RunEQSExt*
UAITask_RunEQSExt::RunEQSExt(AAIController* Controller, UEnvQuery* QueryTemplate,
                             TEnumAsByte<EEnvQueryRunMode::Type> RunMode, bool bLockAILogic)
{
    if (QueryTemplate == nullptr || Controller == nullptr) {
        return nullptr;
    }

    UAITask_RunEQSExt* MyTask = UAITask::NewAITask<UAITask_RunEQSExt>(*Controller, EAITaskPriority::High);
    if (MyTask) {
        FEQSExtTaskRequestSpec Spec;
        Spec.QueryTemplate = QueryTemplate;
        Spec.RunMode = RunMode;

        MyTask->RequestSpec = Spec;

        if (bLockAILogic) {
            MyTask->RequestAILogicLocking();
        }
    }

    return MyTask;
}

void
UAITask_RunEQSExt::Activate()
{
    Super::Activate();

    const auto* Controller = GetAIController();
    auto* Pawn = Controller->GetPawn();

    QueryInstance = CastChecked<UEnvQueryInstanceBlueprintWrapperExt>(UEnvQueryManager::RunEQSQuery(
        Pawn, RequestSpec.QueryTemplate, Pawn, RequestSpec.RunMode,
        UEnvQueryInstanceBlueprintWrapperExt::StaticClass()));
    QueryInstance->GetOnQueryFinishedEvent().AddDynamic(this, &ThisClass::OnQueryFinished);
}

void
UAITask_RunEQSExt::OnDestroy(bool bInOwnerFinished)
{
    QueryInstance->GetOnQueryFinishedEvent().RemoveDynamic(this, &ThisClass::OnQueryFinished);

    // auto* QueryResult = QueryInstance->GetQueryResult();
    // if (!QueryResult->IsFinished()) {
    //     // TODO: Possible to abort externally?
    // }

    Super::OnDestroy(bInOwnerFinished);
}

void
UAITask_RunEQSExt::OnQueryFinished(UEnvQueryInstanceBlueprintWrapper* InQueryInstance,
                                   EEnvQueryStatus::Type QueryStatus)
{
    EndTask();

    auto* InstanceExt = CastChecked<UEnvQueryInstanceBlueprintWrapperExt>(InQueryInstance);

    if (QueryStatus == EEnvQueryStatus::Success) {
        OnRequestFinished.Broadcast(InstanceExt);
    } else {
        OnRequestFailed.Broadcast(InstanceExt);
    }
}
