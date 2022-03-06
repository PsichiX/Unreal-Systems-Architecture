#pragma once

#include "CoreMinimal.h"

#include "Misc/TVariant.h"
#include "Systems/Public/Archetype.h"
#include "Systems/Public/ArchetypeSignature.h"
#include "Systems/Public/Iterator.h"

#include "Query.generated.h"

class USystemsWorld;

USTRUCT()
struct SYSTEMS_API FStartQueryState
{
	GENERATED_BODY()

	UPROPERTY()
	USystemsWorld* Owner = nullptr;
};

USTRUCT()
struct SYSTEMS_API FCurrentQueryState
{
	GENERATED_BODY()

	UPROPERTY()
	USystemsWorld* Owner = nullptr;

	UPROPERTY()
	uint32 ArchetypeIndex = 0;

	UPROPERTY()
	uint32 ActorIndex = 0;
};

USTRUCT()
struct SYSTEMS_API FEndQueryState
{
	GENERATED_BODY()
};

UENUM(BlueprintType)
enum class EDynamicQueryBranch : uint8
{
	Continue,
	End,
};

/// Dynamic query useful for performing system world queries in blueprints.
///
/// One of the goals of Systems Architecture is to provide game designers with
/// means to prototype systems without need for programmers. Dynamic query is a
/// way of performing systems world queries in blueprints. Although blueprint
/// API doesn't have same ergonomics as C++ API because of the lack of lazy
/// iterators on blueprints side, technical game designers still might benefit
/// from quickly testing simple systems without involving programmers into the
/// process and being able to quickly test the idea - it makes failing quicker
/// than it would without technical game designers getting their hands dirty
/// once in a while, which is beneficial to the whole production process in long
/// term.
///
/// See [`class: USystemsWorld::SpawnQuery`]().
///
/// # Example
/// ```snippet
/// dynamic_query
/// ```
UCLASS(BlueprintType)
class SYSTEMS_API UDynamicQuery : public UObject
{
	GENERATED_BODY()

public:
	UDynamicQuery();

	/// Resets internal iterator to state for interating current state of
	/// systems world.
	void Setup(
		/// Pointer to systems world to iterate on.
		USystemsWorld* Systems,
		/// Class of object that contains fields where yielded actor and
		/// components are gonna be stored.
		const UClass* Type);

	/// Performs iteration and stores yielded values in provided object.
	///
	/// # Note
	/// > This method uses reflection to figure out properties that gonna store
	/// yielded actor and components.
	UFUNCTION()
	bool Next(
		/// Pointer to object where yielded actor and components are gonna be
		/// stored in its properties.
		UObject* Output);

	/// Performs iteration and stores yielded values in provided object.
	///
	/// Handy wrapper for [`class: UDynamicQuery::Next`]() for use in
	/// blueprints, where iteration can branch to `yielded` and `completed`
	/// execution nodes.
	UFUNCTION(BlueprintCallable,
		Category = Systems,
		Meta = (DisplayName = "Next", ExpandEnumAsExecs = "Branches"))
	void NextBranched(UObject* Output, EDynamicQueryBranch& Branches);

private:
	void SignatureFromBundle(const USystemsWorld* Systems, const UClass* Type);

	bool NextNone();

	bool NextStart(FStartQueryState& Data, UObject* Output);

	bool NextCurrent(FCurrentQueryState& Data, UObject* Output);

	void FillQueryItem(UObject* Output,
		const FArchetype* Archetype,
		uint32 ActorIndex);

	UPROPERTY()
	const UClass* BundleType = nullptr;

	UPROPERTY()
	FArchetypeSignature Signature = {};

	UPROPERTY()
	TArray<FArchetypeSignature> CachedArchetypes = {};

	TVariant<FStartQueryState, FCurrentQueryState, FEndQueryState> State = {};
};

/// Systems world tagged query iterator.
///
/// This is basically an extension to [`struct: TQuery`]() that allows to ensure
/// additional components without the need for reading them - useful for tag
/// components (ones that just mark actors and do not store any data).
///
/// # Note
/// > User should rather construct queries with
/// [`class:USystemsWorld::TaggedQuery`]() instead o constructing queries by
/// hand.
///
/// # Example
/// ```inject
///	systems_world_tagged_query
/// ```
template <class... T>
struct TTaggedQuery
{
public:
	TTaggedQuery() : Systems(), Tags()
	{
	}

	/// Constructs tagged query from systems.
	///
	/// An equivalent of calling [`class: USystemsWorld::TaggedQuery`]()
	TTaggedQuery(
		/// Pointer to systems world of which actor components user wants to
		/// iterate on.
		USystemsWorld* InSystems)
		: Systems(InSystems), WithTypes()
	{
	}

	/// Request given component type to exists in queried actor without accesing
	/// its contents.
	///
	/// # Note
	/// > Make sure `Type` inherits from `UActorComponent`.
	TTaggedQuery& WithRaw(const UClass* Type)
	{
		//// [ignore]
		this->WithTypes.Add(Type);
		return *this;
		//// [/ignore]
	}

