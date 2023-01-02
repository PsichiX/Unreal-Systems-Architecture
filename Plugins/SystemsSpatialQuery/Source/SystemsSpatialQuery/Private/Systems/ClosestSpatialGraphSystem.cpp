#include "SystemsSpatialQuery/Public/Systems/ClosestSpatialGraphSystem.h"

#include "Systems/Public/SystemsWorld.h"
#include "SystemsSpatialQuery/Public/Components/SpatialComponent.h"
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
	Query.Iter().ForEach(
		[&](const auto& QueryItem)
		{
			auto* Actor = QueryItem.Get<0>();
			const auto Position = Actor->GetActorLocation();
			auto Query2 = SpatialQuery(Systems, *Spatial, Position, this->ExtraComponentTypes);
			Query2.Iter()
				.Filter([&](const auto& QueryItem) { return QueryItem.Get<0>() != Actor; })
				.Filter(
					[&](const auto& QueryItem)
					{
						auto* Actor2 = QueryItem.Get<0>();
						const auto Position2 = Actor2->GetActorLocation();
						const auto Center = (Position + Position2) * 0.5;
						const auto Radius = FVector::Distance(Center, Position);
						auto Query3 = SpatialQuery(Systems, *Spatial, Center, this->ExtraComponentTypes);
						return Query3.Iter().Any(
								   [&](const auto QueryItem)
								   {
									   auto* Actor3 = QueryItem.Get<0>();
									   const auto Distance = QueryItem.Get<1>();
									   return Actor3 != Actor && Actor3 != Actor2 && Distance < Radius;
								   }) == false;
					})
				.Filter(
					[&](const auto& QueryItem)
					{
						const auto Actor2 = QueryItem.Get<0>();
						return IterStdConst(this->ConnectionValidators)
							.All([&](const auto& Validator)
								{ return Validator == false || Validator->Validate(Actor, Actor2); });
					})
				.ForEach(
					[&](const auto& QueryItem)
					{
						auto* Actor2 = QueryItem.Get<0>();
						Graph->Add(Actor, Actor2, true);
					});
		});
}
