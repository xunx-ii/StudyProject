// Fill out your copyright notice in the Description page of Project Settings.


#include "Graphs/GraphNodes/TaskTransitionEdGraphNode.h"
#include "SubTaskEdGraphNode.h"
#include "Kismet2/BlueprintEditorUtils.h"
#include "ConnectionDrawingPolicy.h"

#define LOCTEXT_NAMESPACE "TaskTransitionEdGraphNode"

void UTaskTransitionEdGraphNode::PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
}

void UTaskTransitionEdGraphNode::AllocateDefaultPins()
{
	UEdGraphPin* Inputs = CreatePin(EGPD_Input, TEXT("Transition"), TEXT("In"));
	Inputs->bHidden = true;
	UEdGraphPin* Outputs = CreatePin(EGPD_Output, TEXT("Transition"), TEXT("Out"));
	Outputs->bHidden = true;
}

FText UTaskTransitionEdGraphNode::GetNodeTitle(ENodeTitleType::Type TitleType) const
{
	USubTaskEdGraphNode* PrevState = Cast<USubTaskEdGraphNode>(GetPreviousState());
	USubTaskEdGraphNode* NextState = Cast<USubTaskEdGraphNode>(GetNextState());

	if ((PrevState != NULL) && (NextState != NULL))
	{
		FFormatNamedArguments Args;
		Args.Add(TEXT("PrevState"), FText::FromString(PrevState->GetTaskNodeName()));
		Args.Add(TEXT("NextState"), FText::FromString(NextState->GetTaskNodeName()));
		return FText::Format(LOCTEXT("PrevNewState", "{PrevState} to {NextState}"), Args);
	}
	return Super::GetNodeTitle(TitleType);
}

FText UTaskTransitionEdGraphNode::GetTooltipText() const
{
	return LOCTEXT("TaskTransitionTooltip", "This is a task transition");
}

FLinearColor UTaskTransitionEdGraphNode::GetNodeTitleColor() const
{
	return FColorList::Red;
}

void UTaskTransitionEdGraphNode::PinConnectionListChanged(UEdGraphPin* Pin)
{
	if (Pin->LinkedTo.Num() == 0)
	{
		// Commit suicide; transitions must always have an input and output connection
		Modify();

		// Our parent graph will have our graph in SubGraphs so needs to be modified to record that.
		if (UEdGraph* ParentGraph = GetGraph())
		{
			ParentGraph->Modify();
		}

		DestroyNode();
	}
}

void UTaskTransitionEdGraphNode::PostPlacedNewNode()
{

}

void UTaskTransitionEdGraphNode::DestroyNode()
{
	UEdGraph* GraphToRemove = IsBoundGraphShared() ? NULL : GetBoundGraph();

	BoundGraph = NULL;
	Super::DestroyNode();

	if (GraphToRemove)
	{
		UBlueprint* Blueprint = FBlueprintEditorUtils::FindBlueprintForNodeChecked(this);
		FBlueprintEditorUtils::RemoveGraph(Blueprint, GraphToRemove, EGraphRemoveFlags::Recompile);
	}

	if (CustomTransitionGraph)
	{
		UBlueprint* Blueprint = FBlueprintEditorUtils::FindBlueprintForNodeChecked(this);
		FBlueprintEditorUtils::RemoveGraph(Blueprint, CustomTransitionGraph, EGraphRemoveFlags::Recompile);
	}
}

UEdGraphPin* UTaskTransitionEdGraphNode::GetInputPin() const
{
	return Pins[0];
}

UEdGraphPin* UTaskTransitionEdGraphNode::GetOutputPin() const
{
	return Pins[1];
}

UEdGraphNode* UTaskTransitionEdGraphNode::GetPreviousState() const
{
	if (Pins[0]->LinkedTo.Num() > 0)
	{
		return Cast<USubTaskEdGraphNode>(Pins[0]->LinkedTo[0]->GetOwningNode());
	}
	else
	{
		return NULL;
	}
}

UEdGraphNode* UTaskTransitionEdGraphNode::GetNextState() const
{
	if (Pins[1]->LinkedTo.Num() > 0)
	{
		return Cast<USubTaskEdGraphNode>(Pins[1]->LinkedTo[0]->GetOwningNode());
	}
	else
	{
		return NULL;
	}
}

