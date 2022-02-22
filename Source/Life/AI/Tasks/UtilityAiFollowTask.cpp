#include "Life/AI/Tasks/UtilityAiFollowTask.h"

#include "NavigationPath.h"
#include "NavigationSystem.h"
#include "Systems/Public/SystemsWorld.h"

#include "Life/AI/Reasoner/UtilityAiMemory.h"
#include "Life/Components/FollowPathComponent.h"

bool UUtilityAiFollowTask::OnEnter(AActor* Actor,
	USystemsWorld& Systems,
	FUtilityAiMemory& Memory,
	bool bChanged)
{
	Super::OnEnter(Actor, Systems, Memory, bChanged);

	auto* Follow = Systems.Component<UFollowPathComponent>(Actor);
	if (IsValid(Follow) == false)
	{
		return false;
	}

	const auto Position = Actor->GetActorLocation();
	if (this->bTargetCanMove == false && bChanged == false &&
		Follow->HasReachedPathEnd(FVector2D(Position)) == false)
	{
		return true;
	}

	const auto* TargetActor = Memory.Access(this->TargetMemoryProperty).CastObject<AActor>();
	if (IsValid(TargetActor))
	{
		const auto TargetPosition = TargetActor->GetActorLocation();
		auto* Path = UNavigationSystemV1::FindPathToLocationSynchronously(
			Systems.GetWorld(), Position, TargetPosition);

		if (IsValid(Path))
		{
			Follow->SetPathFromPointsList(Path->PathPoints);
			return true;
		}
	}
	return false;
}

void UUtilityAiFollowTask::OnExit(AActor* Actor, USystemsWorld& Systems, FUtilityAiMemory& Memory)
{
	Super::OnExit(Actor, Systems, Memory);

	auto* Follow = Systems.Component<UFollowPathComponent>(Actor);
	if (IsValid(Follow))
	{
		Follow->ClearPath();
	}
}
