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

USTRUCT(BlueprintType)
struct FItemSlotData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UTexture2D* ItemIcon;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int ID = -1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int Value = 0;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bIsStackable = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<ALPBaseItem> ItemClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int MaxStackSize = 0;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Instanced)
	TArray<UBaseItemEffect*> ItemEffects;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	EItemCapability Capability;

	void CopyFrom(const FItemSlotData& Other)
	{
		ItemIcon = Other.ItemIcon;
		ID = Other.ID;
		Value = Other.Value;
		bIsStackable = Other.bIsStackable;
		ItemClass = Other.ItemClass;
		MaxStackSize = Other.MaxStackSize;
		ItemEffects = Other.ItemEffects;
		Capability = Other.Capability;
	}
};

UCLASS()
class LASTPOTIONER_API ALPBaseItem : public AActor, public IInteractable
{
	GENERATED_BODY()

public:
	ALPBaseItem();

	const FItemSlotData& GetSlotData() const { return SlotData; }
	int GetMaxStackSize() const { return SlotData.MaxStackSize; };
	bool IsStackable() const { return SlotData.bIsStackable; }
	int GetValue() const { return SlotData.Value; }
	int GetID() const { return SlotData.ID; }
	EItemState GetItemState() const { return ItemState; }

	UFUNCTION(BlueprintCallable)
	void SetValue(int NewValue) { SlotData.Value = FMath::Clamp(NewValue, 0, SlotData.MaxStackSize); }

	void SubtractValue(int Value);

	virtual void Interact_Implementation(ALPCharacter* Character) override;

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

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	FItemSlotData SlotData;

	UPROPERTY(EditDefaultsOnly)
	UStaticMeshComponent* StaticMesh;

	UFUNCTION()
	void OnItemHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	               FVector NormalImpulse, const FHitResult& Hit);
};
