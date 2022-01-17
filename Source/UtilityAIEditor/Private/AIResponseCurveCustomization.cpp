#include "AIResponseCurveCustomization.h"
#include "DetailLayoutBuilder.h"
#include "DetailWidgetRow.h"
#include "IDetailChildrenBuilder.h"
#include "IDetailGroup.h"
#include "SResponseCurvePreviewWidget.h"
#include "UtilityAITest.h"

#define LOCTEXT_NAMESPACE "UtilityAIEditor"

#define GET_CHILD_PROPERTY_HANDLE(InPropertyHandle, OutputPropertyHandle, PropertyClass, PropertyName) \
const TSharedPtr<IPropertyHandle> OutputPropertyHandle = InPropertyHandle->GetChildHandle(GET_MEMBER_NAME_CHECKED(PropertyClass, PropertyName)); \
check(OutputPropertyHandle.IsValid()); \
check(OutputPropertyHandle->IsValidHandle());

namespace {
    // @formatter:off
    const TArray<FAIResponseCurveCustomization::FCurvePreset> CurvePresetList = {
        { LOCTEXT("CurvePresetConstant", "Constant"), EAIResponseCurveType::Linear, 0.0f, 0.0f, 0.0f, 0.5 },
        { LOCTEXT("CurvePresetBasicLinear", "Basic linear"), EAIResponseCurveType::Linear, 1.0f, 0.0f, 0.0f, 0.0f },
        { LOCTEXT("CurvePresetInverseLinear", "Inverse linear"), EAIResponseCurveType::Linear, -1.0f, 0.0f, 0.0f, 1.0f },

        { LOCTEXT("CurvePresetStandardCooldown", "Standard cooldown"), EAIResponseCurveType::Polynomial, 1.0f, 6.0f, 0.0f, 0.0f },
        { LOCTEXT("CurvePresetStandardRuntime", "Standard runtime"), EAIResponseCurveType::Polynomial, -1.0f, 6.0f, 0.0f, 1.0f },
        { LOCTEXT("CurvePresetBasicQuadraticLowerLeft", "Basic quadric lower left"), EAIResponseCurveType::Polynomial, 1.0f, 4.0f, 1.0f, 0.0f },
        { LOCTEXT("CurvePresetBasicQuadraticLowerRight", "Basic quadric lower right"), EAIResponseCurveType::Polynomial, 1.0f, 4.0f, 0.0f, 0.0f },
        { LOCTEXT("CurvePresetBasicQuadraticUpperLeft", "Basic quadric upper left"), EAIResponseCurveType::Polynomial, -1.0f, 4.0f, 1.0f, 1.0f },
        { LOCTEXT("CurvePresetBasicQuadraticUpperRight", "Basic quadric upper right"), EAIResponseCurveType::Polynomial, -1.0f, 4.0f, 0.0f, 1.0f },

        { LOCTEXT("CurvePresetBasicLogistic", "Basic logistic"), EAIResponseCurveType::Logistic, 1.0f, 1.0f, 0.0f, 0.0f },
        { LOCTEXT("CurvePresetInverseLogistic", "Inverse logistic"), EAIResponseCurveType::Logistic, -1.0f, 1.0f, 0.0f, 1.0f },

        { LOCTEXT("CurvePresetBasicLogit", "Basic logit"), EAIResponseCurveType::Logit, 1.0f, 1.0f, 0.0f, 0.0f },
        { LOCTEXT("CurvePresetInverseLogit", "Inverse logit"), EAIResponseCurveType::Logit, -1.0f, 1.0f, 0.0f, 0.0f },

        { LOCTEXT("CurvePresetBasicBell", "Basic bell curve"), EAIResponseCurveType::Normal, 1.0f, 1.0f, 0.0f, 0.0f },
        { LOCTEXT("CurvePresetInverseBell", "Inverse bell curve"), EAIResponseCurveType::Normal, -1.0f, 1.0f, 0.0f, 1.0f },

        { LOCTEXT("CurvePresetBasicSine", "Basic sine wave"), EAIResponseCurveType::Sine, 1.0f, 1.0f, 0.0f, 0.0f },
        { LOCTEXT("CurvePresetInverseSine", "Inverse sine wave"), EAIResponseCurveType::Sine, -1.0f, 1.0f, 0.0f, 0.0f },
    };
    // @formatter:on

