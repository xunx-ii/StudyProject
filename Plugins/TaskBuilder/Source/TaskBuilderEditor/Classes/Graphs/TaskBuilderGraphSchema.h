
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
	virtual bool TryCreateConnection(UEdGraphPin* PinA, UEdGraphPin* PinB) const override;
	virtual const FPinConnectionResponse CanCreateConnection(const UEdGraphPin* PinA, const UEdGraphPin* PinB) const override;
	virtual bool CreateAutomaticConversionNodeAndConnections(UEdGraphPin* PinA, UEdGraphPin* PinB) const override;
};