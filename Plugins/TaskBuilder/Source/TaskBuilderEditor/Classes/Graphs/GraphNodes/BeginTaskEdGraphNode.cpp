// Fill out your copyright notice in the Description page of Project Settings.


#include "Graphs/GraphNodes/BeginTaskEdGraphNode.h"
#include "Graphs/TaskBuilderEdGraph.h"
#include "Widgets/Text/SInlineEditableTextBlock.h"

#define LOCTEXT_NAMESPACE "BeginTaskEdGraphNode"

//~ Begin UBeginTaskEdGraphNode


void UBeginTaskEdGraphNode::AllocateDefaultPins()
{
	CreatePin(EGPD_Output, FName("BeginTaskGraphPin"), FName("Begin")); //Pins[0]	
}

FText UBeginTaskEdGraphNode::GetNodeTitle(ENodeTitleType::Type TitleType) const
{
	UEdGraph* Graph = GetGraph();
	return Graph ? FText::FromString(Graph->GetName()) : FText::FromString(TEXT("BeginTaskEdGraphNode"));
}

FText UBeginTaskEdGraphNode::GetTooltipText() const
{
	return LOCTEXT("BeginTaskEdGraphNodeTooltip", "Begin Task");
}

UEdGraphNode* UBeginTaskEdGraphNode::GetOutputNode() const
{
	if (Pins.Num() > 0 && Pins[0] != NULL)
	{
		check(Pins[0]->LinkedTo.Num() <= 1);
		if (Pins[0]->LinkedTo.Num() > 0 && Pins[0]->LinkedTo[0]->GetOwningNode() != NULL)
		{
			return Pins[0]->LinkedTo[0]->GetOwningNode();
		}
	}
	return NULL;
}

#undef LOCTEXT_NAMESPACE
//~ End UBeginTaskEdGraphNode

/** Begin SBeginTaskGraphPin */
void SBeginTaskGraphPin::Construct(const FArguments& InArgs, UEdGraphPin* InPin)
{
	this->SetCursor(EMouseCursor::Default);

	typedef SBeginTaskGraphPin ThisClass;

	bShowLabel = true;

	GraphPinObj = InPin;
	check(GraphPinObj != NULL);

	// Set up a hover for pins that is tinted the color of the pin.
	SBorder::Construct(SBorder::FArguments()
		.BorderImage(this, &SBeginTaskGraphPin::GetPinBorder)
		.BorderBackgroundColor(this, &SBeginTaskGraphPin::GetPinColor)
		.OnMouseButtonDown(this, &ThisClass::OnPinMouseDown)
		.Cursor(this, &ThisClass::GetPinCursor)
	);
}

TSharedRef<SWidget> SBeginTaskGraphPin::GetDefaultValueWidget()
{
	return SNew(STextBlock);
}

FSlateColor SBeginTaskGraphPin::GetPinColor() const
{
	return FSlateColor(FLinearColor(1.0f, 1.0f, 0.15f));
}

const FSlateBrush* SBeginTaskGraphPin::GetPinBorder() const
{
	return (IsHovered())
		? FEditorStyle::GetBrush(TEXT("Graph.StateNode.Pin.BackgroundHovered"))
		: FEditorStyle::GetBrush(TEXT("Graph.StateNode.Pin.Background"));
}
/** End SBeginTaskGraphPin */

/** Begin SBeginTaskGraphNode */
void SBeginTaskGraphNode::Construct(const FArguments& InArgs, UBeginTaskEdGraphNode* InNode)
{
	this->GraphNode = InNode;
	this->SetCursor(EMouseCursor::CardinalCross);
	this->UpdateGraphNode();
}

void SBeginTaskGraphNode::UpdateGraphNode()
{
	InputPins.Empty();
	OutputPins.Empty();

	// Reset variables that are going to be exposed, in case we are refreshing an already setup node.
	RightNodeBox.Reset();
	LeftNodeBox.Reset();


	FLinearColor TitleShadowColor(0.6f, 0.6f, 0.6f);

	this->ContentScale.Bind(this, &SGraphNode::GetContentScale);
	this->GetOrAddSlot(ENodeZone::Center)
		.HAlign(HAlign_Center)
		.VAlign(VAlign_Center)
		[
			SNew(SBorder)
			.BorderImage(FEditorStyle::GetBrush("Graph.StateNode.Body"))
		.Padding(0)
		.BorderBackgroundColor(this, &SBeginTaskGraphNode::GetBorderBackgroundColor)
		[
			SNew(SOverlay)

			// PIN AREA
		+ SOverlay::Slot()
		.HAlign(HAlign_Fill)
		.VAlign(VAlign_Fill)
		.Padding(10.0f)
		[
			SAssignNew(RightNodeBox, SVerticalBox)
		]
		]
		];

	CreatePinWidgets();
}

void SBeginTaskGraphNode::AddPin(const TSharedRef<SGraphPin>& PinToAdd)
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

FSlateColor SBeginTaskGraphNode::GetBorderBackgroundColor() const
{
	FLinearColor StateColor(0.08f, 0.08f, 0.08f);
	return StateColor;
}

FText SBeginTaskGraphNode::GetPreviewCornerText() const
{
	return NSLOCTEXT("SBeginTaskGraphNode", "TextDescription", "Begin Task Graph Node");
}

/** End SBeginTaskGraphNode */