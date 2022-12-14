#include "SystemsUnitTests/Public/SystemsUnitTest.h"

#include "Engine/World.h"

bool SystemsUnitTest(TFunction<bool(UWorld*)> Functor)
{
	auto* World = UWorld::CreateWorld(EWorldType::Game, true);
	if (IsValid(World))
	{
		const auto bStatus = Functor(World);
		World->DestroyWorld(true, nullptr);
		return bStatus;
	}
	return false;
}

USystemsWorld* AcquireMockWorld(UWorld* TheWorld, TFunction<void(USystemsWorld&)> Setup)
{
	if (IsValid(TheWorld))
	{
		auto* Systems = NewObject<USystemsWorld>(TheWorld, USystemsWorld::StaticClass());
		if (IsValid(Systems))
		{
			Setup(*Systems);
			Systems->SealAndInitialize();
			return Systems;
		}
	}
	return nullptr;
}