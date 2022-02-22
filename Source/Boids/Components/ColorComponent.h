#pragma once

#include "CoreMinimal.h"

#include "Systems/Public/SystemsComponent.h"

#include "ColorComponent.generated.h"

UCLASS(BlueprintType, Blueprintable, Meta = (BlueprintSpawnableComponent))
class BOIDS_API UColorComponent : public USystemsActorComponent
{
	GENERATED_BODY()

public:
	UFUNCTION()
	void ApplyColor(const FLinearColor Color);

	UFUNCTION()
	void ApplyDefaultColor();

	UPROPERTY(EditAnywhere)
	FLinearColor DefaultColor = FLinearColor::White;

private:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere)
	UMaterialInterface* MaterialInterface = nullptr;

	UPROPERTY()
	UMaterialInstanceDynamic* CachedInstance = nullptr;
};
