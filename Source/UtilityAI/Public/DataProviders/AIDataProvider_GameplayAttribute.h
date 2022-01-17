#pragma once

#include "AttributeSet.h"
#include "DataProviders/AIDataProvider.h"
#include "AIDataProvider_GameplayAttribute.generated.h"

UCLASS(BlueprintType, EditInlineNew, meta=(DisplayName="Gameplay Attribute"))
class UTILITYAI_API UAIDataProvider_GameplayAttribute : public UAIDataProvider {
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, Category = "Provider")
    FGameplayAttribute Attribute;

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
