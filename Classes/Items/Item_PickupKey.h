// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interactable.h"
#include "Radiance/Inventory/Database.h"
#include "GameFramework/Actor.h"
#include "Item_PickupKey.generated.h"


class USphereComponent;
class UWidgetComponent;
class AInventoryManager;

//-----------------------------------------------------------------------------------------------------------------------------
// Class Name			: AItem_PickupKey
// Purpose				: A key item which can be picked up and stored in the inventory
//-----------------------------------------------------------------------------------------------------------------------------
UCLASS()
class RADIANCE_API AItem_PickupKey : public AActor, public IInteractable
{
	GENERATED_BODY()
	

public:
	//----------------------------------------------------------------------------------------------------------------------------
	// Constructor Name	: AItem_Pickup()
	// Notes			: Sets up the components for the class
	//----------------------------------------------------------------------------------------------------------------------------
	AItem_PickupKey();

	// Called every frame
	virtual void Tick(float DeltaTime) override;


protected:

	// The mesh of the item.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (DisplayName = "Mesh"))
		UStaticMeshComponent* m_Mesh;

	// Sphere Collider used to trigger event
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (DisplayName = "TriggerCollider"))
		USphereComponent* m_Collider;

	// Widget component used to show interaction text
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (DisplayName = "Widget"))
		UWidgetComponent* m_Widget;

	// Pointer to inventory manager instance
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Item", meta = (DisplayName = "Inventory Manager"))
		AInventoryManager* m_InventoryManager;

	// Pointer to inventory manager instance
	UPROPERTY(EditDefaultsOnly, Category = "Item", meta = (DisplayName = "Database"))
		class UDatabase* m_Database;

	// Item ID of the pickup
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item", meta = (DisplayName = "ItemID"))
		FName m_ItemID;

	// Quantity added to inventory when collected
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item", meta = (DisplayName = "Quantity"))
		int m_iQuantity = 1;

	// Item category when displayed in the inventory widget
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item", meta = (DisplayName = "Category"))
		EPickupKeyCategory m_ItemCategory = EPickupKeyCategory::None;

	// Sound to play when collected
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item", meta = (DisplayName = "Picked up Sound"))
		USoundBase* m_PickupSound;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


	//----------------------------------------------------------------------------------------------------------------------------
	// Function Name	: OnTriggerBeginOverlap()
	// Parameters		: UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent,
	//					: int32 OtherBodyIndex, bool bFromSweep, FHitResult& Hit
	// Purpose			: This is the callback function needed to set "OnBeginOverlap" function
	//----------------------------------------------------------------------------------------------------------------------------
	UFUNCTION()
		virtual void OnTriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
										   UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& Hit);

	//----------------------------------------------------------------------------------------------------------------------------
	// Function Name	: OnTriggerEndOverlap()
	// Parameters		: class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, 
	//					: class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex
	// Purpose			: This is the callback function needed to set "OnEndOverlap" function
	//----------------------------------------------------------------------------------------------------------------------------
	UFUNCTION()
		virtual void OnTriggerEndOverlap(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	//----------------------------------------------------------------------------------------------------------------------------
	// Function Name	: OnInteract()
	// Parameters		: AActor* Other
	// Purpose			: Describe the item behaviour when interacted
	//----------------------------------------------------------------------------------------------------------------------------
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interaction")
		void OnInteract(AActor* Caller);
	virtual void OnInteract_Implementation(AActor* Other) override;

private:

	//----------------------------------------------------------------------------------------------------------------------------
	// Function Name	: RotateToCharacter()
	// Purpose			: Rotate and move the widget around the item actor to face the player in direction and orientation
	//----------------------------------------------------------------------------------------------------------------------------
	UFUNCTION(BlueprintCallable)
		void RotateToCharacter();

	// Timer handle to call the RotateToCharacter function
	FTimerHandle m_TimerHandle;

	// Bool to keep track when the player is inside the trigger collider
	bool m_bIsPlayerInsideTriggerRadius = false;

	//----------------------------------------------------------------------------------------------------------------------------
	// Function Name	: PostEditChangeProperty()
	// Parameters		: FPropertyChangedEvent& PropertyChangedEvent
	// Purpose			: Update properties values based on the changes made in editor
	//----------------------------------------------------------------------------------------------------------------------------
	void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) ;

	void PostInitProperties();
};