UEdGraph* UTaskTransitionEdGraphNode::GetBoundGraph() const
{
	return BoundGraph;
}

void UTaskTransitionEdGraphNode::CreateConnections(USubTaskEdGraphNode* PreviousState, USubTaskEdGraphNode* NextState)
{
	// Previous to this
	Pins[0]->Modify();
	Pins[0]->LinkedTo.Empty();

	PreviousState->GetOutputPin()->Modify();
	Pins[0]->MakeLinkTo(PreviousState->GetOutputPin());

	// This to next
	Pins[1]->Modify();
	Pins[1]->LinkedTo.Empty();

	NextState->GetInputPin()->Modify();
	Pins[1]->MakeLinkTo(NextState->GetInputPin());
}

bool UTaskTransitionEdGraphNode::IsBoundGraphShared()
{
	if (BoundGraph)
	{
		UEdGraph* ParentGraph = GetGraph();
		for (int32 NodeIdx = 0; NodeIdx < ParentGraph->Nodes.Num(); NodeIdx++)
		{
			UTaskTransitionEdGraphNode* TaskNode = Cast<UTaskTransitionEdGraphNode>(ParentGraph->Nodes[NodeIdx]);
			if ((TaskNode != NULL) && (TaskNode != this) && (TaskNode->GetBoundGraph() == BoundGraph))
			{
				return true;
			}
		}
	}

	return false;
}

void UTaskTransitionEdGraphNode::CreateBoundGraph()
{

}
#undef LOCTEXT_NAMESPACE

/////////////////////////////////////////////////////
// STaskTransitionEdGraphNode
void STaskTransitionEdGraphNode::Construct(const FArguments& InArgs, UTaskTransitionEdGraphNode* InNode)
{
	this->GraphNode = InNode;
	this->UpdateGraphNode();
}

void STaskTransitionEdGraphNode::UpdateGraphNode()
{
	InputPins.Empty();
	OutputPins.Empty();

	// Reset variables that are going to be exposed, in case we are refreshing an already setup node.
	RightNodeBox.Reset();
	LeftNodeBox.Reset();

	this->Visibility = EVisibility::Visible;
	this->ContentScale.Bind(this, &SGraphNode::GetContentScale);
	this->GetOrAddSlot(ENodeZone::Center)
		.HAlign(HAlign_Center)
		.VAlign(VAlign_Center)
		[
			SNew(SOverlay)
			+ SOverlay::Slot()
		[
			SNew(SImage)
			.Image(FEditorStyle::GetBrush("Graph.TransitionNode.ColorSpill"))
		.ColorAndOpacity(this, &STaskTransitionEdGraphNode::GetTransitionColor)
		]
	+ SOverlay::Slot()
		[
			SNew(SImage)
			.Image(FEditorStyle::GetBrush("Graph.TransitionNode.Icon_Inertialization"))
		]
		];
}

bool STaskTransitionEdGraphNode::RequiresSecondPassLayout() const
{
	return true;
}

void STaskTransitionEdGraphNode::PerformSecondPassLayout(const TMap< UObject*, TSharedRef<SNode> >& NodeToWidgetLookup) const
{
	UTaskTransitionEdGraphNode* TransNode = CastChecked<UTaskTransitionEdGraphNode>(GraphNode);

	// Find the geometry of the state nodes we're connecting
	FGeometry StartGeom;
	FGeometry EndGeom;

	int32 TransIndex = 0;
	int32 NumOfTrans = 1;

	UEdGraphNode* PrevState = TransNode->GetPreviousState();
	UEdGraphNode* NextState = TransNode->GetNextState();
	if ((PrevState != NULL) && (NextState != NULL))
	{
		const TSharedRef<SNode>* pPrevNodeWidget = NodeToWidgetLookup.Find(PrevState);
		const TSharedRef<SNode>* pNextNodeWidget = NodeToWidgetLookup.Find(NextState);
		if ((pPrevNodeWidget != NULL) && (pNextNodeWidget != NULL))
		{
			const TSharedRef<SNode>& PrevNodeWidget = *pPrevNodeWidget;
			const TSharedRef<SNode>& NextNodeWidget = *pNextNodeWidget;

			StartGeom = FGeometry(FVector2D(PrevState->NodePosX, PrevState->NodePosY), FVector2D::ZeroVector, PrevNodeWidget->GetDesiredSize(), 1.0f);
			EndGeom = FGeometry(FVector2D(NextState->NodePosX, NextState->NodePosY), FVector2D::ZeroVector, NextNodeWidget->GetDesiredSize(), 1.0f);
		}
	}

	//Position Node
	PositionBetweenTwoNodesWithOffset(StartGeom, EndGeom, TransIndex, NumOfTrans);
}

