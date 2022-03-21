#include "TaskBuilderGraphSchema.h"
#include "TaskBuilderGraphSchemaActions.h"
#include "GraphNodes/BeginTaskEdGraphNode.h"
#include "TaskBuilderEdGraph.h"

#define LOCTEXT_NAMESPACE "DialogueBuilderGraphSchema"

void UTaskBuilderGraphSchema::GetGraphContextActions(FGraphContextMenuBuilder& ContextMenuBuilder) const
{
	FText Category = LOCTEXT("AddStateMachine", "Add State Node");
	FText MenuDesc = LOCTEXT("MenuDesc", "State");
	FText ToolTip = LOCTEXT("StateToolTip", "A State Node");
	TSharedPtr<FTaskBuilderGraphSchemaActions> NewStateNode(new FTaskBuilderGraphSchemaActions(Category, MenuDesc, ToolTip, 0));
	ContextMenuBuilder.AddAction(NewStateNode);
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