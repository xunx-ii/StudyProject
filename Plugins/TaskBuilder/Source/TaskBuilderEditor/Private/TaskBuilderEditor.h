#pragma once

#include "CoreMinimal.h"
#include "Kismet2/BlueprintEditorUtils.h"
#include "Editor/Kismet/Public/BlueprintEditor.h"

class FTaskBuilderEditor : public FBlueprintEditor
{
public:
	virtual FName GetToolkitFName() const override;
	virtual FText GetBaseToolkitName() const override;
	virtual FString GetWorldCentricTabPrefix() const override;
	virtual FLinearColor GetWorldCentricTabColorScale() const override;
	virtual UBlueprint* GetBlueprintObj() const override;

	void InitTaskBuilderEditor(const EToolkitMode::Type Mode, const TSharedPtr<IToolkitHost>& InitToolkitHost, const TArray<UBlueprint*>& InBlueprints, bool bShouldOpenInDefaultsMode);
};

