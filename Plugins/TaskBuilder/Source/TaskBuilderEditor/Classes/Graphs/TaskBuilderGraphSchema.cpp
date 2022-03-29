#include "TaskBuilderGraphSchema.h"
#include "TaskBuilderGraphSchemaActions.h"
#include "GraphNodes/BeginTaskEdGraphNode.h"
#include "TaskBuilderEdGraph.h"
#include "GraphNodes/SubTaskEdGraphNode.h"
#include "GraphNodes/TaskTransitionEdGraphNode.h"
#include "Kismet2/BlueprintEditorUtils.h"


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

	// Add SubTask
	{
		TSharedPtr<FTaskBuilderGraphSchemaActions> Action = AddNewTaskNodeAction(ContextMenuBuilder, FText::GetEmpty(), LOCTEXT("AddSubTask", "Add SubTask..."), LOCTEXT("AddSubTaskTooltip", "A New SubTask"));
		Action->NodeTemplate = NewObject<USubTaskEdGraphNode>(ContextMenuBuilder.OwnerOfTemporaries);
	}

	// Entry point (only if doesn't already exist)
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

			if (UTaskBuilderEdGraph* CurrentGraph = CastChecked<UTaskBuilderEdGraph>(const_cast<UEdGraph*>(ContextMenuBuilder.CurrentGraph)))
			{
				if (CurrentGraph->BeginTaskEdGraphNode == NULL)
				{
					CurrentGraph->BeginTaskEdGraphNode = Action->NodeTemplate;
				}
			}
		}
	}
}

void UTaskBuilderGraphSchema::CreateDefaultNodesForGraph(UEdGraph& Graph) const
{
	if (UTaskBuilderEdGraph* CurrentGraph = CastChecked<UTaskBuilderEdGraph>(&Graph))
	{
		if (CurrentGraph->BeginTaskEdGraphNode == NULL)
		{
			FGraphNodeCreator<UBeginTaskEdGraphNode> NodeCreator(Graph);
			UBeginTaskEdGraphNode* NewNode = NodeCreator.CreateNode();
			NodeCreator.Finalize();
			SetNodeMetaData(NewNode, FNodeMetadata::DefaultGraphNode);
			CurrentGraph->BeginTaskEdGraphNode = NewNode;
		}
	}
}

bool UTaskBuilderGraphSchema::TryCreateConnection(UEdGraphPin* PinA, UEdGraphPin* PinB) const
{
	if (PinB->Direction == PinA->Direction)
	{
		if (USubTaskEdGraphNode* Node = Cast<USubTaskEdGraphNode>(PinB->GetOwningNode()))
		{
			if (PinA->Direction == EGPD_Input)
			{
				PinB = Node->GetOutputPin();
			}
			else
			{
				PinB = Node->GetInputPin();
			}
		}
	}

	const bool bModified = UEdGraphSchema::TryCreateConnection(PinA, PinB);

	if (bModified)
	{
		UBlueprint* Blueprint = FBlueprintEditorUtils::FindBlueprintForNodeChecked(PinA->GetOwningNode());
		FBlueprintEditorUtils::MarkBlueprintAsModified(Blueprint);
	}

	return bModified;
}

