// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Sound/SoundCue.h"

#include "LoopManager.generated.h"

class AItem_GLightOrb;
class IInfluentiableThroughCycles;
class ATriggerVolume_TimerStart;
class APlayerCharacter;
class UGameOverMenuWidget;

DECLARE_MULTICAST_DELEGATE_OneParam( FResetForNextCycleSignature, int32 )

//-----------------------------------------------------------------------------------------------------------------------------
// Class Name			: ALoopManager
// Purpose				: Manage the transition from one game`s cycle to the next one and the related timers
//-----------------------------------------------------------------------------------------------------------------------------
UCLASS()
class RADIANCE_API ALoopManager : public AActor
{
	GENERATED_BODY()

protected:
	// Default time of a cycle
	UPROPERTY( EditAnywhere, BlueprintReadOnly, Category = "Timer", meta = ( DisplayName = "Cycle Timer in Seconds" ) )
		float f_CycleTimeInSeconds = 65.0f;

	// Default time of a cycle
	UPROPERTY( EditAnywhere, BlueprintReadOnly, Category = "Timer", meta = ( DisplayName = "Max Cycle Timer in Seconds" ) )
		float f_MaxCycleTimeInSeconds = 75.0f;

	// Amount of cycle passed from the beginning
	UPROPERTY( EditAnywhere, BlueprintReadOnly, Category = "Cycle", meta = ( DisplayName = "Maximum cycles amount" ) )
		int32 m_MaxCyclesAmount = 5;

	// Amount of cycle passed from the beginning
	UPROPERTY( EditAnywhere, BlueprintReadOnly, Category = "Cycle", meta = ( DisplayName = "Current cycle",
																			 UIMin = 1, UIMax = 5,
																			 ClampMin = 1, ClampManx = 5 ) )
		int32 m_CurrentCycle = m_MaxCyclesAmount;

	// Bool to check if the sound is playing 
	bool m_IsPlayingSound = false;

	// Bool to know if cycle is in progress
	bool m_IsCycling = false;

	// Amount of remaining seconds when the alert will start play
	UPROPERTY( EditAnywhere, BlueprintReadOnly, Category = "Timer", meta = ( DisplayName = "Cycle Alert Start in Seconds" ) )
		float m_CycleAlertStart = 10.0f;

	// Amount of seconds seconds for fading the alert after the cycle ends
	UPROPERTY( EditAnywhere, BlueprintReadOnly, Category = "Audio", meta = ( DisplayName = "Alert fading time in seconds" ) )
		float m_VolumeFadingTime = 4.0f;

	// Volume curve used for loop alert sound
	UPROPERTY( EditDefaultsOnly, BlueprintReadOnly, Category = "Audio", meta = ( DisplayName = "Alert volume float curve" ) )
		UCurveFloat* m_LoopAlertVolumeCurve;

	UPROPERTY( EditDefaultsOnly, BlueprintReadOnly )
		UAudioComponent* BackgroundAudioComponent;

	UPROPERTY( EditDefaultsOnly, BlueprintReadOnly )
		UAudioComponent* CountdownAudioComponent;

	UPROPERTY( EditDefaultsOnly, BlueprintReadOnly, Category = "Audio", meta = ( DisplayName = "Main loop music" ) )
		USoundWave* m_LoopMusic;

	UPROPERTY( EditDefaultsOnly, BlueprintReadOnly, Category = "Audio", meta = ( DisplayName = "Final loop music" ) )
		USoundWave* m_FinalLoopMusic;

	UPROPERTY( EditDefaultsOnly, BlueprintReadOnly, Category = "Audio", meta = ( DisplayName = "Countdown alert sound" ) )
		USoundWave* m_Cooldown;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		TSubclassOf<UGameOverMenuWidget>	m_GameOverMenuWidget;

	// Array of G-Light Orbs in the game
	TArray<AItem_GLightOrb*> m_LightOrbArray;

	// Actors which are influenced by the cycles
	TArray<IInfluentiableThroughCycles*> m_Cyclables;

