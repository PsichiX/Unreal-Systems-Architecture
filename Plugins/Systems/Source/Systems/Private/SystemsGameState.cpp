#include "Systems/Public/SystemsGameState.h"

#include "Systems/Public/SystemsPipeline.h"

void ASystemsGameState::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	const auto NetMode = GetWorld()->GetNetMode();
	if (NetMode == NM_Client && this->SystemsPipeline)
	{
		this->SystemsPipeline->Install(GetWorld());
	}
}

void ASystemsGameState::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	const auto NetMode = GetWorld()->GetNetMode();
	if (NetMode == NM_Client && this->SystemsPipeline)
	{
		this->SystemsPipeline->Uninstall(GetWorld());
	}
}
