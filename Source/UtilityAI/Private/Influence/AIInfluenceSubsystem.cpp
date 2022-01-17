#include "Influence/AIInfluenceSubsystem.h"

#include "DrawDebugHelpers.h"
#include "UtilityAILog.h"

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

void
UAIInfluenceSubsystem::Tick(float DeltaTime)
{
    if (InfluenceOctree) {
        const UWorld* World = GetWorld();
        InfluenceOctree->FindAllElements([=](const auto& Element) {
            const FBox Box = Element.Bounds.GetBox();
            DrawDebugBox(World, Box.GetCenter(), Box.GetExtent(), FColor::Green, false, -1.f, 0, 5.f);
        });
    }
}

TStatId
UAIInfluenceSubsystem::GetStatId() const
{
    RETURN_QUICK_DECLARE_CYCLE_STAT(UAIInfluenceSubsystem, STATGROUP_Tickables);
}

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

void
UAIInfluenceSubsystem::TestInfluenceOctree()
{
    // InfluenceOctree->Destroy();
    //
    // {
    //     FAIInfluenceOctreeElement Elem(*this);
    //     Elem.Bounds = FBox(FVector(-5.f, -5.f, 0.f), FVector(-3.f, -3.f, 1.f));
    //     InfluenceOctree->AddElement(Elem);
    // }
    //
    // {
    //     FAIInfluenceOctreeElement Elem(*this);
    //     Elem.Bounds = FBox(FVector(-4.f, -4.f, 0.f), FVector(-2.f, -2.f, 1.f));
    //     InfluenceOctree->AddElement(Elem);
    // }
    //
    // {
    //     FAIInfluenceOctreeElement Elem(*this);
    //     Elem.Bounds = FBox(FVector(5.f, 5.f, 0.f), FVector(7.f, 7.f, 1.f));
    //     InfluenceOctree->AddElement(Elem);
    // }
    //
    // {
    //     FAIInfluenceOctreeElement Elem(*this);
    //     Elem.Bounds = FBox(FVector(10.f, 10.f, 0.f), FVector(12.f, 12.f, 1.f));
    //     InfluenceOctree->AddElement(Elem);
    // }
    //
    // const auto Box = FBox(FVector(-3.5f, -3.5f, 0.f), FVector(6.f, 6.f, 1.f));
    // UE_LOG(LogUAI, Warning, TEXT("FindElementsWithBoundsTest: %s"), *Box.ToString());
    //
    // InfluenceOctree->FindElementsWithBoundsTest(Box, [](const auto& Element) {
    //     UE_LOG(LogUAI, Warning, TEXT("Found element: object = %s bounds = %s"),
    //            *GetNameSafe(Element.GetOwner()), *Element.Bounds.ToString());
    // });
}
