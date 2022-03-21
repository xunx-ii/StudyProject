#include "TaskBuilderGraphFactories.h"
#include "GraphNodes/BeginTaskEdGraphNode.h"

TSharedPtr<class SGraphNode> FTaskBuilderGraphNodeFactory::CreateNode(class UEdGraphNode* InNode) const
{
	if (UBeginTaskEdGraphNode* NewNode = Cast<UBeginTaskEdGraphNode>(InNode))
	{
		return SNew(SBeginTaskGraphNode, NewNode);
	}

	return nullptr;
}

