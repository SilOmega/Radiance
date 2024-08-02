// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryManager.h"

// Sets default values
AInventoryManager::AInventoryManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void AInventoryManager::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AInventoryManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

bool AInventoryManager::AddItem(FName ItemID, int32 iQuantity)
{
	// Get the current item quantity form character`s inventory
	int32* iItemQuantity = m_Inventory.Find(ItemID);

	FPickup NewItem;

	
	// Has item in inventory already, just increase quantity
	if ( iItemQuantity != nullptr)
	{
		*iItemQuantity += iQuantity;
		UE_LOG(LogTemp, Warning, TEXT("Picked %s - Quantity: %d - Total: %d"), *ItemID.ToString(), iQuantity, *iItemQuantity);

		InventoryChanged.Broadcast(ItemID, iQuantity);
		return true;
	}
	// Check if the ItemID corresponds to a valid item in the database and add the entry to player`s inventory
	else if (m_ItemDatabase && m_ItemDatabase->RetrieveItem(ItemID, NewItem))
	{
		m_Inventory.Add(ItemID, iQuantity);
		UE_LOG(LogTemp, Warning, TEXT("Picked new item %s - Quantity: %d"), *ItemID.ToString(), iQuantity);
		
		InventoryChanged.Broadcast(ItemID, iQuantity);
		return true;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Unable to add %s invalid ItemID or invalid Database"), *ItemID.ToString());
	}
	return false;
}

bool AInventoryManager::RemoveItem(FName ItemID, int32 iQuantity)
{
	int32* iItemQuantity = m_Inventory.Find(ItemID);

	// Has item in inventory already, just increase quantity
	if (iItemQuantity != nullptr && *iItemQuantity >= iQuantity)
	{
		*iItemQuantity -= iQuantity;
		UE_LOG(LogTemp, Warning, TEXT("Removed %s - Quantity: %d"), *ItemID.ToString(), iQuantity);

		// Not removing item from the actual map to imrove efficiency
		if (*iItemQuantity <= 0)
		{
			*iItemQuantity = 0;
			//m_Inventory.Remove(ItemID);
		}

		InventoryChanged.Broadcast(ItemID, -iQuantity);
		return true;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("You don`t possess %s"), *ItemID.ToString());
		return false;
	}


}

UDatabase* AInventoryManager::GetDatabase() const
{
	if (m_ItemDatabase == nullptr)
	{
		GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Red, TEXT("Inventory Manager has no database"));
	}

	return m_ItemDatabase;
}

