#include "ReactiveUserInterfaceSystems/Public/UiChangeDetectionBlueprintLibrary.h"

#include "ReactiveUserInterfaceSystems/Public/Resources/UiChangeDetection.h"
#include "Systems/Public/SystemsWorld.h"

FUiChangeSignature UUiChangeDetectionBlueprintLibrary::NewUiChangeSignature(
	const USystemsWorld* Systems,
	const TArray<UClass*>& Components,
	const TArray<UClass*>& Resources)
{
	FUiChangeSignature Result = {};
	for (const auto* Type : Components)
	{
		if (IsValid(Type))
		{
			Result.ComponentRaw(Type, Systems);
		}
	}
	for (const auto* Type : Resources)
	{
		if (IsValid(Type))
		{
			Result.ResourceRaw(Type);
		}
	}
	return Result;
}