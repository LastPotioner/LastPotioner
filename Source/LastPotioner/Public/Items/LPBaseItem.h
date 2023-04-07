// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LPBaseItem.generated.h"

class USphereComponent;
class UNiagaraComponent;

UENUM(BlueprintType)
enum class EItemState : uint8
{
	EIS_Hovering,
	EIS_Equipped,
};

UCLASS()
class LASTPOTIONER_API ALPBaseItem : public AActor
{
	GENERATED_BODY()

public:
	ALPBaseItem();

	virtual void Tick(float DeltaTime) override;

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
};
