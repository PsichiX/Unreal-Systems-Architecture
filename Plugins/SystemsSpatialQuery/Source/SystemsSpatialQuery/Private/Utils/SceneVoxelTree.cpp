#include "SystemsSpatialQuery/Public/Utils/SceneVoxelTree.h"

FSceneVoxelCells::FSceneVoxelCells(TUniquePtr<FSceneVoxelNode> (&&InCells)[8])
{
	for (auto Index = 0; Index < 8; ++Index)
	{
		this->Cells[Index] = MoveTemp(InCells[Index]);
	}
}

TUniquePtr<FSceneVoxelNode> FSceneVoxelNode::Make(FBox InBoundingBox, double InMinSize)
{
	return MakeUnique<FSceneVoxelNode>(InBoundingBox, InMinSize);
}

void FSceneVoxelNode::Add(TObjectPtr<UPrimitiveComponent> Primitive)
{
	if (Primitive == false)
	{
		return;
	}
	const auto PrimitiveBox = Primitive->Bounds.GetBox();
	if (PrimitiveBox.Intersect(this->BoundingBox) == false)
	{
		return;
	}
	if (this->Content.IsType<FSceneVoxelEmpty>())
	{
		const auto Extent = this->BoundingBox.GetExtent();
		if (Extent.X < this->MinSize || Extent.Y < this->MinSize || Extent.Z < this->MinSize)
		{
			this->Content.Set<FSceneVoxelPrimitives>({});
		}
		else
		{
			const auto Min = this->BoundingBox.Min;
			const auto Max = this->BoundingBox.Max;
			const auto Center = this->BoundingBox.GetCenter();
			TUniquePtr<FSceneVoxelNode> Cells[8] = {
				Make(FBox(FVector(Min.X, Min.Y, Min.Z), FVector(Center.X, Center.Y, Center.Z)), this->MinSize),
				Make(FBox(FVector(Center.X, Min.Y, Min.Z), FVector(Max.X, Center.Y, Center.Z)), this->MinSize),
				Make(FBox(FVector(Center.X, Center.Y, Min.Z), FVector(Max.X, Max.Y, Center.Z)), this->MinSize),
				Make(FBox(FVector(Min.X, Center.Y, Min.Z), FVector(Min.X, Max.Y, Center.Z)), this->MinSize),
				Make(FBox(FVector(Min.X, Min.Y, Center.Z), FVector(Center.X, Center.Y, Max.Z)), this->MinSize),
				Make(FBox(FVector(Center.X, Min.Y, Center.Z), FVector(Max.X, Center.Y, Max.Z)), this->MinSize),
				Make(FBox(FVector(Center.X, Center.Y, Center.Z), FVector(Max.X, Max.Y, Max.Z)), this->MinSize),
				Make(FBox(FVector(Min.X, Center.Y, Center.Z), FVector(Min.X, Max.Y, Max.Z)), this->MinSize),
			};
			this->Content.Set<FSceneVoxelCells>(MoveTemp(Cells));
		}
	}
	if (this->Content.IsType<FSceneVoxelCells>())
	{
		auto& Data = this->Content.Get<FSceneVoxelCells>();
		for (auto Index = 0; Index < 8; ++Index)
		{
			auto& Cell = Data.Cells[Index];
			Cell->Add(Primitive);
			if (Cell->ClosestDistanceToSurface < this->ClosestDistanceToSurface)
			{
				this->ClosestDistanceToSurface = Cell->ClosestDistanceToSurface;
				this->ClosestPointOnSurface = Cell->ClosestPointOnSurface;
			}
		}
	}
	else if (this->Content.IsType<FSceneVoxelPrimitives>())
	{
		auto& Data = this->Content.Get<FSceneVoxelPrimitives>();
		if (Data.Components.Contains(Primitive) == false)
		{
			FVector Point = {};
			const auto Distance = Primitive->GetClosestPointOnCollision(this->BoundingBox.GetCenter(), Point);
			if (Distance < this->ClosestDistanceToSurface)
			{
				this->ClosestDistanceToSurface = Distance;
				this->ClosestPointOnSurface = Point;
			}
		}
		Data.Components.Add(Primitive);
	}
}

void FSceneVoxelNode::ForEachNode(const TFunctionRef<void(const FSceneVoxelNode& Node)> Callback,
	bool bPrimitivesOnly) const
{
	if (this->Content.IsType<FSceneVoxelCells>())
	{
		if (bPrimitivesOnly == false)
		{
			Callback(*this);
		}
		auto& Data = this->Content.Get<FSceneVoxelCells>();
		for (auto Index = 0; Index < 8; ++Index)
		{
			Data.Cells[Index]->ForEachNode(Callback);
		}
	}
	else if (this->Content.IsType<FSceneVoxelPrimitives>())
	{
		Callback(*this);
	}
}

const FSceneVoxelNode* FSceneVoxelNode::FindNode(FVector Position, bool bPrimitivesOnly) const
{
	if (this->BoundingBox.IsInsideOrOn(Position) == false)
	{
		return nullptr;
	}
	if (this->Content.IsType<FSceneVoxelCells>())
	{
		auto& Data = this->Content.Get<FSceneVoxelCells>();
		for (auto Index = 0; Index < 8; ++Index)
		{
			const auto& Cell = Data.Cells[Index];
			if (const auto* Node = Cell->FindNode(Position, bPrimitivesOnly))
			{
				return Node;
			}
		}
	}
	if (bPrimitivesOnly && this->Content.IsType<FSceneVoxelPrimitives>() == false)
	{
		return nullptr;
	}
	return this;
}

TOptional<FVector> FSceneVoxelNode::FindClosestPointOnSurface(FVector Position) const
{
	const auto* Node = FindNode(Position);
	if (Node != nullptr)
	{
		return Node->ClosestPointOnSurface;
	}
	return {};
}

bool FSceneVoxelNode::IsOccupied() const
{
	return this->Content.IsType<FSceneVoxelEmpty>() == false;
}