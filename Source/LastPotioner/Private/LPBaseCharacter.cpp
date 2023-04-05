// Fill out your copyright notice in the Description page of Project Settings.


#include "LPBaseCharacter.h"

// Sets default values
ALPBaseCharacter::ALPBaseCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ALPBaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ALPBaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ALPBaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

