#include "TaskBuilderGraphFactories.h"
#include "GraphNodes/BeginTaskEdGraphNode.h"
#include "GraphNodes/SubTaskEdGraphNode.h"
#include "TaskBuilderConnectionDrawingPolicy.h"
#include "TaskBuilderGraphSchema.h"
#include "GraphNodes/TaskTransitionEdGraphNode.h"

TSharedPtr<class SGraphNode> FTaskBuilderGraphNodeFactory::CreateNode(class UEdGraphNode* InNode) const
{
	if (UBeginTaskEdGraphNode* BeginTaskNode = Cast<UBeginTaskEdGraphNode>(InNode))
	{
		return SNew(SBeginTaskGraphNode, BeginTaskNode);
	}
	else if (USubTaskEdGraphNode* SubTaskNode = Cast<USubTaskEdGraphNode>(InNode))
	{
		return SNew(SSubTaskEdGraphNode, SubTaskNode);
	}
	else if (UTaskTransitionEdGraphNode* TaskTransitionNode = Cast<UTaskTransitionEdGraphNode>(InNode))
	{
		return SNew(STaskTransitionEdGraphNode, TaskTransitionNode);
	}

	return nullptr;
}

class FConnectionDrawingPolicy* FTaskBuilderGraphPinConnectionFactory::CreateConnectionPolicy(const class UEdGraphSchema* Schema, int32 InBackLayerID, int32 InFrontLayerID, float ZoomFactor, const class FSlateRect& InClippingRect, class FSlateWindowElementList& InDrawElements, class UEdGraph* InGraphObj) const
{
	if (Schema->IsA(UTaskBuilderGraphSchema::StaticClass()))
	{
		return new FTaskBuilderConnectionDrawingPolicy(InBackLayerID, InFrontLayerID, ZoomFactor, InClippingRect, InDrawElements, InGraphObj);
	}
	return nullptr;
}
