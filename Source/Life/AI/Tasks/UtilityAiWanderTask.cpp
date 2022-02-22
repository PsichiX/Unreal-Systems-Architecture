#include "Life/AI/Tasks/UtilityAiWanderTask.h"

#include "NavigationPath.h"
#include "NavigationSystem.h"
#include "Systems/Public/SystemsWorld.h"

#include "Life/Components/FollowPathComponent.h"

bool UUtilityAiWanderTask::OnEnter(AActor* Actor,
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
	if (bChanged == false && Follow->HasReachedPathEnd(FVector2D(Position)) == false)
	{
		return true;
	}

	const auto Offset = FMath::RandPointInCircle(this->Range);
	const auto TargetPosition = Position + FVector(Offset, 0);
	auto* Path = UNavigationSystemV1::FindPathToLocationSynchronously(
		Systems.GetWorld(), Position, TargetPosition);

	if (IsValid(Path) && Path->PathPoints.Num() > 0)
	{
		Follow->SetPathFromPointsList(Path->PathPoints);
		return true;
	}
	return false;
}

void UUtilityAiWanderTask::OnExit(AActor* Actor, USystemsWorld& Systems, FUtilityAiMemory& Memory)
{
	Super::OnExit(Actor, Systems, Memory);

	auto* Follow = Systems.Component<UFollowPathComponent>(Actor);
	if (IsValid(Follow))
	{
		Follow->ClearPath();
	}
}
