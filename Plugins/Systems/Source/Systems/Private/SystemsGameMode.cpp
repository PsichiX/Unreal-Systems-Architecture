#include "Systems/Public/SystemsGameMode.h"

#include "Systems/Public/SystemsPipeline.h"

void ASystemsGameMode::InitGame(const FString& MapName,
	const FString& Options,
	FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);

	const auto NetMode = GetWorld()->GetNetMode();
	if (NetMode < NM_Client && this->SystemsPipeline)
	{
		this->SystemsPipeline->Install(GetWorld());
	}
}

void ASystemsGameMode::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	const auto NetMode = GetWorld()->GetNetMode();
	if (NetMode < NM_Client && this->SystemsPipeline)
	{
		this->SystemsPipeline->Uninstall(GetWorld());
	}
}
