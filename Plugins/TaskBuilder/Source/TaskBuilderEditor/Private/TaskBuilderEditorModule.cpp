// Copyright Epic Games, Inc. All Rights Reserved.

#include "TaskBuilderEditorModule.h"
#include "TaskBuilderActions.h"

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
}

void FTaskBuilderEditorModule::ShutdownModule()
{
	if (FModuleManager::Get().IsModuleLoaded("PropertyEditor"))
	{
		FPropertyEditorModule& PropertyModule = FModuleManager::GetModuleChecked<FPropertyEditorModule>("PropertyEditor");
		PropertyModule.NotifyCustomizationModuleChanged();
	}
}

#undef LOCTEXT_NAMESPACE
IMPLEMENT_MODULE(FTaskBuilderEditorModule, TaskBuilderEditor)