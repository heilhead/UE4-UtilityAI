#include "SResponseCurvePreviewWidget.h"

void
SResponseCurvePreviewWidget::Construct(const FArguments& InArgs)
{
}

FVector2D
SResponseCurvePreviewWidget::GetWidgetPosition(float X, float Y, const FGeometry& Geom) const
{
    return FVector2D((X * Geom.GetLocalSize().X), (Geom.GetLocalSize().Y - 1) - (Y * Geom.GetLocalSize().Y));
}

int32
SResponseCurvePreviewWidget::OnPaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry,
                                     const FSlateRect& MyCullingRect,
                                     FSlateWindowElementList& OutDrawElements, int32 LayerId,
                                     const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const
{
    const bool bEnabled = ShouldBeEnabled(bParentEnabled);
    const ESlateDrawEffect DrawEffects = bEnabled ? ESlateDrawEffect::None : ESlateDrawEffect::DisabledEffect;

    // const FSlateBrush* TimelineAreaBrush = FEditorStyle::GetBrush("Profiler.LineGraphArea");
    const FSlateBrush* WhiteBrush = FEditorStyle::GetBrush("WhiteTexture");

    const float SizeX = AllottedGeometry.Size.X;
    const float SizeY = AllottedGeometry.Size.Y;

    FSlateDrawElement::MakeBox(OutDrawElements, LayerId,
                               AllottedGeometry.ToPaintGeometry(FVector2D(0.f, 0.f), FVector2D(SizeX, SizeY)),
                               WhiteBrush, DrawEffects,
                               WhiteBrush->GetTint(InWidgetStyle) * InWidgetStyle.
                               GetColorAndOpacityTint());

    LayerId++;

    const auto GraphGridTint = InWidgetStyle.GetColorAndOpacityTint() * FLinearColor(0.8f, 0.8f, 0.8f, 1.0f);
    const auto GraphPlotTint = InWidgetStyle.GetColorAndOpacityTint() * FLinearColor(0.0f, 0.0f, 1.0f, 1.0f);

    constexpr float Padding = 5.f;
    const float GraphWidth = SizeX - Padding * 2.f;
    const float GraphHeight = SizeY - Padding * 2.f;
    const FVector2D GraphOffset(Padding);

    TArray<FVector2D> LinePoints = {
        {Padding, Padding},
        {SizeX - Padding, Padding},
        {SizeX - Padding, SizeY - Padding},
        {Padding, SizeY - Padding},
        {Padding, Padding},
    };

    FSlateDrawElement::MakeLines(OutDrawElements, LayerId, AllottedGeometry.ToPaintGeometry(),
                                 LinePoints, DrawEffects, GraphGridTint, false);
    LayerId++;

    LinePoints = {
        {SizeX * 0.5f, Padding},
        {SizeX * 0.5f, SizeY - Padding},
    };

    FSlateDrawElement::MakeLines(OutDrawElements, LayerId, AllottedGeometry.ToPaintGeometry(),
                                 LinePoints, DrawEffects, GraphGridTint, false);
    LayerId++;

    LinePoints = {
        {Padding, SizeY * 0.5f},
        {SizeX - Padding, SizeY * 0.5f},
    };

    FSlateDrawElement::MakeLines(OutDrawElements, LayerId, AllottedGeometry.ToPaintGeometry(),
                                 LinePoints, DrawEffects, GraphGridTint, false);
    LayerId++;

    LinePoints.Reset(PreviewData.NumSamples + 1);
    for (int32 Idx = 0; Idx <= PreviewData.NumSamples; Idx++) {
        const float Input = static_cast<float>(Idx) / static_cast<float>(PreviewData.NumSamples);
        const float PosX = GraphWidth * Input;
        const float PosY = GraphHeight - GraphHeight * PreviewData.Curve.ComputeValue(Input);

        LinePoints.Add(FVector2D(PosX, PosY) + GraphOffset);
    }

    FSlateDrawElement::MakeLines(OutDrawElements, LayerId, AllottedGeometry.ToPaintGeometry(),
                                 LinePoints, DrawEffects, GraphPlotTint);

    LayerId++;

    return LayerId;
}

FVector2D
SResponseCurvePreviewWidget::ComputeDesiredSize(float) const
{
    return FVector2D(92.f, 92.f);
}
