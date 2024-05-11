// Fill out your copyright notice in the Description page of Project Settings.


#include "CPP_MainGameCharacter.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "Perception/AISense_Sight.h"

// Sets default values
ACPP_MainGameCharacter::ACPP_MainGameCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	SetupStimulusSource();
}
void ACPP_MainGameCharacter::SetupStimulusSource() {
	StimulusSource = CreateDefaultSubobject<UAIPerceptionStimuliSourceComponent>(TEXT("Stimulus"));
	if (StimulusSource) {
		StimulusSource->RegisterForSense(TSubclassOf<UAISense_Sight>());
		StimulusSource->RegisterWithPerceptionSystem();
	}
}

// Called when the game starts or when spawned
void ACPP_MainGameCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACPP_MainGameCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ACPP_MainGameCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