    const FAIResponseCurveCustomization::FCurvePreset*
    MatchPreset(const FAIResponseCurve& Data)
    {
        for (const auto& Preset : CurvePresetList) {
            if (Preset == Data) {
                return &Preset;
            }
        }

        return nullptr;
    }
}

FAIResponseCurveCustomization::FCurvePreset::FCurvePreset(const FText& InLabel,
                                                          EAIResponseCurveType
                                                          CurveType, float Slope, float Exponent,
                                                          float XShift, float YShift)
    : Label(InLabel)
{
    CurveData.Type = CurveType;
    CurveData.Slope = Slope;
    CurveData.Exponent = Exponent;
    CurveData.XShift = XShift;
    CurveData.YShift = YShift;
}

bool
FAIResponseCurveCustomization::FCurvePreset::operator==(
    const FAIResponseCurve& Other) const
{
    return CurveData.Type == Other.Type && CurveData.Slope == Other.Slope && CurveData.Exponent == Other.
           Exponent && CurveData.XShift == Other.XShift && CurveData.YShift == Other.YShift;
}

TSharedRef<IPropertyTypeCustomization>
FAIResponseCurveCustomization::MakeInstance()
{
    return MakeShareable(new FAIResponseCurveCustomization());
}

void
FAIResponseCurveCustomization::CustomizeHeader(TSharedRef<IPropertyHandle> StructPropertyHandle,
                                               FDetailWidgetRow& HeaderRow,
                                               IPropertyTypeCustomizationUtils&
                                               StructCustomizationUtils)
{
    RootPropertyHandle = StructPropertyHandle;

    StructPropertyHandle->SetOnChildPropertyValueChanged(
        FSimpleDelegate::CreateSP(this, &FAIResponseCurveCustomization::OnChanged));

    HeaderRow.NameContent()
    [
        StructPropertyHandle->CreatePropertyNameWidget()
    ];
}

void
FAIResponseCurveCustomization::CustomizeChildren(TSharedRef<IPropertyHandle> StructPropertyHandle,
                                                 IDetailChildrenBuilder& StructBuilder,
                                                 IPropertyTypeCustomizationUtils&
                                                 StructCustomizationUtils)
{
    GET_CHILD_PROPERTY_HANDLE(StructPropertyHandle, TypeHandle, FAIResponseCurve, Type);
    GET_CHILD_PROPERTY_HANDLE(StructPropertyHandle, SlopeHandle, FAIResponseCurve, Slope);
    GET_CHILD_PROPERTY_HANDLE(StructPropertyHandle, ExponentHandle, FAIResponseCurve, Exponent);
    GET_CHILD_PROPERTY_HANDLE(StructPropertyHandle, XShiftHandle, FAIResponseCurve, XShift);
    GET_CHILD_PROPERTY_HANDLE(StructPropertyHandle, YShiftHandle, FAIResponseCurve, YShift);

    auto& CurvePresetRow = StructBuilder.AddCustomRow(LOCTEXT("CurvePresetRow", "Curve Preset"));
    CurvePresetRow
        .NameContent()
        [
            SNew(STextBlock)
                .Text(LOCTEXT("CurvePreset", "Preset"))
                .Font(IDetailLayoutBuilder::GetDetailFont())
        ]
        .ValueContent()
        [
            SNew(SComboButton)
                    .OnGetMenuContent(
                                  this, &FAIResponseCurveCustomization::OnGetPresetListContent)
                    .ContentPadding(FMargin(2.0f, 2.0f))
                    .ButtonContent()
            [
                SNew(STextBlock)
                    .Text(MakeAttributeLambda([=] {
                                    return GetPresetButtonDesc(StructPropertyHandle);
                                }))
                    .Font(IDetailLayoutBuilder::GetDetailFont())
            ]
        ];

    auto& PreviewRow = StructBuilder.AddCustomRow(LOCTEXT("PreviewRow", "Preview"));
    PreviewRow
        .NameContent()
        .VAlign(VAlign_Top)
        [
            SNew(STextBlock)
                .Text(LOCTEXT("PreviewDesc",
                              "X-axis is the input score returned by the test, clamped and normalized (0 to 1 range). "
                              "Y-axis is the final score representing how close to optimal (a value of 1) is the "
                              "behavior in current circumstances. The value of 0 means that the behavior is invalid "
                              "and should not be selected."))
                .WrappingPolicy(ETextWrappingPolicy::DefaultWrapping)
                .AutoWrapText(true)
                .Font(IDetailLayoutBuilder::GetDetailFont())
        ]
        .ValueContent()
        [
            SAssignNew(PreviewWidget, SResponseCurvePreviewWidget)
        ];

    auto& AdvancedGroup = StructBuilder.
        AddGroup("Customize", LOCTEXT("CustomizeResponseCurveLabel", "Customize"));
    AdvancedGroup.AddPropertyRow(TypeHandle.ToSharedRef());
    AdvancedGroup.AddPropertyRow(SlopeHandle.ToSharedRef());
    AdvancedGroup.AddPropertyRow(ExponentHandle.ToSharedRef());
    AdvancedGroup.AddPropertyRow(XShiftHandle.ToSharedRef());
    AdvancedGroup.AddPropertyRow(YShiftHandle.ToSharedRef());

    UpdatePreviewWidget();
}

