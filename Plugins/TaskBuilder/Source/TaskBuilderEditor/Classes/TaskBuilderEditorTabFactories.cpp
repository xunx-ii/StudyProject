#include "TaskBuilderEditorTabFactories.h"

#define LOCTEXT_NAMESPACE "TaskBuilderEditor"



FTaskBuilderGraphTabFactory::FTaskBuilderGraphTabFactory(TSharedPtr<FTaskBuilderEditor> InTaskBuilderEditor)
	:FDocumentTabFactoryForObjects<UTaskBuilderGraph>(FName("DialogueBuilderGraph"), InTaskBuilderEditor)
{

}

void FTaskBuilderGraphTabFactory::OnTabActivated(TSharedPtr<SDockTab> Tab) const
{
	check(TaskBuilderEditor.IsValid());
	TSharedRef<SGraphEditor> TaskBuilderGraphEditor = StaticCastSharedRef<SGraphEditor>(Tab->GetContent());
	TaskBuilderEditor.Pin()->OnGraphEditorFocused(TaskBuilderGraphEditor);
}

void FTaskBuilderGraphTabFactory::OnTabRefreshed(TSharedPtr<SDockTab> Tab) const
{
	TSharedRef<SGraphEditor> TaskBuilderGraphEditor = StaticCastSharedRef<SGraphEditor>(Tab->GetContent());
	TaskBuilderGraphEditor->NotifyGraphChanged();
}

TAttribute<FText> FTaskBuilderGraphTabFactory::ConstructTabNameForObject(UTaskBuilderGraph* DocumentID) const
{
	FString Name = "TaskGraph";
	return TAttribute<FText>(FText::FromString(Name));
}

TSharedRef<SWidget> FTaskBuilderGraphTabFactory::CreateTabBodyForObject(const FWorkflowTabSpawnInfo& Info, UTaskBuilderGraph* DocumentID) const
{
	check(DocumentID);

	SGraphEditor::FGraphEditorEvents InEvents;

	FGraphAppearanceInfo AppearanceInfo;

	// Make title bar
	TSharedRef<SWidget> TitleBarWidget =
		SNew(SBorder)
		.BorderImage(FEditorStyle::GetBrush(TEXT("Graph.TitleBackground")))
		.HAlign(HAlign_Fill)
		[
			SNew(SHorizontalBox)
			+ SHorizontalBox::Slot()
		.HAlign(HAlign_Center)
		.FillWidth(1.f)
		[
			SNew(STextBlock)
			.Text(LOCTEXT("TestGraphLabel", "Test"))
		.TextStyle(FEditorStyle::Get(), TEXT("GraphBreadcrumbButtonText"))
		]
		];

	// Make full graph editor
	//const bool bGraphIsEditable = InGraph->bEditable;
	return SNew(SGraphEditor)
		//.AdditionalCommands(GraphEditorCommands)
		.IsEditable(true)
		.Appearance(AppearanceInfo)
		.TitleBar(TitleBarWidget)
		.GraphToEdit(DocumentID)
		.GraphEvents(InEvents);
}

const FSlateBrush* FTaskBuilderGraphTabFactory::GetTabIconForObject(const FWorkflowTabSpawnInfo& Info, UTaskBuilderGraph* DocumentID) const
{
	return  FEditorStyle::GetBrush(TEXT("Graph.StateNode.Icon"));
}

#undef LOCTEXT_NAMESPACE