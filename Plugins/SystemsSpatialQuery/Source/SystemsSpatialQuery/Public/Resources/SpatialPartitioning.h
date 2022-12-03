#pragma once

#include "CoreMinimal.h"

#include "Systems/Public/ArchetypeSignature.h"
#include "Systems/Public/Iterator.h"

#include "SpatialPartitioning.generated.h"

class USystemsWorld;
class USpatialPartitioningSettings;
struct FSpatialNode;
struct FSpatialLeafs;

UENUM(BlueprintType)
enum class ESpatialPreferedPlane : uint8
{
	None = 0b000 UMETA(Hidden),
	X = 0b001 UMETA(Hidden),
	Y = 0b010 UMETA(Hidden),
	Z = 0b100 UMETA(Hidden),
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

	FArea(FVector Center, FVector::FReal Radius)
		: Lower(Center - FVector(Radius)), Upper(Center + FVector(Radius))
	{
	}

	FVector Center() const;

	FVector Extents() const;

	FVector Dimensions() const;

	bool Contains(FVector Position) const;

	bool Overlaps(const FArea& Other) const;

	bool Overlaps(FVector Center, FVector::FReal Radius) const;

	ESpatialPreferedPlane SubdividePreferedAxis(
		ESpatialPreferedPlane PreferedPlane) const;

	void Subdivide(TUniquePtr<FSpatialNode> (&Result)[2],
		FVector Center,
		uint32 Capacity,
		ESpatialPreferedPlane PreferedPlane) const;

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
	FSpatialNodeClosest(TObjectPtr<AActor> InActor, FVector::FReal InDistance)
		: Actor(InActor), Distance(InDistance)
	{
	}

	TObjectPtr<AActor> Actor = {};

	FVector::FReal Distance = INFINITY;
};

struct FSpatialNode
{
	FSpatialNode(FArea InArea,
		uint32 Capacity,
		ESpatialPreferedPlane InPreferedPlane)
		: Area(InArea), Signature({}), Count(0), PreferedPlane(InPreferedPlane)
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

	bool FindNodesPath(TArray<const FSpatialNode*>& Result,
		FVector Position) const;

	void ForEachArea(const TFunction<void(const FArea&, bool)> Callback) const;

	FArea Area = {};

	FArchetypeSignature Signature = {};

	FSpatialContent Content = {};

	uint32 Count = 0;

	ESpatialPreferedPlane PreferedPlane = ESpatialPreferedPlane::Any;
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

using FSpatialQueryLimits = TVariant<FSpatialQueryLimitsNone,
	FSpatialQueryLimitsArea,
	FSpatialQueryLimitsRadius>;

template <class... T>
struct TSpatialQuery
{
public:
	using Self = TSpatialQuery<T...>;
	using Item = TTuple<AActor*, FVector::FReal, T*...>;

	TSpatialQuery()
		: Systems(nullptr)
		, Partitioning(nullptr)
		, Limits()
		, Signature()
		, Visited()
		, Position(0.0)
	{
	}

	TSpatialQuery(USystemsWorld* InSystems,
		USpatialPartitioning* InPartitioning,
		FVector InPosition,
		FSpatialQueryLimits InLimits)
		: Systems(InSystems)
		, Partitioning(InPartitioning)
		, Limits(InLimits)
		, Signature()
		, Visited()
		, Position(InPosition)
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
				const auto Distance =
					FVector::Distance(this->Position, Location);
				if (this->Limits.IsType<FSpatialQueryLimitsArea>() &&
					this->Limits.Get<FSpatialQueryLimitsArea>().Area.Contains(
						Location) == false)
				{
					return false;
				}
				else if (this->Limits.IsType<FSpatialQueryLimitsRadius>() &&
					Distance >
						this->Limits.Get<FSpatialQueryLimitsRadius>().Radius)
				{
					return false;
				}
				const auto Signature = this->Systems->ActorSignature(Actor);
				return Signature.IsSet() &&
					Signature.GetValue().ContainsAll(this->Signature);
			});
		if (Found)
		{
			const auto Distance =
				FVector::Distance(Found->GetActorLocation(), this->Position);
			this->Visited.Add(Found);
			return MakeTuple(
				Found.Get(), Distance, this->Systems->Component<T>(Found)...);
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
		checkf(Found.IsSet(),
			TEXT("Trying to query non-registered component: %s"),
			*Type->GetName());
		this->Signature.EnableBit(Found.GetValue());
	}

	USystemsWorld* Systems = nullptr;

	USpatialPartitioning* Partitioning = nullptr;

	FSpatialQueryLimits Limits = {};

	FArchetypeSignature Signature = {};

	TSet<TObjectPtr<AActor>> Visited = {};

	FVector Position = FVector(0.0);

