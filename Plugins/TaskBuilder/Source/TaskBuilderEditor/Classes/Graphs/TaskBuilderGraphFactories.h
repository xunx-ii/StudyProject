#pragma once
#include "EdGraphUtilities.h"
#include "EdGraph/EdGraphNode.h"
#include "SGraphNode.h"

struct FTaskBuilderGraphNodeFactory : public FGraphPanelNodeFactory
{
	virtual TSharedPtr<class SGraphNode> CreateNode(class UEdGraphNode* InNode) const override;
};