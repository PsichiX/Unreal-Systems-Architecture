#pragma once

#include "CoreMinimal.h"

#include "Kismet/BlueprintFunctionLibrary.h"
#include "Systems/Public/SystemsWorld.h"

#include "SystemsReflection.generated.h"

/// Systems reflection singleton.
///
/// Stores application-wide database of named system functions to be used by
/// [`class: ULambdaSystem`].
class SYSTEMS_API FSystemsReflection
{
public:
	static FSystemsReflection& Get();

	/// Register or overwrite system function under given name.
	void Register(
		/// Unique name for given system function.
		FName Name,
		/// Reference to system function.
		TFunction<USystemsWorld::LambdaSystemType> Callback);

	/// Unregister system function under given name.
	void Unregister(
		/// Unique name for given system function.
		FName Name);

	/// Gets list of system function names.
	void GetNames(
		/// Output array to receive names.
		TArray<FString>& Result) const;

	/// Find system function by name.
	TOptional<TFunction<USystemsWorld::LambdaSystemType>> FindByName(
		/// Unique name for given system function.
		FName Name) const;

private:
	TMap<FName, TFunction<USystemsWorld::LambdaSystemType>> Registry = {};

	mutable FCriticalSection CriticalSection = {};
};

/// Systems reflection blueprint library.
UCLASS()
class SYSTEMS_API USystemsReflection : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

	/// Gets list of system function names as possible options along with None
	/// option.
	UFUNCTION(BlueprintCallable, Category = "Systems Reflection")
	static TArray<FString> GetOptions();
};

// Handy wrapper to register system function under its function name.
//
// Should be called in module `StartupModule` method to register all systems
// provided by given module.
//
// > NOTE: Make sure `SYSTEMS_NAMESPACE` define is set to module name string
// since it is required to properly prefix registered function names.
#define REGISTER_SYSTEM_FUNCTION(Function)                \
	{                                                     \
		FSystemsReflection::Get().Register(               \
			(SYSTEMS_NAMESPACE "." #Function), Function); \
	}

// Handy wrapper to unregister system function under its function name.
//
// Should be called in module `ShutdownModule` method to register all systems
// provided by given module.
//
// > NOTE: Make sure `SYSTEMS_NAMESPACE` define is set to module name string
// since it is required to properly prefix registered function names.
#define UNREGISTER_SYSTEM_FUNCTION(Function)    \
	{                                           \
		FSystemsReflection::Get().Unregister(   \
			(SYSTEMS_NAMESPACE "." #Function)); \
	}
