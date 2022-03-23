#pragma once

#include "CoreMinimal.h"
#include "EdGraph/EdGraphSchema.h"
#include "TaskBuilderGraphSchemaActions.generated.h"

USTRUCT()
struct TASKBUILDEREDITOR_API FTaskBuilderGraphSchemaActions : public FEdGraphSchemaAction
{
	GENERATED_USTRUCT_BODY()
public:
	FTaskBuilderGraphSchemaActions()
		: FEdGraphSchemaAction(),
		NodeTemplate(nullptr)
	{};

	FTaskBuilderGraphSchemaActions(FText InNodeCategory, FText InMenuDesc, FText InToolTip, const int32 InGrouping)
		: FEdGraphSchemaAction(MoveTemp(InNodeCategory), MoveTemp(InMenuDesc), MoveTemp(InToolTip), InGrouping)
		, NodeTemplate(nullptr)
	{}

	virtual UEdGraphNode* PerformAction(class UEdGraph* ParentGraph, UEdGraphPin* FromPin, const FVector2D Location, bool bSelectNewNode = true) override;
	virtual void AddReferencedObjects(FReferenceCollector& Collector) override;

	template <typename NodeType>
	static NodeType* SpawnNodeFromTemplate(class UEdGraph* ParentGraph, NodeType* InTemplateNode, const FVector2D Location = FVector2D(0.0f, 0.0f), bool bSelectNewNode = true)
	{
		FTaskBuilderGraphSchemaActions Action;
		Action.NodeTemplate = InTemplateNode;

		return Cast<NodeType>(Action.PerformAction(ParentGraph, NULL, Location, bSelectNewNode));
	}
public:
	UEdGraphNode* NodeTemplate;
};