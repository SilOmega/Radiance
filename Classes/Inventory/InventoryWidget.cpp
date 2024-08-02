// Fill out your copyright notice in the Description page of Project Settings.

#include "InventoryWidget.h"
#include <Kismet/GameplayStatics.h>
#include "InventoryWidgetEntry.h"
#include "InventoryManager.h"

void UInventoryWidget::NativeConstruct()
{
	Super::NativeConstruct();

	// Retrieve inventory manager level instance
	m_InventoryManager = Cast<AInventoryManager>(UGameplayStatics::GetActorOfClass(GetWorld(), AInventoryManager::StaticClass()));

	if( m_InventoryManager)
	{
		// Subscribe to InventoryChanged manager to update widget accordingly
		m_InventoryManager->InventoryChanged.AddDynamic(this, &UInventoryWidget::UpdateWidgetLists);
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Red, TEXT("No Inventory Manager class or blueprint in the level"));
	}

	// Subscribe to UMG button delegates
	Clarus_Button->OnClicked.AddDynamic(this, &UInventoryWidget::OnClarusButtonClicked);
	Dark_Button->OnClicked.AddDynamic(this, &UInventoryWidget::OnDarkButtonClicked);
	Radiance_Button->OnClicked.AddDynamic(this, &UInventoryWidget::OnRadianceButtonClicked);
	Erudition_Button->OnClicked.AddDynamic(this, &UInventoryWidget::OnEruditionButtonClicked);
	//Objective_Button->OnClicked.AddDynamic(this, &UInventoryWidget::OnObjectivesButtonClicked);

	// Subscribe to list view selection changed delegate
	//ObjectiveList->OnItemSelectionChanged().AddUObject(this, &UInventoryWidget::OnItemListSelectionChanged);

	// Subscribe to list view selection changed delegate
	EruditionList->OnItemSelectionChanged().AddUObject( this, &UInventoryWidget::OnItemListSelectionChanged );
	
	// Subscribe to list view selection changed delegate
	RadianceList->OnItemSelectionChanged().AddUObject( this, &UInventoryWidget::OnItemListSelectionChanged );
	
	// Subscribe to list view selection changed delegate
	DarkList->OnItemSelectionChanged().AddUObject( this, &UInventoryWidget::OnItemListSelectionChanged );
	
	// Subscribe to list view selection changed delegate
	ClarusList->OnItemSelectionChanged().AddUObject( this, &UInventoryWidget::OnItemListSelectionChanged );

	ClarusList->SetVisibility( ESlateVisibility::Collapsed );
	DarkList->SetVisibility( ESlateVisibility::Collapsed );
	RadianceList->SetVisibility( ESlateVisibility::Collapsed );
	EruditionList->SetVisibility( ESlateVisibility::Collapsed );
	//ObjectiveList->SetVisibility( ESlateVisibility::Collapsed );
}

void UInventoryWidget::UpdateWidgetLists(FName Name, int32 Quantity)
{
	FPickup PickupData;

	// If an item is added to inventory (Quantity > 0), fetch it and save it into the correct array
	if (Quantity > 0 && m_InventoryManager->GetDatabase()->RetrieveItem(Name, PickupData))
	{
		UInventoryWidgetEntryDataObject* Entry = nullptr;

		if( PickupData.ItemCategory != EPickupKeyCategory::Objective )
		{
			Entry = NewObject<UInventoryWidgetEntryDataObject>();
			Entry->Data = PickupData;
		}

		switch ( PickupData.ItemCategory)
		{
			case EPickupKeyCategory::Objective :
				// Format the string to have new lines
				ObjectiveDescription->SetText( FText::FormatNamed( PickupData.sDescription, TEXT( "nextLine" ), FText::FromString( TEXT( "\n" ) ) ) );
				break;

			case EPickupKeyCategory::Clarus	:
				m_ClarusEntries.Insert(Entry, 0);
				ClarusList->SetListItems(  m_ClarusEntries );
				break;

			case EPickupKeyCategory::Radiance :
				m_RadianceEntries.Insert(Entry, 0);
				RadianceList->SetListItems( m_RadianceEntries );
				break;

			case EPickupKeyCategory::Dark:
				m_DarkEntries.Insert(Entry, 0);
				DarkList->SetListItems( m_DarkEntries );
				break;

			case EPickupKeyCategory::Erudition:
				m_EruditionEntries.Insert(Entry, 0);
				EruditionList->SetListItems( m_EruditionEntries );
				break;

			default:
				break;
		}
	}


}

