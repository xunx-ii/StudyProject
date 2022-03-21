#pragma once
#include "TaskBuilderEditor.h"
#include "Graphs/TaskBuilderEdGraph.h"
#include "WorkflowOrientedApp/WorkflowTabFactory.h"
#include "WorkflowOrientedApp/WorkflowUObjectDocuments.h"


struct FTaskBuilderGraphTabFactory : public FDocumentTabFactoryForObjects<UTaskBuilderEdGraph>
{
public:
	FTaskBuilderGraphTabFactory(TSharedPtr<FTaskBuilderEditor> InTaskBuilderEditor);
	virtual void OnTabActivated(TSharedPtr<SDockTab> Tab) const override;
	virtual void OnTabRefreshed(TSharedPtr<SDockTab> Tab) const override;

protected:
	virtual TAttribute<FText> ConstructTabNameForObject(UTaskBuilderEdGraph* DocumentID) const;
	virtual TSharedRef<SWidget> CreateTabBodyForObject(const FWorkflowTabSpawnInfo& Info, UTaskBuilderEdGraph* DocumentID) const;
	virtual const FSlateBrush* GetTabIconForObject(const FWorkflowTabSpawnInfo& Info, UTaskBuilderEdGraph* DocumentID) const;

private:
	TWeakPtr<FTaskBuilderEditor> TaskBuilderEditor;
};