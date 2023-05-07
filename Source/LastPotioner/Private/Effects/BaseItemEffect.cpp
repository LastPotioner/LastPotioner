// Fill out your copyright notice in the Description page of Project Settings.


#include "Effects/BaseItemEffect.h"


UBaseItemEffect::UBaseItemEffect()
{
}

UBaseItemEffect::UBaseItemEffect(ALPCharacter* CharacterRef)
{
	Character = CharacterRef;
}
