// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AttackAnimNotifyState.h"
#include "CppAICharacterBase.generated.h"

UCLASS()
class LASTPOTIONER_API ACppAICharacterBase : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ACppAICharacterBase();
	virtual void Tick(float DeltaTime) override;
    
    	// Called to bind functionality to input
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
    

    float GetHealth() const;
    float GetMaxHealth() const;
    void SetHealth(float const NewHealth);
    void AttackStart() const;
    void AttackEnd() const;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
private:
    UPROPERTY()
    class UWidgetComponent* WidgetComponent;
    float const MaxHealth{100.f};
    float Health;
    
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Collision, meta=(AllowPrivateAccess="true"))
	class UBoxComponent* LeftFirstCollisionBox;
	
	UFUNCTION()
	void OnAttackOverlapBegin(
	    UPrimitiveComponent* const OverlappedComponent,
	    AActor* const OtherActor,
	    UPrimitiveComponent* const OtherComponent,
	    int const OtherBodyIndex,
	    bool const FromSweep,
	    FHitResult const& SweepResult
	    );
	   UFUNCTION()
	   void OnAttackOverlapEnd(
	        UPrimitiveComponent* const OverlappedComponent,
	        AActor* const OtherActor,
	        UPrimitiveComponent* OtherComponent,
	        int const OtherBodyIndex
	   );
	
};
