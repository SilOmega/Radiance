// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "InfluentiableThroughCycles.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI, Blueprintable)
class UInfluentiableThroughCycles : public UInterface
{
	GENERATED_BODY()
};

DECLARE_DELEGATE(FOnFinishedResettingSignature)

//-----------------------------------------------------------------------------------------------------------------------------
// Interface Name		: IInfluentiableThroughCycles
// Purpose				: Define the functions required for an object to be influenced by game cycles
//-----------------------------------------------------------------------------------------------------------------------------
class RADIANCE_API IInfluentiableThroughCycles
{
	GENERATED_BODY()

public:
	
	// Delegate to communicate when the actor finished to respawn for 
	FOnFinishedResettingSignature OnFinishedResetting;

	// Respawn for the next cycle function
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
		void ResetForNextCycle(const int32 CurrentCycle);

};
