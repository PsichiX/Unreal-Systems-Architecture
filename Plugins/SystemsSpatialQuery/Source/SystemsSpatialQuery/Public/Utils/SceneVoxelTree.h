#pragma once

#include "CoreMinimal.h"

struct FSceneVoxelNode;

struct FSceneVoxelEmpty
{
};

struct FSceneVoxelCells
{
	FSceneVoxelCells(TUniquePtr<FSceneVoxelNode> (&&InCells)[8]);

	TUniquePtr<FSceneVoxelNode> Cells[8] = {};
};

struct FSceneVoxelPrimitives
{
	TSet<TObjectPtr<UPrimitiveComponent>> Components = {};
};

using FSceneVoxelContent = TVariant<FSceneVoxelEmpty, FSceneVoxelCells, FSceneVoxelPrimitives>;

struct FSceneVoxelNode
{
	FSceneVoxelNode(FBox InBoundingBox, double InMinSize = 100.0) : BoundingBox(InBoundingBox), MinSize(InMinSize)
	{
		this->Content.Set<FSceneVoxelEmpty>({});
	}

	static TUniquePtr<FSceneVoxelNode> Make(FBox InBoundingBox, double InMinSize = 100.0);

	void Add(TObjectPtr<UPrimitiveComponent> Primitive);

	void ForEachNode(const TFunctionRef<void(const FSceneVoxelNode& Node)> Callback, bool bPrimitivesOnly = true) const;

	const FSceneVoxelNode* FindNode(FVector Position, bool bPrimitivesOnly = true) const;

	TOptional<FVector> FindClosestPointOnSurface(FVector Position) const;

	bool IsOccupied() const;

	FBox BoundingBox = {};

	double MinSize = 100.0;

	FSceneVoxelContent Content = {};

	FVector ClosestPointOnSurface = {};

	double ClosestDistanceToSurface = INFINITY;
};