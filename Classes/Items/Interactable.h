// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Interactable.generated.h"

//-----------------------------------------------------------------------------------------------------------------------------
// Class Name			: UInteractable
// Purpose				: Basic interface to allow interactability to objects
//-----------------------------------------------------------------------------------------------------------------------------
UINTERFACE(MinimalAPI, Blueprintable)
class UInteractable : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class RADIANCE_API IInteractable
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	//-----------------------------------------------------------------------------------------------------------------------------
	// Function Name		: OnInteract
	// Parameters			: Caller - The actor who called the function
	// Purpose				: Signature of the function which allows an interaction with another actor
	//-----------------------------------------------------------------------------------------------------------------------------
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interaction")
		void OnInteract(AActor* Caller);

};
