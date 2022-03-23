// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EdGraph/EdGraphNode.h"
#include "SGraphPin.h"
#include "SGraphNode.h"
#include "BeginTaskEdGraphNode.generated.h"


UCLASS()
class TASKBUILDEREDITOR_API UBeginTaskEdGraphNode : public UEdGraphNode
{
	GENERATED_BODY()
public:
	//~ Begin UEdGraphNode Interface
	virtual void AllocateDefaultPins() override;
	virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override;
	virtual FText GetTooltipText() const override;
	//~ End UEdGraphNode Interface

	UEdGraphNode* GetOutputNode() const;
};

class SBeginTaskGraphPin : public SGraphPin
{
public:
	SLATE_BEGIN_ARGS(SBeginTaskGraphPin) {}
	SLATE_END_ARGS()

public:
	void Construct(const FArguments& InArgs, UEdGraphPin* InPin);

protected:
	// Begin SGraphPin interface
	virtual TSharedRef<SWidget>	GetDefaultValueWidget() override;
	virtual FSlateColor GetPinColor() const override;
	// End SGraphPin interface

	const FSlateBrush* GetPinBorder() const;
};


class SBeginTaskGraphNode : public SGraphNode
{
public:
	SLATE_BEGIN_ARGS(SBeginTaskGraphNode) {}
	SLATE_END_ARGS()

public:
	void Construct(const FArguments& InArgs, UBeginTaskEdGraphNode* InNode);

	// SGraphNode interface
	virtual void UpdateGraphNode() override;
	virtual void AddPin(const TSharedRef<SGraphPin>& PinToAdd) override;
	// End of SGraphNode interface

protected:
	FSlateColor GetBorderBackgroundColor() const;
	FText GetPreviewCornerText() const;
};

