#pragma once

#include "CoreMinimal.h"

#include "Systems/Public/ArchetypeSignature.h"
#include "Systems/Public/Iterator.h"
#include "Systems/Public/SystemsWorld.h"

#include "SpatialPartitioning.generated.h"

class USpatialPartitioningSettings;
struct FSpatialNode;
struct FSpatialLeafs;

UENUM(BlueprintType)
enum class ESpatialPreferredPlane : uint8
{
	None = 0b000 UMETA(Hidden),
	X = 0b001,
	Y = 0b010,
	Z = 0b100,
	XY = 0b011,
	YZ = 0b110,
	ZX = 0b101,
	Any = 0b111,
};

USTRUCT(BlueprintType)
struct SYSTEMSSPATIALQUERY_API FArea
{
	GENERATED_BODY()

	FArea() : Lower(FVector(0)), Upper(FVector(0))
	{
	}

	FArea(FVector InLower, FVector InUpper) : Lower(InLower), Upper(InUpper)
	{
	}

	FArea(FVector Center, FVector::FReal Radius) : Lower(Center - FVector(Radius)), Upper(Center + FVector(Radius))
	{
	}

	FVector Center() const;

	FVector Extents() const;

	FVector Dimensions() const;

	bool Contains(FVector Position) const;

	bool Overlaps(const FArea& Other) const;

	bool Overlaps(FVector Center, FVector::FReal Radius) const;

	ESpatialPreferredPlane SubdividePreferredAxis(ESpatialPreferredPlane PreferredPlane) const;

	bool Subdivide(TUniquePtr<FSpatialNode> (&Result)[2],
		FVector Center,
		uint32 Capacity,
		ESpatialPreferredPlane PreferredPlane) const;

	UPROPERTY(EditAnywhere)
	FVector Lower = FVector(0);

	UPROPERTY(EditAnywhere)
	FVector Upper = FVector(0);
};

struct FSpatialActors
{
	FSpatialActors() : Actors({})
	{
	}

	FSpatialActors(uint32 Capacity) : Actors({})
	{
		this->Actors.Reserve(Capacity);
	}

	TArray<TObjectPtr<AActor>> Actors = {};
};

struct FSpatialLeafs
{
	FSpatialLeafs(TUniquePtr<FSpatialNode> (&&InLeafs)[2]);

	TUniquePtr<FSpatialNode> Leafs[2] = {};
};

using FSpatialContent = TVariant<FSpatialActors, FSpatialLeafs>;

struct FSpatialNodeClosest
{
	FSpatialNodeClosest(TObjectPtr<AActor> InActor, FVector::FReal InDistance) : Actor(InActor), Distance(InDistance)
	{
	}

	TObjectPtr<AActor> Actor = {};

	FVector::FReal Distance = INFINITY;
};

struct FSpatialNode
{
	FSpatialNode(FArea InArea, uint32 Capacity, ESpatialPreferredPlane InPreferredPlane)
		: Area(InArea), Signature({}), Count(0), PreferredPlane(InPreferredPlane)
	{
		this->Content.Set<FSpatialActors>(Capacity);
	}

	uint32 GetCount() const;

	bool Add(TObjectPtr<AActor> Actor, FArchetypeSignature InSignature);

	void GetActorsInArea(TSet<TObjectPtr<AActor>>& Result,
		const FArea& InArea,
		const TFunction<bool(AActor*)>& Validator = {}) const;

	void GetActorsInRadius(TSet<TObjectPtr<AActor>>& Result,
		FVector Center,
		FVector::FReal Radius,
		const TFunction<bool(AActor*)>& Validator = {}) const;

	void FindClosestActor(FVector Position,
		TOptional<FSpatialNodeClosest>& State,
		const TFunction<bool(AActor*)>& Validator = {}) const;

	const FSpatialNode* FindNode(FVector Position) const;

	bool FindNodesPath(TArray<const FSpatialNode*>& Result, FVector Position) const;

	void ForEachArea(const TFunction<void(const FArea&, bool)> Callback) const;

	FArea Area = {};

	FArchetypeSignature Signature = {};

	FSpatialContent Content = {};

	uint32 Count = 0;

	ESpatialPreferredPlane PreferredPlane = ESpatialPreferredPlane::Any;
};

struct FSpatialQueryLimitsNone
{
};

struct FSpatialQueryLimitsArea
{
	FArea Area = {};
};

struct FSpatialQueryLimitsRadius
{
	FVector::FReal Radius = 0.0;
};

using FSpatialQueryLimits = TVariant<FSpatialQueryLimitsNone, FSpatialQueryLimitsArea, FSpatialQueryLimitsRadius>;

template <class... T>
struct TSpatialQuery
{
	template <class... T>
	friend struct TTaggedSpatialQuery;

public:
	using Self = TSpatialQuery<T...>;
	using Item = TTuple<AActor*, FVector::FReal, T*...>;

	TSpatialQuery()
		: Systems(nullptr)
		, Partitioning(nullptr)
		, Position(0.0)
		, Limits()
		, IncludeSignature()
		, ExcludeSignature()
		, Visited()
	{
	}

