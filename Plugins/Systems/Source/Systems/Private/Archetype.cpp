#include "Systems/Public/Archetype.h"

#include "GameFramework/Actor.h"
#include "Systems/Public/SystemsComponent.h"
#include "Systems/Public/SystemsWorld.h"

FArchetype::FArchetype()
	: Stride(0)
	, Signature(FArchetypeSignature())
	, ComponentTypesMap()
	, Actors()
	, Components()
{
}

FArchetype::FArchetype(const FArchetypeSignature Signature)
	: Stride(Signature.CountBits())
	, Signature(Signature)
	, ComponentTypesMap()
	, Actors()
	, Components()
{
}

FArchetype::FArchetype(const FArchetypeSignature Signature,
	const uint32 Capacity)
	: Stride(Signature.CountBits())
	, Signature(Signature)
	, ComponentTypesMap()
	, Actors()
	, Components()
{
	this->Actors.Reserve(Capacity);
	this->ActorsIds.Reserve(Capacity);
	this->Components.Reserve(Capacity * this->Stride);
}

TOptional<uint32> FArchetype::FindActorIndex(AActor* Actor) const
{
	if (IsValid(Actor) == false)
	{
		return TOptional<uint32>();
	}
	const auto Found = this->Actors.Find(Actor);
	return Found >= 0 ? static_cast<uint32>(Found) : TOptional<uint32>();
}

TOptional<uint32> FArchetype::FindActorIdIndex(uint32 Id) const
{
	const auto Found = this->ActorsIds.Find(Id);
	return Found >= 0 ? static_cast<uint32>(Found) : TOptional<uint32>();
}

AActor* FArchetype::FindActorById(uint32 Id) const
{
	const auto Index = FindActorIdIndex(Id);
	return Index.IsSet() ? this->Actors[Index.GetValue()] : nullptr;
}

UActorComponent* FArchetype::ComponentRaw(AActor* Actor,
	const UClass* Type) const
{
	const auto Index = FindActorIndex(Actor);
	return Index.IsSet() ? IndexedComponentRaw(Index.GetValue(), Type)
						 : nullptr;
}

UActorComponent* FArchetype::IndexedComponentRaw(uint32 ActorIndex,
	const UClass* Type) const
{
	if (ActorIndex >= static_cast<uint32>(this->Actors.Num()) ||
		IsValid(Type) == false)
	{
		return nullptr;
	}
	const auto Id = Type->GetUniqueID();
	const auto* OffsetFound = this->ComponentTypesMap.Find(Id);
	if (OffsetFound != nullptr)
	{
		const auto Offset = ActorIndex * this->Stride + *OffsetFound;
		if (Offset < static_cast<uint32>(this->Components.Num()))
		{
			return this->Components[Offset];
		}
	}
	return nullptr;
}

TOptional<TArray<UActorComponent*>> FArchetype::ConsumeSwapActorComponents(
	AActor* Actor)
{
	const auto Location = FindActorIndex(Actor);
	return Location.IsSet() ? ConsumeSwapComponents(Location.GetValue())
							: TOptional<TArray<UActorComponent*>>();
}

TOptional<TArray<UActorComponent*>> FArchetype::ConsumeSwapActorIdComponents(
	uint32 Id)
{
	const auto Location = FindActorIdIndex(Id);
	return Location.IsSet() ? ConsumeSwapComponents(Location.GetValue())
							: TOptional<TArray<UActorComponent*>>();
}

TOptional<TArray<UActorComponent*>> FArchetype::ConsumeSwapComponents(
	uint32 Location)
{
	if (Location < static_cast<uint32>(this->Actors.Num()))
	{
		this->Actors.RemoveAtSwap(Location, 1, false);
		this->ActorsIds.RemoveAtSwap(Location, 1, false);
		const auto View =
			MakeArrayView(this->Components).Slice(Location * Stride, Stride);
		const auto Result = TArray<UActorComponent*>(View);
		// We go from last to first because of remove-swap always moves last
		// item to removed location so we need to keep ordering when swapping
		// components.
		for (uint32 Index = 0; Index < this->Stride; ++Index)
		{
			this->Components.RemoveAtSwap(
				Location * Stride + this->Stride - 1 - Index);
		}
		return Result;
	}
	return TOptional<TArray<UActorComponent*>>();
}

void FArchetype::RebuildComponentTypesMap(const USystemsWorld& Systems)
{
	for (uint32 Index = 0; Index < Systems.ComponentTypesCount; ++Index)
	{
		if (this->Signature.HasBit(Index))
		{
			const auto TypeId = Systems.ComponentTypes[Index];
			this->ComponentTypesMap.Add(TypeId, this->ComponentTypesMap.Num());
		}
	}
}

TOptional<uint32> FArchetype::ComponentOffset(
	const UActorComponent* Component) const
{
	if (IsValid(Component) == false)
	{
		return TOptional<uint32>();
	}
	const auto Id = Component->GetClass()->GetUniqueID();
	const auto* Found = this->ComponentTypesMap.Find(Id);
	if (Found != nullptr)
	{
		return TOptional<uint32>(*Found);
	}
	return TOptional<uint32>();
}
