#include "TaskBuilderEditor.h"
#include "EditorReimportHandler.h"

#if WITH_EDITOR
#include "Editor.h"
#endif

#include "TaskBuilderBlueprint.h"
#include "Kismet2/BlueprintEditorUtils.h"
#include "Graphs/TaskBuilderEdGraph.h"
#include "Graphs/TaskBuilderGraphSchema.h"
#include "WorkflowOrientedApp/WorkflowUObjectDocuments.h"
#include "TaskBuilderEditorApplicationMode.h"
#include "Graphs/GraphNodes/BeginTaskEdGraphNode.h"
#include "SBlueprintEditorToolbar.h"
#include "BlueprintEditorSettings.h"
#include "Kismet2/DebuggerCommands.h"
#include "BlueprintEditor.h"
#include "Kismet2/CompilerResultsLog.h"


#define LOCTEXT_NAMESPACE "TaskBuilderEditor"

const FName FTaskBuilderEditorModes::TaskBuilderEditorMode("TaskBuilderEditorMode");

FName FTaskBuilderEditor::GetToolkitFName() const
{
	return FName("TaskBuilderEdGraph");
}

FText FTaskBuilderEditor::GetBaseToolkitName() const
{
	return LOCTEXT("TaskBuilderEdGraph", "Task Builder Editor");
}

FString FTaskBuilderEditor::GetWorldCentricTabPrefix() const
{
	return FString(TEXT(""));
}

FLinearColor FTaskBuilderEditor::GetWorldCentricTabColorScale() const
{
	return FLinearColor();
}


void FTaskBuilderEditor::LoadEditorSettings()
{
	UBlueprintEditorSettings* LocalSettings = GetMutableDefault<UBlueprintEditorSettings>();

	if (LocalSettings->bHideUnrelatedNodes)
	{
		ToggleHideUnrelatedNodes();
	}
}

TSharedPtr<FDocumentTracker> FTaskBuilderEditor::GetDocumentManager()
{
	return DocumentManager;
}

UBlueprint* FTaskBuilderEditor::GetBlueprintObj() const
{
	const TArray<UObject*>& EditingObjs = GetEditingObjects();
	for (int32 i = 0; i < EditingObjs.Num(); ++i)
	{
		if (EditingObjs[i]->IsA<UTaskBuilderBlueprint>())
		{
			return (UBlueprint*)EditingObjs[i];
		}
	}
	return nullptr;
}

void FTaskBuilderEditor::InvokeTaskBuilderGraphTab()
{
	TaskBuilderBlueprint = Cast<UTaskBuilderBlueprint>(GetBlueprintObj());
	UTaskBuilderEdGraph* TaskBuilderEdGraph = NULL;

	bool bNewGraph = false;
	if (TaskBuilderBlueprint->TaskBuilderGraph == NULL)
	{
		bNewGraph = true;
		UEdGraph* NewCreatedGraph = FBlueprintEditorUtils::CreateNewGraph(
			TaskBuilderBlueprint.Get(),
			FName("TaskBuilderGraph"),
			UTaskBuilderEdGraph::StaticClass(),
			UTaskBuilderGraphSchema::StaticClass());

		TaskBuilderBlueprint->TaskBuilderGraph = NewCreatedGraph;
		TaskBuilderGraph = NewCreatedGraph;

		TaskBuilderGraph->GetSchema()->CreateDefaultNodesForGraph(*TaskBuilderGraph);
	}

	if (!TaskBuilderEdGraph)
	{
		TaskBuilderEdGraph = Cast<UTaskBuilderEdGraph>(TaskBuilderBlueprint->TaskBuilderGraph);
		TaskBuilderGraph = TaskBuilderBlueprint->TaskBuilderGraph;
	}

	TSharedRef<FTabPayload_UObject> Payload = FTabPayload_UObject::Make(TaskBuilderGraph);
	TSharedPtr<SDockTab> DocumentTab = DocumentManager->OpenDocument(Payload, bNewGraph ? FDocumentTracker::OpenNewDocument :
		FDocumentTracker::RestorePreviousDocument);
}

