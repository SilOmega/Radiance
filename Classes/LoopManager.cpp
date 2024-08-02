// Fill out your copyright notice in the Description page of Project Settings.


#include "LoopManager.h"

#include "Radiance/Items/Item_GLightOrb.h"
#include "Radiance/triggerVolumes/TriggerVolume_TimerStart.h"
#include "Radiance/InfluentiableThroughCycles.h"
#include "Radiance/Player/PlayerHUD.h"
#include "Radiance/Player/PlayerCharacter.h"
#include "Radiance/Menus/GameOverMenuWidget.h"

#include <Blueprint/WidgetBlueprintLibrary.h>
#include <Kismet/GameplayStatics.h>
#include "EngineUtils.h"

ALoopManager::ALoopManager()
{
	// Deactivate tick for actors 
	PrimaryActorTick.bCanEverTick = true;

	// Create audio component and set is parameter
	BackgroundAudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("Music audio source"));
	BackgroundAudioComponent->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	BackgroundAudioComponent->bAutoActivate = false;

	// Create audio component and set is parameter
	CountdownAudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("Countdown Source"));
	CountdownAudioComponent->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	CountdownAudioComponent->bAutoActivate = false;

	//UE_LOG( LogTemp, Warning, TEXT("LoopManager Created "));
}

// Called when the game starts or when spawned
void ALoopManager::BeginPlay()
{
	Super::BeginPlay();

	// Retrieve all G-Light Orbs in the game
	TActorIterator<AItem_GLightOrb> iterator = TActorIterator<AItem_GLightOrb>(GetWorld(), AItem_GLightOrb::StaticClass());

	while (iterator)
	{
		// Store all G-Light Orbs in an array
		m_LightOrbArray.Add(*iterator);

		// Subscribe to the Orb picked up delegate to increase timer every time an orb is picked up
		(m_LightOrbArray.Last())->OnLightOrbPickedUp.BindUObject(this, &ALoopManager::IncreaseTimer);

		iterator.operator++();
	}

	//Creating the Game Over Menu Widget 
	m_MenuWidget = CreateWidget<UGameOverMenuWidget>( UGameplayStatics::GetPlayerController( GetWorld(), 0 ),
													  m_GameOverMenuWidget );
	// Adding the MenuWidget on the screen 
	m_MenuWidget->AddToViewport(1);
	m_MenuWidget->SetVisibility( ESlateVisibility::Hidden );

	// Retrieve all actors which can be influenced by the cycles (by design they all implement UInfluentiableThroughCycles interface
	TArray<AActor*> Influentiables;
	UGameplayStatics::GetAllActorsWithInterface(GetWorld(), UInfluentiableThroughCycles::StaticClass(), Influentiables);

	for (AActor* Influentiable : Influentiables)
	{
		// Store all the influentiable actors
		m_Cyclables.Add(Cast<IInfluentiableThroughCycles>(Influentiable));

		// Subscribe to the Finished Resetting delegate from the influentiable actor
		m_Cyclables.Last()->OnFinishedResetting.BindUObject(this, &ALoopManager::ResetCompleted);
	}

	// Retrive the trigger which will start the cycle
	m_TimerStartTrigger = Cast<ATriggerVolume_TimerStart>(UGameplayStatics::GetActorOfClass(GetWorld(), ATriggerVolume_TimerStart::StaticClass()));

	UE_LOG(LogTemp, Warning, TEXT("%d"), m_Cyclables.Num());

	// Retrive character pointer
	m_PlayerCharacter = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

	// Listen for the player died even so that the cycle can end in that occasion
	m_PlayerCharacter->OnPlayerDied.BindUObject(this, &ALoopManager::TimerEnded);

	// Assert that there is at least one cycle
	check( m_MaxCyclesAmount >= 1 );

	m_CurrentCycle = m_MaxCyclesAmount;

	// Set the cycle music depending on the amount of cycles in the game
	if ( m_CurrentCycle > 1)
	{
		BackgroundAudioComponent->SetSound(m_LoopMusic);
	}
	else
	{
		BackgroundAudioComponent->SetSound( m_FinalLoopMusic );
	}

	CountdownAudioComponent->SetSound(m_Cooldown);



	// All sounds need to loop
	m_LoopMusic->bLooping = true;
	m_FinalLoopMusic->bLooping = true;
}

void ALoopManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	BatteryTimer();
}

void ALoopManager::BatteryTimer()
{
	float fRemainingTime = GetWorldTimerManager().GetTimerRemaining(CycleTimerHandle);
	if (fRemainingTime > 0)
	{
		// Store cycle remaining time
		m_PlayerCharacter->GetHUD()->SetLoopTimeRemaining(fRemainingTime, f_MaxCycleTimeInSeconds );
	}
}

void ALoopManager::ResetCycle()
{
	// Clear the delay timer handle
	GetWorldTimerManager().ClearTimer(CycleEndingDelayHandle);

	//BackgroundAudioComponent->SetPaused(true);

	if (m_CurrentCycle > 2)
	{
		// Advance to next game cycle
		UE_LOG(LogTemp, Warning, TEXT("ResetCycle"));
		m_CurrentCycle--;

		// Broadcast to influentiable actors that they need to reset for next cycle
		OnResetForNextCycle.Broadcast(m_CurrentCycle);
	}
	else if (m_CurrentCycle == 2)
	{
		// Advance to next game cycle
		UE_LOG(LogTemp, Warning, TEXT("ResetCycle"));
		m_CurrentCycle--;

		BackgroundAudioComponent->SetSound(m_FinalLoopMusic);

		// Broadcast to influentiable actors that they need to reset for next cycle
		OnResetForNextCycle.Broadcast(m_CurrentCycle);

	}
	else
	{
		// Call game over screen
		//UKismetSystemLibrary::QuitGame( GetWorld(), UGameplayStatics::GetPlayerController( GetWorld(), 0 ), EQuitPreference::Quit, false );

		if( m_MenuWidget != nullptr )
		{

			m_MenuWidget->SetVisibility( ESlateVisibility::Visible );
			
			// Show mouse coursor
			UGameplayStatics::GetPlayerController( GetWorld(), 0 )->SetShowMouseCursor( true );
			
			// Calling the blueprint function to set input mode to UI Only
			UWidgetBlueprintLibrary::SetInputMode_UIOnly( GetWorld()->GetFirstPlayerController(), m_MenuWidget, false );
		}

		UE_LOG( LogTemp, Warning, TEXT( "Called" ) );
	}

	m_PlayerCharacter->GetHUD()->SetLoopCount(m_CurrentCycle);
}

