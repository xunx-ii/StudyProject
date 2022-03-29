// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"
#include "TaskBuilderEditor.h"

class FTaskBuilderEditorModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

	TSharedRef<FTaskBuilderEditor> CreateTaskBuilderEditor(const EToolkitMode::Type Mode, const TSharedPtr< class IToolkitHost >& InitToolkitHost, class UTaskBuilderBlueprint* Blueprint);
private:
	void OnNewBlueprintCreated(UBlueprint* InBlueprint);
	void OnPostEngineInit();
};
