// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

struct FCornerCoordinates
{
	int32 UpperLeftX;
	int32 UpperLeftY;
	int32 LowerRightX;
	int32 LowerRightY;
};

class LASTPOTIONER_API FloorNode
{
public:
	FloorNode();
	~FloorNode();

private:
	FCornerCoordinates CornerCoordinates;
};
