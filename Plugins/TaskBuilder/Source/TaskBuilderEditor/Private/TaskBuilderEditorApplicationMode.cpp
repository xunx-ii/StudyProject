#include "TaskBuilderEditorApplicationMode.h"
#include "TaskBuilderEditorTabFactories.h"
#include "SBlueprintEditorToolbar.h"


FTaskBuilderEditorApplicationMode::FTaskBuilderEditorApplicationMode(TSharedPtr<FTaskBuilderEditor> InTaskBuilderEditor)
	:FBlueprintEditorApplicationMode(InTaskBuilderEditor, FTaskBuilderEditorModes::TaskBuilderEditorMode, FTaskBuilderEditorModes::GetLocalizedMode, false, false)
{
	TaskBuilderEditor = InTaskBuilderEditor;
	StardardTabFactories.RegisterFactory(MakeShareable(new FTaskBuilderGraphTabFactory(InTaskBuilderEditor)));

	TabLayout =
		FTabManager::NewLayout("Standalone_BlueprintEditor_Layout_v17")
		->AddArea
		(
			FTabManager::NewPrimaryArea()->SetOrientation(Orient_Vertical)
			->Split
			(
				FTabManager::NewStack()
				->SetSizeCoefficient(0.186721f)
				->SetHideTabWell(true)
				->AddTab(InTaskBuilderEditor->GetToolbarTabId(), ETabState::OpenedTab)
			)
			->Split
			(
				FTabManager::NewSplitter()->SetOrientation(Orient_Horizontal)
				->Split
				(
					FTabManager::NewSplitter()->SetOrientation(Orient_Vertical)
					->SetSizeCoefficient(0.15f)
					->Split
					(
						FTabManager::NewStack()->SetSizeCoefficient(1.f)
						->AddTab(FBlueprintEditorTabs::MyBlueprintID, ETabState::OpenedTab)
					)
				)

				->Split
				(
					FTabManager::NewSplitter()->SetOrientation(Orient_Vertical)
					->SetSizeCoefficient(0.70f)
					->Split
					(
						FTabManager::NewStack()
						->SetSizeCoefficient(0.80f)
						->AddTab("Document", ETabState::ClosedTab)
					)
					->Split
					(
						FTabManager::NewStack()
						->SetSizeCoefficient(0.20f)
						->AddTab(FBlueprintEditorTabs::CompilerResultsID, ETabState::ClosedTab)
						->AddTab(FBlueprintEditorTabs::FindResultsID, ETabState::ClosedTab)
						->AddTab(FBlueprintEditorTabs::BookmarksID, ETabState::ClosedTab)
					)
				)
				->Split
				(
					FTabManager::NewSplitter()->SetOrientation(Orient_Vertical)
					->SetSizeCoefficient(0.15f)
					->Split
					(
						FTabManager::NewStack()->SetSizeCoefficient(1.f)
						->AddTab(FBlueprintEditorTabs::DetailsID, ETabState::OpenedTab)
						->AddTab(FBlueprintEditorTabs::PaletteID, ETabState::ClosedTab)
						->AddTab(FBlueprintEditorTabs::DefaultEditorID, ETabState::ClosedTab)
					)
				)
			)
		);


	FBlueprintEditorModule& BlueprintEditorModule = FModuleManager::LoadModuleChecked<FBlueprintEditorModule>("Kismet");
	BlueprintEditorModule.OnRegisterTabsForEditor().Broadcast(StardardTabFactories, FTaskBuilderEditorModes::TaskBuilderEditorMode, InTaskBuilderEditor);
	BlueprintEditorModule.OnRegisterLayoutExtensions().Broadcast(*LayoutExtender);
	
	ToolbarExtender = MakeShareable(new FExtender);

	if (UToolMenu* Toolbar = InTaskBuilderEditor->RegisterModeToolbarIfUnregistered(GetModeName()))
	{
		InTaskBuilderEditor->GetToolbarBuilder()->AddCompileToolbar(Toolbar);
		InTaskBuilderEditor->GetToolbarBuilder()->AddBlueprintGlobalOptionsToolbar(Toolbar);
		InTaskBuilderEditor->GetToolbarBuilder()->AddDebuggingToolbar(Toolbar);
		InTaskBuilderEditor->GetToolbarBuilder()->AddScriptingToolbar(Toolbar);
		InTaskBuilderEditor->GetToolbarBuilder()->AddNewToolbar(Toolbar);
	}
	LayoutExtender = MakeShared<FLayoutExtender>();

	TabLayout->ProcessExtensions(*LayoutExtender.Get());
}

void FTaskBuilderEditorApplicationMode::RegisterTabFactories(TSharedPtr<FTabManager> InTabManager)
{
	TSharedPtr<FTaskBuilderEditor> Editor = TaskBuilderEditor.Pin();
	// Tool bar tab
	Editor->RegisterToolbarTab(InTabManager.ToSharedRef());

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

	FBlueprintEditorApplicationMode::PostActivateMode();
}

