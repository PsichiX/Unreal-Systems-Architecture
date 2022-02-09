#pragma once

#include "CoreMinimal.h"

#include "BoidsSystemsRunCriteria.generated.h"

UCLASS(BlueprintType)
class SYSTEMS_WORKSPACE_API UBoidsSystemsRunCriteria : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	bool bRunPopulationControl = true;

	UPROPERTY(EditAnywhere)
	bool bRunSeparation = true;

	UPROPERTY(EditAnywhere)
	bool bRunAlignment = true;

	UPROPERTY(EditAnywhere)
	bool bRunCohesion = true;

	UPROPERTY(EditAnywhere)
	bool bRunLimitVelocity = true;

	UPROPERTY(EditAnywhere)
	bool bRunKeepInSpaceBounds = true;

	UPROPERTY(EditAnywhere)
	bool bRunApplyImpulse = true;

	UPROPERTY(EditAnywhere)
	bool bRunMovement = true;

	UPROPERTY(EditAnywhere)
	bool bRunFaceDirection = true;

	UPROPERTY(EditAnywhere)
	bool bRunHunt = true;

	UPROPERTY(EditAnywhere)
	bool bRunHunger = true;

	UPROPERTY(EditAnywhere)
	bool bRunEat = true;

	UPROPERTY(EditAnywhere)
	bool bRunProximityColor = true;

	UPROPERTY(EditAnywhere)
	bool bRunDebugDraw = true;
};
