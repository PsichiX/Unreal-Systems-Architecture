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
		this->CachedArchetypes = Systems->FindQueryArchetypes(this->Signature);
		if (this->CachedArchetypes.Num() > 0)
		{
			this->State.Set<FStartQueryState>({Systems});
		}
	}
}

bool UDynamicQuery::Next(UObject* Output)
{
	if (Output == nullptr || Output->GetClass() != this->BundleType)
	{
		return NextNone();
	}
	if (this->State.IsType<FStartQueryState>())
	{
		return NextStart(this->State.Get<FStartQueryState>(), Output);
	}
	else if (this->State.IsType<FCurrentQueryState>())
	{
		return NextCurrent(this->State.Get<FCurrentQueryState>(), Output);
	}
	return NextNone();
}

void UDynamicQuery::NextBranched(UObject* Output, EDynamicQueryBranch& Branches)
{
	Branches =
		Next(Output) ? EDynamicQueryBranch::Continue : EDynamicQueryBranch::End;
}

void UDynamicQuery::SignatureFromBundle(const USystemsWorld* Systems,
	const UClass* Type)
{
	this->Signature.Clear();
	for (TFieldIterator<FProperty> Iter(
			 Type, EFieldIteratorFlags::IncludeSuper);
		 Iter;
		 ++Iter)
	{
		const auto* Property = *Iter;
		if (Property->IsA<FObjectProperty>())
		{
			const auto* ObjectProperty = CastField<FObjectProperty>(Property);
			if (IsValid(ObjectProperty->PropertyClass) &&
				ObjectProperty->PropertyClass->IsChildOf<UActorComponent>())
			{
				const auto Found =
					Systems->ComponentTypeIndex(ObjectProperty->PropertyClass);
				if (Found.IsSet())
				{
					this->Signature.EnableBit(Found.GetValue());
				}
			}
		}
	}
}

bool UDynamicQuery::NextNone()
{
	this->State.Set<FEndQueryState>({});
	return false;
}

bool UDynamicQuery::NextStart(FStartQueryState& Data, UObject* Output)
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
	this->State.Set<FCurrentQueryState>({Data.Owner, 0, 0});
	FillQueryItem(Output, Archetype, 0);
	return true;
}

bool UDynamicQuery::NextCurrent(FCurrentQueryState& Data, UObject* Output)
{
	if (IsValid(Data.Owner) == false)
	{
		return NextNone();
	}
	while (
		Data.ArchetypeIndex < static_cast<uint32>(this->CachedArchetypes.Num()))
	{
		auto* Archetype = Data.Owner->Archetypes.Find(
			this->CachedArchetypes[Data.ArchetypeIndex]);
		if (Archetype == nullptr)
		{
			return NextNone();
		}
		++Data.ActorIndex;
		if (Data.ActorIndex < static_cast<uint32>(Archetype->Actors.Num()))
		{
			FillQueryItem(Output, Archetype, Data.ActorIndex);
			return true;
		}
		++Data.ArchetypeIndex;
	}
	return NextNone();
}

void UDynamicQuery::FillQueryItem(UObject* Output,
	const FArchetype* Archetype,
	uint32 ActorIndex)
{
	for (TFieldIterator<FProperty> Iter(
			 this->BundleType, EFieldIteratorFlags::IncludeSuper);
		 Iter;
		 ++Iter)
	{
		const auto* Property = *Iter;
		if (Property->IsA<FObjectProperty>())
		{
			const auto* ObjectProperty = CastField<FObjectProperty>(Property);
			if (IsValid(ObjectProperty->PropertyClass))
			{
				if (ObjectProperty->PropertyClass->IsChildOf<AActor>())
				{
					auto Ptr =
						ObjectProperty->ContainerPtrToValuePtr<AActor*>(Output);
					*Ptr = Archetype->Actors[0];
				}
				else if (ObjectProperty->PropertyClass
							 ->IsChildOf<UActorComponent>())
				{
					auto* Component = Archetype->IndexedComponentRaw(
						ActorIndex, ObjectProperty->PropertyClass);
					if (IsValid(Component))
					{
						auto Ptr =
							ObjectProperty
								->ContainerPtrToValuePtr<UActorComponent*>(
									Output);
						*Ptr = Component;
					}
				}
			}
		}
	}
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
	while (
		Data.ArchetypeIndex < static_cast<uint32>(this->CachedArchetypes.Num()))
	{
		auto* Archetype = Data.Owner->Archetypes.Find(
			this->CachedArchetypes[Data.ArchetypeIndex]);
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
