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
	}

	TSharedRef<FTabPayload_UObject> Payload = FTabPayload_UObject::Make(TaskBuilderGraph);
	TSharedPtr<SDockTab> DocumentTab = DocumentManager->OpenDocument(Payload, bNewGraph ? FDocumentTracker::OpenNewDocument :
		FDocumentTracker::RestorePreviousDocument);
}

void FTaskBuilderEditor::InitTaskBuilderEditor(const EToolkitMode::Type Mode, const TSharedPtr<IToolkitHost>& InitToolkitHost, const TArray<UBlueprint*>& InBlueprints, bool bShouldOpenInDefaultsMode)
{
	InitBlueprintEditor(Mode, InitToolkitHost, InBlueprints, bShouldOpenInDefaultsMode);

	// RegisterApplicationModes
	AddApplicationMode(FTaskBuilderEditorModes::TaskBuilderEditorMode, MakeShareable(new FTaskBuilderEditorApplicationMode(SharedThis(this))));
	SetCurrentMode(FTaskBuilderEditorModes::TaskBuilderEditorMode);

	TArray<UBeginTaskEdGraphNode*> EntryNodeRef;
	TaskBuilderGraph->GetNodesOfClass(EntryNodeRef);

	if (EntryNodeRef.Num() > 0)
	{
		UTaskBuilderEdGraph* TaskBuilderEdGraph = NULL;
		TaskBuilderEdGraph = Cast<UTaskBuilderEdGraph>(TaskBuilderBlueprint->TaskBuilderGraph);
		if (TaskBuilderEdGraph)
		{

		}
	}
}



#undef LOCTEXT_NAMESPACE