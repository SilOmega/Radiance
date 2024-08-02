// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <CoreMinimal.h>
#include <Blueprint/UserWidget.h>
#include <Components/ListView.h>
#include <Components/Button.h>
#include <Components/RichTextBlock.h>

#include "InventoryWidget.generated.h"

class UInventoryWidgetEntryDataObject;
class AInventoryManager;

//-----------------------------------------------------------------------------------------------------------------------------
// Class Name			: UInventoryWidget
// Purpose				: Class representing the inventory`s widget, shows collected pickups divided in categories
//-----------------------------------------------------------------------------------------------------------------------------
UCLASS()
class RADIANCE_API UInventoryWidget : public UUserWidget
{
	GENERATED_BODY()

protected:

	//-----------------------------------------------------------------------------------------------------------------------------
	// Function Name	: NativeConstruct
	// Purpose			: Construct the widget and initialise parameters/delegates
	//-----------------------------------------------------------------------------------------------------------------------------
	virtual void NativeConstruct() override;

	// Pointer to inventory manager
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (DisplayName = "Inventory Manager"));
		AInventoryManager* m_InventoryManager;

#pragma region UMGBinds

	UPROPERTY( BlueprintReadWrite, meta = ( BindWidget ) )
		UListView* EruditionList;	
	
	UPROPERTY( BlueprintReadWrite, meta = ( BindWidget ) )
		UListView* ClarusList;	
	
	UPROPERTY( BlueprintReadWrite, meta = ( BindWidget ) )
		UListView* DarkList;	
	
	UPROPERTY( BlueprintReadWrite, meta = ( BindWidget ) )
		UListView* RadianceList;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UButton* Clarus_Button;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UButton* Dark_Button;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UButton* Radiance_Button;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UButton* Erudition_Button;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, MultiLine = true))
		URichTextBlock* ItemDescription;

	UPROPERTY( BlueprintReadWrite, meta = ( BindWidget, MultiLine = true ) )
		URichTextBlock* ObjectiveDescription;

#pragma endregion

	// Sound to play when collected
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Sounds", meta = (DisplayName = "Entry Selected Sound"))
		USoundBase* m_EntrySelected;

	// Sound to play when collected
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Sounds", meta = (DisplayName = "Category Selected Sound"))
		USoundBase* m_CategorySelected;

private:

	//-----------------------------------------------------------------------------------------------------------------------------
	// Function Name	: UpdateWidgetLists
	// Purpose			: Create a new item and adds it to the correct category array. Binded to the "InventoryChanged" delegate  
	// Parameters		: Name, Quantity
	//-----------------------------------------------------------------------------------------------------------------------------
	UFUNCTION(BlueprintCallable)
		void UpdateWidgetLists(FName Name, int32 Quantity);

	// Arrays storing picked up items divided by their narrative category
//	TArray<UInventoryWidgetEntryDataObject*> m_ObjectivesEntries;
	TArray<UInventoryWidgetEntryDataObject*> m_EruditionEntries;
	TArray<UInventoryWidgetEntryDataObject*> m_ClarusEntries;
	TArray<UInventoryWidgetEntryDataObject*> m_RadianceEntries;
	TArray<UInventoryWidgetEntryDataObject*> m_DarkEntries;

	//-----------------------------------------------------------------------------------------------------------------------------
	// Function Name	: UpdateWidgetLists
	// Purpose			: Create a new item and adds it to the correct category array. Binded to the "InventoryChanged" delegate  
	// Parameters		: Name, Quantity
	//-----------------------------------------------------------------------------------------------------------------------------
	UFUNCTION(BlueprintCallable)
		void OnClarusButtonClicked();

	//-----------------------------------------------------------------------------------------------------------------------------
	// Function Name	: OnEruditionButtonClicked
	// Purpose			: What to to when the Erudition button is clicked
	//-----------------------------------------------------------------------------------------------------------------------------
	UFUNCTION()
		void OnEruditionButtonClicked();

	//-----------------------------------------------------------------------------------------------------------------------------
	// Function Name	: OnDarkButtonClicked
	// Purpose			: What to to when the Dark button is clicked
	//-----------------------------------------------------------------------------------------------------------------------------
	UFUNCTION()
		void OnDarkButtonClicked();

	//-----------------------------------------------------------------------------------------------------------------------------
	// Function Name	: OnRadianceButtonClicked
	// Purpose			: What to to when the Radiance button is clicked
	//-----------------------------------------------------------------------------------------------------------------------------
	UFUNCTION()
		void OnRadianceButtonClicked();

	//-----------------------------------------------------------------------------------------------------------------------------
	// Function Name	: OnObjectivesButtonClicked
	// Purpose			: Display entry item`s description text when an entry is selected
	//-----------------------------------------------------------------------------------------------------------------------------
	UFUNCTION()
		void OnItemListSelectionChanged(UObject* Item);
};
