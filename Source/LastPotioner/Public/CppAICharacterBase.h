// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
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


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
private:
    UPROPERTY()
    class UWidgetComponent* WidgetComponent;
    float const MaxHealth{100.f};
    float Health;
    
	
};
