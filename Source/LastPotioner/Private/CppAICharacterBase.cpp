// Fill out your copyright notice in the Description page of Project Settings.


#include "CppAICharacterBase.h"

#include "Consts.h"
#include "CPP_MainGameCharacter.h"
#include "NPCEnemy.h"
#include "HealthBarWidget.h"
#include "Components/BoxComponent.h"
#include "Components/WidgetComponent.h"

// Sets default values
ACppAICharacterBase::ACppAICharacterBase():
WidgetComponent{CreateDefaultSubobject<UWidgetComponent>(TEXT("HealthValue"))},
Health{MaxHealth},
LeftFirstCollisionBox{CreateDefaultSubobject<UBoxComponent>(TEXT("LeftFirstCollisionBox"))}
{
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
    
    if(LeftFirstCollisionBox){
        LeftFirstCollisionBox->SetBoxExtent(defs::LeftFirstBoxSize, false);
        FAttachmentTransformRules const Rules {
            EAttachmentRule::SnapToTarget,
            EAttachmentRule::SnapToTarget,
            EAttachmentRule::KeepWorld,
            false};
         LeftFirstCollisionBox->AttachToComponent(GetMesh(), Rules, defs::LeftFirstSocketName);
         LeftFirstCollisionBox->SetRelativeLocation(defs::CollisionBoxLocation);
        
    }
}


// Called when the game starts or when spawned
void ACppAICharacterBase::BeginPlay()
{
	Super::BeginPlay();
	LeftFirstCollisionBox->OnComponentBeginOverlap.AddDynamic(this, &ACppAICharacterBase::OnAttackOverlapBegin);
	LeftFirstCollisionBox->OnComponentEndOverlap.AddDynamic(this, &ACppAICharacterBase::OnAttackOverlapEnd);
	
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
void ACppAICharacterBase::OnAttackOverlapBegin(
    UPrimitiveComponent* const OverlappedComponent,
    AActor* const OtherActor,
    UPrimitiveComponent* const OtherComponent,
    int const OtherBodyIndex,
    bool const FromSweep,
    FHitResult const& SweepResult){
    if(OtherActor == this){
        return;
    }
    if(auto const Enemy = Cast<ANPCEnemy>(OtherActor)){
        auto const NewHealth = Enemy->GetHealth() - Enemy->GetMaxHealth() * 0.1f;
        Enemy->SetHealth(NewHealth); 
    }
//     else if(auto const Player = Cast<ACPP_MainGameCharacter>(OtherActor)){
//         auto const NewHealth = Player->GetHealth() - Player->GetMaxHealth() * 0.07f;
//         Player->SetHealth(NewHealth);
//     }
}
void ACppAICharacterBase::OnAttackOverlapEnd(
    UPrimitiveComponent* const OverlappedComponent,
    AActor* const OtherActor,
    UPrimitiveComponent* OtherComponent,
    int const OtherBodyIndex
){}

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
    
//     if(Cast<ACPP_MainGameCharacter>(this)){
//         if(Health <= 0){
// 
//         }
//   
//     }
    //  else 
      if (Cast<ANPCEnemy>(this)){
            if(Health <= 0 ){
                
            }
        }
}

void ACppAICharacterBase::AttackStart() const{
LeftFirstCollisionBox->SetCollisionProfileName(("First"));
LeftFirstCollisionBox->SetNotifyRigidBodyCollision(true);
}

void ACppAICharacterBase::AttackEnd() const{
LeftFirstCollisionBox->SetCollisionProfileName(("First"));
LeftFirstCollisionBox->SetNotifyRigidBodyCollision(false);
}


