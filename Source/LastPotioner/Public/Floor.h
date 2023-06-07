// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

enum class ESplitOrientation
{
	ESO_Horizontal,
	ESO_Vertlass
};

class FloorNode;

class LASTPOTIONER_API Floor
{
public:
	Floor();
	~Floor();

	void Partition();
	int32 RandomNumber();
	bool ShouldSplitNode(TSharedPtr<FloorNode> InNode, ESplitOrientation Orientation);
	bool SplitAttempt(TSharedPtr<FloorNode> InNode);
	void SplitHorizontal(TSharedPtr<FloorNode> InA, TSharedPtr<FloorNode> InB, TSharedPtr<FloorNode> InC);
	void SplitVertical(TSharedPtr<FloorNode> InA, TSharedPtr<FloorNode> InB, TSharedPtr<FloorNode> InC);

	FORCEINLINE TArray<TSharedPtr<FloorNode>> GetPartitionFloor() const { return PartitionedFloor; }

private:
	TArray<TSharedPtr<FloorNode>> FloorNodeStack;
	TArray<TSharedPtr<FloorNode>> PartitionedFloor;

	int32 groundGridSizeX;
	int32 groundGridSizeY;
	int32 roomMinX;
	int32 roomMinY;
};
