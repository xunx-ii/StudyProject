#pragma once

#include "Toolkits/IToolkitHost.h"
#include "AssetTypeActions_Base.h"

class FTaskBuilderActions : public FAssetTypeActions_Base
{
public:
	FTaskBuilderActions(EAssetTypeCategories::Type AsstCategory);
	~FTaskBuilderActions();

	virtual FText GetName() const override;
	virtual FColor GetTypeColor() const override;
	virtual UClass* GetSupportedClass() const override;
	virtual void OpenAssetEditor(const TArray<UObject*>& InObjects, TSharedPtr<class IToolkitHost> EditWithinLevelEditor = TSharedPtr<IToolkitHost>()) override;
	virtual uint32 GetCategories() override;

private:
	EAssetTypeCategories::Type TaskBuilderAssetCategory;
};