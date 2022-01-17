#include "Components/UtilityAITargetSourceComponent.h"
#include "UtilityAITypes.h"
#include "Perception/AISenseConfig.h"

void
UUtilityAITargetSourceComponent::StartTargetProcessing()
{
    bTargetListDirty = true;
    OnPerceptionUpdated.AddDynamic(this, &ThisClass::HandlePerceptionUpdate);
}

void
UUtilityAITargetSourceComponent::StopTargetProcessing()
{
    TargetList.Reset();
    OnPerceptionUpdated.RemoveDynamic(this, &ThisClass::HandlePerceptionUpdate);
}

const TArray<FAITarget>&
UUtilityAITargetSourceComponent::GetTargetList() const
{
    return TargetList;
}

void
UUtilityAITargetSourceComponent::UpdateTargetList()
{
    if (!bTargetListDirty) {
        return;
    }

    // For testing purposes. Adds the local player.
    // if (auto* LocalPlayer = GetWorld()->GetFirstLocalPlayerFromController()) {
    //     if (auto* PlayerController = LocalPlayer->GetPlayerController(GetWorld())) {
    //         if (auto* Pawn = PlayerController->GetPawn()) {
    //             TargetList.Add(FAITarget(Pawn));
    //         }
    //     }
    // }

    // For testing purposes. Adds the `FocusActor` of this agent.
    // auto* FocusActor = GetAIOwner()->GetFocusActor();
    // if (FocusActor) {
    //     TargetList.Add(FAITarget(FocusActor));
    // }

    TargetList.Reset(GetPerceptualData().Num());

    for (auto DataIt = GetPerceptualDataConstIterator(); DataIt; ++DataIt) {
        const FActorPerceptionInfo& ActorPerceptionInfo = DataIt->Value;
        if (IsValidPerceivedTarget(ActorPerceptionInfo)) {
            auto* Actor = ActorPerceptionInfo.Target.Get();
            if (IsValid(Actor)) {
                TargetList.Add(FAITarget(Actor));
            }
        }
    }

    OnPopulateTargetList.Broadcast(TargetList);
    bTargetListDirty = false;
}

bool
UUtilityAITargetSourceComponent::IsValidPerceivedTarget(
    const FActorPerceptionInfo& PerceptionInfo) const
{
    return true;
}

void
UUtilityAITargetSourceComponent::SetAllSensesEnabled(bool bEnable)
{
    for (auto It = GetSensesConfigIterator(); It; ++It) {
        const auto* SenseConfig = *It;
        SetSenseEnabled(SenseConfig->GetSenseImplementation(), bEnable);
    }
}

void
UUtilityAITargetSourceComponent::HandlePerceptionUpdate(const TArray<AActor*>& UpdatedActors)
{
    bTargetListDirty = true;
}
