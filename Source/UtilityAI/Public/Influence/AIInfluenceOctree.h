#pragma once

enum class EAIInfluenceChannel {
    Channel0 = 0,
    Channel1 = 1,
    Channel2 = 2,
    Channel3 = 3,
    Channel4 = 4,
    Channel5 = 5,
    Channel6 = 6,
    Channel7 = 7,
};

struct FAIInfluenceQueryParams {
    FBox Bounds{};
    EAIInfluenceChannel Channel{EAIInfluenceChannel::Channel0};
    uint8 MinInfluence{0};
    uint8 MaxInfluence{255};
    bool bIncludeNodeOwners{false};
};

struct FAIInfluenceQueryResult {
    TArray<TWeakObjectPtr<UObject>> NodeOwners;
    int32 NumNodes{0};
    int32 CombinedInfluence{0};
};

struct FAIInfluenceChannelData {
private:
    uint8 ChannelData[8];

public:
    FAIInfluenceChannelData() = default;

    FAIInfluenceChannelData(const FAIInfluenceChannelData& Other) = default;

    FAIInfluenceChannelData&
    operator=(const FAIInfluenceChannelData& Other) = default;

    FORCEINLINE void
    SetChannelData(const EAIInfluenceChannel Channel, const uint8 Data)
    {
        ChannelData[static_cast<int32>(Channel)] = Data;
    }

    FORCEINLINE uint8
    GetChannelData(const EAIInfluenceChannel Channel) const
    {
        return ChannelData[static_cast<int32>(Channel)];
    }
};

struct UTILITYAI_API FAIInfluenceData {
    TWeakObjectPtr<UObject> SourceObject;
    FAIInfluenceChannelData ChannelData;

    FAIInfluenceData(UObject& InSourceObject, FAIInfluenceChannelData InChannelData)
        : SourceObject(&InSourceObject), ChannelData(InChannelData)
    {
    }
};

struct UTILITYAI_API FAIInfluenceOctreeElement {
    using FDataRef = TSharedRef<FAIInfluenceData, ESPMode::ThreadSafe>;

    FBoxSphereBounds Bounds;
    FDataRef Data;
    uint32 OwnerUniqueId = INDEX_NONE;

public:
    explicit
    FAIInfluenceOctreeElement(UObject& SourceObject, FAIInfluenceChannelData ChannelData)
        : Data(new FAIInfluenceData(SourceObject, ChannelData)), OwnerUniqueId(SourceObject.GetUniqueID())
    {
    }

    FAIInfluenceOctreeElement(const FAIInfluenceOctreeElement& Other)
        : Bounds(Other.Bounds), Data(Other.Data), OwnerUniqueId(Other.OwnerUniqueId)
    {
    }

    FORCEINLINE FAIInfluenceOctreeElement&
    operator=(const FAIInfluenceOctreeElement& Other)
    {
        new(this) FAIInfluenceOctreeElement(Other);
        return *this;
    }

    FORCEINLINE UObject*
    GetOwner(bool bEvenIfPendingKill = false) const
    {
        return Data->SourceObject.Get(bEvenIfPendingKill);
    }

    FORCEINLINE const FDataRef&
    GetData() const
    {
        return Data;
    }
};

struct FAIInfluenceOctreeSemantics {
    typedef TOctree2<FAIInfluenceOctreeElement, FAIInfluenceOctreeSemantics> FOctree;

    enum { MaxElementsPerLeaf = 16 };

    enum { MinInclusiveElementsPerNode = 7 };

    enum { MaxNodeDepth = 12 };

    typedef TInlineAllocator<MaxElementsPerLeaf> ElementAllocator;

    FORCEINLINE static const FBoxSphereBounds&
    GetBoundingBox(const FAIInfluenceOctreeElement& NavData)
    {
        return NavData.Bounds;
    }

    FORCEINLINE static bool
    AreElementsEqual(const FAIInfluenceOctreeElement& A, const FAIInfluenceOctreeElement& B)
    {
        return A.Data->SourceObject == B.Data->SourceObject;
    }

    FORCEINLINE static void
    ApplyOffset(FAIInfluenceOctreeElement& Element, const FVector& InOffset)
    {
        ensureMsgf(false, TEXT("Not implemented yet"));
    }

    static void
    SetElementId(FOctree& OctreeOwner, const FAIInfluenceOctreeElement& Element, FOctreeElementId2 Id);
};

class UTILITYAI_API FAIInfluenceOctree
    : public TOctree2<FAIInfluenceOctreeElement, FAIInfluenceOctreeSemantics> {

    friend struct FAIInfluenceOctreeSemantics;

public:
    using FElementID = FOctreeElementId2;
    using FDataRef = FAIInfluenceOctreeElement::FDataRef;

    FAIInfluenceOctree(const FVector& Origin, float Radius);

    ~FAIInfluenceOctree();

private:
    TMap<uint32, FElementID> ObjectToOctreeId;

public:
    void
    AddNode(UObject& NodeOwner, const FBox& Bounds, const FAIInfluenceChannelData ChannelData);

    void
    UpdateNode(const FElementID ID, const FBox& Bounds);

    void
    RemoveNode(const FElementID ID);

    const FDataRef&
    GetNodeData(const FElementID ID) const;

    FElementID
    FindElementId(const UObject& OwnerObject) const;

    FAIInfluenceQueryResult
    RunQuery(const FAIInfluenceQueryParams& Params) const;

    FORCEINLINE static uint32
    HashObject(const UObject& Object)
    {
        return Object.GetUniqueID();
    }

private:
    FORCEINLINE void
    SetElementIdImpl(const uint32 OwnerUniqueId, FElementID Id)
    {
        ObjectToOctreeId.Add(OwnerUniqueId, Id);
    }
};

FORCEINLINE void
FAIInfluenceOctreeSemantics::SetElementId(FOctree& OctreeOwner, const FAIInfluenceOctreeElement& Element,
                                          FOctreeElementId2 Id)
{
    static_cast<FAIInfluenceOctree&>(OctreeOwner).SetElementIdImpl(Element.OwnerUniqueId, Id);
}

using FAIInfluenceID = FAIInfluenceOctree::FElementID;
