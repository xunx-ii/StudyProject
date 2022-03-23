#pragma once

#include "CoreMinimal.h"
#include "Kismet2/BlueprintEditorUtils.h"
#include "Editor/Kismet/Public/BlueprintEditor.h"
#include "WorkflowOrientedApp/WorkflowTabManager.h"
#include "TaskBuilderBlueprint.h"


#define LOCTEXT_NAMESPACE "TaskBuilderEditor"

// Create Struct to set it up to DialogueBuilderApplicationMode;
struct FTaskBuilderEditorModes
{
	// Mode constants
	static const FName TaskBuilderEditorMode;

	static FText GetLocalizedMode(const FName InMode)
	{
		static TMap< FName, FText > LocModes;

		if (LocModes.Num() == 0)
		{
			LocModes.Add(TaskBuilderEditorMode, NSLOCTEXT("TaskBuilderEditorMode", "TaskBuilderEditororMode", "Task Builder Editor Mode"));
		}

		check(InMode != NAME_None);
		const FText* OutDesc = LocModes.Find(InMode);
		check(OutDesc);
		return *OutDesc;
	}
private:
	FTaskBuilderEditorModes() {}
};
#undef LOCTEXT_NAMESPACE

class FTaskBuilderEditor : public FBlueprintEditor
{
public:
	void InitTaskBuilderEditor(const EToolkitMode::Type Mode, const TSharedPtr<IToolkitHost>& InitToolkitHost, const TArray<UBlueprint*>& InBlueprints, bool bShouldOpenInDefaultsMode);

	//~ Begin IToolkit Interface
	virtual FName GetToolkitFName() const override;
	virtual FText GetBaseToolkitName() const override;
	virtual FString GetWorldCentricTabPrefix() const override;
	virtual FLinearColor GetWorldCentricTabColorScale() const override;
	//~ End IToolkit Interface

	virtual void LoadEditorSettings();

	virtual UBlueprint* GetBlueprintObj() const override;

	void InvokeTaskBuilderGraphTab();

private:
	TWeakObjectPtr<UTaskBuilderBlueprint> TaskBuilderBlueprint;
	UEdGraph* TaskBuilderGraph;
};

