#pragma once
#include "EdGraphUtilities.h"
#include "EdGraph/EdGraphNode.h"
#include "SGraphNode.h"

struct TASKBUILDEREDITOR_API FTaskBuilderGraphNodeFactory : public FGraphPanelNodeFactory
{
	virtual TSharedPtr<class SGraphNode> CreateNode(class UEdGraphNode* InNode) const override;
};

struct TASKBUILDEREDITOR_API FTaskBuilderGraphPinConnectionFactory : public FGraphPanelPinConnectionFactory
{
public:
	virtual class FConnectionDrawingPolicy* CreateConnectionPolicy(const class UEdGraphSchema* Schema, int32 InBackLayerID, int32 InFrontLayerID, float ZoomFactor, const class FSlateRect& InClippingRect, class FSlateWindowElementList& InDrawElements, class UEdGraph* InGraphObj) const override;
};