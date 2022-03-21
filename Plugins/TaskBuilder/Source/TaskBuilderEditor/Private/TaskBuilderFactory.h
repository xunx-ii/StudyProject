// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Factories/Factory.h"
#include "TaskBuilderFactory.generated.h"

/**
 * 
 */
UCLASS()
class UTaskBuilderFactory : public UFactory
{
	GENERATED_BODY()
public:
	UTaskBuilderFactory();
	bool ConfigureProperties() override;
	UObject* FactoryCreateNew(UClass* InClass, UObject* InParent, FName InName, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn, FName CallingContext) override;
	UObject* FactoryCreateNew(UClass* InClass, UObject* InParent, FName InName, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn) override;

public:
	UClass* ParentClass;
};
