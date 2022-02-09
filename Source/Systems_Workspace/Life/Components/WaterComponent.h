#pragma once

#include "CoreMinimal.h"

#include "Systems/Public/SystemsComponent.h"

#include "WaterComponent.generated.h"

UCLASS(BlueprintType, Blueprintable, Meta = (BlueprintSpawnableComponent))
class SYSTEMS_WORKSPACE_API UWaterComponent : public USystemsActorComponent
{
	GENERATED_BODY()

public:
	void ApplyToMaterial();

	UPROPERTY()
	float Time = 0;

private:
	virtual void BeginPlay() override;

	UPROPERTY()
	UMaterialInstanceDynamic* CachedInstance = nullptr;
};
