// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "K2Node_Event.h"
#include "K2Node_TaskEvent.generated.h"

/**
 * 
 */
UCLASS()
class TASKBUILDER_API UK2Node_TaskEvent : public UK2Node_Event
{
	GENERATED_BODY()
public:
	virtual bool CanUserDeleteNode() const override { return false; }
};
