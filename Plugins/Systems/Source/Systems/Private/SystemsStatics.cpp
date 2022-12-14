#include "Systems/Public/SystemsStatics.h"

#include "Systems/Public/SystemsComponent.h"
#include "Systems/Public/SystemsSubsystem.h"
#include "Systems/Public/SystemsWorld.h"

USystemsWorld* USystemsStatics::GetSystemsWorld(FName Id, UWorld* World)
{
	auto* Subsystem = USystemsSubsystem::Get(World);
	return IsValid(Subsystem) ? Subsystem->GetSystemsWorld(Id) : nullptr;
}

void USystemsStatics::AddComponent(UActorComponent* Component, const TSet<FName>& SystemsWorlds, UWorld* World)
{
	auto* Subsystem = USystemsSubsystem::Get(World);
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

void USystemsStatics::AddComponentEverywhere(UActorComponent* Component, UWorld* World)
{
	auto* Subsystem = USystemsSubsystem::Get(World);
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

void USystemsStatics::RemoveComponent(UActorComponent* Component, UWorld* World)
{
	auto* Subsystem = USystemsSubsystem::Get(World);
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

UObject* USystemsStatics::GetResourceRaw(FName Id, const UClass* Type, UWorld* World)
{
	auto* Systems = ThisClass::GetSystemsWorld(Id, World);
	return IsValid(Systems) ? Systems->ResourceRaw(Type) : nullptr;
}