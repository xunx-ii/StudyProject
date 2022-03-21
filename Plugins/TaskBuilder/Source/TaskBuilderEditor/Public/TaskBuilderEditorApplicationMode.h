#pragma once

#include "CoreMinimal.h"
#include "BlueprintEditorModes.h"
#include "TaskBuilderEditor.h"
#include "BlueprintEditorTabs.h"
#include "BlueprintEditorModes.h"

class TASKBUILDEREDITOR_API FTaskBuilderEditorApplicationMode : public FBlueprintEditorApplicationMode
{
public:
	// Interface of FBlueprintEditorApplicationMode
	FTaskBuilderEditorApplicationMode(TSharedPtr<FTaskBuilderEditor> InTaskBuilderEditor);
	virtual void RegisterTabFactories(TSharedPtr<FTabManager> InTabManager) override;
	virtual void PostActivateMode() override;

protected:
	TWeakPtr<FTaskBuilderEditor> TaskBuilderEditor;
	FWorkflowAllowedTabSet StardardTabFactories;
};