	/// Handy wrapper for [`struct: TTaggedQuery::WithRaw`]().
	///
	/// # Note
	/// > Make sure `W` inherits from `UActorComponent`.
	template <class W>
	TTaggedQuery& With()
	{
		//// [ignore]
		return WithRaw(W::StaticClass());
		//// [/ignore]
	}

	/// Request given component type to not exists in queried actors.
	///
	/// # Note
	/// > Make sure `Type` inherits from `UActorComponent`.
	TTaggedQuery& WithoutRaw(const UClass* Type)
	{
		//// [ignore]
		this->WithoutTypes.Add(Type);
		return *this;
		//// [/ignore]
	}

	/// Handy wrapper for [`struct: TTaggedQuery::WithoutRaw`]().
	///
	/// # Note
	/// > Make sure `W` inherits from `UActorComponent`.
	template <class W>
	TTaggedQuery& Without()
	{
		//// [ignore]
		return WithoutRaw(W::StaticClass());
		//// [/ignore]
	}

	/// Constructs [`struct: TQuery`]() from requested actor components and tag
	/// components.
	TQuery<T...> Iter() const
	{
		//// [ignore]
		auto Result = TQuery<T...>();
		Result.State.Set<FEndQueryState>({});
		if (IsValid(this->Systems))
		{
			TArray<const UClass*> Types = {T::StaticClass()...};
			for (const auto* Type : Types)
			{
				Result.IncludeType(Systems, Type);
			}
			for (const auto* Type : this->WithTypes)
			{
				Result.IncludeType(Systems, Type);
			}
			for (const auto* Type : this->WithoutTypes)
			{
				Result.ExcludeType(Systems, Type);
			}
			Result.CachedArchetypes = Systems->FindQueryArchetypes(
				Result.IncludeSignature, Result.ExcludeSignature);
			if (Result.CachedArchetypes.Num() > 0)
			{
				Result.State.Set<FStartQueryState>({this->Systems});
			}
		}
		return Result;
		//// [/ignore]
	}

private:
	USystemsWorld* Systems = nullptr;

	TSet<const UClass*> WithTypes = {};

	TSet<const UClass*> WithoutTypes = {};
};

/// Systems world query iterator.
///
/// Allows to iterate over actors and their components that comply to requested
/// types signature.
///
/// More about iterators in [this architecture book
/// page](/book/architecture/iterators.md).
///
/// # Note
/// > User should rather construct queries with [`class:USystemsWorld::Query`]()
/// instead o constructing queries by hand.
///
/// > Returned query iterator has always put actor as first item of item tuple
/// and then follow requested components. So `Systems->Query<A, B, C>()`
/// iterator will yield given tuple `TTuple<AActor*, A*, B*, C*>`
///
/// # Example
/// ```snippet
/// systems_world_query
/// ```
template <class... T>
struct TQuery
{
	template <class... T>
	friend struct TTaggedQuery;

public:
	using Self = TQuery<T...>;
	using Item = TTuple<AActor*, T*...>;

	TQuery()
		: IncludeSignature(), ExcludeSignature(), CachedArchetypes(), State()
	{
		//// [ignore]
		this->State.Set<FEndQueryState>({});
		//// [/ignore]
	}

	/// Constructs query from systems.
	///
	/// An equivalent of calling [`class: USystemsWorld::Query`]()
	TQuery(
		/// Pointer to systems world of which actor components user wants to
		/// iterate on.
		USystemsWorld* Systems)
		: IncludeSignature(), ExcludeSignature(), CachedArchetypes(), State()
	{
		//// [ignore]
		this->State.Set<FEndQueryState>({});
		if (IsValid(Systems))
		{
			const UClass* Types[] = {T::StaticClass()...};
			for (const auto* Type : Types)
			{
				IncludeType(Systems, Type);
			}
			this->CachedArchetypes = Systems->FindQueryArchetypes(
				this->IncludeSignature, FArchetypeSignature());
			if (this->CachedArchetypes.Num() > 0)
			{
				this->State.Set<FStartQueryState>({Systems});
			}
		}
		//// [/ignore]
	}

	/// Yields tuple to next actors component set.
	///
	/// Usually user would rather want to use iterator methods for ergonomic
	/// iteration over the query, but in case user has valid reasons to handle
	/// iteration different way, this is the single point that performs
	/// iteration and yields an item.
	///
	/// # Example
	/// ```snippet
	/// query_next
	/// ```
	TOptional<Item> Next()
	{
		//// [ignore]
		if (this->State.IsType<FStartQueryState>())
		{
			return NextStart(this->State.Get<FStartQueryState>());
		}
		else if (this->State.IsType<FCurrentQueryState>())
		{
			return NextCurrent(this->State.Get<FCurrentQueryState>());
		}
		return NextNone();
		//// [/ignore]
	}

