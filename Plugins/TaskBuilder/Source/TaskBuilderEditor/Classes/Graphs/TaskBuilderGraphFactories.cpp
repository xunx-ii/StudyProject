#include "TaskBuilderGraphFactories.h"
#include "GraphNodes/BeginTaskEdGraphNode.h"
#include "GraphNodes/SubTaskEdGraphNode.h"

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

	return nullptr;
}

