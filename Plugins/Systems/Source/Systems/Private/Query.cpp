#include "Systems/Public/Query.h"

#include "GameFramework/Actor.h"
#include "Systems/Public/SystemsComponent.h"
#include "Systems/Public/SystemsWorld.h"

UDynamicQuery::UDynamicQuery()
{
	this->BundleType = nullptr;
	this->Signature.Clear();
	this->CachedArchetypes.Reset();
	this->State.Set<FEndQueryState>({});
}

void UDynamicQuery::Setup(USystemsWorld* Systems, const UClass* Type)
{
	this->BundleType = Type;
	this->Signature.Clear();
	this->CachedArchetypes.Reset();
	this->State.Set<FEndQueryState>({});
	if (IsValid(Systems) && IsValid(Type))
	{
		SignatureFromBundle(Systems, Type);
		this->CachedArchetypes = Systems->FindQueryArchetypes(this->Signature, FArchetypeSignature());
		if (this->CachedArchetypes.Num() > 0)
		{
			this->State.Set<FStartQueryState>({Systems});
		}
	}
}

UObject* UDynamicQuery::Next()
{
	if (this->State.IsType<FStartQueryState>())
	{
		return NextStart(this->State.Get<FStartQueryState>());
	}
	else if (this->State.IsType<FCurrentQueryState>())
	{
		return NextCurrent(this->State.Get<FCurrentQueryState>());
	}
	Reset();
	return nullptr;
}

int UDynamicQuery::EstimateSizeLeft() const
{
	const USystemsWorld* Systems = nullptr;
	if (this->State.IsType<FStartQueryState>())
	{
		Systems = this->State.Get<FStartQueryState>().Owner;
	}
	else if (this->State.IsType<FCurrentQueryState>())
	{
		Systems = this->State.Get<FCurrentQueryState>().Owner;
	}
	if (IsValid(Systems) == false)
	{
		return 0;
	}
	auto Result = 0;
	for (const auto& Item : this->CachedArchetypes)
	{
		const auto* Archetype = Systems->Archetypes.Find(Item);
		if (Archetype != nullptr)
		{
			Result += Archetype->Actors.Num();
		}
	}
	return Result;
}

void UDynamicQuery::SignatureFromBundle(const USystemsWorld* Systems, const UClass* Type)
{
	this->Signature.Clear();
	for (TFieldIterator<FProperty> Iter(Type, EFieldIteratorFlags::IncludeSuper); Iter; ++Iter)
	{
		const auto* Property = *Iter;
		if (Property->IsA<FObjectProperty>())
		{
			const auto* ObjectProperty = CastField<FObjectProperty>(Property);
			if (IsValid(ObjectProperty->PropertyClass) && ObjectProperty->PropertyClass->IsChildOf<UActorComponent>())
			{
				const auto Found = Systems->ComponentTypeIndex(ObjectProperty->PropertyClass);
				if (Found.IsSet())
				{
					this->Signature.EnableBit(Found.GetValue());
				}
			}
		}
	}
}

void UDynamicQuery::Reset()
{
	this->State.Set<FEndQueryState>({});
}

UObject* UDynamicQuery::NextStart(FStartQueryState& Data)
{
	if (IsValid(Data.Owner) == false || this->CachedArchetypes.Num() <= 0)
	{
		Reset();
		return nullptr;
	}
	auto* Archetype = Data.Owner->Archetypes.Find(this->CachedArchetypes[0]);
	if (Archetype == nullptr || Archetype->Actors.Num() <= 0)
	{
		Reset();
		return nullptr;
	}
	this->State.Set<FCurrentQueryState>({Data.Owner, 0, 1});
	return CreateQueryItem(Archetype, 0);
}

UObject* UDynamicQuery::NextCurrent(FCurrentQueryState& Data)
{
	if (IsValid(Data.Owner) == false)
	{
		Reset();
		return nullptr;
	}
	while (Data.ArchetypeIndex < static_cast<uint32>(this->CachedArchetypes.Num()))
	{
		auto* Archetype = Data.Owner->Archetypes.Find(this->CachedArchetypes[Data.ArchetypeIndex]);
		if (Archetype == nullptr)
		{
			Reset();
			return nullptr;
		}
		if (Data.ActorIndex < static_cast<uint32>(Archetype->Actors.Num()))
		{
			const auto ActorIndex = Data.ActorIndex;
			++Data.ActorIndex;
			return CreateQueryItem(Archetype, ActorIndex);
		}
		Data.ActorIndex = 0;
		++Data.ArchetypeIndex;
	}
	Reset();
	return nullptr;
}

