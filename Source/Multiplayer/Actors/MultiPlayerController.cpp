#include "Multiplayer/Actors/MultiPlayerController.h"

#include "Systems/Public/SystemsStatics.h"

#include "Multiplayer/MultiplayerGameMode.h"
#include "Multiplayer/Resources/MultiPlayerInput.h"

const FName ACTION_BINDING = FName(TEXT("Action"));
const FName CAMERA_RELATIVE_DIRECTION_HORIZONTAL_BINDING =
	FName(TEXT("CameraRelativeDirectionHorizontal"));
const FName CAMERA_RELATIVE_DIRECTION_VERTICAL_BINDING =
	FName(TEXT("CameraRelativeDirectionVertical"));
const FName CAMERA_RELATIVE_MOVEMENT_HORIZONTAL_BINDING =
	FName(TEXT("CameraRelativeMovementHorizontal"));
const FName CAMERA_RELATIVE_MOVEMENT_VERTICAL_BINDING =
	FName(TEXT("CameraRelativeMovementVertical"));

void AMultiPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	this->InputComponent->BindAction(
		ACTION_BINDING, EInputEvent::IE_Pressed, this, &ThisClass::OnActionPressed);

	this->InputComponent->BindAction(
		ACTION_BINDING, EInputEvent::IE_Released, this, &ThisClass::OnActionReleased);

	this->InputComponent->BindAxis(CAMERA_RELATIVE_DIRECTION_HORIZONTAL_BINDING,
		this,
		&ThisClass::OnRelativeDirectionHorizontal);

	this->InputComponent->BindAxis(
		CAMERA_RELATIVE_DIRECTION_VERTICAL_BINDING, this, &ThisClass::OnRelativeDirectionVertical);

	this->InputComponent->BindAxis(CAMERA_RELATIVE_MOVEMENT_HORIZONTAL_BINDING,
		this,
		&ThisClass::OnRelativeMovementHorizontal);

	this->InputComponent->BindAxis(
		CAMERA_RELATIVE_MOVEMENT_VERTICAL_BINDING, this, &ThisClass::OnRelativeMovementVertical);
}

void AMultiPlayerController::OnActionPressed()
{
	auto* Input = USystemsStatics::GetResource<UMultiPlayerInput>(
		AMultiplayerGameMode::SYSTEMS_WORLD, GetWorld());
	if (IsValid(Input))
	{
		Input->bAction = true;
	}
}

void AMultiPlayerController::OnActionReleased()
{
	auto* Input = USystemsStatics::GetResource<UMultiPlayerInput>(
		AMultiplayerGameMode::SYSTEMS_WORLD, GetWorld());
	if (IsValid(Input))
	{
		Input->bAction = false;
	}
}

void AMultiPlayerController::OnRelativeDirectionHorizontal(float Delta)
{
	auto* Input = USystemsStatics::GetResource<UMultiPlayerInput>(
		AMultiplayerGameMode::SYSTEMS_WORLD, GetWorld());
	if (IsValid(Input))
	{
		Input->RelativeLook.X = Delta;
	}
}

void AMultiPlayerController::OnRelativeDirectionVertical(float Delta)
{
	auto* Input = USystemsStatics::GetResource<UMultiPlayerInput>(
		AMultiplayerGameMode::SYSTEMS_WORLD, GetWorld());
	if (IsValid(Input))
	{
		Input->RelativeLook.Y = Delta;
	}
}

void AMultiPlayerController::OnRelativeMovementHorizontal(float Delta)
{
	auto* Input = USystemsStatics::GetResource<UMultiPlayerInput>(
		AMultiplayerGameMode::SYSTEMS_WORLD, GetWorld());
	if (IsValid(Input))
	{
		Input->RelativeMovement.X = Delta;
	}
}

void AMultiPlayerController::OnRelativeMovementVertical(float Delta)
{
	auto* Input = USystemsStatics::GetResource<UMultiPlayerInput>(
		AMultiplayerGameMode::SYSTEMS_WORLD, GetWorld());
	if (IsValid(Input))
	{
		Input->RelativeMovement.Y = Delta;
	}
}
