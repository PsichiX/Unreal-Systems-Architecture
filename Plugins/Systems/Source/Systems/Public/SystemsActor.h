#pragma once

#include "CoreMinimal.h"

#include "GameFramework/Actor.h"
#include "GameFramework/Pawn.h"

#include "SystemsActor.generated.h"

/// Base class for systems actors.
///
/// Automatically registers this actor components that are not inheriting from
/// [`class: USystemsActorComponent`]() or [`class: USystemsSceneComponent`]()
/// and have `Systems` component tag applied to them. This simply allows Systems
/// World to recognize and query built-in Unreal Engine components without
/// registering them manually - one less boilerplate to make on user side.
UCLASS(BlueprintType, Blueprintable, Abstract)
class SYSTEMS_API ASystemsActor : public AActor
{
	GENERATED_BODY()

public:
	ASystemsActor();

private:
	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	/// Tells into which global systems world this component should register.
	UPROPERTY(EditAnywhere)
	FName SystemsWorld = FName();
};

/// Base class for systems actors.
///
/// Does exactly the same as [`class: ASystemsActor`]() but for Pawns.
UCLASS(BlueprintType, Blueprintable, Abstract)
class SYSTEMS_API ASystemsPawn : public APawn
{
	GENERATED_BODY()

public:
	ASystemsPawn();

private:
	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	/// Tells into which global systems world this component should register.
	UPROPERTY(EditAnywhere)
	FName SystemsWorld = FName();
};