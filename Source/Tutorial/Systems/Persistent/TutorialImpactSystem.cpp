#include "Tutorial/Systems/Persistent/TutorialImpactSystem.h"

#include "Resources/SpatialDiscretization.h"
#include "Systems/Public/SystemsWorld.h"
#include "SystemsSpatialQuery/Public/Resources/SpatialInformation.h"

const FName Z_BUTTON = FName(TEXT("Z"));

void UTutorialImpactSystem::Run(USystemsWorld& Systems)
{
	const auto* PlayerController = Systems.GetWorld()->GetFirstPlayerController();
	if (IsValid(PlayerController) == false || PlayerController->IsInputKeyDown(Z_BUTTON) == false)
	{
		return;
	}

	auto* Information = Systems.Resource<USpatialInformation>();
	const auto* Discretization = Systems.Resource<USpatialDiscretization>();
	if (IsValid(Information) == false || IsValid(Discretization) == false)
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