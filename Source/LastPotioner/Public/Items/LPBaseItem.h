// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/Interactable.h"
#include "LPBaseItem.generated.h"

class USphereComponent;
class UNiagaraComponent;
class ALPCharacter;

UENUM(BlueprintType)
enum class EItemState : uint8
{
	EIS_Hovering,
	EIS_Equipped,
};

USTRUCT(BlueprintType)
struct FItemSlotData
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UTexture2D* ItemIcon;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int ID = -1;

	UPROPERTY(EditAnywhere)
	int Value = 0;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	bool bIsStackable = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TSubclassOf<ALPBaseItem> ItemClass;

	UPROPERTY(EditDefaultsOnly)
	int MaxStackSize = 0;
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

	UFUNCTION(BlueprintCallable)
	void SetValue(int NewValue) { SlotData.Value = FMath::Clamp(NewValue, 0, SlotData.MaxStackSize); }
	void SubtractValue(int Value);

	void Interact_Implementation(ALPCharacter* Character);

	UFUNCTION(BlueprintCallable)
	void AddForce(const FVector& Force);

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere)
	EItemState ItemState = EItemState::EIS_Hovering;

	UPROPERTY(VisibleAnywhere)
	USphereComponent* Sphere;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* ItemMesh;

	void SetItemState(EItemState NewState);

	UFUNCTION()
	virtual void OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	                                  UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
	                                  const FHitResult& SweepResult);

	UFUNCTION()
	virtual void OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	                                UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

private:
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

	UPROPERTY(EditAnywhere)
	FItemSlotData SlotData;

	UPROPERTY(EditDefaultsOnly)
	UStaticMeshComponent* StaticMesh;

	UFUNCTION()
	void OnItemHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
				   FVector NormalImpulse, const FHitResult& Hit);
};
