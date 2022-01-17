#include "Tests/AITest_Identity.h"
#include "UtilityAIStatics.h"

UAITest_Identity::UAITest_Identity()
{
    ReturnValue.DefaultValue = 1.f;
}

float
UAITest_Identity::ComputeScore(const FAIEvaluationContext& Context, const FAITarget& Target) const
{
    return UUtilityAIStatics::GetAIDataProviderFloatValue_AnyThread(Context.Controller, ReturnValue);
}
