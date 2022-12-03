#pragma once

#include "CoreMinimal.h"

#include "GameFramework/Pawn.h"

#include "SystemsPawn.generated.h"

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
