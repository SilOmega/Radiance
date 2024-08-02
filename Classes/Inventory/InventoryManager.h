// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Database.h"

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InventoryManager.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FInventoryUpdatedSignature, FName, Name, int32, Quantity);

//-----------------------------------------------------------------------------------------------------------------------------
// Class Name			: AInventoryManager
// Purpose				: Class that handles player`s inventory in the game
//-----------------------------------------------------------------------------------------------------------------------------
UCLASS()
class RADIANCE_API AInventoryManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AInventoryManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item System", meta = (DisplayName = "Game Database"))
		class UDatabase* m_ItemDatabase;

	// Map of the item id and relative quantities present in inventory
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Inventory", meta = (DisplayName = "Current Inventory"))
		TMap<FName, int32> m_Inventory;

	/*UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Inventory", meta = (DisplayName = "Total Slots"))
	int32 m_TotalEquipmentSlots = 6;*/

	//----------------------------------------------------------------------------------------------------------------------------
	// Function Name	: InventoryUpdated()
	// Parameters		: FName ItemID, int32 iQuantity
	// Purpose			: Empty function used as delegate signature for Unreal Engine Delegate
	//----------------------------------------------------------------------------------------------------------------------------
	void InventoryUpdated(FName Name, int32 Quantity);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//----------------------------------------------------------------------------------------------------------------------------
	// Function Name	: AddItem()
	// Parameters		: FName ItemID, int32 iQuantity
	// Return			: Boolean depending of the success of the operation
	// Purpose			: Add the given quantity of the given item from the character inventory
	//----------------------------------------------------------------------------------------------------------------------------
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	bool AddItem(FName ItemID, int32 iQuantity);

	//----------------------------------------------------------------------------------------------------------------------------
	// Function Name	: RemoveItem()
	// Parameters		: FName ItemID, int32 iQuantity
	// Return			: Boolean depending of the success of the operation
	// Purpose			: Remove the given quantity of the given item from the character inventory
	//----------------------------------------------------------------------------------------------------------------------------
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	bool RemoveItem(FName ItemID, int32 iQuantity);

	//----------------------------------------------------------------------------------------------------------------------------
	// Function Name	: InventoryUpdated Delegate
	// Purpose			: Unreal Engine Delegate used to broadcast listeners of an update in the inventory
	//----------------------------------------------------------------------------------------------------------------------------
	UPROPERTY(BlueprintAssignable, BlueprintCallable)
		FInventoryUpdatedSignature InventoryChanged;

	UFUNCTION(BlueprintPure)
		UDatabase* GetDatabase() const;
};
