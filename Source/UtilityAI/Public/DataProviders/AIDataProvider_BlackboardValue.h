#pragma once

#include "DataProviders/AIDataProvider.h"
#include "AIDataProvider_BlackboardValue.generated.h"

UCLASS(BlueprintType, EditInlineNew, meta=(DisplayName="Blackboard Value"))
class UTILITYAI_API UAIDataProvider_BlackboardValue : public UAIDataProvider {
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, Category = "Provider")
    FName KeyName;

    UPROPERTY()
    float FloatValue{0.f};

    UPROPERTY()
    int32 IntValue{0};

    UPROPERTY()
    bool BoolValue{false};

public:
    virtual void
    BindData(const UObject& Owner, int32 RequestId) override;

    virtual FString
    ToString(FName PropName) const override;
};
