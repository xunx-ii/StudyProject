// Fill out your copyright notice in the Description page of Project Settings.


#include "Graphs/GraphNodes/SubTaskEdGraphNode.h"
#include "Widgets/Text/SInlineEditableTextBlock.h"
#include "IDocumentation.h"
#include "SGraphPreviewer.h"
#include "Kismet2/Kismet2NameValidators.h"
#include "Graphs/TaskBuilderEdGraph.h"

#define LOCTEXT_NAMESPACE "SubTaskEdGraphNode"

/////////////////////////////////////////////////////
// FAnimStateNodeNameValidator

class FSubTaskNodeNameValidator : public FStringSetNameValidator
{
public:
	FSubTaskNodeNameValidator(const USubTaskEdGraphNode* InTaskNode)
		: FStringSetNameValidator(FString())
	{
		TArray<USubTaskEdGraphNode*> Nodes;
		UTaskBuilderEdGraph* TaskBuilderEdGraph = CastChecked<UTaskBuilderEdGraph>(InTaskNode->GetOuter());

		TaskBuilderEdGraph->GetNodesOfClass<USubTaskEdGraphNode>(Nodes);
		for (auto NodeIt = Nodes.CreateIterator(); NodeIt; ++NodeIt)
		{
			auto Node = *NodeIt;
			if (Node != InTaskNode)
			{
				Names.Add(Node->GetTaskNodeName());
			}
		}
	}
};

/////////////////////////////////////////////////////
// SSubTaskEdGraphNodePin
class SSubTaskEdGraphNodePin : public SGraphPin
{
public:
	SLATE_BEGIN_ARGS(SSubTaskEdGraphNodePin) {}
	SLATE_END_ARGS()

		void Construct(const FArguments& InArgs, UEdGraphPin* InPin);
protected:
	// Begin SGraphPin interface
	virtual TSharedRef<SWidget>	GetDefaultValueWidget() override;
	// End SGraphPin interface

	const FSlateBrush* GetPinBorder() const;
};

void SSubTaskEdGraphNodePin::Construct(const FArguments& InArgs, UEdGraphPin* InPin)
{
	this->SetCursor(EMouseCursor::Default);

	typedef SSubTaskEdGraphNodePin ThisClass;

	bShowLabel = true;

	GraphPinObj = InPin;
	check(GraphPinObj != NULL);

	const UEdGraphSchema* Schema = GraphPinObj->GetSchema();
	check(Schema);

	// Set up a hover for pins that is tinted the color of the pin.
	SBorder::Construct(SBorder::FArguments()
		.BorderImage(this, &SSubTaskEdGraphNodePin::GetPinBorder)
		.BorderBackgroundColor(this, &ThisClass::GetPinColor)
		.OnMouseButtonDown(this, &ThisClass::OnPinMouseDown)
		.Cursor(this, &ThisClass::GetPinCursor)
	);
}

TSharedRef<SWidget>	SSubTaskEdGraphNodePin::GetDefaultValueWidget()
{
	return SNew(STextBlock);
}

const FSlateBrush* SSubTaskEdGraphNodePin::GetPinBorder() const
{
	return (IsHovered())
		? FEditorStyle::GetBrush(TEXT("Graph.StateNode.Pin.BackgroundHovered"))
		: FEditorStyle::GetBrush(TEXT("Graph.StateNode.Pin.Background"));
}

/////////////////////////////////////////////////////
// USubTaskEdGraphNode
USubTaskEdGraphNode::USubTaskEdGraphNode()
{
	bCanRenameNode = true;
}

void USubTaskEdGraphNode::AllocateDefaultPins()
{
	CreatePin(EGPD_Input, TEXT("Transition"), TEXT("In"));
	CreatePin(EGPD_Output, TEXT("Transition"), TEXT("Out"));
}

void USubTaskEdGraphNode::AutowireNewNode(UEdGraphPin* FromPin)
{
	Super::AutowireNewNode(FromPin);

	if (FromPin)
	{
		if (GetSchema()->TryCreateConnection(FromPin, GetInputPin()))
		{
			FromPin->GetOwningNode()->NodeConnectionListChanged();
		}
	}
}

FText USubTaskEdGraphNode::GetNodeTitle(ENodeTitleType::Type TitleType) const
{
	return FText::FromString(GetTaskNodeName());
}

FText USubTaskEdGraphNode::GetTooltipText() const
{
	return LOCTEXT("SubTaskNodeTooltip", "This is a SubTask");
}

void USubTaskEdGraphNode::PostPasteNode()
{
	Super::PostPasteNode();
}

void USubTaskEdGraphNode::PostPlacedNewNode()
{
	Super::PostPlacedNewNode();
}

void USubTaskEdGraphNode::DestroyNode()
{
	Super::DestroyNode();
}

void USubTaskEdGraphNode::ValidateNodeDuringCompilation(class FCompilerResultsLog& MessageLog) const
{
	Super::ValidateNodeDuringCompilation(MessageLog);
}

TSharedPtr<class INameValidatorInterface> USubTaskEdGraphNode::MakeNameValidator() const
{
	return MakeShareable(new FSubTaskNodeNameValidator(this));
}

UEdGraphPin* USubTaskEdGraphNode::GetInputPin() const
{
	return Pins[0];
}

UEdGraphPin* USubTaskEdGraphNode::GetOutputPin() const
{
	return Pins[1];
}

FString USubTaskEdGraphNode::GetTaskNodeName() const
{
	return FString(TEXT(""));
}

