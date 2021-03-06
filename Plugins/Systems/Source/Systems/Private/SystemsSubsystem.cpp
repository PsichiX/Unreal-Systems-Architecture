#include "Systems/Public/SystemsSubsystem.h"

#include "Engine/World.h"
#include "Systems/Public/SystemsWorld.h"
#include "TimerManager.h"

USystemsSubsystem* USystemsSubsystem::Get(UWorld* World)
{
	if (IsValid(World))
	{
		auto* GameInstance = World->GetGameInstance<UGameInstance>();
		if (IsValid(GameInstance))
		{
			return GameInstance->GetSubsystem<ThisClass>();
		}
	}
	return nullptr;
}

void USystemsSubsystem::AcquireSystemsWorld(FName Id,
	TFunction<ThisClass::SystemSetupFunctor> SetupFunctor)
{
	auto* Systems =
		NewObject<USystemsWorld>(this, USystemsWorld::StaticClass());

	if (IsValid(Systems))
	{
		SetupFunctor(*Systems);
		Systems->SealAndInitialize();
		this->ToAdd.Add(Id, Systems);
	}
}

void USystemsSubsystem::BlueprintAcquireSystemsWorld(FName Id,
	FOnSetupSystemsWorld SetupDelegate)
{
	AcquireSystemsWorld(
		Id, [&](auto& Systems) { SetupDelegate.ExecuteIfBound(&Systems); });
}

void USystemsSubsystem::ReleaseSystemsWorld(FName Id)
{
	this->ToRemove.Add(Id);
}

USystemsWorld* USystemsSubsystem::GetSystemsWorld(FName Id)
{
	auto** Found = this->SystemsWorlds.Find(Id);
	if (Found != nullptr)
	{
		return *Found;
	}
	Found = this->ToAdd.Find(Id);
	return Found != nullptr ? *Found : nullptr;
}

void USystemsSubsystem::GetSystemsWorldsIds(TSet<FName>& Output) const
{
	this->SystemsWorlds.GetKeys(Output);
	this->ToAdd.GetKeys(Output);
}

void USystemsSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	if (IsValid(GetGameInstance()))
	{
		GetGameInstance()->GetTimerManager().SetTimerForNextTick(
			this, &ThisClass::OnTick);
	}
}

void USystemsSubsystem::OnTick()
{
	if (IsValid(GetGameInstance()) == false)
	{
		this->ToRemove.Reset();
		this->ToAdd.Reset();
		this->SystemsWorlds.Reset();
		return;
	}

	for (auto& Id : this->ToRemove)
	{
		this->SystemsWorlds.Remove(Id);
	}

	for (auto& Pair : this->ToAdd)
	{
		this->SystemsWorlds.Add(Pair.Key, Pair.Value);
	}

	for (auto& Pair : this->SystemsWorlds)
	{
		if (IsValid(Pair.Value))
		{
			Pair.Value->Process();
		}
	}

	GetGameInstance()->GetTimerManager().SetTimerForNextTick(
		this, &ThisClass::OnTick);
}
