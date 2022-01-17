#include "UtilityAITypes.h"

FAITarget FAITarget::InvalidTarget = FAITarget();

namespace {
    FORCEINLINE float
    Sanitize(float Value)
    {
        if (FMath::IsNaN(Value)) {
            return 0.f;
        }

        return FMath::Clamp(Value, 0.f, 1.f);
    }
}

float
FAIResponseCurve::ComputeValue(float Value) const
{
    switch (Type) {
    case EAIResponseCurveType::Linear:
        return Sanitize(Slope * (Value - XShift) + YShift);

    case EAIResponseCurveType::Polynomial:
        return Sanitize(Slope * FMath::Pow(Value - XShift, Exponent) + YShift);

    case EAIResponseCurveType::Logistic:
        return Sanitize(Slope / (1.f + FMath::Exp(-10.0f * Exponent * (Value - 0.5f - XShift))) + YShift);

    case EAIResponseCurveType::Logit:
        return Sanitize(
            Slope * FMath::Loge((Value - XShift) / (1.0f - (Value - XShift))) / 5.0f + 0.5f + YShift);

    case EAIResponseCurveType::Normal:
        return Sanitize(
            Slope * FMath::Exp(-30.0f * Exponent * (Value - XShift - 0.5f) * (Value - XShift - 0.5f)) +
            YShift);

    case EAIResponseCurveType::Sine:
        return Sanitize(0.5f * Slope * FMath::Sin(2.0f * PI * (Value - XShift)) + 0.5f + YShift);

    default:
        checkNoEntry();
        return 0.f;
    }
}
