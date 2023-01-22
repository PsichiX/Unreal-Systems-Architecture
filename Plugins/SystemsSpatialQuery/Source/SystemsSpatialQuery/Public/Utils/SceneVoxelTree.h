#pragma once

#include "CoreMinimal.h"

#include "Systems/Public/Iterator.h"

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

enum class ESceneVoxelQueryOptions : uint8
{
	Any,
	Occupied,
	PrimitivesOnly,
};

struct FSceneVoxelNode
{
	FSceneVoxelNode(FBox InBoundingBox, double InMinSize = 100.0) : BoundingBox(InBoundingBox), MinSize(InMinSize)
	{
		this->Content.Set<FSceneVoxelEmpty>({});
	}

	static TUniquePtr<FSceneVoxelNode> Make(FBox InBoundingBox, double InMinSize = 100.0);

	void Add(TObjectPtr<UPrimitiveComponent> Primitive);

	void ForEachNode(const TFunctionRef<void(const FSceneVoxelNode& Node)> Callback,
		ESceneVoxelQueryOptions Options = ESceneVoxelQueryOptions::Any) const;

	const FSceneVoxelNode* FindNode(FVector Position,
		ESceneVoxelQueryOptions Options = ESceneVoxelQueryOptions::Any) const;

	TOptional<FVector> FindClosestPointOnSurface(FVector Position) const;

	bool IsOccupied() const;

	auto PrimitivesIter() const
	{
		static TSet<TObjectPtr<UPrimitiveComponent>> EMPTY = {};
		auto& Result = EMPTY;
		if (this->Content.IsType<FSceneVoxelPrimitives>())
		{
			Result = this->Content.Get<FSceneVoxelPrimitives>().Components;
		}
		return IterStdConst(Result);
	}

	FBox BoundingBox = {};

	double MinSize = 100.0;

	FSceneVoxelContent Content = {};

	FVector ClosestPointOnSurface = {};

	double ClosestDistanceToSurface = INFINITY;
};