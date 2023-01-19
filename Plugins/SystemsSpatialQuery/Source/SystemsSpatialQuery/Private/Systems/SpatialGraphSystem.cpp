#include "SystemsSpatialQuery/Public/Systems/SpatialGraphSystem.h"

#include "CompGeom/Delaunay2.h"
#include "Systems/Public/SystemsWorld.h"
#include "SystemsSpatialQuery/Public/Components/SpatialComponent.h"
#include "SystemsSpatialQuery/Public/Resources/SpatialDiscretization.h"
#include "SystemsSpatialQuery/Public/Resources/SpatialGraph.h"
#include "SystemsSpatialQuery/Public/Resources/SpatialPartitioning.h"

auto SpatialQuery(USystemsWorld& Systems,
	const USpatialPartitioning& Partitioning,
	FVector Position,
	const TSet<TSubclassOf<UActorComponent>>& ExtraComponentTypes)
{
	auto Query = Partitioning.TaggedQuery<USpatialComponent>(Systems, Position);
	for (const auto& Type : ExtraComponentTypes)
	{
		if (IsValid(Type.Get()))
		{
			Query.WithRaw(Type.Get());
		}
	}
	return Query;
}

bool UClusteringConnectionValidator::Validate(const TObjectPtr<AActor>& From, const TObjectPtr<AActor>& To) const
{
	return From->GetDistanceTo(To) < this->MaxDistance;
}

bool UOcclusionConnectionValidator::Validate(const TObjectPtr<AActor>& From, const TObjectPtr<AActor>& To) const
{
	FHitResult Hit = {};
	const auto FromPos = From->GetActorLocation();
	const auto ToPos = To->GetActorLocation();
	return GetWorld()->LineTraceSingleByChannel(Hit, FromPos, ToPos, this->CollisionChannel) != this->bClearSight;
}

void UClosestSpatialGraphSystem::Run(USystemsWorld& Systems)
{
	if (IsValid(this->ResourceType.Get()) == false)
	{
		return;
	}
	if (this->bRebuildOnlyOnChange && Systems.ResourceDidChangedRaw(this->ResourceType.Get()) == false)
	{
		return;
	}
	auto* Graph = Cast<USpatialGraph>(Systems.ResourceRaw(this->ResourceType.Get()));
	if (IsValid(Graph) == false)
	{
		return;
	}
	const auto* Spatial = Systems.Resource<USpatialPartitioning>();
	if (IsValid(Spatial) == false)
	{
		return;
	}
	Graph->Reset();
	auto Query = Systems.TaggedQuery<USpatialComponent>();
	for (const auto& Type : this->ExtraComponentTypes)
	{
		if (IsValid(Type.Get()))
		{
			Query.WithRaw(Type.Get());
		}
	}
	for (const auto& QueryItem : Query.Iter())
	{
		auto* Actor = QueryItem.Get<0>();
		const auto Position = Actor->GetActorLocation();
		auto Limit = this->ConnectionsLimitPerNode > 0 ? this->ConnectionsLimitPerNode : -1;
		auto Query2 = SpatialQuery(Systems, *Spatial, Position, this->ExtraComponentTypes);
		for (const auto& QueryItem2 : Query2.Iter())
		{
			auto* Actor2 = QueryItem2.Get<0>();
			if (Actor2 == Actor || Graph->HasConnection(Actor, Actor2, true))
			{
				continue;
			}
			const auto Position2 = Actor2->GetActorLocation();
			const auto Center = (Position + Position2) * 0.5;
			const auto Radius = FVector::Distance(Center, Position) - this->CollisionCircumsphereTolerance;
			auto Query3 = SpatialQuery(Systems, *Spatial, Center, this->ExtraComponentTypes);
			const auto Found = Query3.Iter()
								   .Filter(
									   [&](const auto& QueryItem3)
									   {
										   auto* Actor3 = QueryItem3.Get<0>();
										   return Actor3 != Actor && Actor3 != Actor2;
									   })
								   .First();
			if (Found.IsSet() && Found.GetValue().Get<1>() < Radius)
			{
				continue;
			}
			if (IterStdConst(this->ConnectionValidators)
					.All([&](const auto& Validator)
						{ return Validator == false || Validator->Validate(Actor, Actor2); }))
			{
				Graph->Add(Actor, Actor2, true);
				--Limit;
				if (Limit == 0)
				{
					break;
				}
			}
		}
	}
}

void UDelaunay2dSpatialGraphSystem::Run(USystemsWorld& Systems)
{
	if (IsValid(this->GraphResourceType.Get()) == false)
	{
		return;
	}
	if (this->bRebuildOnlyOnChange && Systems.ResourceDidChangedRaw(this->GraphResourceType.Get()) == false)
	{
		return;
	}
	auto* Graph = Cast<USpatialGraph>(Systems.ResourceRaw(this->GraphResourceType.Get()));
	if (IsValid(Graph) == false)
	{
		return;
	}
	auto* Discretization = Cast<USpatialDiscretization>(Systems.ResourceRaw(this->DiscretizationResourceType.Get()));
	const auto* Spatial = Systems.Resource<USpatialPartitioning>();
	if (IsValid(Spatial) == false)
	{
		return;
	}
	Graph->Reset();
	if (IsValid(Discretization))
	{
		Discretization->Reset();
	}
	auto Query = Systems.TaggedQuery<USpatialComponent>();
	for (const auto& Type : this->ExtraComponentTypes)
	{
		if (IsValid(Type.Get()))
		{
			Query.WithRaw(Type.Get());
		}
	}
	const auto Actors =
		Query.Iter().Map<AActor*>([](const auto& QueryItem) { return QueryItem.Get<0>(); }).CollectArray();
	const auto Points =
		Query.Iter()
			.Map<FVector2d>([](const auto& QueryItem) { return FVector2d(QueryItem.Get<0>()->GetActorLocation()); })
			.CollectArray();
	UE::Geometry::FDelaunay2 Solver;
	if (Solver.Triangulate(Points) == false)
	{
		return;
	}
	const auto Triangles = Solver.GetTriangles();
	for (const auto& Triangle : Triangles)
	{
		auto* A = Actors[Triangle.A];
		auto* B = Actors[Triangle.B];
		auto* C = Actors[Triangle.C];
		if (IterStdConst(this->ConnectionValidators)
				.All(
					[&](const auto& Validator)
					{
						return Validator == false ||
							(Validator->Validate(A, B) && Validator->Validate(B, C) && Validator->Validate(C, A));
					}))
		{
			Graph->Add(A, B, true);
			Graph->Add(B, C, true);
			Graph->Add(C, A, true);
			if (IsValid(Discretization))
			{
				Discretization->AddTriangle(FSpatialDiscretizationTriangle(A, B, C));
			}
		}
	}
}