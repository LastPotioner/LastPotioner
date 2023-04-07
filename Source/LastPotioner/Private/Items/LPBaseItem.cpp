// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/LPBaseItem.h"
#include "Components/SphereComponent.h"
#include "NiagaraComponent.h"
#include "Characters/LPCharacter.h"
#include "Kismet/GameplayStatics.h"

ALPBaseItem::ALPBaseItem()
{
	PrimaryActorTick.bCanEverTick = true;

	ItemMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ItemMesh"));
	ItemMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	ItemMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	SetRootComponent(ItemMesh);

	Sphere = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	Sphere->SetupAttachment(GetRootComponent());

	EmbersEffect = CreateDefaultSubobject<UNiagaraComponent>(TEXT("EmbersEffect"));
	EmbersEffect->SetupAttachment(GetRootComponent());
}

void ALPBaseItem::BeginPlay()
{
	Super::BeginPlay();

	SetItemState(ItemState);
	Sphere->OnComponentBeginOverlap.AddDynamic(this, &ALPBaseItem::OnSphereBeginOverlap);
	Sphere->OnComponentEndOverlap.AddDynamic(this, &ALPBaseItem::OnSphereEndOverlap);

	if (ItemState == EItemState::EIS_Hovering)
	{
		GetWorldTimerManager().SetTimer(SineTimerHandle, this, &ALPBaseItem::SineMovement, SineTimerRate, true);
	}
}

void ALPBaseItem::SineMovement()
{
	TimePassed += SineTimerRate;
	TimePassed = TimePassed >= (2.0f * PI) * SinePeriod ? 0.0f : TimePassed;

	const float DeltaZ = SineAmplitude * FMath::Sin(TimePassed / SinePeriod);
	SetActorLocation(GetActorLocation() + FVector(0.0f, 0.0f, DeltaZ));
}

void ALPBaseItem::SetItemState(EItemState NewState)
{
	ItemState = NewState;

	if (NewState == EItemState::EIS_Hovering)
	{
		GetWorldTimerManager().SetTimer(SineTimerHandle, this, &ALPBaseItem::SineMovement, SineTimerRate, true);
		if (EmbersEffect)
		{
			EmbersEffect->Activate();
		}
	}
	else
	{
		GetWorldTimerManager().ClearTimer(SineTimerHandle);
		if (EmbersEffect)
		{
			EmbersEffect->Deactivate();
		}
	}
}

void ALPBaseItem::OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                 UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                                 const FHitResult& SweepResult)
{
	if (ALPCharacter* LPCharacter = Cast<ALPCharacter>(OtherActor))
	{
		LPCharacter->SetOverlappingItem(this);
	}
}

void ALPBaseItem::OnSphereEndOverlap(
	UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (ALPCharacter* LPCharacter = Cast<ALPCharacter>(OtherActor))
	{
		LPCharacter->SetOverlappingItem(nullptr);
	}
}

void ALPBaseItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}