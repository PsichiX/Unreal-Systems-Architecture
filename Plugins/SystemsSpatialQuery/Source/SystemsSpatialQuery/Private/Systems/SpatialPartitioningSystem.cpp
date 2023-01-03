#include "SystemsSpatialQuery/Public/Systems/SpatialPartitioningSystem.h"

#include "DrawDebugHelpers.h"
#include "Systems/Public/SystemsWorld.h"
#include "SystemsSpatialQuery/Public/Components/DebugSpatialQueryEpicenterComponent.h"
#include "SystemsSpatialQuery/Public/Components/SpatialComponent.h"
#include "SystemsSpatialQuery/Public/Resources/SpatialPartitioning.h"

void SpatialPartitioningSystem(USystemsWorld& Systems)
{
	const auto* Settings = Systems.Resource<USpatialPartitioningSettings>();
	if (IsValid(Settings) == false)
	{
		return;
	}

	if (Settings->bRebuildOnlyOnChange && Systems.ResourceDidChanged<USpatialPartitioning>() == false &&
		Systems.ComponentsDidChanged<USpatialComponent>() == false)
	{
		return;
	}

	auto* Partitioning = Systems.Resource<USpatialPartitioning>();
	if (IsValid(Partitioning))
	{
		Partitioning->Reset(Settings->CoverWorldArea, Settings->CellActorsCapacity, Settings->PreferedSubdivisionPlane);
		for (auto& QueryItem : Systems.Query<USpatialComponent>())
		{
			auto* Actor = QueryItem.Get<0>();
			Partitioning->Add(Systems, Actor);
		}
	}
}

void DebugSpatialPartitioningSystem(USystemsWorld& Systems)
{
	auto* Settings = Systems.Resource<USpatialPartitioningSettings>();
	if (IsValid(Settings) == false)
	{
		return;
	}

	auto* Partitioning = Systems.Resource<USpatialPartitioning>();
	if (IsValid(Partitioning) == false)
	{
		return;
	}

	auto* World = Systems.GetWorld();
	if (IsValid(Settings->StressTestActor.Get()))
	{
		const auto Area = Settings->CoverWorldArea;
		auto CurrentCount =
			Systems.Actors()
				.Filter([&](const auto* Actor) { return Actor->GetClass() == Settings->StressTestActor.Get(); })
				.Count();

		if (CurrentCount > Settings->StressTestSpawnCount)
		{
			for (auto* Actor :
				Systems.Actors()
					.Filter([&](const auto* Actor) { return Actor->GetClass() == Settings->StressTestActor.Get(); })
					.Take(CurrentCount > Settings->StressTestSpawnCount))
			{
				Actor->Destroy();
			}
		}

		while (CurrentCount < Settings->StressTestSpawnCount)
		{
			++CurrentCount;
			auto Position = FVector(FMath::FRandRange(Area.Lower.X, Area.Upper.X),
				FMath::FRandRange(Area.Lower.Y, Area.Upper.Y),
				FMath::FRandRange(Area.Lower.Z, Area.Upper.Z));
			if (Settings->bStressTestSpawnOnGround)
			{
				FHitResult Hit;
				if (World->LineTraceSingleByChannel(Hit,
						FVector(Position.X, Position.Y, Area.Upper.Z),
						FVector(Position.X, Position.Y, Area.Lower.Z),
						ECollisionChannel::ECC_GameTraceChannel1))
				{
					Position.Z = Hit.ImpactPoint.Z;
				}
				else
				{
					continue;
				}
			}
			Position += Settings->StressTestSpawnOffset;
			const auto Rotator = FRotator(0.0);
			World->SpawnActor(Settings->StressTestActor, &Position, &Rotator);
		}
	}

	if (Settings->bDrawOccupiedDebugAreas || Settings->bDrawEmptyDebugAreas)
	{
		Partitioning->ForEachArea(
			[&](const auto& Area, auto bOccupied)
			{
				if (bOccupied)
				{
					if (Settings->bDrawOccupiedDebugAreas)
					{
						DrawDebugBox(World, Area.Center(), Area.Extents(), Settings->DebugOccupiedAreasColor);
					}
				}
				else
				{
					if (Settings->bDrawEmptyDebugAreas)
					{
						DrawDebugBox(World, Area.Center(), Area.Extents(), Settings->DebugEmptyAreasColor);
					}
				}
			});
	}

	if (Settings->bDrawSpatialQueries)
	{
		const auto Epicenter = Systems.Query<UDebugSpatialQueryEpicenterComponent>().Next();

		if (Epicenter.IsSet())
		{
			const auto Position = Epicenter.GetValue().Get<0>()->GetActorLocation();
			const auto Mode = Epicenter.GetValue().Get<1>()->Mode;
			auto Query = Partitioning->Query<USpatialComponent>(Systems, Position);
			if (Mode == EDebugSpatialQueryEpicenterMode::Area)
			{
				const auto Extents = Epicenter.GetValue().Get<1>()->Extents;
				const auto Area = FArea(Position - Extents, Position + Extents);
				DrawDebugBox(World, Area.Center(), Area.Extents(), Settings->DebugOrderedActorsColor);
				Query = Partitioning->QueryInArea<USpatialComponent>(Systems, Position, Area);
			}
			else if (Mode == EDebugSpatialQueryEpicenterMode::Radius)
			{
				const auto Radius = Epicenter.GetValue().Get<1>()->Radius;
				DrawDebugSphere(World, Position, Radius, 32, Settings->DebugOrderedActorsColor);
				Query = Partitioning->QueryInRadius<USpatialComponent>(Systems, Position, Radius);
			}

			for (const auto& EnumeratedQueryItem : Query.Enumerate())
			{
				const auto Index = EnumeratedQueryItem.Get<0>();
				const auto QueryItem = EnumeratedQueryItem.Get<1>();
				auto* Actor = QueryItem.Get<0>();
				const auto Distance = QueryItem.Get<1>();

				const auto Location = Actor->GetActorLocation();
				const auto Content = FString::Printf(TEXT("#%u (%f)"), Index, Distance);
				DrawDebugString(World, FVector(0.0), Content, Actor, Settings->DebugOrderedActorsColor);
			}
		}
	}
}