void ALoopManager::StartCycle()
{
	// Start the timer of the cycle
	GetWorldTimerManager().SetTimer(CycleTimerHandle, this, &ALoopManager::TimerEnded, f_CycleTimeInSeconds, false);
	
	// Call a function that updates hud and alert volume each second depending on the remaining time
	GetWorldTimerManager().SetTimer(CycleTimerHandleText, this, &ALoopManager::ShowRemainingTime, 1.f, true);

	m_IsCycling = true;
}

void ALoopManager::GameWon()
{
	// Clear all timers binded to the loop manager
	GetWorldTimerManager().ClearAllTimersForObject( this );

	m_IsCycling = true;
}

void ALoopManager::StartMusic()
{
	BackgroundAudioComponent->Play();
}

int32 ALoopManager::GetCurrentCycle()
{
	return m_CurrentCycle;
}

void ALoopManager::ShowRemainingTime()
{
	// Store cycle remaining time 
	float fRemainingTime = GetWorldTimerManager().GetTimerRemaining(CycleTimerHandle);

	if (fRemainingTime <= 11)
	{
		m_PlayerCharacter->GetHUD()->OutOfTimerAlert();
	}

	// Play the alert audio when the remaining time is less than the alert start time
	if (fRemainingTime < m_CycleAlertStart && !m_IsPlayingSound)
	{
		CountdownAudioComponent->Play(0.0f);
		CountdownAudioComponent->SetVolumeMultiplier(1.0f);
		m_IsPlayingSound = true;
	}

	if (m_IsPlayingSound)
	{
		// Adjust alert volume based on remaining time and provided volume curve
		CountdownAudioComponent->SetVolumeMultiplier(m_LoopAlertVolumeCurve->FloatCurve.Eval(m_CycleAlertStart - fRemainingTime));
	}
}

void ALoopManager::IncreaseTimer(const float fTimeIncrease)
{
	// Safety measure in case an orb is collected before the cycle starts
	if (m_IsCycling)
	{
		// Increase current timer
		m_fCurrentCycleTimer = GetWorldTimerManager().GetTimerRemaining(CycleTimerHandle) + fTimeIncrease;

		if( m_fCurrentCycleTimer > m_CycleAlertStart )
		{
			m_IsPlayingSound = false;
			CountdownAudioComponent->Stop();
		}

		BackgroundAudioComponent->SetBoolParameter(FName("Cooldown"), false);

		// Update Reset cycle timer based on the new amount
		GetWorldTimerManager().SetTimer(CycleTimerHandle, this, &ALoopManager::TimerEnded, m_fCurrentCycleTimer, false);
	}

}

void ALoopManager::ResetCompleted()
{
	m_fActorsFinishedResetting++;

	// When all influentiable actors in the level have finished resetting fade out the alert audio
	if (m_fActorsFinishedResetting == m_Cyclables.Num())
	{
		// Stop the alert from playing when the cycle start again
		m_IsPlayingSound = false;

		// Play background music from start at default volume
		BackgroundAudioComponent->Play();
		BackgroundAudioComponent->SetVolumeMultiplier(1.0f);

		m_fActorsFinishedResetting = 0;
	}
}


void ALoopManager::TimerEnded()
{
	// If player died by timer exit to not ending in a loop where
	// this function kills the player and the player death trigger this again
	if( m_IsPlayerDeadByTimer )
	{
		return;
	}

	// Clear timer to show remaining time
	GetWorldTimerManager().ClearTimer(CycleTimerHandleText);
	GetWorldTimerManager().ClearTimer( CycleTimerHandle );
	
	//GEngine->ClearOnScreenDebugMessages();

	m_IsCycling = false;


	if (m_CurrentCycle > 1)
	{
		m_PlayerCharacter->LoopReset();
	}
	else 
	{			
		m_IsPlayerDeadByTimer = true;
		m_PlayerCharacter->Die();
	}

	// Fade loop countdown music
	CountdownAudioComponent->FadeOut(m_VolumeFadingTime, 0.0f);
	CountdownAudioComponent->StopDelayed(m_VolumeFadingTime);

	// Fade loop background music
	BackgroundAudioComponent->FadeOut(m_VolumeFadingTime, 0.0f);
	BackgroundAudioComponent->StopDelayed(m_VolumeFadingTime);

	// Set timer to reset cycle after X amount of seconds
	GetWorldTimerManager().SetTimer(CycleEndingDelayHandle, this, &ALoopManager::ResetCycle, m_VolumeFadingTime, false);
}

//void ALoopManager::PostEditChangeProperty( FPropertyChangedEvent& PropertyChangedEvent )
//{
//	Super::PostEditChangeProperty( PropertyChangedEvent );
//
//	m_CurrentCycle = FMath::Clamp( m_CurrentCycle, 1, m_MaxCyclesAmount );
//}
