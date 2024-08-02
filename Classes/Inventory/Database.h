// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Database.generated.h"

//-----------------------------------------------------------------------------------------------------------------------------
// Class Name			: EPickupKeyCategory
// Purpose				: Pickup category for narrative purposes
//-----------------------------------------------------------------------------------------------------------------------------
UENUM(BlueprintType)
enum class EPickupKeyCategory : uint8
{
	None,
	Objective,
	Clarus,
	Erudition,
	Radiance,
	Dark
};

//-----------------------------------------------------------------------------------------------------------------------------
// Class Name			: FPickup
// Purpose				: Basic structure for a pickup item
//-----------------------------------------------------------------------------------------------------------------------------
USTRUCT(BlueprintType)
struct FPickup
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item", meta = (DisplayName = "Name"))
		FText sName;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item", meta = (DisplayName = "Description"))
		FText sDescription;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item", meta = (DisplayName = "Category"))
		EPickupKeyCategory ItemCategory;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
		UTexture2D* ItemIcon;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item", meta = (DisplayName = "Quantity"))
		int32 iQuantity = 1;

	FPickup()
	{
		sName = FText::GetEmpty();
		sDescription = FText::GetEmpty();
		ItemCategory = EPickupKeyCategory::None;
		ItemIcon = nullptr;
		iQuantity = 0;
	};

	FORCEINLINE void operator=(const FPickup& other)
	{
		sName = other.sName;
		sDescription = other.sDescription;
		ItemCategory = other.ItemCategory;
		ItemIcon = other.ItemIcon;
		iQuantity = other.iQuantity;

	};
};


//-----------------------------------------------------------------------------------------------------------------------------
// Class Name			: UDatabase
// Purpose				: Hold a list of the items present in the game and retrieve them
//-----------------------------------------------------------------------------------------------------------------------------
UCLASS(BlueprintType)
class RADIANCE_API UDatabase : public UDataAsset
{
	GENERATED_BODY()
	
protected:

	// Database map used to store all pickups of the game
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Database", meta = (DisplayName = "Database"));
		TMap<FName, FPickup> m_Database;

public:

	//-----------------------------------------------------------------------------------------------------------------------------
	// Function Name	: RetrieveItem
	// Purpose			: This function retrieves an item from the database given an ItemID
	// Parameters		: ItemID, OutItem
	// Returns			: bool about the operation success state and the found item as out parameter if succeeded
	//-----------------------------------------------------------------------------------------------------------------------------
	UFUNCTION(BlueprintPure)
		bool RetrieveItem(FName ItemID, FPickup& OutItem);
};
