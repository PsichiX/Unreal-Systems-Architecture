#include "Boids/UI/WidgetBoidsInterface.h"

#include "Components/Button.h"
#include "Systems/Public/SystemsStatics.h"
#include "Systems/Public/SystemsWorld.h"

#include "Boids/BoidsGameMode.h"
#include "Boids/Resources/GameEvents.h"
#include "Boids/Systems/OneShot/SpawnFoodSystem.h"

void UWidgetBoidsInterface::NativeConstruct()
{
	Super::NativeConstruct();

	if (IsValid(this->MovementStep))
	{
		this->MovementStep->OnClicked.AddUniqueDynamic(this, &ThisClass::OnMovementStepAction);
	}

	if (IsValid(this->SpawnFood))
	{
		this->SpawnFood->OnClicked.AddUniqueDynamic(this, &ThisClass::OnSpawnFoodAction);
	}
}

void UWidgetBoidsInterface::NativeDestruct()
{
	Super::NativeDestruct();

	if (IsValid(this->MovementStep))
	{
		this->MovementStep->OnClicked.RemoveAll(this);
	}

	if (IsValid(this->SpawnFood))
	{
		this->SpawnFood->OnClicked.RemoveAll(this);
	}
}

void UWidgetBoidsInterface::OnMovementStepAction()
{
	auto* GameEvents =
		USystemsStatics::GetResource<UGameEvents>(ABoidsGameMode::SYSTEMS_WORLD, GetWorld());
	if (IsValid(GameEvents))
	{
		GameEvents->MovementStep.Send(FMovementStep{});
	}
}

void UWidgetBoidsInterface::OnSpawnFoodAction()
{
	auto* Systems = USystemsStatics::GetSystemsWorld(ABoidsGameMode::SYSTEMS_WORLD, GetWorld());
	if (IsValid(Systems))
	{
		SpawnFoodSystem(*Systems);
	}
}
