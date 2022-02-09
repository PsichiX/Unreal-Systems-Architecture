#pragma once

#include "CoreMinimal.h"

#include "Systems/Public/SystemsComponent.h"

#include "GodComponent.generated.h"

class UCameraComponent;
class USpringArmComponent;

UCLASS(BlueprintType, Blueprintable, Meta = (BlueprintSpawnableComponent))
class SYSTEMS_WORKSPACE_API UGodComponent : public USystemsActorComponent
{
	GENERATED_BODY()

public:
	UFUNCTION()
	FVector GetCameraPosition() const;

	UFUNCTION()
	FRotator GetCameraRotation() const;

	UFUNCTION()
	float GetCameraZoom() const
	{
		return this->CameraZoomFactor;
	}

	UFUNCTION()
	void SetCameraZoom(float ZoomFactor);

	UPROPERTY()
	bool bCameraZoomRotateMode = false;

	UPROPERTY()
	bool bAction = false;

	UPROPERTY()
	FVector2D CameraRelativeDirection = FVector2D(0);

	UPROPERTY()
	FVector2D CameraRelativeMovement = FVector2D(0);

	UPROPERTY(EditAnywhere)
	FRuntimeFloatCurve DistanceCurve = {};

	UPROPERTY(EditAnywhere)
	FRuntimeFloatCurve AngleCurve = {};

private:
	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	UFUNCTION()
	void OnCameraZoomRotateModePressed();

	UFUNCTION()
	void OnCameraZoomRotateModeReleased();

	UFUNCTION()
	void OnActionPressed();

	UFUNCTION()
	void OnActionReleased();

	UFUNCTION()
	void OnCameraRelativeDirectionHorizontal(float Delta);

	UFUNCTION()
	void OnCameraRelativeDirectionVertical(float Delta);

	UFUNCTION()
	void OnCameraRelativeMovementHorizontal(float Delta);

	UFUNCTION()
	void OnCameraRelativeMovementVertical(float Delta);

	UPROPERTY()
	float CameraZoomFactor = 0.5;

	UPROPERTY()
	UCameraComponent* CachedCamera = nullptr;

	UPROPERTY()
	USpringArmComponent* CachedSpringArm = nullptr;
};
