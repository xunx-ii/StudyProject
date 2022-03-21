#include "TaskBuilderGraphSchema.h"
#include "TaskBuilderGraphSchemaActions.h"

#define LOCTEXT_NAMESPACE "DialogueBuilderGraphSchema"

void UTaskBuilderGraphSchema::GetGraphContextActions(FGraphContextMenuBuilder& ContextMenuBuilder) const
{
	FText Category = LOCTEXT("AddStateMachine", "Add State Node");
	FText MenuDesc = LOCTEXT("MenuDesc", "State");
	FText ToolTip = LOCTEXT("StateToolTip", "A State Node");
	TSharedPtr<FTaskBuilderGraphSchemaActions> NewStateNode(new FTaskBuilderGraphSchemaActions(Category, MenuDesc, ToolTip, 0));
	ContextMenuBuilder.AddAction(NewStateNode);
}

#undef LOCTEXT_NAMESPACE