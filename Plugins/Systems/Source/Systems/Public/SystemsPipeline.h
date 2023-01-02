#pragma once

#include "CoreMinimal.h"

#include "Systems/Public/SystemsWorld.h"

#include "SystemsPipeline.generated.h"

/// Pipeline component descriptor.
USTRUCT(BlueprintType)
struct SYSTEMS_API FSystemsPipelineComponent
{
	GENERATED_BODY()

	/// Tells if this component should be used in development builds only.
	///
	/// Useful for debug or prototype game features.
	UPROPERTY(EditAnywhere, Category = "Options")
	bool bDevelopmentBuildOnly = false;

	/// Tells if this component should be used.
	///
	/// Useful for quick enable/disable of component without removing it from
	/// the pipeline (quick tests).
	UPROPERTY(EditAnywhere, Category = "Options")
	bool bUse = true;
};

/// Pipeline resource descriptor.
USTRUCT(BlueprintType)
struct SYSTEMS_API FSystemsPipelineResource
{
	GENERATED_BODY()

	/// Tells if this resource should be used in development builds only.
	///
	/// Useful for debug or prototype game features.
	UPROPERTY(EditAnywhere, Category = "Options")
	bool bDevelopmentBuildOnly = false;

	/// Tells if this resource should be used.
	///
	/// Useful for quick enable/disable of resource without removing it from the
	/// pipeline (quick tests).
	UPROPERTY(EditAnywhere, Category = "Options")
	bool bUse = true;

	/// Tells if this resource should be stored in global storage when pipeline
	/// gets uninstalled and restored from global storage when pipeline gets installed.
	///
	/// Useful to transfer resources between persistent maps, like player username
	/// typed in main menu and used in game world, or after mission ends, score gets
	/// transfered to main menu for display.
	///
	/// # Note
	/// > It doesn't affect asset resources since they have "global" lifetime anyway,
	/// no need to pass them between systems world other way than just using same
	/// reference in shared systems worlds.
	UPROPERTY(EditAnywhere, Category = "Options")
	bool bUseGlobalStorage = false;
};

/// Pipeline system descriptor.
USTRUCT(BlueprintType)
struct SYSTEMS_API FSystemsPipelineSystem
{
	GENERATED_BODY()

	/// Template of system to instantiate.
	UPROPERTY(EditAnywhere, Export, Instanced, Category = "System")
	TObjectPtr<USystem> Template = {};

	/// Tells if this system should be used in development builds only.
	///
	/// Useful for debug or prototype game features.
	UPROPERTY(EditAnywhere, Category = "Options")
	bool bDevelopmentBuildOnly = false;

	/// Tells if this system should be used.
	///
	/// Useful for quick enable/disable of this system without removing it from
	/// the pipeline (quick tests).
	UPROPERTY(EditAnywhere, Category = "Options")
	bool bUse = true;
};

/// Data asset used to describe Systems pipeline.
UCLASS(BlueprintType)
class SYSTEMS_API USystemsPipeline : public UDataAsset
{
	GENERATED_BODY()

public:
	/// Creates new global systems world under [`class: Self::WorldId`]() name and
	/// installs this pipeline content into that systems world.
	void Install(
		/// World that has running [`class: USystemsSubsystem`]().
		UWorld* World) const;

	/// Destroys systems world registered under [`class: Self::WorldId`]() name.
	void Uninstall(
		/// World that has running [`class: USystemsSubsystem`]().
		UWorld* World) const;

	/// Typed components to be registered into systems world.
	UPROPERTY(EditAnywhere, Category = "Components|Types")
	TMap<TSubclassOf<UActorComponent>, FSystemsPipelineComponent> ComponentsToRegister = {};

	/// Typed resources to be registered into systems world.
	///
	/// Usually these are used as game managers.
	UPROPERTY(EditAnywhere, Category = "Resources|Types")
	TMap<TSubclassOf<UObject>, FSystemsPipelineResource> TypeResourcesToInstall = {};

	/// Asset resources to be registered into systems world.
	///
	/// Usually these are used as settings/config data sources.
	UPROPERTY(EditAnywhere, Category = "Resources|Assets")
	TMap<TObjectPtr<UDataAsset>, FSystemsPipelineResource> AssetResourcesToInstall = {};

	/// System templates to instantiate and run on systems world creation.
	///
	/// Order in the list represents order of execution.
	UPROPERTY(EditAnywhere, Category = "Systems|Startup")
	TArray<FSystemsPipelineSystem> StartupSystemsToRun = {};

	/// System templates to instantiate and register into systems world to run
	/// every tick.
	///
	/// Order in the list represents order of execution.
	UPROPERTY(EditAnywhere, Category = "Systems|Persistent")
	TArray<FSystemsPipelineSystem> PersistentSystemsToInstall = {};

	/// System templates to instantiate and run on systems world destruction.
	///
	/// Order in the list represents order of execution.
	UPROPERTY(EditAnywhere, Category = "Systems|Cleanup")
	TArray<FSystemsPipelineSystem> CleanupSystemsToRun = {};

private:
	/// Name of the global systems world creeated for this pipeline.
	UPROPERTY(EditAnywhere, Category = "World")
	FName WorldId = FName();
};
