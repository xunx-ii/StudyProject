#include "TaskBuilderGraphSchema.h"
#include "TaskBuilderGraphSchemaActions.h"
#include "GraphNodes/BeginTaskEdGraphNode.h"
#include "TaskBuilderEdGraph.h"
#include "GraphNodes/SubTaskEdGraphNode.h"

#define LOCTEXT_NAMESPACE "TaskBuilderGraphSchema"

/////////////////////////////////////////////////////
TSharedPtr<FTaskBuilderGraphSchemaActions> AddNewTaskNodeAction(FGraphContextMenuBuilder& ContextMenuBuilder, const FText& Category, const FText& MenuDesc, const FText& Tooltip, const int32 Grouping = 0)
{
	TSharedPtr<FTaskBuilderGraphSchemaActions> NewStateNode(new FTaskBuilderGraphSchemaActions(Category, MenuDesc, Tooltip, Grouping));
	ContextMenuBuilder.AddAction(NewStateNode);
	return NewStateNode;
}
/////////////////////////////////////////////////////
// UTaskBuilderGraphSchema
void UTaskBuilderGraphSchema::GetGraphContextActions(FGraphContextMenuBuilder& ContextMenuBuilder) const
{
	{
		TSharedPtr<FTaskBuilderGraphSchemaActions> Action = AddNewTaskNodeAction(ContextMenuBuilder, FText::GetEmpty(), LOCTEXT("AddSubTask", "Add SubTask..."), LOCTEXT("AddSubTaskTooltip", "A New SubTask"));
		Action->NodeTemplate = NewObject<USubTaskEdGraphNode>(ContextMenuBuilder.OwnerOfTemporaries);
	}

	{
		bool bHasEntry = false;
		for (auto NodeIt = ContextMenuBuilder.CurrentGraph->Nodes.CreateConstIterator(); NodeIt; ++NodeIt)
		{
			UEdGraphNode* Node = *NodeIt;
			if (const UBeginTaskEdGraphNode* StateNode = Cast<UBeginTaskEdGraphNode>(Node))
			{
				bHasEntry = true;
				break;
			}
		}

		if (!bHasEntry)
		{
			TSharedPtr<FTaskBuilderGraphSchemaActions> Action = AddNewTaskNodeAction(ContextMenuBuilder, FText::GetEmpty(), LOCTEXT("AddEntryPoint", "Add Entry Point..."), LOCTEXT("AddEntryPointTooltip", "Define State Machine's Entry Point"));
			Action->NodeTemplate = NewObject<UBeginTaskEdGraphNode>(ContextMenuBuilder.OwnerOfTemporaries);
		}
	}
}

void UTaskBuilderGraphSchema::CreateDefaultNodesForGraph(UEdGraph& Graph) const
{
	if (UTaskBuilderEdGraph* CurrentGraph = CastChecked<UTaskBuilderEdGraph>(&Graph))
	{
		if (CurrentGraph->EdGraphNode == NULL)
		{
			FGraphNodeCreator<UBeginTaskEdGraphNode> NodeCreator(Graph);
			UBeginTaskEdGraphNode* NewNode = NodeCreator.CreateNode();
			NodeCreator.Finalize();
			SetNodeMetaData(NewNode, FNodeMetadata::DefaultGraphNode);
			CurrentGraph->EdGraphNode = NewNode;
		}
	}
}

#undef LOCTEXT_NAMESPACE