	TSpatialQuery(USystemsWorld* InSystems,
		const USpatialPartitioning* InPartitioning,
		FVector InPosition,
		FSpatialQueryLimits InLimits)
		: Systems(InSystems)
		, Partitioning(InPartitioning)
		, Position(InPosition)
		, Limits(InLimits)
		, IncludeSignature()
		, ExcludeSignature()
		, Visited()
	{
		if (IsValid(Systems) && IsValid(Partitioning))
		{
			const UClass* Types[] = {T::StaticClass()...};
			for (const auto* Type : Types)
			{
				IncludeType(Systems, Type);
			}
		}
	}

	TOptional<Item> Next()
	{
		if (IsValid(Systems) == false || IsValid(Partitioning) == false)
		{
			return {};
		}
		auto Found = Partitioning->FindClosestActor(this->Position,
			[&](auto* Actor)
			{
				if (this->Visited.Contains(Actor))
				{
					return false;
				}
				const auto Location = Actor->GetActorLocation();
				const auto Distance = FVector::Distance(this->Position, Location);
				if (this->Limits.IsType<FSpatialQueryLimitsArea>() &&
					this->Limits.Get<FSpatialQueryLimitsArea>().Area.Contains(Location) == false)
				{
					return false;
				}
				else if (this->Limits.IsType<FSpatialQueryLimitsRadius>() &&
					Distance > this->Limits.Get<FSpatialQueryLimitsRadius>().Radius)
				{
					return false;
				}
				const auto Signature = this->Systems->ActorSignature(Actor);
				return Signature.IsSet() && Signature.GetValue().ContainsAll(this->IncludeSignature) &&
					Signature.GetValue().ContainsAny(this->ExcludeSignature) == false;
			});
		if (Found)
		{
			const auto Distance = FVector::Distance(Found->GetActorLocation(), this->Position);
			this->Visited.Add(Found);
			return MakeTuple(Found.Get(), Distance, this->Systems->Component<T>(Found)...);
		}
		return {};
	}

	IterSizeHint SizeHint() const
	{
		return IterSizeHint{0, TOptional<uint32>()};
	}

private:
	void IncludeType(const USystemsWorld* Owner, const UClass* Type)
	{
		const auto Found = Owner->ComponentTypeIndex(Type);
		checkf(Found.IsSet(), TEXT("Trying to query non-registered component: %s"), *Type->GetName());
		this->IncludeSignature.EnableBit(Found.GetValue());
	}

	void ExcludeType(const USystemsWorld* Owner, const UClass* Type)
	{
		const auto Found = Owner->ComponentTypeIndex(Type);
		checkf(Found.IsSet(), TEXT("Trying to query non-registered component: %s"), *Type->GetName());
		this->ExcludeSignature.EnableBit(Found.GetValue());
	}

	USystemsWorld* Systems = nullptr;

	const USpatialPartitioning* Partitioning = nullptr;

	FVector Position = FVector(0.0);

	FSpatialQueryLimits Limits = {};

	FArchetypeSignature IncludeSignature = {};

	FArchetypeSignature ExcludeSignature = {};

	TSet<TObjectPtr<AActor>> Visited = {};

public:
	ITER_IMPL
};

template <class... T>
struct TTaggedSpatialQuery
{
public:
	TTaggedSpatialQuery()
		: Systems(nullptr), Partitioning(nullptr), Position(0.0), Limits(), WithTypes(), WithoutTypes()
	{
	}

	TTaggedSpatialQuery(USystemsWorld* InSystems,
		const USpatialPartitioning* InPartitioning,
		FVector InPosition,
		FSpatialQueryLimits InLimits)
		: Systems(InSystems)
		, Partitioning(InPartitioning)
		, Position(InPosition)
		, Limits(InLimits)
		, WithTypes()
		, WithoutTypes()
	{
	}

	TTaggedSpatialQuery& WithRaw(const UClass* Type)
	{
		this->WithTypes.Add(Type);
		return *this;
	}

	template <class W>
	TTaggedSpatialQuery& With()
	{
		return WithRaw(W::StaticClass());
	}

	TTaggedSpatialQuery& WithoutRaw(const UClass* Type)
	{
		this->WithoutTypes.Add(Type);
		return *this;
	}

	template <class W>
	TTaggedSpatialQuery& Without()
	{
		return WithoutRaw(W::StaticClass());
	}

	TSpatialQuery<T...> Iter() const
	{
		auto Result = TSpatialQuery<T...>(this->Systems, this->Partitioning, this->Position, this->Limits);
		if (IsValid(this->Systems))
		{
			for (const auto* Type : this->WithTypes)
			{
				Result.IncludeType(Systems, Type);
			}
			for (const auto* Type : this->WithoutTypes)
			{
				Result.ExcludeType(Systems, Type);
			}
		}
		return Result;
	}

private:
	USystemsWorld* Systems = nullptr;

	const USpatialPartitioning* Partitioning = nullptr;

	FVector Position = FVector(0.0);

	FSpatialQueryLimits Limits = {};

