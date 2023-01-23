#pragma once

#include "CoreMinimal.h"

#include "Systems/Public/Iterator.h"

#include "SceneVoxelTree.generated.h"

struct FSceneVoxelNode;
struct FSceneVoxelDataBase;

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
	friend struct SYSTEMSSPATIALQUERY_API FSceneVoxelDataBaseItem;

	FSceneVoxelNode(FBox InBoundingBox, double InMinSize = 100.0) : BoundingBox(InBoundingBox), MinSize(InMinSize)
	{
		this->Content.Set<FSceneVoxelEmpty>({});
	}

	static TUniquePtr<FSceneVoxelNode> Make(FBox InBoundingBox, double InMinSize = 100.0);

	bool Add(TObjectPtr<UPrimitiveComponent> Primitive, bool bIgnoreInside = false);

	void ForEachNode(const TFunctionRef<void(const FSceneVoxelNode& Node)> Callback,
		ESceneVoxelQueryOptions Options = ESceneVoxelQueryOptions::Any) const;

	const FSceneVoxelNode* FindNode(FVector Position,
		ESceneVoxelQueryOptions Options = ESceneVoxelQueryOptions::Any) const;

	TOptional<FVector> FindClosestPointOnSurface(FVector Position) const;

	TOptional<FVector> GetClosestPointOnSurface() const;

	TOptional<double> GetClosestDistanceToSurface() const;

	bool IsInsideSurface() const;

	bool IsOccupied() const;

	uint32 CellsCount() const;

	const FSceneVoxelContent& AccessContent() const;

	void GetAllPrimitives(TSet<TObjectPtr<UPrimitiveComponent>>& Result) const;

	FSceneVoxelDataBase Store() const;

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

private:
	int StoreInner(FSceneVoxelDataBase& Result) const;

	FBox BoundingBox = {};

	double MinSize = 100.0;

	FSceneVoxelContent Content = {};

	TOptional<FVector> ClosestPointOnSurface = {};

	TOptional<double> ClosestDistanceToSurface = {};

	bool bInsideSurface = false;
};

USTRUCT(BlueprintType)
struct SYSTEMSSPATIALQUERY_API FSceneVoxelDataBaseItem
{
	GENERATED_BODY()

public:
	TUniquePtr<FSceneVoxelNode> Restore(const FSceneVoxelDataBase& DataBase) const;

	UPROPERTY(VisibleAnywhere)
	FBox BoundingBox = {};

	UPROPERTY(VisibleAnywhere)
	TArray<int> Cells = {};

	UPROPERTY(VisibleAnywhere)
	TSet<TObjectPtr<UPrimitiveComponent>> Components = {};

	UPROPERTY(VisibleAnywhere)
	FVector ClosestPointOnSurface = {};

	UPROPERTY(VisibleAnywhere)
	double ClosestDistanceToSurface = INFINITY;

	UPROPERTY(VisibleAnywhere)
	bool bInsideSurface = false;
};

USTRUCT(BlueprintType)
struct SYSTEMSSPATIALQUERY_API FSceneVoxelDataBase
{
	GENERATED_BODY()

public:
	TUniquePtr<FSceneVoxelNode> Restore() const;

	UPROPERTY(VisibleAnywhere)
	double MinSize = 0.0;

	UPROPERTY(VisibleAnywhere)
	TArray<FSceneVoxelDataBaseItem> Items = {};
};