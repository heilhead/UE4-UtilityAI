#pragma once

#include "UtilityAITargetSource.generated.h"

struct FAITarget;

UINTERFACE()
class UTILITYAI_API UUtilityAITargetSource : public UInterface {
    GENERATED_BODY()
};

class UTILITYAI_API IUtilityAITargetSource {
    GENERATED_BODY()

public:
    virtual void
    StartTargetProcessing() = 0;

    virtual void
    StopTargetProcessing() = 0;

    virtual void
    UpdateTargetList() = 0;

    virtual const TArray<FAITarget>&
    GetTargetList() const = 0;
};