UObject* UDynamicQuery::CreateQueryItem(const FArchetype* Archetype, uint32 ActorIndex)
{
	auto* Result = NewObject<UObject>(GetTransientPackage(), this->BundleType);
	for (TFieldIterator<FProperty> Iter(this->BundleType, EFieldIteratorFlags::IncludeSuper); Iter; ++Iter)
	{
		const auto* Property = *Iter;
		if (Property->IsA<FObjectProperty>())
		{
			const auto* ObjectProperty = CastField<FObjectProperty>(Property);
			if (IsValid(ObjectProperty->PropertyClass))
			{
				if (ObjectProperty->PropertyClass->IsChildOf<AActor>())
				{
					auto Ptr = ObjectProperty->ContainerPtrToValuePtr<AActor*>(Result);
					*Ptr = Archetype->Actors[ActorIndex];
				}
				else if (ObjectProperty->PropertyClass->IsChildOf<UActorComponent>())
				{
					auto* Component = Archetype->IndexedComponentRaw(ActorIndex, ObjectProperty->PropertyClass);
					if (IsValid(Component))
					{
						auto Ptr = ObjectProperty->ContainerPtrToValuePtr<UActorComponent*>(Result);
						*Ptr = Component;
					}
				}
			}
		}
	}
	return Result;
}

FActorsIter::FActorsIter() : CachedArchetypes(), State()
{
	this->State.Set<FEndQueryState>({});
}

FActorsIter::FActorsIter(USystemsWorld* Systems) : CachedArchetypes(), State()
{
	this->State.Set<FEndQueryState>({});
	if (IsValid(Systems))
	{
		this->CachedArchetypes = Systems->AllArchetypes();
		if (this->CachedArchetypes.Num() > 0)
		{
			this->State.Set<FStartQueryState>({Systems});
		}
	}
}

TOptional<FActorsIter::Item> FActorsIter::Next()
{
	if (this->State.IsType<FStartQueryState>())
	{
		return NextStart(this->State.Get<FStartQueryState>());
	}
	else if (this->State.IsType<FCurrentQueryState>())
	{
		return NextCurrent(this->State.Get<FCurrentQueryState>());
	}
	return NextNone();
}

IterSizeHint FActorsIter::SizeHint() const
{
	if (this->State.IsType<FStartQueryState>())
	{
		const auto& Data = this->State.Get<FStartQueryState>();
		if (IsValid(Data.Owner))
		{
			const auto Count = Data.Owner->ActorsCount();
			return IterSizeHint{0, TOptional<uint32>(Count)};
		}
	}
	else if (this->State.IsType<FCurrentQueryState>())
	{
		const auto& Data = this->State.Get<FCurrentQueryState>();
		if (IsValid(Data.Owner))
		{
			const auto Count = Data.Owner->ActorsCount();
			return IterSizeHint{0, TOptional<uint32>(Count)};
		}
	}
	return IterSizeHint{0, TOptional<uint32>()};
}

TOptional<FActorsIter::Item> FActorsIter::NextNone()
{
	this->State.Set<FEndQueryState>({});
	return TOptional<FActorsIter::Item>();
}

TOptional<FActorsIter::Item> FActorsIter::NextStart(FStartQueryState& Data)
{
	if (IsValid(Data.Owner) == false || this->CachedArchetypes.Num() <= 0)
	{
		return NextNone();
	}
	auto* Archetype = Data.Owner->Archetypes.Find(this->CachedArchetypes[0]);
	if (Archetype == nullptr || Archetype->Actors.Num() <= 0)
	{
		return NextNone();
	}
	this->State.Set<FCurrentQueryState>({Data.Owner, 0, 1});
	return Archetype->Actors[0];
}

TOptional<FActorsIter::Item> FActorsIter::NextCurrent(FCurrentQueryState& Data)
{
	if (IsValid(Data.Owner) == false)
	{
		return NextNone();
	}
	while (Data.ArchetypeIndex < static_cast<uint32>(this->CachedArchetypes.Num()))
	{
		auto* Archetype = Data.Owner->Archetypes.Find(this->CachedArchetypes[Data.ArchetypeIndex]);
		if (Archetype == nullptr)
		{
			return NextNone();
		}
		if (Data.ActorIndex < static_cast<uint32>(Archetype->Actors.Num()))
		{
			const auto ActorIndex = Data.ActorIndex;
			++Data.ActorIndex;
			return Archetype->Actors[ActorIndex];
		}
		Data.ActorIndex = 0;
		++Data.ArchetypeIndex;
	}
	return NextNone();
}