TSharedRef<SWidget>
FAIResponseCurveCustomization::OnGetPresetListContent()
{
    FMenuBuilder MenuBuilder(true, nullptr);

    for (int32 Index = 0; Index < CurvePresetList.Num(); Index++) {
        FUIAction ItemAction(FExecuteAction::CreateSP(
            this, &FAIResponseCurveCustomization::OnCurvePresetChanged, Index));
        MenuBuilder.AddMenuEntry(CurvePresetList[Index].Label, TAttribute<FText>(), FSlateIcon(),
                                 ItemAction);
    }

    return MenuBuilder.MakeWidget();
}

void
FAIResponseCurveCustomization::OnChanged()
{
    UpdatePreviewWidget();
}

void
FAIResponseCurveCustomization::OnCurvePresetChanged(const int32 PresetIndex)
{
    GET_CHILD_PROPERTY_HANDLE(RootPropertyHandle, TypeHandle, FAIResponseCurve, Type);
    GET_CHILD_PROPERTY_HANDLE(RootPropertyHandle, SlopeHandle, FAIResponseCurve, Slope);
    GET_CHILD_PROPERTY_HANDLE(RootPropertyHandle, ExponentHandle, FAIResponseCurve, Exponent);
    GET_CHILD_PROPERTY_HANDLE(RootPropertyHandle, XShiftHandle, FAIResponseCurve, XShift);
    GET_CHILD_PROPERTY_HANDLE(RootPropertyHandle, YShiftHandle, FAIResponseCurve, YShift);

    const auto& Preset = CurvePresetList[PresetIndex];

    TypeHandle->SetValue(static_cast<uint8>(Preset.CurveData.Type));
    SlopeHandle->SetValue(Preset.CurveData.Slope);
    ExponentHandle->SetValue(Preset.CurveData.Exponent);
    XShiftHandle->SetValue(Preset.CurveData.XShift);
    YShiftHandle->SetValue(Preset.CurveData.YShift);
}

FText
FAIResponseCurveCustomization::GetPresetButtonDesc(
    TSharedPtr<IPropertyHandle> StructPropertyHandle)
{
    TArray<const void*> RawData;
    StructPropertyHandle->AccessRawData(RawData);

    if (!RawData.Num()) {
        return FText::GetEmpty();
    }

    bool bFirst = true;
    const FCurvePreset* PresetFound = nullptr;

    for (const auto* RawDataPtr : RawData) {
        check(RawDataPtr);

        const auto* Data = static_cast<const FAIResponseCurve*>(RawDataPtr);
        const auto* Preset = MatchPreset(*Data);

        if (bFirst) {
            bFirst = false;

            if (Preset) {
                PresetFound = Preset;
            }
        } else if (Preset != PresetFound) {
            return FText::GetEmpty();
        }
    }

    if (PresetFound) {
        return PresetFound->Label;
    }

    return LOCTEXT("CurvePresetCustom", "Custom");
}

void
FAIResponseCurveCustomization::UpdatePreviewWidget()
{
    TArray<const void*> RawData;
    RootPropertyHandle->AccessRawData(RawData);
    PreviewWidget->PreviewData.Curve = *static_cast<const FAIResponseCurve*>(RawData[0]);
    PreviewWidget->Invalidate(EInvalidateWidgetReason::Paint);
}

#undef LOCTEXT_NAMESPACE
