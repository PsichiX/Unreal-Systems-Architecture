#include "Tutorial/Systems/Persistent/TutorialImpactSystem.h"

#include "Resources/SpatialDiscretization.h"
#include "Systems/Public/SystemsWorld.h"
#include "SystemsSpatialQuery/Public/Resources/SpatialInformation.h"

const FName Z_BUTTON = FName(TEXT("Z"));
const FName X_BUTTON = FName(TEXT("X"));

void UTutorialImpactSystem::Run(USystemsWorld& Systems)
{
	const auto* PlayerController = Systems.GetWorld()->GetFirstPlayerController();
	if (IsValid(PlayerController) == false)
	{
		return;
	}

	auto* Information = Systems.Resource<USpatialInformation>();
	if (IsValid(Information) == false)
	{
		return;
	}
	if (PlayerController->IsInputKeyDown(X_BUTTON))
	{
		Information->Reset();
		return;
	}
	if (PlayerController->IsInputKeyDown(Z_BUTTON) == false)
	{
		return;
	}

	const auto* Discretization = Systems.Resource<USpatialDiscretization>();
	if (IsValid(Information) == false)
	{
		return;
	}

	const auto DeltaTime = GetWorld()->GetDeltaSeconds();
	FHitResult Hit;
	if (PlayerController->GetHitResultUnderCursor(ECC_GameTraceChannel1, true, Hit))
	{
		const auto ActorsWeights =
			Discretization->TriangleActorWeightsForPoint(FVector2D(Hit.ImpactPoint));
		Information->ApplyWeighted(this->ImpactId, this->ImpactValue * DeltaTime, ActorsWeights);
	}
}