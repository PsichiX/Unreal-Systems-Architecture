#include "ReactiveSystems/Public/SystemsChangeDetectionBlueprintLibrary.h"

#include "ReactiveSystems/Public/Resources/SystemsChangeDetection.h"
#include "Systems/Public/SystemsWorld.h"

FSystemsChangeSignature
USystemsChangeDetectionBlueprintLibrary::NewChangeSignature(
	const USystemsWorld* Systems,
	const TArray<UClass*>& Components,
	const TArray<UClass*>& Resources)
{
	FSystemsChangeSignature Result = {};
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