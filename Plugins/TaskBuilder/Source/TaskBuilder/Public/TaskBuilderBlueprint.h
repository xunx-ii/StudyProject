// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/Blueprint.h"
#include "EdGraph/EdGraph.h"
#include "TaskBuilderBlueprint.generated.h"

/**
 * 
 */
UCLASS()
class TASKBUILDER_API UTaskBuilderBlueprint : public UBlueprint
{
	GENERATED_BODY()
public:
	UPROPERTY()
		UEdGraph* TaskBuilderGraph;
	UPROPERTY()
		UEdGraph* EvenGraphRef;
};
