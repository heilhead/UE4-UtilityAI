#include "UtilityAIEditorModule.h"
#include "AIResponseCurveCustomization.h"
#include "Modules/ModuleInterface.h"
#include "Modules/ModuleManager.h"
#include "UtilityAITest.h"
#include "PropertyEditor/Public/PropertyEditorModule.h"

IMPLEMENT_GAME_MODULE(FUtilityAIEditorModule, UtilityAIEditor);

#define LOCTEXT_NAMESPACE "UtilityAIEditor"

void
FUtilityAIEditorModule::StartupModule()
{
    FPropertyEditorModule& PropertyModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>(
        "PropertyEditor");

    PropertyModule.RegisterCustomPropertyTypeLayout(FAIResponseCurve::StaticStruct()->GetFName(),
                                                    FOnGetPropertyTypeCustomizationInstance::CreateStatic(
                                                        &FAIResponseCurveCustomization::MakeInstance));
    PropertyModule.NotifyCustomizationModuleChanged();
}

void
FUtilityAIEditorModule::ShutdownModule()
{
    if (FModuleManager::Get().IsModuleLoaded("PropertyEditor")) {
        FPropertyEditorModule& PropertyModule = FModuleManager::GetModuleChecked<FPropertyEditorModule>(
            "PropertyEditor");

        PropertyModule.UnregisterCustomPropertyTypeLayout(FAIResponseCurve::StaticStruct()->GetFName());
        PropertyModule.NotifyCustomizationModuleChanged();
    }
}

#undef LOCTEXT_NAMESPACE
