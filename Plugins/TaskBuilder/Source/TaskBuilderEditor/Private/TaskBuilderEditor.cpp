#include "TaskBuilderEditor.h"
#include "EditorReimportHandler.h"

#if WITH_EDITOR
#include "Editor.h"
#endif

#include "TaskBuilderBlueprint.h"
#include "Kismet2/BlueprintEditorUtils.h"


#define LOCTEXT_NAMESPACE "TaskBuilderEditor"

FName FTaskBuilderEditor::GetToolkitFName() const
{
	return FName("TaskBuilderEditor");
}

FText FTaskBuilderEditor::GetBaseToolkitName() const
{
	return LOCTEXT("TaskBuilderEditor", "Task Builder Editor");
}

FString FTaskBuilderEditor::GetWorldCentricTabPrefix() const
{
	return FString(TEXT(""));
}

FLinearColor FTaskBuilderEditor::GetWorldCentricTabColorScale() const
{
	return FLinearColor();
}

UBlueprint* FTaskBuilderEditor::GetBlueprintObj() const
{
	const TArray<UObject*>& EditingObjs = GetEditingObjects();
	for (int32 i = 0; i < EditingObjs.Num(); ++i)
	{
		if (EditingObjs[i]->IsA<UTaskBuilderBlueprint>())
		{
			return (UBlueprint*)EditingObjs[i];
		}
	}
	return nullptr;
}

void FTaskBuilderEditor::InitTaskBuilderEditor(const EToolkitMode::Type Mode, const TSharedPtr<IToolkitHost>& InitToolkitHost, const TArray<UBlueprint*>& InBlueprints, bool bShouldOpenInDefaultsMode)
{
	InitBlueprintEditor(Mode, InitToolkitHost, InBlueprints, bShouldOpenInDefaultsMode);
}



#undef LOCTEXT_NAMESPACE