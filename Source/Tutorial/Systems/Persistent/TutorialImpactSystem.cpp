#include "Tutorial/Systems/Persistent/TutorialImpactSystem.h"

#include "Components/SpatialComponent.h"
#include "Resources/SpatialDiscretization.h"
#include "Resources/SpatialPartitioning.h"
#include "Systems/Public/SystemsWorld.h"
#include "SystemsSpatialQuery/Public/Resources/SpatialInformation.h"

const FName ACCUMULATE_BUTTON = FName(TEXT("Z"));
const FName HIT_BUTTON = FName(TEXT("X"));
const FName CLEAR_BUTTON = FName(TEXT("C"));

void UTutorialImpactSystem::Run(USystemsWorld& Systems)
{
	const auto* PlayerController = Systems.GetWorld()->GetFirstPlayerController();
	if (IsValid(PlayerController) == false)
	{
		return;
	}

	if (PlayerController->IsInputKeyDown(CLEAR_BUTTON))
	{
		ClearImpact(Systems);
	}
	else if (PlayerController->WasInputKeyJustPressed(HIT_BUTTON))
	{
		ApplyImpact(1.0, PlayerController, Systems);
	}
	else if (PlayerController->IsInputKeyDown(ACCUMULATE_BUTTON))
	{
		const auto DeltaTime = GetWorld()->GetDeltaSeconds();
		ApplyImpact(DeltaTime, PlayerController, Systems);
	}
}

void UTutorialImpactSystem::ClearImpact(USystemsWorld& Systems)
{
	auto* Information = Systems.Resource<USpatialInformation>();
	if (IsValid(Information))
	{
		Information->Reset();
	}
}

void UTutorialImpactSystem::ApplyImpact(double Scale,
	const APlayerController* PlayerController,
	USystemsWorld& Systems) const
{
	auto* Information = Systems.Resource<USpatialInformation>();
	if (IsValid(Information) == false)
	{
		return;
	}
	const auto* Discretization = Systems.Resource<USpatialDiscretization>();
	if (IsValid(Discretization) == false)
	{
		return;
	}

	FHitResult Hit;
	if (PlayerController->GetHitResultUnderCursor(ECC_GameTraceChannel1, true, Hit))
	{
		const auto ActorsWeights =
			Discretization->TriangleActorWeightsForPoint(FVector2D(Hit.ImpactPoint));
		Information->ApplyWeighted(this->ImpactId, this->ImpactValue * Scale, ActorsWeights);
	}
}