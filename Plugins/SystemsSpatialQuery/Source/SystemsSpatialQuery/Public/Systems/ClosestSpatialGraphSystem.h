#pragma once

#include "CoreMinimal.h"

#include "Systems/Public/System.h"

#include "ClosestSpatialGraphSystem.generated.h"

class USystemsWorld;
class USpatialGraph;

UCLASS(BlueprintType, Blueprintable, EditInlineNew)
class SYSTEMSSPATIALQUERY_API USpatialGraphConnectionValidator : public UObject
{
	GENERATED_BODY()

public:
	virtual bool Validate(const TObjectPtr<AActor>& From, const TObjectPtr<AActor>& To) const
	{
		return true;
	}
};

UCLASS(BlueprintType)
class SYSTEMSSPATIALQUERY_API UClusteringConnectionValidator : public USpatialGraphConnectionValidator
{
	GENERATED_BODY()

public:
	virtual bool Validate(const TObjectPtr<AActor>& From, const TObjectPtr<AActor>& To) const override;

	UPROPERTY(EditAnywhere)
	float MaxDistance = 1000;
};

UCLASS(BlueprintType)
class SYSTEMSSPATIALQUERY_API UOcclusionConnectionValidator : public USpatialGraphConnectionValidator
{
	GENERATED_BODY()

public:
	virtual bool Validate(const TObjectPtr<AActor>& From, const TObjectPtr<AActor>& To) const override;

	UPROPERTY(EditAnywhere)
	bool bClearSight = true;

	UPROPERTY(EditAnywhere)
	TEnumAsByte<ECollisionChannel> CollisionChannel = ECollisionChannel::ECC_WorldStatic;
};

/// Fills given graph with connections only between two closests spatial nodes.
UCLASS(BlueprintType)
class SYSTEMSSPATIALQUERY_API UClosestSpatialGraphSystem : public USystem
{
	GENERATED_BODY()

public:
	virtual void Run(USystemsWorld& Systems) override;

private:
	/// Rebuilds given graph only if that graph resource has been marked as changed.
	UPROPERTY(EditAnywhere);
	bool bRebuildOnlyOnChange = false;

	/// Maximal number of connections node can have.
	///
	/// 0 means no limit. This property is useful for optimizations purposes only,
	/// it will ensure order of nodes selected by proximity.
	UPROPERTY(EditAnywhere);
	uint32 ConnectionsLimitPerNode = 0;

	/// Additional radius tolerance for circumsphere constructed by two nodes.
	///
	/// General rule is that two nodes can be connected if there is no other node in
	/// circumsphere produced by these two nodes. This tolerance makes circumsphere
	/// radius lower so more error is accepted.
	///
	/// It is useful to in cases when we work with crystaline lattice graphs structures
	/// (rectangular or hexagonal grids for example - uniform structure is what matters).
	UPROPERTY(EditAnywhere);
	float CollisionCircumsphereTolerance = 0;

	UPROPERTY(EditAnywhere, Export, Instanced);
	TSet<TObjectPtr<USpatialGraphConnectionValidator>> ConnectionValidators = {};

	/// Type of spatial graph resource to fill.
	///
	/// Since spatial graphs are resources, in case user wants to have more than one graph at
	/// the same time, it's advised to create classes for each one and make them inherit
	/// [`class: USpatialGraph`]().
	UPROPERTY(EditAnywhere)
	TSubclassOf<USpatialGraph> ResourceType = {};

	/// Set of additional components that has to be present in actors to make these actors
	/// recognized by this system.
	///
	/// Useful when wanting to have multiple types of graphs, using components to tag selected
	/// actors to be assigned to given graph.
	UPROPERTY(EditAnywhere)
	TSet<TSubclassOf<UActorComponent>> ExtraComponentTypes = {};
};
