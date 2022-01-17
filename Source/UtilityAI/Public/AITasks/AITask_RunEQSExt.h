#pragma once

#include "Tasks/AITask.h"
#include "EnvironmentQuery/EnvQueryInstanceBlueprintWrapper.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "AITask_RunEQSExt.generated.h"

class UEnvQuery;

USTRUCT()
struct UTILITYAI_API FEQSExtTaskRequestSpec {
    GENERATED_BODY()

    UPROPERTY()
    UEnvQuery* QueryTemplate{nullptr};

    UPROPERTY()
    TEnumAsByte<EEnvQueryRunMode::Type> RunMode{};

    FEQSExtTaskRequestSpec() = default;
};

UCLASS()
class UTILITYAI_API UEnvQueryInstanceBlueprintWrapperExt : public UEnvQueryInstanceBlueprintWrapper {
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "AI|EQS")
    bool
    GetBestResultLocation(FVector& ResultLocation);

    UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "AI|EQS")
    bool
    GetBestResultActor(AActor*& ResultActor);
};

UCLASS()
class UTILITYAI_API UAITask_RunEQSExt : public UAITask {
    GENERATED_BODY()

protected:
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FEQSExtTaskCompletedSignature,
                                                UEnvQueryInstanceBlueprintWrapperExt*,
                                                QueryInstance);

    UPROPERTY(Transient)
    FEQSExtTaskRequestSpec RequestSpec;

    UPROPERTY(Transient)
    UEnvQueryInstanceBlueprintWrapperExt* QueryInstance;

    UPROPERTY(BlueprintAssignable)
    FEQSExtTaskCompletedSignature OnRequestFailed;

    UPROPERTY(BlueprintAssignable)
    FEQSExtTaskCompletedSignature OnRequestFinished;

public:
    UFUNCTION(BlueprintCallable, Category = "AI|Tasks",
        meta = (AdvancedDisplay = "bLockAILogic", DefaultToSelf = "Controller", BlueprintInternalUseOnly =
            "TRUE"))
    static UAITask_RunEQSExt*
    RunEQSExt(AAIController* Controller, UEnvQuery* QueryTemplate,
              TEnumAsByte<EEnvQueryRunMode::Type> RunMode, bool bLockAILogic = false);

protected:
    virtual void
    Activate() override;

    virtual void
    OnDestroy(bool bInOwnerFinished) override;

    UFUNCTION()
    void
    OnQueryFinished(UEnvQueryInstanceBlueprintWrapper* InQueryInstance, EEnvQueryStatus::Type QueryStatus);
};
