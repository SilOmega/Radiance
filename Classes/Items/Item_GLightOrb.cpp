// Fill out your copyright notice in the Description page of Project Settings.


#include "Item_GLightOrb.h"
#include "Radiance/Player/PlayerCharacter.h"
#include "Radiance/LoopManager.h"

#include <Kismet/GameplayStatics.h>

AItem_GLightOrb::AItem_GLightOrb()
{
	// Create and setup a sphere trigger collider
	TriggerSphere = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	TriggerSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	TriggerSphere->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	TriggerSphere->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	TriggerSphere->SetSphereRadius(150.0f);

	// Attatch the trigger to the actor`s root component
	TriggerSphere->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);

	// Bind the function to the begin overlap event
	TriggerSphere->OnComponentBeginOverlap.AddDynamic(this, &AItem_GLightOrb::OnTriggerBeginOverlap);

	// Creating audio source and attatching it to the actor`s root
	AudioSource = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioSourceComponent"));
	AudioSource->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);

}


void AItem_GLightOrb::BeginPlay()
{
	Super::BeginPlay();

	// Get the Loop manager instance and save it
	m_LoopManager = Cast<ALoopManager>(UGameplayStatics::GetActorOfClass(GetWorld(), ALoopManager::StaticClass()));

	if (m_LoopManager)
	{
		// Bind to the loop manager delegate when actor needs to be reset for next cycle
		m_LoopManager->OnResetForNextCycle.AddUObject(this, &AItem_GLightOrb::ResetForNextCycle_Implementation);
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Red, TEXT("NO LOOP MANAGER BLUEPRINT IN THE LEVEL"));
	}

	// Save original location for reset purposes
	m_OriginalLocation = GetActorLocation();
	//AudioSource->Play();
}

void AItem_GLightOrb::OnTriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& Hit)
{
	if (OtherActor->Tags.Contains("Player"))
	{
		if (bItemHeld)
		{
			m_PlayerCharacter->DropItem();
		}

		// Make actor invisible and untouchable
		Mesh->SetSimulatePhysics(false);
		Mesh->SetVisibility(false, true);
		Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		TriggerSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		// Deactivate sound source
		AudioSource->SetActive(false);
		
		// Play sound when picked up and spawn a particle system
		UGameplayStatics::PlaySound2D(GetWorld(), PickupSound);
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), PickupParticle, GetActorLocation(), GetActorRotation(), true);

		// Broadcast that the orb has been picked up and the timer extension related to it
		OnLightOrbPickedUp.ExecuteIfBound(m_fTimerExentionInSecond);
	}
}


void AItem_GLightOrb::ResetForNextCycle_Implementation(const int32 CurrentCycleIndex)
{
	// Revert actor location to original
	SetActorLocation(m_OriginalLocation);

	// Rever mesh settings to be visible, collectable and triggerable
	Mesh->SetSimulatePhysics(true);
	Mesh->SetEnableGravity(false);
	Mesh->SetVisibility(true, true);
	Mesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	AudioSource->SetActive(true);
	TriggerSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);

	// Broadcast that the actor has been fully reset
	OnFinishedResetting.ExecuteIfBound();
}

