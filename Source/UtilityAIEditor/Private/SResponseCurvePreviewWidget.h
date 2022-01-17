#pragma once

#include "AIResponseCurveCustomization.h"
#include "Widgets/DeclarativeSyntaxSupport.h"
#include "Widgets/SLeafWidget.h"

class FPaintArgs;
class FSlateWindowElementList;

class SResponseCurvePreviewWidget : public SLeafWidget {
public:
    struct FCurvePreviewData {
        FAIResponseCurve Curve;
        int32 NumSamples{60};
    };

    FCurvePreviewData PreviewData;

public:
    SLATE_BEGIN_ARGS(SResponseCurvePreviewWidget)
        {
        }

    SLATE_END_ARGS()

    void
    Construct(const FArguments& InArgs);

    virtual FVector2D
    ComputeDesiredSize(float) const override;

    virtual int32
    OnPaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect,
            FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle,
            bool bParentEnabled) const override;

private:
    FVector2D
    GetWidgetPosition(float X, float Y, const FGeometry& Geom) const;
};
