#pragma once

#include "CoreMinimal.h"

#include "Systems/Public/SystemsComponent.h"

#include "FlipbookBlendSpaceComponent.generated.h"

class UPaperFlipbook;

USTRUCT(BlueprintType)
struct FFlipbookBlendSpaceItem
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	FVector2D Point = FVector2D(0);

	UPROPERTY(EditAnywhere)
	float Weight = 1;

	UPROPERTY(EditAnywhere)
	UPaperFlipbook* Flipbook = nullptr;
};

USTRUCT(BlueprintType)
struct FFlipbookBlendSpace
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	float PlayRateToSpeedFactor = 1;

	UPROPERTY(EditAnywhere)
	TArray<FFlipbookBlendSpaceItem> Items = {};
};

UCLASS(BlueprintType, Blueprintable, Meta = (BlueprintSpawnableComponent))
class SYSTEMS_WORKSPACE_API UFlipbookBlendSpaceComponent : public USystemsActorComponent
{
	GENERATED_BODY()

public:
	UFUNCTION()
	UPaperFlipbook* FindCurrentFlipbook(const FVector2D& Point) const;

	UFUNCTION()
	float FindCurrentPlayRateToSpeedFactor() const;

	UPROPERTY(EditAnywhere)
	TMap<FName, FFlipbookBlendSpace> Animations = {};

	UPROPERTY(EditAnywhere)
	FName CurrentAnimation = FName();
};
