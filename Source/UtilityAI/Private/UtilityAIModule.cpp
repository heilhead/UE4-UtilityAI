#include "UtilityAIModule.h"

#if WITH_GAMEPLAY_DEBUGGER
#include "GameplayDebugger.h"
#include "GameplayDebuggerCategory_UtilityAI.h"
#endif

#define LOCTEXT_NAMESPACE "UtilityAI"

class FUtilityAIModule : public IUtilityAIModule {
    virtual void
    StartupModule() override;

    virtual void
    ShutdownModule() override;
};

IMPLEMENT_MODULE(FUtilityAIModule, UtilityAI)

void
FUtilityAIModule::StartupModule()
{
#if WITH_GAMEPLAY_DEBUGGER
    IGameplayDebugger& GameplayDebuggerModule = IGameplayDebugger::Get();
    GameplayDebuggerModule.RegisterCategory("UtilityAI",
                                            IGameplayDebugger::FOnGetCategory::CreateStatic(
                                                &FGameplayDebuggerCategory_UtilityAI::MakeInstance));
    GameplayDebuggerModule.NotifyCategoriesChanged();
#endif // WITH_GAMEPLAY_DEBUGGER

    if (!IsRunningDedicatedServer()) {
        // AHUD::OnShowDebugInfo.AddStatic(&UAbilitySystemComponent::OnShowDebugInfo);
    }
}

void
FUtilityAIModule::ShutdownModule()
{
#if WITH_GAMEPLAY_DEBUGGER
    if (IGameplayDebugger::IsAvailable()) {
        IGameplayDebugger& GameplayDebuggerModule = IGameplayDebugger::Get();
        GameplayDebuggerModule.UnregisterCategory("UtilityAI");
        GameplayDebuggerModule.NotifyCategoriesChanged();
    }
#endif // WITH_GAMEPLAY_DEBUGGER
}

#undef LOCTEXT_NAMESPACE
