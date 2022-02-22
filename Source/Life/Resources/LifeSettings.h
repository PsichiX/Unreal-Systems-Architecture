#pragma once

#include "CoreMinimal.h"

#include "LifeSettings.generated.h"

class UPaperSprite;

USTRUCT(BlueprintType)
struct FAiLodInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	float Distance = 0;

	UPROPERTY(EditAnywhere)
	float DecideDelay = 0;

	UPROPERTY(EditAnywhere)
	float DecideOffscreenDelay = 0;
};

UCLASS(BlueprintType)
class LIFE_API ULifeSettings : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	float TimeScale = 1;

	UPROPERTY(EditAnywhere)
	float VisibilityConeAngleDegrees = 60;

	UPROPERTY(EditAnywhere)
	float CameraMovementSpeed = 100;

	UPROPERTY(EditAnywhere)
	float CameraRotationSpeed = 90;

	UPROPERTY(EditAnywhere)
	float CameraZoomSpeed = 1;

	UPROPERTY(EditAnywhere)
	float CameraClearOpacity = 0;

	UPROPERTY(EditAnywhere)
	bool bHumansClearVision = false;

	UPROPERTY(EditAnywhere)
	TMap<UClass*, UPaperSprite*> Spawnables = {};

	UPROPERTY(EditAnywhere)
	TArray<FAiLodInfo> AiLOD = {};

	UPROPERTY(EditAnywhere)
	UClass* CursorType = nullptr;
};
