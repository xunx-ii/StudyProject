
#pragma once

#include "CoreMinimal.h"
#include "EdGraph/EdGraphSchema.h"
#include "TaskBuilderGraphSchema.generated.h"


UCLASS(MinimalAPI)
class UTaskBuilderGraphSchema : public UEdGraphSchema
{
	GENERATED_BODY()
private:
	void GetGraphContextActions(FGraphContextMenuBuilder& ContextMenuBuilder) const override;
	void CreateDefaultNodesForGraph(UEdGraph& Graph) const override;

};