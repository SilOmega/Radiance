// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryWidgetEntry.h"

UInventoryWidgetEntryDataObject::UInventoryWidgetEntryDataObject() 
{
	Data = FPickup();
}

void UInventoryWidgetEntry::NativeConstruct()
{
	Super::NativeConstruct();

	Background->SetBrushFromTexture( ItemNormalImage );

	//m_OriginalBackgroundSelectedColor = Background->BrushColor;
}

void UInventoryWidgetEntry::NativeOnListItemObjectSet(UObject* ListItemObject)
{
	if (!ListItemObject->IsValidLowLevel())
	{
		UE_LOG(LogTemp, Warning, TEXT("Object not valid"));
	}

	// Get the new item object as Entry Object
	m_EntryObject = Cast<UInventoryWidgetEntryDataObject>(ListItemObject);

	// If this is a new entry then display the text in Bold otherwise in Regular font style
	if (m_EntryObject)
	{
		if(!m_EntryObject->bHasEverBeenSelected) {
			//FText NewlyAddedItemName = FText::FromString(m_EntryObject->Data.sName.ToString() + " (New)");
			//ItemName->SetText(NewlyAddedItemName);
			ItemName->Font.TypefaceFontName = FName( "Bold" );
			ItemName->SetFont( ItemName->Font );
		}
		else
		{
			ItemName->Font.TypefaceFontName = FName( "Regular" );
			ItemName->SetFont( ItemName->Font );
		}

		ItemName->SetText(m_EntryObject->Data.sName);
		//ItemImage->SetBrushFromTexture(m_EntryObject->Data.ItemIcon);
	}

}

void UInventoryWidgetEntry::NativeOnItemSelectionChanged(bool bIsSelected)
{
	if (bIsSelected)
	{
		// After this selection change the name of item will use a Regular font style
		if (!m_EntryObject->bHasEverBeenSelected)
		{
			m_EntryObject->bHasEverBeenSelected = true;
			ItemName->Font.TypefaceFontName = FName( "Regular" );
			ItemName->SetFont( ItemName->Font );
			ItemName->SetText(m_EntryObject->Data.sName);
		}

		// Change background image to display selection
		Background->SetBrushFromTexture( ItemClickedImage );
	}
	else
	{
		// When the entry is not selected anymore revert background image to original
		Background->SetBrushFromTexture( ItemNormalImage );
	}
}

void UInventoryWidgetEntry::NativeOnEntryReleased()
{
	// When the entry is released by list view owner revert background image to original
	Background->SetBrushFromTexture( ItemNormalImage );

	// Nullify the entry object pointer
	m_EntryObject = nullptr;

	// Set entry name to empty
	ItemName->SetText(FText::GetEmpty());

	//ItemImage->SetBrush(FSlateBrush::FSlateBrush());
}
