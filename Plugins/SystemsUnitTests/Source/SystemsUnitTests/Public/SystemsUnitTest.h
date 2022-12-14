#pragma once

#include "CoreMinimal.h"

#include "Systems/Public/Iterator.h"
#include "Systems/Public/SystemsWorld.h"
#include "SystemsUnitTests/Public/MockActorBuilder.h"

// TODO: document.
bool SYSTEMSUNITTESTS_API SystemsUnitTest(TFunction<bool(UWorld*)> Functor);

// TODO: document.
USystemsWorld SYSTEMSUNITTESTS_API* AcquireMockWorld(UWorld* TheWorld, TFunction<void(USystemsWorld&)> Setup);

// TODO: document.
template <class T>
T* AcquireMockResource(UWorld* TheWorld, TFunction<void(T&)> Setup)
{
	if (IsValid(TheWorld))
	{
		auto* Resource = NewObject<T>(TheWorld, T::StaticClass());
		if (IsValid(Resource))
		{
			Setup(*Resource);
			return Resource;
		}
	}
	return nullptr;
}