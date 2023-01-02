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
	UPROPERTY(EditAnywhere);
	bool bRebuildOnlyOnChange = false;

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
