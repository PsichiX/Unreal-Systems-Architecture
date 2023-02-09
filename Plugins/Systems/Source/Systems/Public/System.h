#pragma once

#include "CoreMinimal.h"

#include "Systems/Public/ArchetypeSignature.h"

#include "System.generated.h"

class USystemsWorld;

/// Base class for unit of work over some data.
///
/// [Architecture book page](/book/architecture/systems.md) explains more in
/// depth systems.
///
/// System's job is to process data. Usually it involves performing queries
/// over game world and/or resources.
///
/// This is only one of few ways of system representation, used mostly in case
/// your system has to cache some internal state or has to perform some logic
/// on system initialization phase, which is not possible in case of for example
/// lambda systems.
///
/// # Example
/// ```snippet
/// system
/// ```
UCLASS(Abstract, EditInlineNew)
class SYSTEMS_API USystem : public UObject
{
	GENERATED_BODY()

public:
	/// Override to initialize system (setup its internal state).
	///
	/// This method is called by [`class: USystemsWorld::SealAndInitialize`](),
	/// right after systems world is setup and sealed by the user.
	///
	/// Here user can perform initialization of this system internal state, or
	/// do something useful with systems world (for example initialize some
	/// resources). At this stage systems world does not yet have registered any
	/// of world components so any query will yield no results.
	///
	/// # Example
	/// ```snippet
	/// system_init
	/// ```
	virtual void Init(
		/// Reference to systems world that triggers this system initialization.
		USystemsWorld& Systems)
	{
	}

	/// Override to cleanup system (its internal state).
	///
	/// This method is called by [`class: USystemsWorld::Cleanup`](),
	/// right before systems world is scheduled to removal.
	///
	/// Here user can perform cleanup of this system internal state, or
	/// free acquired resources.
	virtual void Cleanup(
		/// Reference to systems world that triggers this system cleanup.
		USystemsWorld& Systems)
	{
	}

	/// Override to run system work logic (perform queries on world components
	/// and resources).
	///
	/// This method is called by [`class: USystemsWorld::Process`]().
	///
	/// Here user can perform system logic, usually performs single task such as
	/// apply velocity accumulated by other systems on actor location, or
	/// perform actors AI tasks, or kill actors that has 0 or less health -
	/// whatever work you want to do make sure it's not monolithic, rather make
	/// it small, like don't do multiple things at once - always divide work to
	/// small data processing work units. Split bigger tasks between multiple
	/// systems and use components/resources/channels to share data between
	/// systems.
	///
	/// # Example
	/// ```snippet
	/// system_run
	/// ```
	virtual void Run(
		/// Reference to systems world that triggers this system run.
		USystemsWorld& Systems)
	{
	}
};

/// Variant of system that works with blueprints.
///
/// User can override OnInit method for system initialization, as well as
/// OnRun for performing system logic.
///
/// See [`class: USystem`]()
UCLASS(BlueprintType, Blueprintable)
class SYSTEMS_API UScriptableSystem : public USystem
{
	GENERATED_BODY()

public:
	/// Override to initialize system (setup its internal state).
	///
	/// See [`class: USystem::Init`]()
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = Systems, Meta = (DevelopmentOnly))
	void OnInit(USystemsWorld* Systems);

	/// Override to run system work logic (perform queries on world components
	/// and resources).
	///
	/// See [`class: USystem::Run`]()
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = Systems, Meta = (DevelopmentOnly))
	void OnRun(USystemsWorld* Systems, float DeltaSeconds);

	UFUNCTION(BlueprintCallable, Category = Systems, Meta = (DeterminesOutputType = "Class", DevelopmentOnly))
	AActor* SpawnActorFromClass(TSubclassOf<AActor> Class, const FTransform& SpawnTransform, AActor* Owner = nullptr);

private:
	virtual void Init(USystemsWorld& Systems) override;

	virtual void Run(USystemsWorld& Systems) override;
};

/// Handy wrapper for state-less systems (functions or lambdas).
///
/// See [`class: USystem`]()
///
/// # Example
/// ```snippet
/// lambda_system
/// ```
///
/// ```snippet
/// install_lambda_system
/// ```
UCLASS(BlueprintType, Blueprintable)
class SYSTEMS_API ULambdaSystem : public USystem
{
	GENERATED_BODY()

public:
	using FunctorType = void(USystemsWorld&);

	/// Binds given function/lambda that runs some system logic.
	///
	/// Method called by [`class: USystemsWorld::InstallLambdaSystem`]()
	void Bind(TFunction<FunctorType>&& Func);

	/// Unbinds stored function/lambda that runs some system logic.
	void Unbind();

private:
	virtual void Init(USystemsWorld& Systems) override;

	virtual void Run(USystemsWorld& Systems) override;

	/// Name of function system registered into reflection.
	///
	/// When not None, this system will run system function found in reflection
	/// registry.
	UPROPERTY(EditAnywhere, Meta = (GetOptions = "Systems.SystemsReflection.GetOptions"))
	FName FunctionName = FName();

	TOptional<TFunction<FunctorType>> Functor = {};
};