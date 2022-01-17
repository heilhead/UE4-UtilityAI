#include "Influence/AIInfluenceOctree.h"

FAIInfluenceOctree::FAIInfluenceOctree(const FVector& Origin, float Radius)
    : TOctree2<FAIInfluenceOctreeElement, FAIInfluenceOctreeSemantics>(Origin, Radius)
{
}

FAIInfluenceOctree::~FAIInfluenceOctree()
{
    ObjectToOctreeId.Empty();
}

void
FAIInfluenceOctree::AddNode(UObject& NodeOwner, const FBox& Bounds, const FAIInfluenceChannelData ChannelData)
{
    check(!ObjectToOctreeId.Find(HashObject(NodeOwner)));

    FAIInfluenceOctreeElement Element(NodeOwner, ChannelData);
    Element.Bounds = Bounds;
    AddElement(Element);
}

void
FAIInfluenceOctree::UpdateNode(const FElementID ID, const FBox& Bounds)
{
    FAIInfluenceOctreeElement ElementCopy = GetElementById(ID);
    RemoveElement(ID);
    ElementCopy.Bounds = Bounds;
    AddElement(ElementCopy);
}

void
FAIInfluenceOctree::RemoveNode(const FElementID ID)
{
    const auto& Element = GetElementById(ID);
    const int32 OwnerID = Element.OwnerUniqueId;

    check(ObjectToOctreeId.Find(OwnerID));

    RemoveElement(ID);
    ObjectToOctreeId.Remove(OwnerID);
}

const FAIInfluenceOctree::FDataRef&
FAIInfluenceOctree::GetNodeData(const FElementID ID) const
{
    return GetElementById(ID).Data;
}

FAIInfluenceOctree::FElementID
FAIInfluenceOctree::FindElementId(const UObject& OwnerObject) const
{
    if (const auto* Result = ObjectToOctreeId.Find(HashObject(OwnerObject))) {
        return *Result;
    }

    return FElementID();
}

FAIInfluenceQueryResult
FAIInfluenceOctree::RunQuery(const FAIInfluenceQueryParams& Params) const
{
    FAIInfluenceQueryResult Result;

    FindElementsWithBoundsTest(Params.Bounds, [&](const auto& Element) {
        const auto& Data = Element.GetData();
        const uint8 InfluenceValue = Data->ChannelData.GetChannelData(Params.Channel);
        if (InfluenceValue < Params.MinInfluence || InfluenceValue > Params.MaxInfluence) {
            return;
        }

        Result.NumNodes++;
        Result.CombinedInfluence += InfluenceValue;

        if (Params.bIncludeNodeOwners) {
            Result.NodeOwners.Add(Data->SourceObject);
        }
    });

    return Result;
}
