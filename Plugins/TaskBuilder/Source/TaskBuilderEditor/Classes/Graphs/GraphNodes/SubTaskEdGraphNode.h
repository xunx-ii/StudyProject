// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EdGraph/EdGraphNode.h"
#include "SGraphNode.h"
#include "SGraphPin.h"
#include "SubTaskEdGraphNode.generated.h"

/**
 * 
 */
UCLASS()
class TASKBUILDEREDITOR_API USubTaskEdGraphNode : public UEdGraphNode
{
	GENERATED_BODY()
public:
	USubTaskEdGraphNode();

	//~ Begin UEdGraphNode Interface
	virtual void AllocateDefaultPins() override;
	virtual void AutowireNewNode(UEdGraphPin* FromPin) override;
	virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override;
	virtual FText GetTooltipText() const override;
	virtual bool CanDuplicateNode() const override { return true; }
	virtual void PostPasteNode() override;
	virtual void PostPlacedNewNode() override;
	virtual void DestroyNode() override;
	virtual void ValidateNodeDuringCompilation(class FCompilerResultsLog& MessageLog) const override;
	virtual TSharedPtr<class INameValidatorInterface> MakeNameValidator() const override;
	//~ End UEdGraphNode Interface
	
	virtual UEdGraphPin* GetInputPin() const;
	virtual UEdGraphPin* GetOutputPin() const;

	FString GetTaskNodeName() const;
};




class SSubTaskEdGraphNode : public SGraphNode
{
public:
	SLATE_BEGIN_ARGS(SSubTaskEdGraphNode) {}
	SLATE_END_ARGS()

public:
	void Construct(const FArguments& InArgs, USubTaskEdGraphNode* InNode);

	// SGraphNode interface
	virtual void UpdateGraphNode() override;
	virtual void CreatePinWidgets() override;
	virtual void AddPin(const TSharedRef<SGraphPin>& PinToAdd) override;
	// End of SGraphNode interface

protected:
	FSlateColor GetBorderBackgroundColor() const;

	virtual FText GetPreviewCornerText() const;
	virtual const FSlateBrush* GetNameIcon() const;
};