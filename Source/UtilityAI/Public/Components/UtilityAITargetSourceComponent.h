#pragma once

#include "UtilityAITargetSource.h"
#include "Perception/AIPerceptionComponent.h"
#include "UtilityAITargetSourceComponent.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class UTILITYAI_API UUtilityAITargetSourceComponent
    : public UAIPerceptionComponent, public IUtilityAITargetSource {
    GENERATED_BODY()

public:
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAIPopulateTargetListSignature, TArray<FAITarget>&,
                                                TargetList);

    UPROPERTY(Transient, BlueprintReadOnly)
    FAIPopulateTargetListSignature OnPopulateTargetList;

private:
    bool bTargetListDirty{true};

    UPROPERTY(Transient)
    TArray<FAITarget> TargetList;

public:
    virtual void
    StartTargetProcessing() override;

    virtual void
    StopTargetProcessing() override;

    virtual const TArray<FAITarget>&
    GetTargetList() const override;

    virtual void
    UpdateTargetList() override;

    void
    SetAllSensesEnabled(bool bEnable);

protected:
    virtual bool
    IsValidPerceivedTarget(const FActorPerceptionInfo& PerceptionInfo) const;

private:
    UFUNCTION()
    void
    HandlePerceptionUpdate(const TArray<AActor*>& UpdatedActors);
};
