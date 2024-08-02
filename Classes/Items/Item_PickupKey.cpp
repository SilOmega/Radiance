// Fill out your copyright notice in the Description page of Project Settings.


#include "Item_PickupKey.h"
#include <Kismet/GameplayStatics.h>
#include <GameFramework/Character.h>
#include <Components/WidgetComponent.h>
#include <Components/SphereComponent.h>
#include <Radiance/Inventory/InventoryManager.h>

AItem_PickupKey::AItem_PickupKey()
{
	// Create a sphere collider and set it to be a trigger which overlap only with dynamic objects
	m_Collider = CreateDefaultSubobject<USphereComponent>(TEXT("SphereCollider"));
	m_Collider->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	m_Collider->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	m_Collider->SetCollisionEnabled(ECollisionEnabled::QueryOnly);

	// Set sphere trigger radius and attach it to root
	m_Collider->SetSphereRadius(150.0f);
	RootComponent = m_Collider;

	// Create a static m_Mesh component and set its properties.
	m_Mesh = CreateDefaultSubobject<UStaticMeshComponent>("StaticMeshComponent");
	m_Mesh->SetEnableGravity(true);
	m_Mesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	m_Mesh->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);

	

	// Create a widget component and attach it to root
	m_Widget = CreateDefaultSubobject<UWidgetComponent>(TEXT("Widget"));
	m_Widget->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);

	if (!m_Database)
	{
		UE_LOG(LogTemp, Warning, TEXT("Pickup has Database selected"));
	}

}

void AItem_PickupKey::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AItem_PickupKey::BeginPlay()
{
	Super::BeginPlay();

	m_InventoryManager = Cast<AInventoryManager>(UGameplayStatics::GetActorOfClass(GetWorld(), AInventoryManager::StaticClass()));
	
	if (!m_InventoryManager)
	{
		UE_LOG(LogTemp, Warning, TEXT("No Inventory Manager Blueprint Found"));
	}

	m_Collider->OnComponentBeginOverlap.AddDynamic(this, &AItem_PickupKey::OnTriggerBeginOverlap);
	m_Collider->OnComponentEndOverlap.AddDynamic(this, &AItem_PickupKey::OnTriggerEndOverlap);
	m_Widget->SetVisibility(false, true);
}

void AItem_PickupKey::OnTriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& Hit)
{
	// Set widget visible if Player hits the sphere trigger
	if (OtherActor->Tags.Contains("Player"))
	{
		m_bIsPlayerInsideTriggerRadius = true;
		m_Widget->SetVisibility(true, true);
		// Call the RotateToCharacter function in loop
		//GetWorldTimerManager().SetTimer(m_TimerHandle, this, &AItem_PickupKey::RotateToCharacter, UGameplayStatics::GetWorldDeltaSeconds(GetWorld()), true, 0.0f);
	}
}

void AItem_PickupKey::OnTriggerEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	m_bIsPlayerInsideTriggerRadius = false;
	
	// Hide the widget 
	m_Widget->SetVisibility(false, true);
	
	// Clear the timer handle related to RotateToCharacter function
	//GetWorldTimerManager().ClearTimer(m_TimerHandle);
}

void AItem_PickupKey::OnInteract_Implementation(AActor* Other)
{
	// Collect the item only if the player is inside the trigger radius
	if (m_bIsPlayerInsideTriggerRadius)
	{
		// Add the item to the character inventory
		m_InventoryManager->AddItem(m_ItemID, m_iQuantity);

		// Make the actor invisible and untouchable
		m_Mesh->SetVisibility(false, true);
		m_Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		m_Collider->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		// Play a plain sound
		UGameplayStatics::PlaySound2D(GetWorld(), m_PickupSound);

		Destroy();
	}
}

void AItem_PickupKey::RotateToCharacter()
{
	// Create a vector representing the direction of the widget with respect of its parent actor
	FVector WidgetToParentVector = m_Widget->GetComponentLocation() - GetActorLocation();
	
	// Calculate widget relative angle to its parent, angle between widget direction and actor's forward direction
	float WidgetAngleToParentForward = FRotationMatrix::MakeFromX(WidgetToParentVector - GetActorForwardVector()).Rotator().Yaw;
	
	// Create a vector representing the direction of the player with respect of its this actor
	FVector PlayerToActorVector = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)->GetActorLocation() - GetActorLocation();
	
	// Calculate player relative angle to this actor, angle between player direction and actor's forward direction
	float PlayerAngleToWidgetForward = FRotationMatrix::MakeFromX(PlayerToActorVector - m_Widget->GetForwardVector()).Rotator().Yaw;

	// Calculate angle required to align widget direction and the player ones
	FRotator ActualAngleWidgetPlayer = FRotator(0.0f, PlayerAngleToWidgetForward - WidgetAngleToParentForward, 0.0f);
	
	// Rotate the relative location of the widget of the angle between the widget and the player
	FVector NewWidgetRelativePositionFacingPlayer = ActualAngleWidgetPlayer.RotateVector(m_Widget->GetRelativeLocation());
	
	// Set relative location to face the player both in direction
	m_Widget->SetRelativeLocationAndRotation(NewWidgetRelativePositionFacingPlayer, FRotator(0.0f, 0.0f, 0.0f));

	m_Widget->SetWorldRotation(FRotator(0.0f, PlayerAngleToWidgetForward, 0.0f));
}

#if WITH_EDITOR
void AItem_PickupKey::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) 
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	FName PropertyName;

	// Get the name of the property which changed
	if (PropertyChangedEvent.Property != NULL)
	{
		PropertyName = PropertyChangedEvent.Property->GetFName();
	}

	// Update the item category based on the new Item ID 
	if (PropertyName == GET_MEMBER_NAME_CHECKED(AItem_PickupKey, m_ItemID))
	{
		// Fetch the correct category from database and set it
		FPickup temp;
		if (m_Database->RetrieveItem(m_ItemID, temp))
		{
			m_ItemCategory = temp.ItemCategory;
		}
		else
		{
			m_ItemCategory = EPickupKeyCategory::None;
		}
	}

}
#endif

void AItem_PickupKey::PostInitProperties()
{
	Super::PostInitProperties();

	// Fetch the correct category from database and set it
	//FPickup temp;
	//if (m_Database->RetrieveItem(m_ItemID, temp))
	//{
	//	m_ItemCategory = temp.ItemCategory;
	//}
	//else
	//{
	//	m_ItemCategory = EPickupKeyCategory::None;
	//}
}