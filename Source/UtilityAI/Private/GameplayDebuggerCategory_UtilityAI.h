#pragma once

#if WITH_GAMEPLAY_DEBUGGER

#include "CoreMinimal.h"
#include "GameplayDebuggerCategory.h"

class AActor;
class APlayerController;

class FGameplayDebuggerCategory_UtilityAI : public FGameplayDebuggerCategory {
public:
    FGameplayDebuggerCategory_UtilityAI();

    virtual void
    CollectData(APlayerController* OwnerPC, AActor* DebugActor) override;

    virtual void
    DrawData(APlayerController* OwnerPC, FGameplayDebuggerCanvasContext& CanvasContext) override;

    static TSharedRef<FGameplayDebuggerCategory>
    MakeInstance();

protected:
    struct FRepData {
        struct FActiveAbilityInfo {
            FString TaskName;
            FString TargetActorName;
            float ActivationTime;
            float Score;
            bool bIsActive;
            bool bHasTargetActor;
        };

        struct FBrainInfo {
            float UpdateInterval;
            bool bIsActive;
        };

        FActiveAbilityInfo ActiveTask;
        FBrainInfo Brain;
        bool bHasData;

        void
        Serialize(FArchive& Ar);
    };

    FRepData DataPack;

    bool
    WrapStringAccordingToViewport(const FString& iStr, FString& oStr,
                                  FGameplayDebuggerCanvasContext& CanvasContext, float ViewportWitdh);
};

#endif // WITH_GAMEPLAY_DEBUGGER
