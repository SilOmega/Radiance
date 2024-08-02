// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <CoreMinimal.h>
#include <Components/SphereComponent.h>
#include <Components/AudioComponent.h>

#include "Radiance/InfluentiableThroughCycles.h"
#include "Item_Pickup.h"
#include "Item_GLightOrb.generated.h"

DECLARE_DELEGATE_OneParam(FOnLightOrbPickedUpSignature, float);

class ALoopManager;

//-----------------------------------------------------------------------------------------------------------------------------
// Class Name			: AItem_GLightOrb
// Purpose				: Pickupable item which increases the loop timer by X amount of seconds
//-----------------------------------------------------------------------------------------------------------------------------
UCLASS()
class RADIANCE_API AItem_GLightOrb : public AItem_Pickup, public IInfluentiableThroughCycles
{
	GENERATED_BODY()

	AItem_GLightOrb();

protected:
	
	ALoopManager* m_LoopManager;

	FVector m_OriginalLocation;

	UPROPERTY( EditDefaultsOnly, BlueprintReadWrite )
		USphereComponent* TriggerSphere;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		UAudioComponent* AudioSource;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		UParticleSystem* PickupParticle;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		USoundBase* PickupSound;

	// Amount of time seconds added to the loop manager timer when the orb is picked up
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (DisplayName = "Seconds restored"))
		float m_fTimerExentionInSecond = 30.0f;

	//-----------------------------------------------------------------------------------------------------------------------------
	// Function Name	: OnTriggerBeginOverlap
	// Purpose			: Define behaviour when an actor overlap the trigger sphere 
	// Parameters		: OverlappedComponent, OtherActor, OtherComponent, OtherBodyIndex, bFromSweep, Hit
	//-----------------------------------------------------------------------------------------------------------------------------
	UFUNCTION()
	void OnTriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, 
						  int32 OtherBodyIndex, bool bFromSweep, const FHitResult& Hit);

	//-----------------------------------------------------------------------------------------------------------------------------
	// Function Name	: BeginPlay
	// Purpose			: Define behaviour when an actor begin play 
	//-----------------------------------------------------------------------------------------------------------------------------
	virtual void BeginPlay() override;

	//-----------------------------------------------------------------------------------------------------------------------------
	// Function Name	: Reset
	// Purpose			: Define behaviour when an actor is re-initialised 
	//-----------------------------------------------------------------------------------------------------------------------------
	void ResetForNextCycle_Implementation(const int32 CurrentCycleIndex) override;

public:

	// Delegate to communicate when the orb has been picked up
	FOnLightOrbPickedUpSignature OnLightOrbPickedUp;
};
