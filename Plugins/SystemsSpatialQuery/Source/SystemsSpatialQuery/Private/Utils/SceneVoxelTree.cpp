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

void FSceneVoxelNode::Add(TObjectPtr<UPrimitiveComponent> Primitive, bool bIgnoreInside)
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
		if (Extent.GetMin() < this->MinSize)
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
			const auto& Cell = Data.Cells[Index];
			Cell->Add(Primitive);
			if (Cell->ClosestPointOnSurface.IsSet())
			{
				const auto Distance =
					FVector::Distance(this->BoundingBox.GetCenter(), Cell->ClosestPointOnSurface.GetValue());
				if (this->ClosestDistanceToSurface.IsSet() == false ||
					Distance < this->ClosestDistanceToSurface.GetValue())
				{
					this->ClosestDistanceToSurface = Distance;
					this->ClosestPointOnSurface = Cell->ClosestPointOnSurface;
					this->bInsideSurface = Cell->bInsideSurface;
				}
			}
		}
	}
	else if (this->Content.IsType<FSceneVoxelPrimitives>())
	{
		auto& Data = this->Content.Get<FSceneVoxelPrimitives>();
		if (Data.Components.Contains(Primitive) == false)
		{
			FVector Point = {};
			float Status = -1;
			if (Primitive->GetSquaredDistanceToCollision(this->BoundingBox.GetCenter(), Status, Point))
			{
				const auto bInside = FMath::IsNearlyZero(Status);
				if (bIgnoreInside == false || bInside == false)
				{
					const auto Distance = FVector::Distance(this->BoundingBox.GetCenter(), Point);
					if (this->ClosestDistanceToSurface.IsSet() == false ||
						Distance < this->ClosestDistanceToSurface.GetValue())
					{
						this->ClosestDistanceToSurface = Distance;
						this->ClosestPointOnSurface = Point;
						this->bInsideSurface = FMath::IsNearlyZero(Status);
					}
					Data.Components.Add(Primitive);
				}
			}
		}
	}
}

void FSceneVoxelNode::ForEachNode(const TFunctionRef<void(const FSceneVoxelNode& Node)> Callback,
	ESceneVoxelQueryOptions Options) const
{
	if (this->Content.IsType<FSceneVoxelCells>())
	{
		if (Options != ESceneVoxelQueryOptions::PrimitivesOnly)
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

const FSceneVoxelNode* FSceneVoxelNode::FindNode(FVector Position, ESceneVoxelQueryOptions Options) const
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
			if (const auto* Node = Cell->FindNode(Position, Options))
			{
				return Node;
			}
		}
	}
	if (Options == ESceneVoxelQueryOptions::PrimitivesOnly && this->Content.IsType<FSceneVoxelPrimitives>() == false)
	{
		return nullptr;
	}
	if (Options == ESceneVoxelQueryOptions::Occupied && this->Content.IsType<FSceneVoxelEmpty>())
	{
		return nullptr;
	}
	return this;
}

TOptional<FVector> FSceneVoxelNode::FindClosestPointOnSurface(FVector Position) const
{
	if (this->BoundingBox.IsInsideOrOn(Position) == false)
	{
		Position = this->BoundingBox.GetClosestPointTo(Position);
	}
	const auto* Node = FindNode(Position);
	if (Node != nullptr)
	{
		return Node->ClosestPointOnSurface;
	}
	return {};
}

TOptional<FVector> FSceneVoxelNode::GetClosestPointOnSurface() const
{
	return this->ClosestPointOnSurface;
}

TOptional<double> FSceneVoxelNode::GetClosestDistanceToSurface() const
{
	return this->ClosestDistanceToSurface;
}

bool FSceneVoxelNode::IsInsideSurface() const
{
	return this->bInsideSurface;
}

bool FSceneVoxelNode::IsOccupied() const
{
	return this->Content.IsType<FSceneVoxelEmpty>() == false;
}

