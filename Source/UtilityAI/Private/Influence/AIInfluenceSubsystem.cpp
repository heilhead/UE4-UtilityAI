#include "Influence/AIInfluenceSubsystem.h"
#include "DrawDebugHelpers.h"

void
UAIInfluenceSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
    Super::Initialize(Collection);

    InfluenceOctree = MakeShareable(new FAIInfluenceOctree(FVector::ZeroVector, 64000.f));
}

void
UAIInfluenceSubsystem::Deinitialize()
{
    InfluenceOctree->Destroy();
    InfluenceOctree = nullptr;

    Super::Deinitialize();
}

// void
// UAIInfluenceSubsystem::Tick(float DeltaTime)
// {
//     if (InfluenceOctree) {
//         const UWorld* World = GetWorld();
//         InfluenceOctree->FindAllElements([=](const auto& Element) {
//             const FBox Box = Element.Bounds.GetBox();
//             DrawDebugBox(World, Box.GetCenter(), Box.GetExtent(), FColor::Green, false, -1.f, 0, 5.f);
//         });
//     }
// }

// TStatId
// UAIInfluenceSubsystem::GetStatId() const
// {
//     RETURN_QUICK_DECLARE_CYCLE_STAT(UAIInfluenceSubsystem, STATGROUP_Tickables);
// }

bool
UAIInfluenceSubsystem::DoesSupportWorldType(EWorldType::Type WorldType) const
{
    return WorldType == EWorldType::Game || WorldType == EWorldType::PIE;
}

void
UAIInfluenceSubsystem::AddNode(UObject& OwnerObject, const FBox& Bounds,
                               const FAIInfluenceChannelData ChannelData) const
{
    return InfluenceOctree->AddNode(OwnerObject, Bounds, ChannelData);
}

void
UAIInfluenceSubsystem::RemoveNode(const FAIInfluenceID ID) const
{
    InfluenceOctree->RemoveNode(ID);
}

void
UAIInfluenceSubsystem::UpdateNodeBounds(const FAIInfluenceID ID, const FBox& Bounds) const
{
    InfluenceOctree->UpdateNode(ID, Bounds);
}

void
UAIInfluenceSubsystem::UpdateNodeChannelData(const FAIInfluenceID ID,
                                             const FAIInfluenceChannelData ChannelData) const
{
    auto& Data = InfluenceOctree->GetNodeData(ID);
    Data->ChannelData = ChannelData;
}

FAIInfluenceQueryResult
UAIInfluenceSubsystem::RunQuery(const FAIInfluenceQueryParams& Params) const
{
    return InfluenceOctree->RunQuery(Params);
}
