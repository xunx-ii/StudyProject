// Copyright Epic Games, Inc. All Rights Reserved.

#include "TaskBuilderEditorModule.h"
#include "TaskBuilderActions.h"
#include "Graphs/TaskBuilderGraphFactories.h"
#include "TaskBuilderBlueprint.h"
#include "Kismet2/KismetEditorUtilities.h"
#include "BlueprintActionDatabase.h"
#include "K2Node_Event.h"

#define LOCTEXT_NAMESPACE "TaskBuilderEditorModule"

void FTaskBuilderEditorModule::StartupModule()
{
	FCoreDelegates::OnPostEngineInit.AddRaw(this, &FTaskBuilderEditorModule::OnPostEngineInit);
}

void FTaskBuilderEditorModule::OnPostEngineInit()
{
	IAssetTools& AssetTools = FModuleManager::LoadModuleChecked<FAssetToolsModule>("AssetTools").Get();
	EAssetTypeCategories::Type TaskBuilderCategoryType = AssetTools.RegisterAdvancedAssetCategory(FName(TEXT("TaskBuilder")), LOCTEXT("TaskBuilderCategory", "Task Builder"));
	TSharedRef<IAssetTypeActions> TaskBuilderActions = MakeShareable(new FTaskBuilderActions(TaskBuilderCategoryType));
	AssetTools.RegisterAssetTypeActions(TaskBuilderActions);

	FKismetEditorUtilities::RegisterOnBlueprintCreatedCallback(this, UTaskBuilderBlueprint::StaticClass(), FKismetEditorUtilities::FOnBlueprintCreated::CreateRaw(this, &FTaskBuilderEditorModule::OnNewBlueprintCreated));

	FEdGraphUtilities::RegisterVisualNodeFactory(MakeShareable(new FTaskBuilderGraphNodeFactory()));
	FEdGraphUtilities::RegisterVisualPinConnectionFactory(MakeShareable(new FTaskBuilderGraphPinConnectionFactory()));

	if (FBlueprintActionDatabase* BAD = FBlueprintActionDatabase::TryGet())
	{
		BAD->RefreshClassActions(UK2Node_Event::StaticClass());
	}
}

void FTaskBuilderEditorModule::OnNewBlueprintCreated(UBlueprint* InBlueprint)
{
	if (ensure(InBlueprint->UbergraphPages.Num() > 0))
	{
		UEdGraph* EventGraph = InBlueprint->UbergraphPages[0];
	}
}

void FTaskBuilderEditorModule::ShutdownModule()
{
	if (FModuleManager::Get().IsModuleLoaded("PropertyEditor"))
	{
		FPropertyEditorModule& PropertyModule = FModuleManager::GetModuleChecked<FPropertyEditorModule>("PropertyEditor");
		PropertyModule.NotifyCustomizationModuleChanged();
	}

	FEdGraphUtilities::UnregisterVisualNodeFactory(MakeShareable(new FTaskBuilderGraphNodeFactory()));
	FEdGraphUtilities::UnregisterVisualPinConnectionFactory(MakeShareable(new FTaskBuilderGraphPinConnectionFactory()));
}

TSharedRef<FTaskBuilderEditor> FTaskBuilderEditorModule::CreateTaskBuilderEditor(const EToolkitMode::Type Mode, const TSharedPtr< class IToolkitHost >& InitToolkitHost, class UTaskBuilderBlueprint* Blueprint)
{
	TSharedRef< FTaskBuilderEditor > NewEditor(new FTaskBuilderEditor());
	NewEditor->InitTaskBuilderEditor(Mode, InitToolkitHost, Blueprint, false);
	return NewEditor;
}

#undef LOCTEXT_NAMESPACE
IMPLEMENT_MODULE(FTaskBuilderEditorModule, TaskBuilderEditor)