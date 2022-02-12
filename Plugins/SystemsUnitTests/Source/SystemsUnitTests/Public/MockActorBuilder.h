#pragma once

#include "CoreMinimal.h"

#include "MockActorBuilder.generated.h"

// TODO: document.
USTRUCT()
struct SYSTEMSUNITTESTS_API FMockActorBuilder
{
	GENERATED_BODY()

public:
	FMockActorBuilder();

	FMockActorBuilder(UWorld* TheWorld,
		UClass* ActorType,
		FVector Position = FVector(0),
		FRotator Rotation = FRotator());

	template <class T>
	static FMockActorBuilder Make(UWorld* TheWorld,
		FVector Position = FVector(0),
		FRotator Rotation = FRotator())
	{
		return FMockActorBuilder(
			TheWorld, T::StaticClass(), Position, Rotation);
	}

	FMockActorBuilder& AddComponentRaw(const UClass* ComponentType);

	template <class T>
	FMockActorBuilder& AddComponent()
	{
		return AddComponentRaw(T::StaticClass());
	}

	template <class T>
	FMockActorBuilder& WithComponent(TFunction<void(T*)> Functor)
	{
		if (IsValid(this->Actor) && Functor)
		{
			auto* Component = this->Actor->FindComponentByClass<T>();
			if (IsValid(Component))
			{
				Functor(Component);
			}
		}
		return *this;
	}

	template <class T>
	FMockActorBuilder& AddComponentWithSetup(TFunction<void(T*)>&& Functor)
	{
		return AddComponent<T>().WithComponent<T>(MoveTempIfPossible(Functor));
	}

	AActor* GetActor();

private:
	UPROPERTY()
	AActor* Actor;
};