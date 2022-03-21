#include "TaskBuilderActions.h"
#include "TaskBuilderBlueprint.h"
#include "TaskBuilderEditor.h"

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

	TArray<UBlueprint*> InBlueprints;

	for (auto Object : InObjects)
	{
		if (UBlueprint* NewAsset = Cast<UBlueprint>(Object))
		{
			InBlueprints.Add(NewAsset);
		}
	}

	if (InBlueprints.Num() > 0)
	{
		TSharedRef< FTaskBuilderEditor > NewEditor(new FTaskBuilderEditor());
		NewEditor->InitTaskBuilderEditor(Mode, EditWithinLevelEditor, InBlueprints, false);
	}
}

uint32 FTaskBuilderActions::GetCategories()
{
	return TaskBuilderAssetCategory;
}

#undef LOCTEXT_NAMESPACE