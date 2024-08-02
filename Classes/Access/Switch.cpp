// Fill out your copyright notice in the Description page of Project Settings.


#include "Switch.h"

// Sets default values
ASwitch::ASwitch()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	// Create a static mesh component and set its properties.
	ButtonMesh = CreateDefaultSubobject<UStaticMeshComponent>( "Terminal Button" );
	ButtonMesh->SetCollisionEnabled( ECollisionEnabled::QueryAndPhysics );
	RootComponent = ButtonMesh;
}

// Called when the game starts or when spawned
void ASwitch::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASwitch::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASwitch::OnInteract_Implementation( AActor* Caller )
{
	OnButtonPressed.ExecuteIfBound( Message );
}