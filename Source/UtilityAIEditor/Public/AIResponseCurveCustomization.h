#pragma once

#include "UtilityAITest.h"
#include "PropertyEditor/Public/IPropertyTypeCustomization.h"

class SResponseCurvePreviewWidget;

class FAIResponseCurveCustomization : public IPropertyTypeCustomization {
protected:
    FText TypeText;

    TSharedPtr<IPropertyHandle> RootPropertyHandle;
    TSharedPtr<SResponseCurvePreviewWidget> PreviewWidget;

public:
    struct FCurvePreset {
        FText Label;
        FAIResponseCurve CurveData;

        FCurvePreset(const FText& InLabel, EAIResponseCurveType CurveType, float Slope,
                     float Exponent, float XShift, float YShift);

        bool
        operator==(const FAIResponseCurve& Other) const;
    };

    static TSharedRef<IPropertyTypeCustomization>
    MakeInstance();

    virtual void
    CustomizeHeader(TSharedRef<IPropertyHandle> StructPropertyHandle, FDetailWidgetRow& HeaderRow,
                    IPropertyTypeCustomizationUtils& StructCustomizationUtils) override;

    virtual void
    CustomizeChildren(TSharedRef<IPropertyHandle> StructPropertyHandle, IDetailChildrenBuilder& StructBuilder,
                      IPropertyTypeCustomizationUtils& StructCustomizationUtils) override;

protected:
    TSharedRef<SWidget>
    OnGetPresetListContent();

    void
    OnChanged();

    void
    OnCurvePresetChanged(const int32 PresetIndex);

    FText
    GetPresetButtonDesc(TSharedPtr<IPropertyHandle> StructPropertyHandle);

    void
    UpdatePreviewWidget();
};
