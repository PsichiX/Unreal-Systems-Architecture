#include "Systems/Public/SystemsStatics.h"

#include "Systems/Public/SystemsComponent.h"
#include "Systems/Public/SystemsSubsystem.h"
#include "Systems/Public/SystemsWorld.h"

USystemsWorld* USystemsStatics::GetSystemsWorld(FName Id, UObject* WorldContext)
{
	if (IsValid(WorldContext) == false)
	{
		return nullptr;
	}
	auto* Subsystem = USystemsSubsystem::Get(WorldContext->GetWorld());
	return IsValid(Subsystem) ? Subsystem->GetSystemsWorld(Id) : nullptr;
}

void USystemsStatics::AddComponent(UActorComponent* Component, const TSet<FName>& SystemsWorlds, UObject* WorldContext)
{
	if (IsValid(WorldContext) == false)
	{
		return;
	}
	auto* Subsystem = USystemsSubsystem::Get(WorldContext->GetWorld());
	if (IsValid(Subsystem))
	{
		for (const auto& Id : SystemsWorlds)
		{
			auto* Systems = Subsystem->GetSystemsWorld(Id);
			if (IsValid(Systems))
			{
				Systems->AddComponent(Component);
			}
		}
	}
}

void USystemsStatics::AddComponentEverywhere(UActorComponent* Component, UObject* WorldContext)
{
	if (IsValid(WorldContext) == false)
	{
		return;
	}
	auto* Subsystem = USystemsSubsystem::Get(WorldContext->GetWorld());
	if (IsValid(Subsystem))
	{
		auto Ids = TSet<FName>();
		Subsystem->GetSystemsWorldsIds(Ids);
		for (auto& Id : Ids)
		{
			auto* Systems = Subsystem->GetSystemsWorld(Id);
			if (IsValid(Systems))
			{
				Systems->AddComponent(Component);
			}
		}
	}
}

void USystemsStatics::RemoveComponent(UActorComponent* Component, UObject* WorldContext)
{
	if (IsValid(WorldContext) == false)
	{
		return;
	}
	auto* Subsystem = USystemsSubsystem::Get(WorldContext->GetWorld());
	if (IsValid(Subsystem))
	{
		auto Ids = TSet<FName>();
		Subsystem->GetSystemsWorldsIds(Ids);
		for (auto& Id : Ids)
		{
			auto* Systems = Subsystem->GetSystemsWorld(Id);
			if (IsValid(Systems))
			{
				Systems->RemoveComponent(Component);
			}
		}
	}
}

UObject* USystemsStatics::GetResourceRaw(FName Id, const UClass* Type, UObject* WorldContext)
{
	if (IsValid(WorldContext) == false)
	{
		return nullptr;
	}
	auto* Systems = ThisClass::GetSystemsWorld(Id, WorldContext->GetWorld());
	return IsValid(Systems) ? Systems->ResourceRaw(Type) : nullptr;
}