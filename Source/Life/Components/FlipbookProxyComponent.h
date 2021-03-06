#pragma once

#include "CoreMinimal.h"

#include "PaperFlipbookComponent.h"

#include "FlipbookProxyComponent.generated.h"

UCLASS(BlueprintType, Blueprintable, Meta = (BlueprintSpawnableComponent))
class LIFE_API UFlipbookProxyComponent : public UPaperFlipbookComponent
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