void FTaskBuilderEditor::InitTaskBuilderEditor(const EToolkitMode::Type Mode, const TSharedPtr<IToolkitHost>& InitToolkitHost, const TArray<UBlueprint*>& InBlueprints, bool bShouldOpenInDefaultsMode)
{
	//InitBlueprintEditor(Mode, InitToolkitHost, InBlueprints, bShouldOpenInDefaultsMode);
	bool bNewlyCreated = InBlueprints.Num() == 1 && InBlueprints[0]->bIsNewlyCreated;

	LoadEditorSettings();

	TArray< UObject* > Objects;
	for (UBlueprint* Blueprint : InBlueprints)
	{
		// Flag the blueprint as having been opened
		Blueprint->bIsNewlyCreated = false;

		Objects.Add(Blueprint);
	}


	if (!Toolbar.IsValid())
	{
		Toolbar = MakeShareable(new FBlueprintEditorToolbar(SharedThis(this)));
	}

	GetToolkitCommands()->Append(FPlayWorldCommands::GlobalPlayWorldActions.ToSharedRef());

	CreateDefaultCommands();

	RegisterMenus();

	// Initialize the asset editor and spawn nothing (dummy layout)
	const bool bCreateDefaultStandaloneMenu = true;
	const bool bCreateDefaultToolbar = true;
	const FName BlueprintEditorAppName = FName(TEXT("BlueprintEditorApp"));
	InitAssetEditor(Mode, InitToolkitHost, BlueprintEditorAppName, FTabManager::FLayout::NullLayout, bCreateDefaultStandaloneMenu, bCreateDefaultToolbar, Objects);

	CommonInitialization(InBlueprints, bShouldOpenInDefaultsMode);

	InitalizeExtenders();

	RegenerateMenusAndToolbars();


	if (UBlueprint* SingleBP = GetBlueprintObj())
	{
		AddApplicationMode(FTaskBuilderEditorModes::TaskBuilderEditorMode, MakeShareable(new FTaskBuilderEditorApplicationMode(SharedThis(this))));
		SetCurrentMode(FTaskBuilderEditorModes::TaskBuilderEditorMode);
	}

	PostLayoutBlueprintEditorInitialization();

	// Find and set any instances of this blueprint type if any exists and we are not already editing one
	FBlueprintEditorUtils::FindAndSetDebuggableBlueprintInstances();

	if (bNewlyCreated)
	{
		if (UBlueprint* Blueprint = GetBlueprintObj())
		{
			if (Blueprint->BlueprintType == BPTYPE_MacroLibrary)
			{
				NewDocument_OnClick(CGT_NewMacroGraph);
			}
			else if (Blueprint->BlueprintType == BPTYPE_Interface)
			{
				NewDocument_OnClick(CGT_NewFunctionGraph);
			}
			else if (Blueprint->BlueprintType == BPTYPE_FunctionLibrary)
			{
				NewDocument_OnClick(CGT_NewFunctionGraph);
			}
		}
	}

	if (UBlueprint* Blueprint = GetBlueprintObj())
	{
		if (Blueprint->GetClass() == UBlueprint::StaticClass() && Blueprint->BlueprintType == BPTYPE_Normal)
		{
			if (!bShouldOpenInDefaultsMode)
			{
				GetToolkitCommands()->ExecuteAction(FFullBlueprintEditorCommands::Get().EditClassDefaults.ToSharedRef());
			}
		}

		// There are upgrade notes, open the log and dump the messages to it
		if (Blueprint->UpgradeNotesLog.IsValid())
		{
			DumpMessagesToCompilerLog(Blueprint->UpgradeNotesLog->Messages, true);
		}
	}
}



#undef LOCTEXT_NAMESPACE