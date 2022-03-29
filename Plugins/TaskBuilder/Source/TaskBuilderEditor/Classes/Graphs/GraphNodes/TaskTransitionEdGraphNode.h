// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EdGraph/EdGraphNode.h"
#include "SGraphNode.h"
#include "SubTaskEdGraphNode.h"
#include "TaskTransitionEdGraphNode.generated.h"

/**
 * 
 */
UCLASS()
class TASKBUILDEREDITOR_API UTaskTransitionEdGraphNode : public UEdGraphNode
{
	GENERATED_BODY()
public:

	//~ Begin UObject Interface
	virtual void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override;
	//~ End UObject Interface

	//~ Begin UEdGraphNode Interface
	virtual void AllocateDefaultPins() override;
	virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override;
	virtual FText GetTooltipText() const override;
	virtual FLinearColor GetNodeTitleColor() const override;
	virtual void PinConnectionListChanged(UEdGraphPin* Pin) override;
	virtual bool CanDuplicateNode() const override { return false; }
	virtual void PostPlacedNewNode() override;

	virtual void DestroyNode() override;
	//~ End UEdGraphNode Interface

	virtual UEdGraphPin* GetInputPin() const; // Pins[0];
	virtual UEdGraphPin* GetOutputPin() const; // Pins[1];
	virtual UEdGraphNode* GetPreviousState() const;
	virtual UEdGraphNode* GetNextState() const;


	virtual UEdGraph* GetBoundGraph() const;
	void CreateConnections(USubTaskEdGraphNode* PreviousState, USubTaskEdGraphNode* NextState);
	bool IsBoundGraphShared();

public:
	UPROPERTY()
		bool Bidirectional;

protected:
	void CreateBoundGraph();
	void CreateCustomTransitionGraph();

private:
	UPROPERTY()
		class UEdGraph* CustomTransitionGraph;
	UPROPERTY()
		FString	SharedRulesName;
	UPROPERTY()
		class UEdGraph* BoundGraph;
};

class STaskTransitionEdGraphNode : public SGraphNode
{
public:
	SLATE_BEGIN_ARGS(STaskTransitionEdGraphNode) {}
	SLATE_END_ARGS()

public:
	// Start SGraphNode Interface override
	void Construct(const FArguments& InArgs, UTaskTransitionEdGraphNode* InNode);
	virtual void UpdateGraphNode() override;
	// End SGraphNode Interface override

	//Start SNodePanel::SNode Interface override
	virtual bool RequiresSecondPassLayout() const override;
	virtual void PerformSecondPassLayout(const TMap< UObject*, TSharedRef<SNode> >& NodeToWidgetLookup) const override;
	//End SNodePanel::SNode Interface override

	static FLinearColor StaticGetTransitionColor(UTaskTransitionEdGraphNode* TransNode, bool bIsHovered);

protected:

	FSlateColor GetTransitionColor() const;
	void PositionBetweenTwoNodesWithOffset(const FGeometry& StartGeom, const FGeometry& EndGeom, int32 NodeIndex, int32 MaxNodes) const;
};