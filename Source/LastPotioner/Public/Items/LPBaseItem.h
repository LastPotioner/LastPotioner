// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/TextRenderComponent.h"
#include "Components/WidgetComponent.h"
#include "Effects/BaseItemEffect.h"
#include "GameFramework/Actor.h"
#include "Interfaces/Interactable.h"
#include "LPBaseItem.generated.h"

class USphereComponent;
class UNiagaraComponent;
class ALPCharacter;
class UWidgetComponent;

UENUM(BlueprintType)
enum class EItemState : uint8
{
	EIS_OnGround UMETA(DisplayName = "OnGround"),
	EIS_Taken UMETA(DisplayName = "Taken"),
	EIS_Thrown UMETA(DisplayName = "Thrown"),
};

UENUM(BlueprintType)
enum class EItemCapability : uint8
{
	EIC_Usable UMETA(DisplayName = "Usable"),
	EIC_Throwable UMETA(DisplayName = "Throwable"),
};

USTRUCT(BlueprintType, meta = (IsBlueprintBase="true"))
struct FItemSignature
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText ItemName = FText();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* ItemIcon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int ID = -1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int Quantity = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsStackable = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<ALPBaseItem> ItemClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int MaxStackSize = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Instanced)
	TArray<UBaseItemEffect*> ItemEffects;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EItemCapability Capability;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName ObjectiveID;

	void CopyFrom(const FItemSignature& Other)
	{
		ItemName = Other.ItemName;
		ItemIcon = Other.ItemIcon;
		ID = Other.ID;
		Quantity = Other.Quantity;
		bIsStackable = Other.bIsStackable;
		ItemClass = Other.ItemClass;
		MaxStackSize = Other.MaxStackSize;
		ItemEffects = Other.ItemEffects;
		Capability = Other.Capability;
		ObjectiveID = Other.ObjectiveID;
	}
};

UCLASS()
class LASTPOTIONER_API ALPBaseItem : public AActor, public IInteractable
{
	GENERATED_BODY()

public:
	ALPBaseItem();

	FItemSignature& GetSlotData() { return ItemSignature; }
	int GetMaxStackSize() const { return ItemSignature.MaxStackSize; };
	bool IsStackable() const { return ItemSignature.bIsStackable; }
	int GetValue() const { return ItemSignature.Quantity; }
	int GetID() const { return ItemSignature.ID; }
	EItemState GetItemState() const { return ItemState; }

	UFUNCTION(BlueprintCallable)
	void SetValue(int NewValue) { ItemSignature.Quantity = FMath::Clamp(NewValue, 0, ItemSignature.MaxStackSize); }

	void SubtractValue(int Value);

	virtual FInteractionResult Interact_Implementation(ALPCharacter* Character) override;

	UFUNCTION(BlueprintCallable)
	void AddForce(const FVector& Force);

	void ToggleMeshVisibility() const;
	virtual void ToggleToolTipTextVisibility_Implementation() override;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	EItemState ItemState = EItemState::EIS_OnGround;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USphereComponent* Sphere;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	UStaticMeshComponent* ItemMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UWidgetComponent* ToolTipWidget;

	void SetItemState(EItemState NewState);

	UFUNCTION()
	virtual void OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	                                  UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
	                                  const FHitResult& SweepResult);

	UFUNCTION()
	virtual void OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	                                UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

private:
	UPROPERTY(EditAnywhere, Category = "Movement")
	bool bShouldPlaySineMovement = false;

	FTimerHandle SineTimerHandle;
	float SineTimerRate = 0.005;
	float TimePassed = 0;

	UPROPERTY(EditAnywhere, Category = "Movement")
	float SinePeriod = 0.5f;

	UPROPERTY(EditAnywhere, Category = "Movement")
	float SineAmplitude = 0.3f;

	UPROPERTY(VisibleAnywhere, Category = "VFX")
	UNiagaraComponent* EmbersEffect;

	UFUNCTION()
	void SineMovement();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	FItemSignature ItemSignature;

	UPROPERTY(EditDefaultsOnly)
	UStaticMeshComponent* StaticMesh;

	UFUNCTION()
	void OnItemHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	               FVector NormalImpulse, const FHitResult& Hit);
};
