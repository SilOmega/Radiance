// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <CoreMinimal.h>
#include <Blueprint/UserWidget.h>
#include <Blueprint/IUserObjectListEntry.h>
#include <Components/Border.h>
#include <Components/TextBlock.h>
#include <Components/Image.h>
#include <Math/Color.h>

#include "Database.h"

#include "InventoryWidgetEntry.generated.h"

//-----------------------------------------------------------------------------------------------------------------------------
// Class Name			: UInventoryWidgetEntryDataObject
// Author				: Gaetano Trovato
// Purpose				: Object used to store inventory widget entry data for list view
//-----------------------------------------------------------------------------------------------------------------------------
UCLASS()
class RADIANCE_API UInventoryWidgetEntryDataObject : public UObject
{
	GENERATED_BODY()

public:

	UInventoryWidgetEntryDataObject();

	// Pickup data holded by this entry
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (DisplayName = "Entry Data"))
		FPickup Data;

	// The element has been ever selected before or not
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (DisplayName = "Has ever been selected"))
		bool bHasEverBeenSelected = false;
};


//-----------------------------------------------------------------------------------------------------------------------------
// Class Name			: UInventoryWidgetEntry
// Author				: Gaetano Trovato
// Purpose				: Represent a widget entry for the list view used in the inventory widget
//-----------------------------------------------------------------------------------------------------------------------------
UCLASS()
class RADIANCE_API UInventoryWidgetEntry : public UUserWidget, public IUserObjectListEntry
{
	GENERATED_BODY()

protected:

	virtual void NativeConstruct() override;

	// Pointer to object used as data source for the entry
	UPROPERTY(BlueprintReadWrite, meta = (DisplayName = "Entry Object"))
		class UInventoryWidgetEntryDataObject* m_EntryObject;

	// Border used for entry background
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UBorder*	Background;

	// Text block displaying the item name in the entry
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UTextBlock* ItemName;

	// Image displaying the item icon in the entry
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Images")
		UTexture2D*		ItemClickedImage;

	// Image displaying the item icon in the entry
	UPROPERTY( EditDefaultsOnly, BlueprintReadWrite, Category = "Images" )
	UTexture2D* ItemNormalImage;

	//// Color to use when entry is selected
	//UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (DisplayName = "Entry Selected Color"))
	//	FColor m_SelectedEntryBackgroundColor = FColor::FromHex("F3AD63FF");

	// Original background color
	//FLinearColor m_OriginalBackgroundSelectedColor;

	//-----------------------------------------------------------------------------------------------------------------------------
	// Function Name	: OnListItemObjectSet
	// Author			: UE4
	// Editors			: Gaetano Trovato
	// Purpose			: Called when this entry is assigned a new item object to represent by the owning list view
	// Parameters		: ListItemObject
	//-----------------------------------------------------------------------------------------------------------------------------
	UFUNCTION(BlueprintCallable)
	virtual void NativeOnListItemObjectSet(UObject* ListItemObject) override;

	//-----------------------------------------------------------------------------------------------------------------------------
	// Function Name	: NativeOnItemSelectionChanged
	// Author			: UE4
	// Editors			: Gaetano Trovato
	// Purpose			: Called when this entry selection is changed
	// Parameters		: bIsSelected
	//-----------------------------------------------------------------------------------------------------------------------------
	UFUNCTION(BlueprintCallable)
	virtual void NativeOnItemSelectionChanged(bool bIsSelected) override;
	
	//-----------------------------------------------------------------------------------------------------------------------------
	// Function Name	: NativeOnEntryReleased
	// Author			: UE4
	// Editors			: Gaetano Trovato
	// Purpose			: Called when this entry is released by the owning list view
	//-----------------------------------------------------------------------------------------------------------------------------
	UFUNCTION(BlueprintCallable)
	virtual void NativeOnEntryReleased() override;

};
