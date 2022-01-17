#pragma once

#include "EnvironmentQuery/EnvQueryContext.h"
#include "EnvQueryContext_FocalPoint.generated.h"

UCLASS()
class UTILITYAI_API UEnvQueryContext_FocalPoint : public UEnvQueryContext {
    GENERATED_BODY()

public:
    virtual void
    ProvideContext(FEnvQueryInstance& QueryInstance, FEnvQueryContextData& ContextData) const override;
};
