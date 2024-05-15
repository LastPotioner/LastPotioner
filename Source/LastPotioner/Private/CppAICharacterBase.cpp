// Fill out your copyright notice in the Description page of Project Settings.


#include "CppAICharacterBase.h"

#include "Consts.h"
#include "HealthBarWidget.h"
#include "Components/WidgetComponent.h"

// Sets default values
ACppAICharacterBase::ACppAICharacterBase():
WidgetComponent{CreateDefaultSubobject<UWidgetComponent>(TEXT("HealthValue"))},
Health{MaxHealth}{
    PrimaryActorTick.bCanEverTick = true;
    if(WidgetComponent){
        WidgetComponent->SetupAttachment(RootComponent);
        WidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);
        WidgetComponent->SetRelativeLocation(defs::HealthBarZ);
        static ConstructorHelpers::FClassFinder<UUserWidget>WidgetClass{TEXT("/Game/NPC/UI/BP_HealthBar")};
        if(WidgetClass.Succeeded()){
            WidgetComponent->SetWidgetClass((WidgetClass.Class));
        }
    }
}


// Called when the game starts or when spawned
void ACppAICharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACppAICharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if(auto const widget = Cast<UHealthBarWidget>(WidgetComponent->GetUserWidgetObject()))
	{
	    widget->SetBarValuePercent(Health / MaxHealth);
	}

}

// Called to bind functionality to input
void ACppAICharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

float ACppAICharacterBase::GetHealth() const {
    return Health;
}


float ACppAICharacterBase::GetMaxHealth()const {
    return MaxHealth;
}


void ACppAICharacterBase::SetHealth(float const NewHealth){
    Health = NewHealth;
}
