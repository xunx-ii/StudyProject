#pragma once
#include "TaskBuilderEditor.h"
#include "Graphs/TaskBuilderGraph.h"
#include "WorkflowOrientedApp/WorkflowTabFactory.h"
#include "WorkflowOrientedApp/WorkflowUObjectDocuments.h"


struct FTaskBuilderGraphTabFactory : public FDocumentTabFactoryForObjects<UTaskBuilderGraph>
{
public:
	FTaskBuilderGraphTabFactory(TSharedPtr<FTaskBuilderEditor> InTaskBuilderEditor);
	virtual void OnTabActivated(TSharedPtr<SDockTab> Tab) const override;
	virtual void OnTabRefreshed(TSharedPtr<SDockTab> Tab) const override;

protected:
	virtual TAttribute<FText> ConstructTabNameForObject(UTaskBuilderGraph* DocumentID) const;
	virtual TSharedRef<SWidget> CreateTabBodyForObject(const FWorkflowTabSpawnInfo& Info, UTaskBuilderGraph* DocumentID) const;
	virtual const FSlateBrush* GetTabIconForObject(const FWorkflowTabSpawnInfo& Info, UTaskBuilderGraph* DocumentID) const;

private:
	TWeakPtr<FTaskBuilderEditor> TaskBuilderEditor;
};