uint32 FSceneVoxelNode::CellsCount() const
{
	auto Result = 1;
	if (this->Content.IsType<FSceneVoxelCells>())
	{
		auto& Data = this->Content.Get<FSceneVoxelCells>();
		for (auto Index = 0; Index < 8; ++Index)
		{
			Result += Data.Cells[Index]->CellsCount();
		}
	}
	return Result;
}

const FSceneVoxelContent& FSceneVoxelNode::AccessContent() const
{
	return this->Content;
}

void FSceneVoxelNode::GetAllPrimitives(TSet<TObjectPtr<UPrimitiveComponent>>& Result) const
{
	if (this->Content.IsType<FSceneVoxelCells>())
	{
		const auto& Data = this->Content.Get<FSceneVoxelCells>();
		for (auto Index = 0; Index < 8; ++Index)
		{
			Data.Cells[Index]->GetAllPrimitives(Result);
		}
	}
	else if (this->Content.IsType<FSceneVoxelPrimitives>())
	{
		const auto& Data = this->Content.Get<FSceneVoxelPrimitives>();
		for (const auto& Primitive : Data.Components)
		{
			Result.Add(Primitive);
		}
	}
}

FSceneVoxelDataBase FSceneVoxelNode::Store() const
{
	FSceneVoxelDataBase Result = {};
	const auto Count = this->CellsCount();
	Result.MinSize = this->MinSize;
	Result.Items.Reserve(Count);
	StoreInner(Result);
	return Result;
}

int FSceneVoxelNode::StoreInner(FSceneVoxelDataBase& Result) const
{
	const auto ItemIndex = Result.Items.Add({this->BoundingBox,
		{},
		{},
		this->ClosestPointOnSurface.IsSet() ? this->ClosestPointOnSurface.GetValue() : FVector(),
		this->ClosestDistanceToSurface.IsSet() ? this->ClosestDistanceToSurface.GetValue() : INFINITY,
		this->bInsideSurface});
	auto& Item = Result.Items[ItemIndex];
	if (this->Content.IsType<FSceneVoxelCells>())
	{
		Item.Cells.Reserve(8);
		const auto& Data = this->Content.Get<FSceneVoxelCells>();
		for (auto Index = 0; Index < 8; ++Index)
		{
			Item.Cells.Add(Data.Cells[Index]->StoreInner(Result));
		}
	}
	else if (this->Content.IsType<FSceneVoxelPrimitives>())
	{
		Item.Components.Append(this->Content.Get<FSceneVoxelPrimitives>().Components);
	}
	return ItemIndex;
}

TUniquePtr<FSceneVoxelNode> FSceneVoxelDataBaseItem::Restore(const FSceneVoxelDataBase& DataBase) const
{
	auto Result = FSceneVoxelNode::Make(this->BoundingBox, DataBase.MinSize);
	if (this->Cells.IsEmpty() == false)
	{
		TUniquePtr<FSceneVoxelNode> InCells[8] = {};
		for (auto Index = 0; Index < 8; ++Index)
		{
			InCells[Index] = DataBase.Items[Index].Restore(DataBase);
		}
		Result->Content.Set<FSceneVoxelCells>(MoveTemp(InCells));
	}
	else if (this->Components.IsEmpty() == false)
	{
		Result->Content.Set<FSceneVoxelPrimitives>({this->Components});
	}
	if (this->ClosestDistanceToSurface < INFINITY)
	{
		Result->ClosestPointOnSurface = this->ClosestPointOnSurface;
		Result->ClosestDistanceToSurface = this->ClosestDistanceToSurface;
		Result->bInsideSurface = this->bInsideSurface;
	}
	return Result;
}

TUniquePtr<FSceneVoxelNode> FSceneVoxelDataBase::Restore() const
{
	if (this->Items.IsEmpty())
	{
		return {};
	}
	return this->Items[0].Restore(*this);
}