#pragma once

#include "CoreMinimal.h"

#include "Systems/Public/SystemsComponent.h"

#include "TutorialHighlightComponent.generated.h"

UCLASS(BlueprintType, Blueprintable, Meta = (BlueprintSpawnableComponent))
class TUTORIAL_API UTutorialHighlightComponent : public USystemsActorComponent
{
	GENERATED_BODY()

public:
	UFUNCTION()
	void ApplyHighlighted(bool bMode);

	UPROPERTY(EditAnywhere)
	FLinearColor DefaultColor = FLinearColor::White;

	UPROPERTY(EditAnywhere)
	FLinearColor HighlightColor = FLinearColor::Yellow;

private:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere)
	UMaterialInterface* MaterialInterface = nullptr;

	UPROPERTY()
	UMaterialInstanceDynamic* CachedInstance = nullptr;
};
