#pragma once

#include "CoreMinimal.h"
#include "EdGraph/EdGraphSchema.h"
#include "TaskBuilderGraphSchemaActions.generated.h"

USTRUCT()
struct TASKBUILDEREDITOR_API FTaskBuilderGraphSchemaActions : public FEdGraphSchemaAction
{
	GENERATED_USTRUCT_BODY()
public:

	FTaskBuilderGraphSchemaActions() {};

	FTaskBuilderGraphSchemaActions(FText InNodeCategory, FText InMenuDesc, FText InToolTip, const int32 InGrouping)
		: FEdGraphSchemaAction(MoveTemp(InNodeCategory), MoveTemp(InMenuDesc), MoveTemp(InToolTip), InGrouping) {}

	// Click the Action call, a node will be created;
	virtual UEdGraphNode* PerformAction(class UEdGraph* ParentGraph, UEdGraphPin* FromPin, const FVector2D Location, bool bSelectNewNode = true) override;
};