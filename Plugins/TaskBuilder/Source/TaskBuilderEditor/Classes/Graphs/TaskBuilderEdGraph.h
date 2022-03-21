// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EdGraph/EdGraph.h"
#include "TaskBuilderEdGraph.generated.h"

/**
 * 
 */
UCLASS()
class TASKBUILDEREDITOR_API UTaskBuilderEdGraph : public UEdGraph
{
	GENERATED_BODY()
public:
	UPROPERTY()
		UEdGraphNode* EdGraphNode;
	UPROPERTY()
		class UTaskBuilderComponent* TaskBuilderComponent;
};
