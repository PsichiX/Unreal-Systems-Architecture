#include "SystemsUnitTests/Public/SystemsUnitTest.h"

#include "Engine/World.h"
#include "SystemsPipeline.h"

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

TObjectPtr<USystemsWorld> AcquireMockWorld(UWorld* TheWorld, TFunction<void(USystemsWorld&)> Setup)
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
	return {};
}

TObjectPtr<USystemsWorld> AcquireMockWorldWithPipeline(UWorld* TheWorld, TObjectPtr<USystemsPipeline> Pipeline)
{
	return AcquireMockWorld(TheWorld,
		[&](auto& Systems)
		{
			TSet<TObjectPtr<USystemsPipeline>> Ignore = {};
			Pipeline->InstallInto(Systems, Ignore);
		});
}

TObjectPtr<USystemsWorld> AcquireMockWorldWithPipeline(UWorld* TheWorld, FString PipelineAssetPath)
{
	const auto AssetObject = FSoftObjectPtr(FSoftObjectPath(PipelineAssetPath));
	auto* Pipeline = Cast<USystemsPipeline>(AssetObject.LoadSynchronous());
	checkf(IsValid(Pipeline), TEXT("Could not load Systems pipeline from: %s"), *PipelineAssetPath);
	return AcquireMockWorldWithPipeline(TheWorld, Pipeline);
}