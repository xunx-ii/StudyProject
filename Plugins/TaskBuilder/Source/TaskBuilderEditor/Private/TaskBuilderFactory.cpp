// Fill out your copyright notice in the Description page of Project Settings.


#include "TaskBuilderFactory.h"
#include "TaskBuilderBlueprint.h"

UTaskBuilderFactory::UTaskBuilderFactory()
{
	bCreateNew = true;
	bEditAfterNew = true;

	SupportedClass = UTaskBuilderBlueprint::StaticClass();
	ParentClass = UActorComponent::StaticClass();
}

bool UTaskBuilderFactory::ConfigureProperties()
{
	return true;
}

UObject* UTaskBuilderFactory::FactoryCreateNew(UClass* InClass, UObject* InParent, FName InName, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn, FName CallingContext)
{
	return FactoryCreateNew(InClass, InParent, InName, Flags, Context, Warn);
}

UObject* UTaskBuilderFactory::FactoryCreateNew(UClass* InClass, UObject* InParent, FName InName, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn)
{
	check(InClass->IsChildOf(UTaskBuilderBlueprint::StaticClass()));
	UTaskBuilderBlueprint* NewBP = NewObject<UTaskBuilderBlueprint>(InParent, InClass, InName, Flags);
	NewBP->ParentClass = UTaskBuilderBlueprint::StaticClass();
	NewBP->BlueprintType = BPTYPE_Normal;
	NewBP->bIsNewlyCreated = true;
	NewBP->bLegacyNeedToPurgeSkelRefs = false;
	NewBP->GenerateNewGuid();

	return NewBP;
}
