// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/LPCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Weapons/LPWeapon.h"
#include "Components/LPAttributeComponent.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/KismetSystemLibrary.h"
#include "InventorySystem/LPInventoryComponent.h"
#include "Interfaces/Interactable.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "Perception/AISense_Sight.h"

ALPCharacter::ALPCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	/*
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(GetMesh());
	Camera->bUsePawnControlRotation = true;
	*/

	GetCharacterMovement()->bOrientRotationToMovement = true;

	AttributeComponent = CreateDefaultSubobject<ULPAttributeComponent>(TEXT("Attribute Component"));
	InventoryComponent = CreateDefaultSubobject<ULPInventoryComponent>(TEXT("Inventory"));

	GetMesh()->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldDynamic, ECollisionResponse::ECR_Overlap);
	GetMesh()->SetGenerateOverlapEvents(true);
	GetMesh()->SetupAttachment(GetRootComponent());
	
 	SetupStimulusSource();
}

void ALPCharacter :: SetupStimulusSource(){
    StimulusSource = CreateDefaultSubobject<UAIPerceptionStimuliSourceComponent>(TEXT("Stimulus"));
    if(StimulusSource){
        StimulusSource->RegisterForSense(TSubclassOf<UAISense_Sight>());
        StimulusSource->RegisterWithPerceptionSystem();
    }
}

void ALPCharacter::BeginPlay()
{
	Super::BeginPlay();

	Tags.Add(FName("LPCharacter"));

	if (const APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem =
			ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(LPMappingContext, 0);
		}
	}

	Camera = Cast<UCameraComponent>(GetComponentByClass(UCameraComponent::StaticClass()));
}

void ALPCharacter::CheckForInteractables()
{
	if (Camera)
	{
		const FVector Start = Camera->GetComponentLocation();
		const FVector End = Start + Camera->GetForwardVector() * 1000.0f;

		FHitResult HitResult;
		const bool HasHit = UKismetSystemLibrary::SphereTraceSingle(this, Start, End, 20.0f,
		                                                            ETraceTypeQuery::TraceTypeQuery1, false, {},
		                                                            EDrawDebugTrace::None, HitResult, true);

		if (HasHit)
		{
			AActor* HitActor = HitResult.GetActor();
			if (CurrentInteractable && HitActor == CurrentInteractable) return;

			if (!UKismetSystemLibrary::DoesImplementInterface(HitActor, UInteractable::StaticClass()))
			{
				UE_LOG(LogTemp, Display, TEXT("Does not implement"));
				if (CurrentInteractable)
				{
					IInteractable::Execute_ToggleToolTipTextVisibility(CurrentInteractable);
				}
				
				CurrentInteractable = nullptr;
				return;
			}
			
			if (CurrentInteractable)
			{
				IInteractable::Execute_ToggleToolTipTextVisibility(CurrentInteractable);
			}
			
			CurrentInteractable = HitActor;
			IInteractable::Execute_ToggleToolTipTextVisibility(CurrentInteractable);
		}
		else
		{
			if (CurrentInteractable)
			{
				IInteractable::Execute_ToggleToolTipTextVisibility(CurrentInteractable);
			}
			CurrentInteractable = nullptr;
		}
	}
}

void ALPCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	CheckForInteractables();
}

void ALPCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent =
		CastChecked<UEnhancedInputComponent>(PlayerInputComponent)) // game ends if cast failed
	{
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ALPCharacter::Move);
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ALPCharacter::Look);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ALPCharacter::CanJump);
		EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Triggered, this, &ALPCharacter::FastAttack);
		EnhancedInputComponent->BindAction(
			EquipAction, ETriggerEvent::Triggered, this, &ALPCharacter::HandleWeaponAction);
		EnhancedInputComponent->BindAction(
			PickUpAction, ETriggerEvent::Triggered, this, &ALPCharacter::PickUpOverlappingItem);
		EnhancedInputComponent->BindAction(
			EscapeAction, ETriggerEvent::Triggered, this, &ALPCharacter::HandleEscape);
		EnhancedInputComponent->BindAction(
			StrongAttackAction, ETriggerEvent::Triggered, this, &ALPCharacter::StrongAttack);
		EnhancedInputComponent->BindAction(
			JumpAttackAction, ETriggerEvent::Triggered, this, &ALPCharacter::JumpAttack);
		EnhancedInputComponent->BindAction(
			ToggleRun, ETriggerEvent::Triggered, this, &ALPCharacter::Run
		);
	}
}

