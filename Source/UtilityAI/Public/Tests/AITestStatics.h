#pragma once

#include "AITestStatics.generated.h"

struct FActorPerceptionInfo;
struct FAITarget;
class UAIPerceptionComponent;

UCLASS(Abstract)
class UTILITYAI_API UAITestStatics : public UBlueprintFunctionLibrary {
    GENERATED_BODY()

public:
    static const FActorPerceptionInfo*
    GetAITargetPerceptionInfo(const UAIPerceptionComponent* Perception, const FAITarget& Target);
};