	TSet<const UClass*> WithTypes = {};

	TSet<const UClass*> WithoutTypes = {};
};

UCLASS(BlueprintType)
class SYSTEMSSPATIALQUERY_API USpatialPartitioning : public UObject
{
	GENERATED_BODY()

public:
	void Reset(const FArea& Area, uint32 Capacity, ESpatialPreferredPlane PreferredPlane = ESpatialPreferredPlane::Any);

	uint32 GetCount() const;

	bool Add(const USystemsWorld& Systems, TObjectPtr<AActor> Actor);

	void GetActorsInArea(TSet<TObjectPtr<AActor>>& Result,
		const FArea& InArea,
		const TFunction<bool(AActor*)>& Validator = {}) const;

	void GetActorsInRadius(TSet<TObjectPtr<AActor>>& Result,
		FVector Center,
		FVector::FReal Radius,
		const TFunction<bool(AActor*)>& Validator = {}) const;

	TObjectPtr<AActor> FindClosestActor(FVector Position, const TFunction<bool(AActor*)>& Validator = {}) const;

	bool FindActorsTriangleContaining(TSet<TObjectPtr<AActor>>& Result,
		FVector Position,
		USystemsWorld& Systems,
		TFunction<bool(AActor*)> Validator = {}) const;

	void ForEachArea(const TFunction<void(const FArea&, bool)> Callback) const;

	template <class... T>
	TSpatialQuery<T...> Query(USystemsWorld& Systems, FVector Position) const
	{
		return TSpatialQuery<T...>(&Systems, this, Position, {});
	}

	template <class... T>
	TSpatialQuery<T...> QueryInArea(USystemsWorld& Systems, FVector Position, FArea Area) const
	{
		FSpatialQueryLimits Limits = {};
		Limits.Set<FSpatialQueryLimitsArea>({Area});
		return TSpatialQuery<T...>(&Systems, this, Position, Limits);
	}

	template <class... T>
	TSpatialQuery<T...> QueryInRadius(USystemsWorld& Systems, FVector Position, FVector::FReal Radius) const
	{
		FSpatialQueryLimits Limits = {};
		Limits.Set<FSpatialQueryLimitsRadius>({Radius});
		return TSpatialQuery<T...>(&Systems, this, Position, Limits);
	}

	template <class... T>
	TTaggedSpatialQuery<T...> TaggedQuery(USystemsWorld& Systems, FVector Position) const
	{
		return TTaggedSpatialQuery<T...>(&Systems, this, Position, {});
	}

	template <class... T>
	TTaggedSpatialQuery<T...> TaggedQueryInArea(USystemsWorld& Systems, FVector Position, FArea Area) const
	{
		FSpatialQueryLimits Limits = {};
		Limits.Set<FSpatialQueryLimitsArea>({Area});
		return TTaggedSpatialQuery<T...>(&Systems, this, Position, Limits);
	}

	template <class... T>
	TTaggedSpatialQuery<T...> TaggedQueryInRadius(USystemsWorld& Systems, FVector Position, FVector::FReal Radius) const
	{
		FSpatialQueryLimits Limits = {};
		Limits.Set<FSpatialQueryLimitsRadius>({Radius});
		return TTaggedSpatialQuery<T...>(&Systems, this, Position, Limits);
	}

private:
	TUniquePtr<FSpatialNode> Root = {};
};

UCLASS(BlueprintType)
class SYSTEMSSPATIALQUERY_API USpatialPartitioningSettings : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	uint32 CellActorsCapacity = 10;

	UPROPERTY(EditAnywhere)
	FArea CoverWorldArea = {};

	UPROPERTY(EditAnywhere);
	ESpatialPreferredPlane PreferredSubdivisionPlane = ESpatialPreferredPlane::Any;

	UPROPERTY(EditAnywhere)
	bool bRebuildOnlyOnChange = false;

	UPROPERTY(EditAnywhere, Category = Debug)
	bool bDrawOccupiedDebugAreas = true;

	UPROPERTY(EditAnywhere, Category = Debug)
	bool bDrawEmptyDebugAreas = true;

	UPROPERTY(EditAnywhere, Category = Debug)
	bool bDrawSpatialQueries = true;

	UPROPERTY(EditAnywhere, Category = Debug)
	FColor DebugOccupiedAreasColor = FColor::Yellow;

	UPROPERTY(EditAnywhere, Category = Debug)
	FColor DebugEmptyAreasColor = FColor::Magenta;

	UPROPERTY(EditAnywhere, Category = Debug)
	FColor DebugOrderedActorsColor = FColor::Red;

	UPROPERTY(EditAnywhere, Category = Debug)
	TSubclassOf<AActor> StressTestActor = {};

	UPROPERTY(EditAnywhere, Category = Debug)
	uint32 StressTestSpawnCount = 0;

	UPROPERTY(EditAnywhere, Category = Debug)
	FVector StressTestSpawnOffset = FVector(0.0);

	UPROPERTY(EditAnywhere, Category = Debug)
	bool bStressTestSpawnOnGround = true;
};