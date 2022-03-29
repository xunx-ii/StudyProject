#include "TaskBuilderActions.h"
#include "TaskBuilderBlueprint.h"
#include "TaskBuilderEditor.h"
#include "TaskBuilderEditorModule.h"

#define LOCTEXT_NAMESPACE "TaskBuilderActions"

FTaskBuilderActions::FTaskBuilderActions(EAssetTypeCategories::Type AsstCategory)
	:TaskBuilderAssetCategory(AsstCategory)
{

}

FTaskBuilderActions::~FTaskBuilderActions()
{

}

FText FTaskBuilderActions::GetName() const
{
	return LOCTEXT("FTaskBuilderActionsName", "Task Builder");
}

FColor FTaskBuilderActions::GetTypeColor() const
{
	return FColor::Emerald;
}

UClass* FTaskBuilderActions::GetSupportedClass() const
{
	return UTaskBuilderBlueprint::StaticClass();
}

void FTaskBuilderActions::OpenAssetEditor(const TArray<UObject*>& InObjects, TSharedPtr<class IToolkitHost> EditWithinLevelEditor /*= TSharedPtr<IToolkitHost>()*/)
{
	const EToolkitMode::Type Mode = EditWithinLevelEditor.IsValid() ? EToolkitMode::WorldCentric : EToolkitMode::Standalone;

	for (auto ObjIt = InObjects.CreateConstIterator(); ObjIt; ++ObjIt)
	{
		UTaskBuilderBlueprint* TaskBuilderBlueprint = Cast<UTaskBuilderBlueprint>(*ObjIt);
		if (TaskBuilderBlueprint != NULL)
		{
			const bool bBringToFrontIfOpen = true;
#if WITH_EDITOR
			if (IAssetEditorInstance* EditorInstance = GEditor->GetEditorSubsystem<UAssetEditorSubsystem>()->FindEditorForAsset(TaskBuilderBlueprint, bBringToFrontIfOpen))
			{
				EditorInstance->FocusWindow(TaskBuilderBlueprint);
			}
			else
#endif
			{
				FTaskBuilderEditorModule& TaskBuilderEditorModule = FModuleManager::LoadModuleChecked<FTaskBuilderEditorModule>("TaskBuilderEditor");
				TaskBuilderEditorModule.CreateTaskBuilderEditor(Mode, EditWithinLevelEditor, TaskBuilderBlueprint);
			}
		}
	}

}

uint32 FTaskBuilderActions::GetCategories()
{
	return TaskBuilderAssetCategory;
}

#undef LOCTEXT_NAMESPACE