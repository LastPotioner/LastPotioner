// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/LPBaseItem.h"
#include "Components/SphereComponent.h"
#include "NiagaraComponent.h"
#include "Characters/LPCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Components/WidgetComponent.h"

ALPBaseItem::ALPBaseItem()
{
	PrimaryActorTick.bCanEverTick = false;

	Sphere = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	SetRootComponent(Sphere);

	ItemMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ItemMesh"));
	ItemMesh->SetCollisionResponseToAllChannels(ECR_Block);
	ItemMesh->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
	ItemMesh->SetSimulatePhysics(true);
	ItemMesh->SetupAttachment(Sphere);

	EmbersEffect = CreateDefaultSubobject<UNiagaraComponent>(TEXT("EmbersEffect"));
	EmbersEffect->SetupAttachment(ItemMesh);

	ToolTipWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("ToolTipText"));
	ToolTipWidget->SetupAttachment(GetRootComponent());
}

void ALPBaseItem::BeginPlay()
{
	Super::BeginPlay();

	SetItemState(ItemState);
	Sphere->OnComponentBeginOverlap.AddDynamic(this, &ALPBaseItem::OnSphereBeginOverlap);
	Sphere->OnComponentEndOverlap.AddDynamic(this, &ALPBaseItem::OnSphereEndOverlap);

	if (!IsStackable())
	{
		ItemSignature.MaxStackSize = 1;
	}

	ItemSignature.Value = ItemSignature.Value > ItemSignature.MaxStackSize ? ItemSignature.MaxStackSize : ItemSignature.Value;
	ItemSignature.ItemClass = this->GetClass();

	if (ItemState == EItemState::EIS_OnGround && bShouldPlaySineMovement)
	{
		GetWorldTimerManager().SetTimer(SineTimerHandle, this, &ALPBaseItem::SineMovement, SineTimerRate, true);
	}

	ItemMesh->OnComponentHit.AddDynamic(this, &ALPBaseItem::OnItemHit);
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

	if (NewState == EItemState::EIS_OnGround)
	{
		if (bShouldPlaySineMovement)
		{
			GetWorldTimerManager().SetTimer(SineTimerHandle, this, &ALPBaseItem::SineMovement, SineTimerRate, true);
		}
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

void ALPBaseItem::OnItemHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
				   FVector NormalImpulse, const FHitResult& Hit)
{
	if (GetVelocity().Size() < 100.0f)
	{
		ItemMesh->SetSimulatePhysics(false);
		ItemMesh->SetNotifyRigidBodyCollision(true);
		ItemMesh->SetCollisionResponseToChannel(ECC_Pawn, ECR_Block);
	}
}

void ALPBaseItem::SubtractValue(int Value)
{
	ItemSignature.Value = FMath::Clamp(ItemSignature.Value - Value, 0, ItemSignature.MaxStackSize);
	if (ItemSignature.Value == 0)
	{
		Destroy();
	}
}

FName ALPBaseItem::Interact_Implementation(ALPCharacter* Character)
{
	if (!Character || ItemState == EItemState::EIS_Taken) return FName("");
	const int AddedValue = Character->AddItemToInventory(this);
	SubtractValue(AddedValue);

	return ObjectiveID;
}

void ALPBaseItem::AddForce(const FVector& Force)
{
	ItemMesh->AddForce(Force, NAME_None, true);
}

void ALPBaseItem::ToggleMeshVisibility() const
{
	ItemMesh->ToggleVisibility();
}

void ALPBaseItem::ToggleToolTipTextVisibility_Implementation()
{
	UE_LOG(LogTemp, Display, TEXT("Toggle visibility"));
	if (ItemState == EItemState::EIS_OnGround || ItemState == EItemState::EIS_Taken && ToolTipWidget->IsVisible())
	{
		ToolTipWidget->ToggleVisibility();
	}
}