	/// Gets hint about minimum and optional maximum items count this iterator
	/// can yield.
	///
	/// Used internally by lazy-iterators, but in case user would like to
	/// implement their own iterators, or iterator consumers, or just wants to
	/// preallocate some memory for later consumed iterator items, this method
	/// is really useful for these usecases.
	///
	/// See [`struct: IterSizeHint`]().
	///
	/// # Example
	/// ```snippet
	/// query_size_hint
	/// ```
	IterSizeHint SizeHint() const
	{
		//// [ignore]
		if (this->State.IsType<FStartQueryState>())
		{
			const auto& Data = this->State.Get<FStartQueryState>();
			if (IsValid(Data.Owner))
			{
				const auto Count = Data.Owner->CountRaw(
					this->IncludeSignature, this->ExcludeSignature);
				return IterSizeHint{0, TOptional<uint32>(Count)};
			}
		}
		else if (this->State.IsType<FCurrentQueryState>())
		{
			const auto& Data = this->State.Get<FCurrentQueryState>();
			if (IsValid(Data.Owner))
			{
				const auto Count = Data.Owner->CountRaw(
					this->IncludeSignature, this->ExcludeSignature);
				return IterSizeHint{0, TOptional<uint32>(Count)};
			}
		}
		return IterSizeHint{0, TOptional<uint32>()};
		//// [/ignore]
	}

private:
	void IncludeType(const USystemsWorld* Owner, const UClass* Type)
	{
		//// [ignore]
		const auto Found = Owner->ComponentTypeIndex(Type);
		checkf(Found.IsSet(),
			TEXT("Trying to query non-registered component: %s"),
			*Type->GetName());
		this->IncludeSignature.EnableBit(Found.GetValue());
		//// [/ignore]
	}

	void ExcludeType(const USystemsWorld* Owner, const UClass* Type)
	{
		//// [ignore]
		const auto Found = Owner->ComponentTypeIndex(Type);
		checkf(Found.IsSet(),
			TEXT("Trying to query non-registered component: %s"),
			*Type->GetName());
		this->ExcludeSignature.EnableBit(Found.GetValue());
		//// [/ignore]
	}

	TOptional<Item> NextNone()
	{
		//// [ignore]
		this->State.Set<FEndQueryState>({});
		return TOptional<Item>();
		//// [/ignore]
	}

	TOptional<Item> NextStart(FStartQueryState& Data)
	{
		//// [ignore]
		if (IsValid(Data.Owner) == false || this->CachedArchetypes.Num() <= 0)
		{
			return NextNone();
		}
		auto* Archetype =
			Data.Owner->Archetypes.Find(this->CachedArchetypes[0]);
		if (Archetype == nullptr || Archetype->Actors.Num() <= 0)
		{
			return NextNone();
		}
		this->State.Set<FCurrentQueryState>({Data.Owner, 0, 1});
		return MakeTuple(
			Archetype->Actors[0], Archetype->IndexedComponent<T>(0)...);
		//// [/ignore]
	}

	TOptional<Item> NextCurrent(FCurrentQueryState& Data)
	{
		//// [ignore]
		if (IsValid(Data.Owner) == false)
		{
			return NextNone();
		}
		while (Data.ArchetypeIndex <
			static_cast<uint32>(this->CachedArchetypes.Num()))
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
				return MakeTuple(Archetype->Actors[ActorIndex],
					Archetype->IndexedComponent<T>(ActorIndex)...);
			}
			Data.ActorIndex = 0;
			++Data.ArchetypeIndex;
		}
		return NextNone();
		//// [/ignore]
	}

	FArchetypeSignature IncludeSignature = {};

	FArchetypeSignature ExcludeSignature = {};

	TArray<FArchetypeSignature> CachedArchetypes = {};

	TVariant<FStartQueryState, FCurrentQueryState, FEndQueryState> State = {};

public:
	//// [inject: iter]
	ITER_IMPL
};

/// Query iterator over systems world actors.
///
/// See [`class: USystemsWorld::Actors`]().
USTRUCT(BlueprintType)
struct SYSTEMS_API FActorsIter
{
	GENERATED_BODY()

public:
	using Self = FActorsIter;
	using Item = AActor*;

	FActorsIter();

	/// Constructs query from systems.
	///
	/// An equivalent of calling [`class: USystemsWorld::Actors`]()
	FActorsIter(
		/// Pointer to systems world of which actors user wants to iterate on.
		USystemsWorld* Systems);

	/// Yields pointers to next actor.
	///
	/// See [`struct: TQuery::Next`]().
	TOptional<Item> Next();

	/// Gets hint about minimum and optional maximum items count this iterator
	/// can yield.
	///
	/// See [`struct: TQuery::SizeHint`]().
	IterSizeHint SizeHint() const;

private:
	TOptional<Item> NextNone();

	TOptional<Item> NextStart(FStartQueryState& Data);

	TOptional<Item> NextCurrent(FCurrentQueryState& Data);

	TArray<FArchetypeSignature> CachedArchetypes = {};
	TVariant<FStartQueryState, FCurrentQueryState, FEndQueryState> State = {};

public:
	//// [inject: iter_converters]
	ITER_IMPL_CONVERTERS
	//// [inject: iter_consumers_base]
	ITER_IMPL_CONSUMERS_BASE
	//// [inject: iter_colectors]
	ITER_IMPL_COLLECTORS
};