void ALPCharacter::Move(const FInputActionValue& Value)
{
	//if (ActionState != EActionState::EAS_Unoccupied) return;

	const FVector2D InputVector = Value.Get<FVector2D>();

	if (!InputVector.IsNearlyZero())
	{
		AddMovementInput(GetActorForwardVector(), InputVector.X * GetWorld()->GetDeltaSeconds() * MaxWalkSpeed);
		AddMovementInput(GetActorRightVector(), InputVector.Y * GetWorld()->GetDeltaSeconds() * MaxWalkSpeed);
	}
}

void ALPCharacter::Look(const FInputActionValue& Value)
{
	const FVector2D LookAxisValue = Value.Get<FVector2D>();

	if (!LookAxisValue.IsZero() && GetController() && GetController()->IsLocalPlayerController())
	{
		AddControllerYawInput(LookAxisValue.X * RotationSensitivityMultiplier);
		AddControllerPitchInput(-LookAxisValue.Y * RotationSensitivityMultiplier);
	}
}

void ALPCharacter::Run(const FInputActionValue& Value)
{
	const bool IsRunning = Value.Get<bool>();

	if (IsRunning) 
	{
		UE_LOG(LogTemp, Warning, TEXT("Run"));
	}
	
}


void ALPCharacter::CanJump(const FInputActionValue& Value)
{
	const bool isJumping = Value.Get<bool>();
	FTimerHandle JumpTimer;

	if (!isFalling)
	{
		if (isJumping)
		{
			GetWorldTimerManager().SetTimer(JumpTimer, this, &ACharacter::Jump, 0.2f, false);
		}
	}
}

void ALPCharacter::Jumping()
{
	if (!readyToJump) {
		readyToJump = true;
	}
}



void ALPCharacter::PickUpOverlappingItem()
{
	if (ALPWeapon* OverlappingWeapon = Cast<ALPWeapon>(OverlappingItem))
	{
		OverlappingWeapon->Equip(GetMesh(), this, this);
		EquippedWeapons.Add(OverlappingWeapon);
		OverlappingItem = nullptr;
		OnObjectiveIDCalled.Broadcast(OverlappingWeapon->GetItemSignature().ObjectiveID, 1);
	}
}

void ALPCharacter::ArmWeapon()
{
	PlayAnimMontage(EquipAnimMontage, 1.0f, FName("Arm"));
}

void ALPCharacter::ToggleCurrentWeaponVisibility() const
{
	if (CurrentWeapon)
	{
		CurrentWeapon->ToggleMeshVisibility();
	}
}

void ALPCharacter::HandleEscape_Implementation()
{
}

void ALPCharacter::HandleWeaponAction()
{
	if (EquippedWeapons.Num() == 0 || ActionState != EActionState::EAS_Unoccupied) return;
	//if (EquippedWeapons.Num() == 1 && CurrentWeapon) return; // if the only weapon is equipped

	ActionState = EActionState::EAS_ArmingWeapon;
	GetCharacterMovement()->MaxWalkSpeed = MaxMovementSpeed / 3.0f;

	CurrentWeaponPosition = 0;
	if (CurrentWeapon)
	{
		CurrentWeaponPosition = EquippedWeapons.IndexOfByKey(CurrentWeapon);
		CurrentWeaponPosition = (CurrentWeaponPosition + 1) % EquippedWeapons.Num();
		if (EquippedWeapons[CurrentWeaponPosition] != CurrentWeapon)
		{
			ActionState = EActionState::EAS_DisarmingBeforeArming;
		}
		else
		{
			ActionState = EActionState::EAS_DisarmingWeapon;
		}

		DisarmWeapon();
		return;
	}

	ArmWeapon();
}

