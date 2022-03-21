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

void FTaskBuilderEditor::InitTaskBuilderEditor(const EToolkitMode::Type Mode, const TSharedPtr<IToolkitHost>& InitToolkitHost, const TArray<UBlueprint*>& InBlueprints, bool bShouldOpenInDefaultsMode)
{
	InitBlueprintEditor(Mode, InitToolkitHost, InBlueprints, bShouldOpenInDefaultsMode);
}



#undef LOCTEXT_NAMESPACE