#pragma once

#include "CoreMinimal.h"

#include "PaperSpriteComponent.h"
#include "Systems/Public/SystemsComponent.h"

#include "SpriteProxyComponent.generated.h"

UCLASS(BlueprintType, Blueprintable, Meta = (BlueprintSpawnableComponent))
class SYSTEMS_WORKSPACE_API USpriteProxyComponent : public UPaperSpriteComponent
{
	GENERATED_BODY()

public:
	UFUNCTION()
	void ApplyVisibility(float Factor);

	UFUNCTION()
	void ApplyShiny(float Factor);

	UFUNCTION()
	void ApplySaturation(float Factor);

protected:
	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
};
