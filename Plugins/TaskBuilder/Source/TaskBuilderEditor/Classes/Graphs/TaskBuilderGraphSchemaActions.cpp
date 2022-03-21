#include "TaskBuilderGraphSchemaActions.h"
#include "EdGraph/EdGraphNode.h"

UEdGraphNode* FTaskBuilderGraphSchemaActions::PerformAction(class UEdGraph* ParentGraph, UEdGraphPin* FromPin, const FVector2D Location, bool bSelectNewNode /*= true*/)
{
	if (ParentGraph == NULL)
	{
		return NULL;
	}

	// Create a system default node temporarily
	UEdGraphNode* ResultNode = NewObject<UEdGraphNode>(ParentGraph);
	ParentGraph->Modify();
	//ResultNode->SetFlags(RF_Transactional);
	//ResultNode->Rename(NULL, ParentGraph, REN_NonTransactional);
	ParentGraph->bEditable = true;
	ParentGraph->bAllowRenaming = true;
	ParentGraph->AddNode(ResultNode, bSelectNewNode);
	ResultNode->CreateNewGuid();
	ResultNode->NodePosX = Location.X;
	ResultNode->NodePosY = Location.Y;



	ResultNode->AllocateDefaultPins();
	ResultNode->AutowireNewNode(FromPin);

	ParentGraph->NotifyGraphChanged();

	return ResultNode;
}

