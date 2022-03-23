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
#include "EdGraphSchema_K2_Actions.h"
#include "K2Node_Event.h"


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

	bool bNewGraph = false;
	if (TaskBuilderBlueprint->TaskBuilderGraph == NULL)
	{
		CreateEventGraph();

		bNewGraph = true;
		UEdGraph* NewCreatedGraph = FBlueprintEditorUtils::CreateNewGraph(
			TaskBuilderBlueprint.Get(),
			FName("TaskBuilderGraph"),
			UTaskBuilderEdGraph::StaticClass(),
			UTaskBuilderGraphSchema::StaticClass());

		TaskBuilderGraph = TaskBuilderBlueprint->TaskBuilderGraph = NewCreatedGraph;
	}

	TaskBuilderGraph = TaskBuilderBlueprint->TaskBuilderGraph;
	
	if (UTaskBuilderEdGraph* CurrentGraph = CastChecked<UTaskBuilderEdGraph>(TaskBuilderGraph))
	{
		if (CurrentGraph->BeginTaskEdGraphNode == NULL)
		{
			TaskBuilderGraph->GetSchema()->CreateDefaultNodesForGraph(*TaskBuilderGraph);
		}
	}
	
	TSharedRef<FTabPayload_UObject> Payload = FTabPayload_UObject::Make(TaskBuilderGraph);
	TSharedPtr<SDockTab> DocumentTab = DocumentManager->OpenDocument(Payload, bNewGraph ? FDocumentTracker::OpenNewDocument :
		FDocumentTracker::RestorePreviousDocument);
}

void FTaskBuilderEditor::CreateEventGraph()
{
	ECreatedDocumentType GraphType = FBlueprintEditor::CGT_NewEventGraph;

	if (!NewDocument_IsVisibleForType(GraphType))
	{
		return;
	}

	FText DocumentNameText = LOCTEXT("NewDocEventGraphName", "EventGraph");
	bool bResetMyBlueprintFilter = true;

	FName DocumentName = FName(*DocumentNameText.ToString());

	check(IsEditingSingleBlueprint());

	const FScopedTransaction Transaction(LOCTEXT("AddNewFunction", "Add New Function"));
	GetBlueprintObj()->Modify();

	EventGraphRef = FBlueprintEditorUtils::CreateNewGraph(GetBlueprintObj(), DocumentName, UEdGraph::StaticClass(), GetDefaultSchemaClass());
	FBlueprintEditorUtils::AddUbergraphPage(GetBlueprintObj(), EventGraphRef);

	OpenDocument(EventGraphRef, FDocumentTracker::OpenNewDocument);
	EventGraphRef->bAllowDeletion = false;
	EventGraphRef->bAllowRenaming = false;

	TaskBuilderBlueprint->EvenGraphRef = EventGraphRef;

	TArray<FName> EventFuncName;
	EventFuncName.Add("ReceiveBeginPlay");
	EventFuncName.Add("ReceiveTick");

	TArray<FString> EventDescription;
	EventDescription.Add("Event when play begins for this component");
	EventDescription.Add("Event called every frame if tick is enabled.");

	UClass* const OverrideFuncClass = GetBlueprintObj()->ParentClass;
	bool bIsOverrideFunc = true;
	const UEdGraphSchema_K2* K2Schema = GetDefault<UEdGraphSchema_K2>();

	TArray<UK2Node_Event*> NewEventNodes;
	TArray<UFunction*> ParentFunctionRef;

	for (int x = 0; x < EventFuncName.Num(); x++)
	{
		if (FindUField<UFunction>(GetBlueprintObj()->ParentClass, EventFuncName[x]))
		{
			ParentFunctionRef.Add(FindUField<UFunction>(GetBlueprintObj()->ParentClass, EventFuncName[x]));

			FVector2D SpawnPos = EventGraphRef->GetGoodPlaceForNewNode();

			FName EventName = EventFuncName[x];
			NewEventNodes.Add(FEdGraphSchemaAction_K2NewNode::SpawnNode<UK2Node_Event>(
				EventGraphRef,
				SpawnPos,
				EK2NewNodeFlags::SelectNewNode,
				[EventName, OverrideFuncClass](UK2Node_Event* NewInstance)
				{
					NewInstance->EventReference.SetExternalMember(EventName, OverrideFuncClass);
					NewInstance->bOverrideFunction = true;
				}
			));
			NewEventNodes[x]->NodeComment = EventDescription[x];
			NewEventNodes[x]->bCommentBubbleMakeVisible = true;

		}
	}
}

void FTaskBuilderEditor::InitTaskBuilderEditor(const EToolkitMode::Type Mode, const TSharedPtr<IToolkitHost>& InitToolkitHost, const TArray<UBlueprint*>& InBlueprints, bool bShouldOpenInDefaultsMode)
{
	//InitBlueprintEditor(Mode, InitToolkitHost, InBlueprints, bShouldOpenInDefaultsMode);
	bool bNewlyCreated = InBlueprints.Num() == 1 && InBlueprints[0]->bIsNewlyCreated;

	LoadEditorSettings();

	TArray< UObject* > Objects;
	for (UBlueprint* Blueprint : InBlueprints)
	{
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