#pragma once

#include "AIInfluenceOctree.h"
#include "AIInfluenceSubsystem.generated.h"

UCLASS()
class UTILITYAI_API UAIInfluenceSubsystem : public UTickableWorldSubsystem {
    GENERATED_BODY()

public:
    TSharedPtr<FAIInfluenceOctree, ESPMode::ThreadSafe> InfluenceOctree;

public:
    virtual void
    Initialize(FSubsystemCollectionBase& Collection) override;

    virtual void
    Deinitialize() override;

    virtual void
    Tick(float DeltaTime) override;

    virtual TStatId
    GetStatId() const override;

    virtual bool
    DoesSupportWorldType(EWorldType::Type WorldType) const override;

    void
    AddNode(UObject& OwnerObject, const FBox& Bounds,
            const FAIInfluenceChannelData ChannelData = FAIInfluenceChannelData()) const;

    void
    RemoveNode(const FAIInfluenceID ID) const;

    void
    UpdateNodeBounds(const FAIInfluenceID ID, const FBox& Bounds) const;

    void
    UpdateNodeChannelData(const FAIInfluenceID ID, const FAIInfluenceChannelData ChannelData) const;

    FAIInfluenceQueryResult
    RunQuery(const FAIInfluenceQueryParams& Params) const;
};