public:
	ITER_IMPL
};

UCLASS(BlueprintType)
class SYSTEMSSPATIALQUERY_API USpatialPartitioning : public UObject
{
	GENERATED_BODY()

public:
	void Reset(const FArea& Area,
		uint32 Capacity,
		ESpatialPreferedPlane PreferedPlane = ESpatialPreferedPlane::Any);

	uint32 GetCount() const;

	bool Add(const USystemsWorld& Systems, TObjectPtr<AActor> Actor);

	void GetActorsInArea(TSet<TObjectPtr<AActor>>& Result,
		const FArea& InArea,
		const TFunction<bool(AActor*)>& Validator = {}) const;

	void GetActorsInRadius(TSet<TObjectPtr<AActor>>& Result,
		FVector Center,
		FVector::FReal Radius,
		const TFunction<bool(AActor*)>& Validator = {}) const;

	TObjectPtr<AActor> FindClosestActor(FVector Position,
		const TFunction<bool(AActor*)>& Validator = {}) const;

	void ForEachArea(const TFunction<void(const FArea&, bool)> Callback) const;

	template <class... T>
	TSpatialQuery<T...> Query(USystemsWorld& Systems, FVector Position)
	{
		return TSpatialQuery<T...>(&Systems, this, Position, {});
	}

	template <class... T>
	TSpatialQuery<T...> QueryInArea(USystemsWorld& Systems,
		FVector Position,
		FArea Area)
	{
		FSpatialQueryLimits Limits = {};
		Limits.Set<FSpatialQueryLimitsArea>({Area});
		return TSpatialQuery<T...>(&Systems, this, Position, Limits);
	}

	template <class... T>
	TSpatialQuery<T...> QueryInRadius(USystemsWorld& Systems,
		FVector Position,
		FVector::FReal Radius)
	{
		FSpatialQueryLimits Limits = {};
		Limits.Set<FSpatialQueryLimitsRadius>({Radius});
		return TSpatialQuery<T...>(&Systems, this, Position, Limits);
	}

private:
	TUniquePtr<FSpatialNode> Root = {};
};

UCLASS(BlueprintType)
class SYSTEMSSPATIALQUERY_API USpatialPartitioningSettings : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere);
	uint32 CellActorsCapacity = 10;

	UPROPERTY(EditAnywhere);
	FArea CoverWorldArea = {};

	UPROPERTY(EditAnywhere);
	ESpatialPreferedPlane PreferedSubdivisionPlane = ESpatialPreferedPlane::Any;

	UPROPERTY(EditAnywhere);
	bool bRebuildOnlyOnChange = false;

	UPROPERTY(EditAnywhere, Category = Debug);
	bool bDrawOccupiedDebugAreas = true;

	UPROPERTY(EditAnywhere, Category = Debug);
	bool bDrawEmptyDebugAreas = true;

	UPROPERTY(EditAnywhere, Category = Debug);
	bool bDrawSpatialQueries = true;

	UPROPERTY(EditAnywhere, Category = Debug);
	FColor DebugOccupiedAreasColor = FColor::Yellow;

	UPROPERTY(EditAnywhere, Category = Debug);
	FColor DebugEmptyAreasColor = FColor::Magenta;

	UPROPERTY(EditAnywhere, Category = Debug);
	FColor DebugOrderedActorsColor = FColor::Red;

	UPROPERTY(EditAnywhere, Category = Debug);
	TSubclassOf<AActor> StressTestActor = {};

	UPROPERTY(EditAnywhere, Category = Debug);
	uint32 StressTestSpawnCount = 0;

	UPROPERTY(EditAnywhere, Category = Debug);
	FVector StressTestSpawnOffset = FVector(0.0);

	UPROPERTY(EditAnywhere, Category = Debug);
	bool bStressTestSpawnOnGround = true;
};
