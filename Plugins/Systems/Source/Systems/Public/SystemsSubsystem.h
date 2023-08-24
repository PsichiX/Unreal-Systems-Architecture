#pragma once

#include "CoreMinimal.h"

#include "Subsystems/GameInstanceSubsystem.h"

#include "SystemsSubsystem.generated.h"

class USystemsWorld;

DECLARE_DYNAMIC_DELEGATE_OneParam(FOnSetupSystemsWorld, USystemsWorld*, Systems);

/// Global accessible registry of systems worlds.
///
/// [Architecture book page](/book/architecture/subsystem.md) explains more in
/// depth how to interact with systems subsystem, but in a brief: This is a
/// registry of globally accessible systems worlds game can have at any time.
///
/// # Example
/// ```snippet
/// subsystem_game_instance
/// ```
UCLASS()
class SYSTEMS_API USystemsSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	using SystemSetupFunctor = void(USystemsWorld& Systems);

	/// Gets instance of this subsystem.
	static USystemsSubsystem* Get(
		/// World context object.
		UWorld* World);

	/// Create, setup and register new systems world.
	///
	/// # Example
	/// ```snippet
	/// subsystem_game_mode
	/// ```
	void AcquireSystemsWorld(
		/// Systems world ID.
		FName Id,
		/// Function/lambda that performs setup of created systems world.
		///
		/// # Note
		/// > `SystemSetupFunctor` has given signature:
		/// `void(USystemsWorld& Systems)`.
		TFunction<SystemSetupFunctor> SetupFunctor);

	/// Create, setup and register new systems world.
	///
	/// Blueprint-side version of [`class: $Self$::AcquireSystemsWorld`]().
	UFUNCTION(BlueprintCallable, Category = "Systems|Subsystem", Meta = (DisplayName = "Acquire Systems World"))
	void BlueprintAcquireSystemsWorld(
		/// Systems world ID.
		FName Id,
		/// Delegate that performs setup of created systems world.
		///
		/// # Note
		/// > This delegate has given signature:
		/// `void(USystemsWorld* Systems)`.
		FOnSetupSystemsWorld SetupDelegate);

	/// Unregister given systems world.
	///
	/// # Example
	/// ```snippet
	/// subsystem_game_mode
	/// ```
	UFUNCTION(BlueprintCallable, Category = "Systems|Subsystem", Meta = (Displayname = "Release Systems World"))
	void ReleaseSystemsWorld(FName Id);

	/// Gets systems world by its label.
	UFUNCTION(BlueprintCallable, Category = "Systems|Subsystem", Meta = (Displayname = "Get Systems World"))
	USystemsWorld* GetSystemsWorld(
		/// Systems world ID.
		FName Id);

	/// Gets list of all registered systems worlds IDs.
	///
	/// Useful only for more advanced usecases like editor tools that might want
	/// to list currently existing systems world.
	void GetSystemsWorldsIds(TSet<FName>& Output) const;

	void Store(UObject* Resource);

	UObject* Restore(const UClass* Type, UObject* NewOwner);

	virtual bool ProcessConsoleExec(const TCHAR* Cmd, FOutputDevice& Ar, UObject* Executor) override;

private:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	virtual void Deinitialize() override;

	void Cleanup();

	UFUNCTION()
	void OnTick();

	UPROPERTY()
	TMap<FName, USystemsWorld*> SystemsWorlds = {};

	UPROPERTY()
	TMap<FName, USystemsWorld*> ToAdd = {};

	UPROPERTY()
	TSet<FName> ToRemove = {};

	UPROPERTY()
	TMap<const UClass*, UObject*> GlobalStorage = {};
};