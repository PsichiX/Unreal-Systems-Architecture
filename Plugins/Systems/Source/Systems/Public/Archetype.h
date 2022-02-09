#pragma once

#include "CoreMinimal.h"

#include "Systems/Public/ArchetypeSignature.h"

#include "Archetype.generated.h"

class UActorComponent;

USTRUCT()
struct SYSTEMS_API FArchetype
{
	GENERATED_BODY()

	friend class USystemsWorld;
	template <class... T>
	friend struct TQuery;
	friend class UDynamicQuery;
	friend struct FActorsIter;

public:
	FArchetype();

	FArchetype(const FArchetypeSignature Signature);

	FArchetype(const FArchetypeSignature Signature, const uint32 Capacity);

	TOptional<uint32> FindActorIndex(AActor* Actor) const;

	TOptional<uint32> FindActorIdIndex(uint32 Id) const;

	AActor* FindActorById(uint32 Id) const;

	UActorComponent* ComponentRaw(AActor* Actor, const UClass* Type) const;

	UActorComponent* IndexedComponentRaw(uint32 ActorIndex,
		const UClass* Type) const;

	template <class T>
	T* IndexedComponent(uint32 ActorIndex) const
	{
		//// [ignore]
		return Cast<T>(IndexedComponentRaw(ActorIndex, T::StaticClass()));
		//// [/ignore]
	}

	TOptional<TArray<UActorComponent*>> ConsumeSwapActorComponents(
		AActor* Actor);

	TOptional<TArray<UActorComponent*>> ConsumeSwapActorIdComponents(uint32 Id);

	void RebuildComponentTypesMap(const USystemsWorld& Systems);

	TOptional<uint32> ComponentOffset(const UActorComponent* Component) const;

private:
	TOptional<TArray<UActorComponent*>> ConsumeSwapComponents(uint32 Location);

	UPROPERTY()
	uint32 Stride = 0;

	UPROPERTY()
	FArchetypeSignature Signature = {};

	UPROPERTY()
	/// {class type uid: offset}
	TMap<uint32, uint32> ComponentTypesMap = {};

	UPROPERTY()
	TArray<AActor*> Actors = {};

	UPROPERTY()
	TArray<uint32> ActorsIds = {};

	UPROPERTY()
	TArray<UActorComponent*> Components = {};
};
