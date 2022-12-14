#include "Systems/Public/SystemsPawn.h"

#include "Systems/Public/Iterator.h"
#include "Systems/Public/SystemsComponent.h"
#include "Systems/Public/SystemsStatics.h"
#include "Systems/Public/SystemsWorld.h"

const FName SYSTEMS_TAG = FName(TEXT("Systems"));

ASystemsPawn::ASystemsPawn()
{
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;
}

void ASystemsPawn::BeginPlay()
{
	Super::BeginPlay();

	auto* Systems = USystemsStatics::GetSystemsWorld(this->SystemsWorld, GetWorld());
	if (IsValid(Systems) == false)
	{
		return;
	}

	for (auto* Component : GetComponents())
	{
		if (Component->GetClass()->IsChildOf<USystemsActorComponent>() == false &&
			Component->ComponentHasTag(SYSTEMS_TAG))
		{
			Systems->AddComponent(Component);
		}
	}
}

void ASystemsPawn::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	auto* Systems = USystemsStatics::GetSystemsWorld(this->SystemsWorld, GetWorld());
	if (IsValid(Systems) == false)
	{
		return;
	}

	for (auto* Component : GetComponents())
	{
		if (Component->GetClass()->IsChildOf<USystemsActorComponent>() == false &&
			Component->ComponentHasTag(SYSTEMS_TAG))
		{
			Systems->RemoveComponent(Component);
		}
	}
}