FLinearColor STaskTransitionEdGraphNode::StaticGetTransitionColor(UTaskTransitionEdGraphNode* TransNode, bool bIsHovered)
{
	FLinearColor HoverColor(0.724f, 0.256f, 0.0f, 1.0f);
	FLinearColor BaseColor(0.9f, 0.9f, 0.9f, 1.0f);

	if (bIsHovered)
	{
		return HoverColor;
	}

	return BaseColor;
}

FSlateColor STaskTransitionEdGraphNode::GetTransitionColor() const
{
	UTaskTransitionEdGraphNode* TransNode = CastChecked<UTaskTransitionEdGraphNode>(GraphNode);
	return StaticGetTransitionColor(TransNode, IsHovered());
}

void STaskTransitionEdGraphNode::PositionBetweenTwoNodesWithOffset(const FGeometry& StartGeom, const FGeometry& EndGeom, int32 NodeIndex, int32 MaxNodes) const
{
	// Get a reasonable seed point (halfway between the boxes)
	const FVector2D StartCenter = FGeometryHelper::CenterOf(StartGeom);
	const FVector2D EndCenter = FGeometryHelper::CenterOf(EndGeom);
	const FVector2D SeedPoint = (StartCenter + EndCenter) * 0.5f;

	// Find the (approximate) closest points between the two boxes
	const FVector2D StartAnchorPoint = FGeometryHelper::FindClosestPointOnGeom(StartGeom, SeedPoint);
	const FVector2D EndAnchorPoint = FGeometryHelper::FindClosestPointOnGeom(EndGeom, SeedPoint);

	// Position ourselves halfway along the connecting line between the nodes, elevated away perpendicular to the direction of the line
	const float Height = 30.0f;

	const FVector2D DesiredNodeSize = GetDesiredSize();

	FVector2D DeltaPos(EndAnchorPoint - StartAnchorPoint);

	if (DeltaPos.IsNearlyZero())
	{
		DeltaPos = FVector2D(10.0f, 0.0f);
	}

	const FVector2D Normal = FVector2D(DeltaPos.Y, -DeltaPos.X).GetSafeNormal();

	const FVector2D NewCenter = StartAnchorPoint + (0.5f * DeltaPos) + (Height * Normal);

	FVector2D DeltaNormal = DeltaPos.GetSafeNormal();

	// Calculate node offset in the case of multiple transitions between the same two nodes
	// MultiNodeOffset: the offset where 0 is the centre of the transition, -1 is 1 <size of node>
	// towards the PrevStateNode and +1 is 1 <size of node> towards the NextStateNode.

	const float MutliNodeSpace = 0.2f; // Space between multiple transition nodes (in units of <size of node> )
	const float MultiNodeStep = (1.f + MutliNodeSpace); //Step between node centres (Size of node + size of node spacer)

	const float MultiNodeStart = -((MaxNodes - 1) * MultiNodeStep) / 2.f;
	const float MultiNodeOffset = MultiNodeStart + (NodeIndex * MultiNodeStep);

	// Now we need to adjust the new center by the node size, zoom factor and multi node offset
	const FVector2D NewCorner = NewCenter - (0.5f * DesiredNodeSize) + (DeltaNormal * MultiNodeOffset * DesiredNodeSize.Size());

	GraphNode->NodePosX = NewCorner.X;
	GraphNode->NodePosY = NewCorner.Y;
}
