#include "TaskBuilderEditorApplicationMode.h"
#include "TaskBuilderEditorTabFactories.h"


FTaskBuilderEditorApplicationMode::FTaskBuilderEditorApplicationMode(TSharedPtr<FTaskBuilderEditor> InTaskBuilderEditor)
	:FBlueprintEditorApplicationMode(InTaskBuilderEditor, FTaskBuilderEditorModes::TaskBuilderEditorMode, FTaskBuilderEditorModes::GetLocalizedMode, false, false)
{
	TaskBuilderEditor = InTaskBuilderEditor;
	StardardTabFactories.RegisterFactory(MakeShareable(new FTaskBuilderGraphTabFactory(InTaskBuilderEditor)));


	FBlueprintEditorModule& BlueprintEditorModule = FModuleManager::LoadModuleChecked<FBlueprintEditorModule>("Kismet");
	BlueprintEditorModule.OnRegisterTabsForEditor().Broadcast(StardardTabFactories, FTaskBuilderEditorModes::TaskBuilderEditorMode, InTaskBuilderEditor);

	LayoutExtender = MakeShared<FLayoutExtender>();
	BlueprintEditorModule.OnRegisterLayoutExtensions().Broadcast(*LayoutExtender);

	/*if (UToolMenu* Toolbar = InTaskBuilderEditor->RegisterModeToolbarIfUnregistered(GetModeName()))
	{
		InTaskBuilderEditor->GetToolbarBuilder()->AddCompileToolbar(Toolbar);
		InTaskBuilderEditor->GetToolbarBuilder()->AddBlueprintGlobalOptionsToolbar(Toolbar);
		InTaskBuilderEditor->GetToolbarBuilder()->AddDebuggingToolbar(Toolbar);
		InTaskBuilderEditor->GetToolbarBuilder()->AddScriptingToolbar(Toolbar);
		InTaskBuilderEditor->GetToolbarBuilder()->AddNewToolbar(Toolbar);
	}*/
}

void FTaskBuilderEditorApplicationMode::RegisterTabFactories(TSharedPtr<FTabManager> InTabManager)
{
	TSharedPtr<FTaskBuilderEditor> Editor = TaskBuilderEditor.Pin();
	// Tool bar tab
	Editor->RegisterToolbarTab(InTabManager.ToSharedRef());

	// Graph tab
	Editor->GetDocumentManager()->RegisterDocumentFactory(MakeShareable(new FTaskBuilderGraphTabFactory(Editor)));

	// Other tabs
	Editor->PushTabFactories(CoreTabFactories);
	Editor->PushTabFactories(BlueprintEditorOnlyTabFactories);
	Editor->PushTabFactories(BlueprintEditorTabFactories);

	FApplicationMode::RegisterTabFactories(InTabManager);
}

void FTaskBuilderEditorApplicationMode::PostActivateMode()
{
	TSharedPtr<FBlueprintEditor> BP = MyBlueprintEditor.Pin();
	BP->RestoreEditedObjectState();
	BP->SetupViewForBlueprintEditingMode();

	TaskBuilderEditor.Pin()->InvokeTaskBuilderGraphTab();

	FApplicationMode::PostActivateMode();
}