	// Trigger which starts the cycle
	ATriggerVolume_TimerStart* m_TimerStartTrigger;

	APlayerCharacter* m_PlayerCharacter;

private:
	// Timer handles to manager cycle timers
	FTimerHandle CycleTimerHandle;
	FTimerHandle CycleTimerHandleText;
	FTimerHandle CycleEndingDelayHandle;

	// Internal current cycle timer
	float m_fCurrentCycleTimer;

	// Actors which have finished they reset fu
	float m_fActorsFinishedResetting;

	bool m_IsPlayerDeadByTimer = false;

	UGameOverMenuWidget* m_MenuWidget;

public:
	//-----------------------------------------------------------------------------------------------------------------------------
	// Function Name		: ALoopManager
	// Purpose				: Initialise Loop manager default values when the object is created
	//-----------------------------------------------------------------------------------------------------------------------------
	ALoopManager();

protected:

	//-----------------------------------------------------------------------------------------------------------------------------
	// Function Name		: BeginPlay
	// Purpose				: Initialise Loop manager additional properties when game starts
	//-----------------------------------------------------------------------------------------------------------------------------
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	void BatteryTimer();

	//-----------------------------------------------------------------------------------------------------------------------------
	// Function Name		: ResetCycle
	// Purpose				: Clear timer text and broadcast the reset for next cycle message 
	//-----------------------------------------------------------------------------------------------------------------------------
	void ResetCycle();

	//-----------------------------------------------------------------------------------------------------------------------------
	// Function Name		: ShowRemainingTime
	// Purpose				: Display remaining timer seconds on screen and manages alert volume based on those 
	//-----------------------------------------------------------------------------------------------------------------------------
	void ShowRemainingTime();

	//-----------------------------------------------------------------------------------------------------------------------------
	// Function Name		: IncreaseTimer
	// Purpose				: Increase internal current timer and update the reset function based on the new amount
	// Parameters			: fTimeIncrease
	//-----------------------------------------------------------------------------------------------------------------------------
	void IncreaseTimer( const float fTimeIncrease );

	//-----------------------------------------------------------------------------------------------------------------------------
	// Function Name		: ResetCompleted
	// Purpose				: Clear delay timer and decide if reset the cycle or trigger the game over screen depending on 
	//						: current cycle count
	//-----------------------------------------------------------------------------------------------------------------------------
	void ResetCompleted();

	//-----------------------------------------------------------------------------------------------------------------------------
	// Function Name		: TimerEnded
	// Purpose				: Acknowledge the end of cycle timer, kills the player and fades loop cooldown audio. Triggers Reset
	//						: Cycle function after an X amount of time
	//-----------------------------------------------------------------------------------------------------------------------------
	void TimerEnded();

//#if WITH_EDITOR
//	virtual void PostEditChangeProperty( FPropertyChangedEvent& PropertyChangedEvent ) override;
//#endif

public:
	//-----------------------------------------------------------------------------------------------------------------------------
	// Function Name		: StartCycle
	// Purpose				: Start the cycle and set all timers required to keep track of it
	//-----------------------------------------------------------------------------------------------------------------------------
	void StartCycle();

	//-----------------------------------------------------------------------------------------------------------------------------
	// Function Name		: GameWon
	// Purpose				: Stop the cycle and set all timers required to stop because the game is won
	//-----------------------------------------------------------------------------------------------------------------------------
	void GameWon();

	//-----------------------------------------------------------------------------------------------------------------------------
	// Function Name		: GameWon
	// Purpose				: Stop the cycle and set all timers required to stop because the game is won
	//-----------------------------------------------------------------------------------------------------------------------------
	UFUNCTION(BlueprintCallable)
	void StartMusic();

	//-----------------------------------------------------------------------------------------------------------------------------
	// Function Name		: GetCurrentCycle
	// Purpose				: Returns the current cycle of the game.
	//-----------------------------------------------------------------------------------------------------------------------------
	int32 GetCurrentCycle();

	// Delegate used to communicate when actors need to prepare for next cycle
	FResetForNextCycleSignature OnResetForNextCycle;

};
