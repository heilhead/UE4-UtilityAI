#include "GameplayDebuggerCategory_UtilityAI.h"

#include "AIController.h"
#include "Components/UtilityAIComponent.h"

#if WITH_GAMEPLAY_DEBUGGER

#include "Engine/Canvas.h"

FGameplayDebuggerCategory_UtilityAI::FGameplayDebuggerCategory_UtilityAI()
{
    SetDataPackReplication<FRepData>(&DataPack, EGameplayDebuggerDataPack::ResetOnTick);
}

TSharedRef<FGameplayDebuggerCategory>
FGameplayDebuggerCategory_UtilityAI::MakeInstance()
{
    return MakeShareable(new FGameplayDebuggerCategory_UtilityAI());
}

void
FGameplayDebuggerCategory_UtilityAI::FRepData::Serialize(FArchive& Ar)
{
    Ar << ActiveTask.TaskName;
    Ar << ActiveTask.TargetActorName;
    Ar << ActiveTask.ActivationTime;
    Ar << ActiveTask.Score;
    Ar << ActiveTask.bIsActive;
    Ar << ActiveTask.bHasTargetActor;

    Ar << Brain.UpdateInterval;
    Ar << Brain.bIsActive;

    Ar << bHasData;
}

void
FGameplayDebuggerCategory_UtilityAI::CollectData(APlayerController* OwnerPC, AActor* DebugActor)
{
    DataPack.bHasData = false;

    auto* Pawn = Cast<APawn>(DebugActor);
    if (!Pawn) {
        return;
    }

    auto* Controller = Cast<AAIController>(Pawn->GetController());
    if (!Controller) {
        return;
    }

    auto* UtilityAIComp = Cast<UUtilityAIComponent>(Controller->GetBrainComponent());
    if (!UtilityAIComp) {
        return;
    }

    DataPack.bHasData = true;
    DataPack.Brain.bIsActive = UtilityAIComp->IsRunning() && !UtilityAIComp->IsPaused();
    DataPack.Brain.UpdateInterval = UtilityAIComp->GetComponentTickInterval();

    auto* ActiveTask = UtilityAIComp->GetActiveTask();
    const bool bHasActiveTask = ActiveTask != nullptr;

    auto& TaskData = DataPack.ActiveTask;
    TaskData.bIsActive = bHasActiveTask;

    if (bHasActiveTask) {
        TaskData.TaskName = ActiveTask->GetName();

        if (auto* TargetActor = ActiveTask->GetTarget().GetActor()) {
            TaskData.TargetActorName = TargetActor->GetName();
            TaskData.bHasTargetActor = true;
        } else {
            TaskData.TargetActorName = "";
            TaskData.bHasTargetActor = false;
        }

        const auto& Context = ActiveTask->GetExecutionContext();

        TaskData.ActivationTime = Context.ActivationTime;
        TaskData.Score = Context.Score;
    }
}

bool
FGameplayDebuggerCategory_UtilityAI::WrapStringAccordingToViewport(
    const FString& StrIn, FString& StrOut, FGameplayDebuggerCanvasContext& CanvasContext, float ViewportWitdh)
{
    if (!StrIn.IsEmpty()) {
        // Clamp the Width
        ViewportWitdh = FMath::Max(ViewportWitdh, 10.0f);

        float StrWidth = 0.0f, StrHeight = 0.0f;
        // Calculate the length(in pixel) of the tags
        CanvasContext.MeasureString(StrIn, StrWidth, StrHeight);

        int32 SubDivision = FMath::CeilToInt(StrWidth / ViewportWitdh);
        if (SubDivision > 1) {
            // Copy the string
            StrOut = StrIn;
            const int32 Step = StrOut.Len() / SubDivision;
            // Start sub divide if needed
            for (int32 i = SubDivision - 1; i > 0; --i) {
                // Insert Line Feed
                StrOut.InsertAt(i * Step - 1, '\n');
            }
            return true;
        }
    }
    // No need to wrap the text
    return false;
}

void
FGameplayDebuggerCategory_UtilityAI::DrawData(APlayerController* OwnerPC,
                                              FGameplayDebuggerCanvasContext& CanvasContext)
{
    if (!DataPack.bHasData) {
        CanvasContext.Printf(TEXT("No Data Available"));
        return;
    }

    if (!DataPack.Brain.bIsActive) {
        CanvasContext.Printf(TEXT("Brain: {grey}Inactive\n"));
    } else {
        CanvasContext.Printf(
            TEXT("Brain: {green}Active\n\t{white}Update Interval: %0.2f\n"), DataPack.Brain.UpdateInterval);
    }

    if (!DataPack.ActiveTask.bIsActive) {
        CanvasContext.Printf(TEXT("Active Task: {grey}None"));
    } else {
        const auto* World = OwnerPC->GetWorld();

        CanvasContext.Printf(
            TEXT("Active Task: {green}%s\n\t{white}Time: %0.2f\tScore: %0.2f"), *DataPack.ActiveTask.TaskName,
            World->TimeSince(DataPack.ActiveTask.ActivationTime), DataPack.ActiveTask.Score);

        if (DataPack.ActiveTask.bHasTargetActor) {
            CanvasContext.Printf(TEXT("Target Actor: %s"), *DataPack.ActiveTask.TargetActorName);
        } else {
            CanvasContext.Printf(TEXT("Target Actor: {grey}None"));
        }
    }
}

#endif // WITH_GAMEPLAY_DEBUGGER