/////////////////////////////////////////////////////
// SSubTaskEdGraphNode
void SSubTaskEdGraphNode::Construct(const FArguments& InArgs, USubTaskEdGraphNode* InNode)
{
	this->GraphNode = InNode;
	this->SetCursor(EMouseCursor::CardinalCross);
	this->UpdateGraphNode();
}

void SSubTaskEdGraphNode::UpdateGraphNode()
{
	InputPins.Empty();
	OutputPins.Empty();

	// Reset variables that are going to be exposed, in case we are refreshing an already setup node.
	RightNodeBox.Reset();
	LeftNodeBox.Reset();

	const FSlateBrush* NodeTypeIcon = GetNameIcon();

	FLinearColor TitleShadowColor(0.6f, 0.6f, 0.6f);
	TSharedPtr<SErrorText> ErrorText;
	TSharedPtr<SNodeTitle> NodeTitle = SNew(SNodeTitle, GraphNode);

	this->ContentScale.Bind(this, &SGraphNode::GetContentScale);
	this->GetOrAddSlot(ENodeZone::Center)
		.HAlign(HAlign_Center)
		.VAlign(VAlign_Center)
		[
			SNew(SBorder)
			.BorderImage(FEditorStyle::GetBrush("Graph.StateNode.Body"))
		.Padding(0)
		.BorderBackgroundColor(this, &SSubTaskEdGraphNode::GetBorderBackgroundColor)
		[
			SNew(SOverlay)

			// PIN AREA
		+ SOverlay::Slot()
		.HAlign(HAlign_Fill)
		.VAlign(VAlign_Fill)
		[
			SAssignNew(RightNodeBox, SVerticalBox)
		]

	// STATE NAME AREA
	+ SOverlay::Slot()
		.HAlign(HAlign_Center)
		.VAlign(VAlign_Center)
		.Padding(10.0f)
		[
			SNew(SBorder)
			.BorderImage(FEditorStyle::GetBrush("Graph.StateNode.ColorSpill"))
		.BorderBackgroundColor(TitleShadowColor)
		.HAlign(HAlign_Center)
		.VAlign(VAlign_Center)
		.Visibility(EVisibility::SelfHitTestInvisible)
		[
			SNew(SHorizontalBox)
			+ SHorizontalBox::Slot()
		.AutoWidth()
		[
			// POPUP ERROR MESSAGE
			SAssignNew(ErrorText, SErrorText)
			.BackgroundColor(this, &SSubTaskEdGraphNode::GetErrorColor)
		.ToolTipText(this, &SSubTaskEdGraphNode::GetErrorMsgToolTip)
		]
	+ SHorizontalBox::Slot()
		.AutoWidth()
		.VAlign(VAlign_Center)
		[
			SNew(SImage)
			.Image(NodeTypeIcon)
		]
	+ SHorizontalBox::Slot()
		.Padding(FMargin(4.0f, 0.0f, 4.0f, 0.0f))
		[
			SNew(SVerticalBox)
			+ SVerticalBox::Slot()
		.AutoHeight()
		[
			SAssignNew(InlineEditableText, SInlineEditableTextBlock)
			.Style(FEditorStyle::Get(), "Graph.StateNode.NodeTitleInlineEditableText")
		.Text(NodeTitle.Get(), &SNodeTitle::GetHeadTitle)
		.OnVerifyTextChanged(this, &SSubTaskEdGraphNode::OnVerifyNameTextChanged)
		.OnTextCommitted(this, &SSubTaskEdGraphNode::OnNameTextCommited)
		.IsReadOnly(this, &SSubTaskEdGraphNode::IsNameReadOnly)
		.IsSelected(this, &SSubTaskEdGraphNode::IsSelectedExclusively)
		]
	+ SVerticalBox::Slot()
		.AutoHeight()
		[
			NodeTitle.ToSharedRef()
		]
		]
		]
		]
		]
		];

	ErrorReporting = ErrorText;
	ErrorReporting->SetError(ErrorMsg);
	CreatePinWidgets();
}

void SSubTaskEdGraphNode::CreatePinWidgets()
{
	USubTaskEdGraphNode* TaskNode = CastChecked<USubTaskEdGraphNode>(GraphNode);

	UEdGraphPin* CurPin = TaskNode->GetOutputPin();
	if (!CurPin->bHidden)
	{
		TSharedPtr<SGraphPin> NewPin = SNew(SSubTaskEdGraphNodePin, CurPin);
		this->AddPin(NewPin.ToSharedRef());
	}
}

void SSubTaskEdGraphNode::AddPin(const TSharedRef<SGraphPin>& PinToAdd)
{
	PinToAdd->SetOwner(SharedThis(this));
	RightNodeBox->AddSlot()
		.HAlign(HAlign_Fill)
		.VAlign(VAlign_Fill)
		.FillHeight(1.0f)
		[
			PinToAdd
		];
	OutputPins.Add(PinToAdd);
}


FSlateColor SSubTaskEdGraphNode::GetBorderBackgroundColor() const
{
	FLinearColor InactiveColor(0.08f, 0.08f, 0.08f);
	return InactiveColor;
}

FText SSubTaskEdGraphNode::GetPreviewCornerText() const
{
	USubTaskEdGraphNode* TaskNode = CastChecked<USubTaskEdGraphNode>(GraphNode);

	return FText::Format(NSLOCTEXT("SSubTaskEdGraphNode", "PreviewTaskNodeText", "{0} Node"), FText::FromString(TaskNode->GetTaskNodeName()));
}

const FSlateBrush* SSubTaskEdGraphNode::GetNameIcon() const
{
	return FEditorStyle::GetBrush(TEXT("Graph.ConduitNode.Icon"));
}

#undef LOCTEXT_NAMESPACE