void UInventoryWidget::OnEruditionButtonClicked()	
{ 
	EruditionList->SetVisibility( ESlateVisibility::Visible );
	DarkList->SetVisibility( ESlateVisibility::Collapsed );
	RadianceList->SetVisibility( ESlateVisibility::Collapsed );
	ClarusList->SetVisibility( ESlateVisibility::Collapsed );

	//ItemList->SetListItems(m_EruditionEntries);	

	ItemDescription->SetText(FText::FromString(""));

	UObject* SelectedEntry = EruditionList->GetSelectedItem();

	if( SelectedEntry )
	{
		// Format the string to have new lines
		FText DescriptionFormatted = FText::FormatNamed( Cast<UInventoryWidgetEntryDataObject>( SelectedEntry )->Data.sDescription, TEXT( "nextLine" ), FText::FromString( TEXT( "\n" ) ) );

		ItemDescription->SetText( DescriptionFormatted );
	}

	// Play a plain sound
	UGameplayStatics::PlaySound2D(GetWorld(), m_CategorySelected);
}

void UInventoryWidget::OnDarkButtonClicked()		
{
	EruditionList->SetVisibility( ESlateVisibility::Collapsed );
	DarkList->SetVisibility( ESlateVisibility::Visible );
	RadianceList->SetVisibility( ESlateVisibility::Collapsed );
	ClarusList->SetVisibility( ESlateVisibility::Collapsed );

	ItemDescription->SetText(FText::FromString(""));

	UObject* SelectedEntry = DarkList->GetSelectedItem();

	if( SelectedEntry )
	{
		// Format the string to have new lines
		FText DescriptionFormatted = FText::FormatNamed( Cast<UInventoryWidgetEntryDataObject>( SelectedEntry )->Data.sDescription, TEXT( "nextLine" ), FText::FromString( TEXT( "\n" ) ) );

		ItemDescription->SetText( DescriptionFormatted );
	}

	// Play a plain sound
	UGameplayStatics::PlaySound2D(GetWorld(), m_CategorySelected);
}

void UInventoryWidget::OnRadianceButtonClicked()
{
	EruditionList->SetVisibility( ESlateVisibility::Collapsed );
	DarkList->SetVisibility( ESlateVisibility::Collapsed );
	RadianceList->SetVisibility( ESlateVisibility::Visible );
	ClarusList->SetVisibility( ESlateVisibility::Collapsed );

	ItemDescription->SetText(FText::FromString(""));

	UObject* SelectedEntry = RadianceList->GetSelectedItem();

	if( SelectedEntry )
	{
		// Format the string to have new lines
		FText DescriptionFormatted = FText::FormatNamed( Cast<UInventoryWidgetEntryDataObject>( SelectedEntry )->Data.sDescription, TEXT( "nextLine" ), FText::FromString( TEXT( "\n" ) ) );

		ItemDescription->SetText( DescriptionFormatted );
	}

	// Play a plain sound
	UGameplayStatics::PlaySound2D(GetWorld(), m_CategorySelected);
}

void UInventoryWidget::OnClarusButtonClicked()
{
	EruditionList->SetVisibility( ESlateVisibility::Collapsed );
	DarkList->SetVisibility( ESlateVisibility::Collapsed );
	RadianceList->SetVisibility( ESlateVisibility::Collapsed );
	ClarusList->SetVisibility( ESlateVisibility::Visible );

	ItemDescription->SetText( FText::FromString( "" ) );

	UObject* SelectedEntry = ClarusList->GetSelectedItem();

	if( SelectedEntry )
	{
		// Format the string to have new lines
		FText DescriptionFormatted = FText::FormatNamed( Cast<UInventoryWidgetEntryDataObject>( SelectedEntry )->Data.sDescription, TEXT( "nextLine" ), FText::FromString( TEXT( "\n" ) ) );

		ItemDescription->SetText( DescriptionFormatted );
	}

	// Play a plain sound
	UGameplayStatics::PlaySound2D(GetWorld(), m_CategorySelected);
}


void UInventoryWidget::OnItemListSelectionChanged(UObject* Item)
{
	// When an item is selected from the list view set its description on the right side of the widget
	if (Item != nullptr)
	{
		// Format the string to have new lines
		FText DescriptionFormatted = FText::FormatNamed(Cast<UInventoryWidgetEntryDataObject>(Item)->Data.sDescription, TEXT("nextLine"), FText::FromString(TEXT("\n")));

		ItemDescription->SetText(DescriptionFormatted);
		
		// Play a plain sound
		UGameplayStatics::PlaySound2D(GetWorld(), m_EntrySelected);
	}
}