void ALPCharacter::DisarmWeapon()
{
	if (!CurrentWeapon) return;
	if (ActionState < EActionState::EAS_DisarmingWeapon && ActionState != EActionState::EAS_Unoccupied) return;

	PlayAnimMontage(EquipAnimMontage, 1.0f, FName("Disarm"));
}

void ALPCharacter::OnDisarmEnd()
{
	if (CurrentWeapon)
	{
		CurrentWeapon->Disarm();
		CurrentWeapon = nullptr;

		if (ActionState == EActionState::EAS_DisarmingBeforeArming)
		{
			ArmWeapon();
			return;
		}

		GetCharacterMovement()->MaxWalkSpeed = MaxMovementSpeed;
		ActionState = EActionState::EAS_Unoccupied;
		CharacterState = ECharacterState::ECS_Unequipped;
		OnCharacterStateChanged.Broadcast(CharacterState);
	}
}

void ALPCharacter::OnArmEnd()
{
	CurrentWeapon = EquippedWeapons[CurrentWeaponPosition];

	if (CurrentWeapon)
	{
		switch (CurrentWeapon->GetWeaponType())
		{
		case EWeaponType::EWT_OneHanded:
			CharacterState = ECharacterState::ECS_EquippedOneHandedWeapon;
			break;
		case EWeaponType::EWT_TwoHanded:
			CharacterState = ECharacterState::ECS_EquippedTwoHandedWeapon;
			break;
		case EWeaponType::EWT_Bow:
			CharacterState = ECharacterState::ECS_EquippedLongRangeWeapon;
			break;
		}

		OnCharacterStateChanged.Broadcast(CharacterState);
		GetCharacterMovement()->MaxWalkSpeed = MaxMovementSpeed;
		CurrentWeapon->Arm();
		ActionState = EActionState::EAS_Unoccupied;
	}
}

void ALPCharacter::FastAttack()
{
	UE_LOG(LogTemp, Display, TEXT("%i"), CanAttack())
	if (CanAttack())
	{
		GetCharacterMovement()->MaxWalkSpeed = MaxWalkSpeed / 2.0f;
		ActionState = EActionState::EAS_Attacking;
		PlayFastAttackMontage();
	}
}

void ALPCharacter::StrongAttack()
{
	/*
	 * TODO: implement enum 'AttackType'
	 */
	if (CanAttack())
	{
		GetCharacterMovement()->SetMovementMode(MOVE_None);
		ActionState = EActionState::EAS_Attacking;
		PlayAnimMontage(StrongAttackMontage);
	}
}


void ALPCharacter::JumpAttack()
{
	if (CanAttack())
	{
		GetCharacterMovement()->SetMovementMode(MOVE_None);
		ActionState = EActionState::EAS_Attacking;
		PlayAnimMontage(JumpAttackMontage);
	}
}

void ALPCharacter::Die()
{
	Super::Die();

	OnCharacterDied.Broadcast();
}

void ALPCharacter::OnAttackEnd()
{
	GetCharacterMovement()->MaxWalkSpeed = MaxWalkSpeed;
	GetCharacterMovement()->SetMovementMode(MOVE_Walking);
	ActionState = EActionState::EAS_Unoccupied;
}

bool ALPCharacter::CanAttack() const
{
	return (ActionState == EActionState::EAS_Unoccupied)
	&& (CharacterState != ECharacterState::ECS_Unequipped)
	&& (!GetCharacterMovement()->IsFalling())
	&& (CharacterState != ECharacterState::ECS_EquippedLongRangeWeapon);
}

void ALPCharacter::GetHit_Implementation(const FHitResult& HitResult)
{
	Super::GetHit_Implementation(HitResult);
}

FInteractionResult ALPCharacter::Interact()
{
	if (CurrentInteractable) //&& UKismetSystemLibrary::DoesImplementInterface(CurrentInteractable, UInteractable::StaticClass()))
	{
		return IInteractable::Execute_Interact(CurrentInteractable, this);
	}

	return FInteractionResult("", 0);
}

int ALPCharacter::AddItemToInventory(ALPBaseItem* Item) const
{
	if (!InventoryComponent) return 0;

	return InventoryComponent->AddItemByRef(Item);
}

void ALPCharacter::SetOpenedStorage(AActor* Storage)
{
	OpenedStorage = Storage;
}
