// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Radiance/Items/Interactable.h"
#include "Switch.generated.h"

// Delegate signature for the event of OnButtonPressed
DECLARE_DELEGATE_OneParam(FOnButtonPressedSignature, FString)

//-----------------------------------------------------------------------------------------------------------------------------
// Class Name			: ASwitch
// Purpose				: A simple actor which sends a message when interacted
//-----------------------------------------------------------------------------------------------------------------------------
UCLASS()
class RADIANCE_API ASwitch : public AActor, public IInteractable
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASwitch();

	// Message sent when the button is interacted
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Message")
	FString Message;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY( VisibleAnywhere, BlueprintReadWrite, Category = "Components" )
		UStaticMeshComponent* ButtonMesh;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//-----------------------------------------------------------------------------------------------------------------------------
	// Function Name		: OnInteract
	// Parameters			: Caller - The actor who called the function
	// Purpose				: Signature of the function which allows an interaction with another actor
	//-----------------------------------------------------------------------------------------------------------------------------
	UFUNCTION( BlueprintNativeEvent, BlueprintCallable, Category = "Interaction" )
		void OnInteract( AActor* Caller );
	void OnInteract_Implementation( AActor* Caller ) override;

	FOnButtonPressedSignature OnButtonPressed;
};