const FPinConnectionResponse UTaskBuilderGraphSchema::CanCreateConnection(const UEdGraphPin* PinA, const UEdGraphPin* PinB) const
{
	// Make sure the pins are not on the same node
	if (PinA->GetOwningNode() == PinB->GetOwningNode())
	{
		return FPinConnectionResponse(CONNECT_RESPONSE_DISALLOW, TEXT("Both are on the same node"));
	}

	// Connect entry node to a state is OK
	const bool bPinAIsEntry = PinA->GetOwningNode()->IsA(UBeginTaskEdGraphNode::StaticClass());
	const bool bPinBIsEntry = PinB->GetOwningNode()->IsA(UBeginTaskEdGraphNode::StaticClass());
	const bool bPinAIsStateNode = PinA->GetOwningNode()->IsA(USubTaskEdGraphNode::StaticClass());
	const bool bPinBIsStateNode = PinB->GetOwningNode()->IsA(USubTaskEdGraphNode::StaticClass());

	if (bPinAIsEntry || bPinBIsEntry)
	{
		if (bPinAIsEntry && bPinBIsStateNode)
		{
			return FPinConnectionResponse(CONNECT_RESPONSE_BREAK_OTHERS_A, TEXT("Connect1"));
		}

		if (bPinBIsEntry && bPinAIsStateNode)
		{
			return FPinConnectionResponse(CONNECT_RESPONSE_DISALLOW, TEXT("Can`t Connect"));
		}

		return FPinConnectionResponse(CONNECT_RESPONSE_DISALLOW, TEXT("Entry must connect to a state node"));
	}


	const bool bPinAIsTransition = PinA->GetOwningNode()->IsA(UTaskTransitionEdGraphNode::StaticClass());
	const bool bPinBIsTransition = PinB->GetOwningNode()->IsA(UTaskTransitionEdGraphNode::StaticClass());

	if (bPinAIsTransition && bPinBIsTransition)
	{
		return FPinConnectionResponse(CONNECT_RESPONSE_DISALLOW, TEXT("Cannot wire a transition to a transition"));
	}

	// Compare the directions
	bool bDirectionsOK = false;

	if ((PinA->Direction == EGPD_Input) && (PinB->Direction == EGPD_Output))
	{
		bDirectionsOK = true;
	}
	else if ((PinB->Direction == EGPD_Input) && (PinA->Direction == EGPD_Output))
	{
		bDirectionsOK = true;
	}

	// Transitions are exclusive (both input and output), but states are not
	if (bPinAIsTransition)
	{
		return FPinConnectionResponse(CONNECT_RESPONSE_BREAK_OTHERS_A, TEXT(""));
	}
	else if (bPinBIsTransition)
	{
		return FPinConnectionResponse(CONNECT_RESPONSE_BREAK_OTHERS_B, TEXT(""));
	}
	else if (!bPinAIsTransition && !bPinBIsTransition)
	{
		return FPinConnectionResponse(CONNECT_RESPONSE_MAKE_WITH_CONVERSION_NODE, TEXT("Create a transition"));
	}
	else
	{
		return FPinConnectionResponse(CONNECT_RESPONSE_MAKE, TEXT(""));
	}
}

bool UTaskBuilderGraphSchema::CreateAutomaticConversionNodeAndConnections(UEdGraphPin* PinA, UEdGraphPin* PinB) const
{
	USubTaskEdGraphNode* NodeA = Cast<USubTaskEdGraphNode>(PinA->GetOwningNode());
	USubTaskEdGraphNode* NodeB = Cast<USubTaskEdGraphNode>(PinB->GetOwningNode());

	if ((NodeA != NULL) && (NodeB != NULL)
		&& (NodeA->GetInputPin() != NULL) && (NodeA->GetOutputPin() != NULL)
		&& (NodeB->GetInputPin() != NULL) && (NodeB->GetOutputPin() != NULL))
	{
		UTaskTransitionEdGraphNode* TransitionNode = FTaskBuilderGraphSchemaActions::SpawnNodeFromTemplate<UTaskTransitionEdGraphNode>(NodeA->GetGraph(), NewObject<UTaskTransitionEdGraphNode>(), FVector2D(0.0f, 0.0f), false);

		if (PinA->Direction == EGPD_Output)
		{
			TransitionNode->CreateConnections(NodeA, NodeB);
		}
		else
		{
			TransitionNode->CreateConnections(NodeB, NodeA);
		}

		//UBlueprint* Blueprint = FBlueprintEditorUtils::FindBlueprintForGraphChecked(TransitionNode->GetBoundGraph());
		//FBlueprintEditorUtils::MarkBlueprintAsStructurallyModified(Blueprint);

		return true;
	}

	return false;
}

#undef LOCTEXT_NAMESPACE