#include "Life/Components/GodComponent.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"

const FName CAMERA_ZOOM_ROTATE_MODE_BINDING = FName(TEXT("CameraZoomRotateMode"));
const FName ACTION_BINDING = FName(TEXT("Action"));
const FName CAMERA_RELATIVE_DIRECTION_HORIZONTAL_BINDING =
	FName(TEXT("CameraRelativeDirectionHorizontal"));
const FName CAMERA_RELATIVE_DIRECTION_VERTICAL_BINDING =
	FName(TEXT("CameraRelativeDirectionVertical"));
const FName CAMERA_RELATIVE_MOVEMENT_HORIZONTAL_BINDING =
	FName(TEXT("CameraRelativeMovementHorizontal"));
const FName CAMERA_RELATIVE_MOVEMENT_VERTICAL_BINDING =
	FName(TEXT("CameraRelativeMovementVertical"));

FVector UGodComponent::GetCameraPosition() const
{
	return IsValid(this->CachedCamera) ? this->CachedCamera->GetComponentLocation()
									   : GetOwner()->GetActorLocation();
}

FRotator UGodComponent::GetCameraRotation() const
{
	return IsValid(this->CachedCamera) ? this->CachedCamera->GetComponentRotation()
									   : GetOwner()->GetActorRotation();
}

void UGodComponent::SetCameraZoom(float ZoomFactor)
{
	this->CameraZoomFactor = FMath::Clamp(ZoomFactor, 0.0f, 1.0f);
	if (IsValid(this->CachedSpringArm) && IsValid(GetOwner()))
	{
		float TimeMin = 0;
		float TimeMax = 0;

		this->DistanceCurve.GetRichCurveConst()->GetTimeRange(TimeMin, TimeMax);
		const auto DistanceTime = FMath::Lerp(TimeMin, TimeMax, this->CameraZoomFactor);
		this->CachedSpringArm->TargetArmLength =
			this->DistanceCurve.GetRichCurveConst()->Eval(DistanceTime);

		this->AngleCurve.GetRichCurveConst()->GetTimeRange(TimeMin, TimeMax);
		const auto AngleTime = FMath::Lerp(TimeMin, TimeMax, this->CameraZoomFactor);
		auto Rotation = this->CachedSpringArm->GetRelativeRotation();
		Rotation.Pitch = -this->AngleCurve.GetRichCurveConst()->Eval(AngleTime);
		this->CachedSpringArm->SetRelativeRotation(Rotation);
	}
}

void UGodComponent::BeginPlay()
{
	Super::BeginPlay();

	this->CachedCamera = GetOwner()->FindComponentByClass<UCameraComponent>();
	this->CachedSpringArm = GetOwner()->FindComponentByClass<USpringArmComponent>();
	SetCameraZoom(this->CameraZoomFactor);

	GetOwner()->EnableInput(GetWorld()->GetFirstPlayerController());

	auto& Input = GetOwner()->InputComponent;
	if (IsValid(Input))
	{
		Input->BindAction(CAMERA_ZOOM_ROTATE_MODE_BINDING,
			EInputEvent::IE_Pressed,
			this,
			&ThisClass::OnCameraZoomRotateModePressed);

		Input->BindAction(CAMERA_ZOOM_ROTATE_MODE_BINDING,
			EInputEvent::IE_Released,
			this,
			&ThisClass::OnCameraZoomRotateModeReleased);

		Input->BindAction(
			ACTION_BINDING, EInputEvent::IE_Pressed, this, &ThisClass::OnActionPressed);

		Input->BindAction(
			ACTION_BINDING, EInputEvent::IE_Released, this, &ThisClass::OnActionReleased);

		Input->BindAxis(CAMERA_RELATIVE_DIRECTION_HORIZONTAL_BINDING,
			this,
			&ThisClass::OnCameraRelativeDirectionHorizontal);

		Input->BindAxis(CAMERA_RELATIVE_DIRECTION_VERTICAL_BINDING,
			this,
			&ThisClass::OnCameraRelativeDirectionVertical);

		Input->BindAxis(CAMERA_RELATIVE_MOVEMENT_HORIZONTAL_BINDING,
			this,
			&ThisClass::OnCameraRelativeMovementHorizontal);

		Input->BindAxis(CAMERA_RELATIVE_MOVEMENT_VERTICAL_BINDING,
			this,
			&ThisClass::OnCameraRelativeMovementVertical);
	}
}

void UGodComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	auto& Input = GetOwner()->InputComponent;
	if (IsValid(Input))
	{
		Input->ClearActionBindings();
		Input->AxisBindings.Reset(0);
	}

	GetOwner()->DisableInput(GetWorld()->GetFirstPlayerController());

	this->CachedCamera = nullptr;
	this->CachedSpringArm = nullptr;
}

void UGodComponent::OnCameraZoomRotateModePressed()
{
	this->bCameraZoomRotateMode = true;
}

void UGodComponent::OnCameraZoomRotateModeReleased()
{
	this->bCameraZoomRotateMode = false;
}

void UGodComponent::OnActionPressed()
{
	this->bAction = true;
}

void UGodComponent::OnActionReleased()
{
	this->bAction = false;
}

void UGodComponent::OnCameraRelativeDirectionHorizontal(float Delta)
{
	this->CameraRelativeDirection.X = Delta;
}

void UGodComponent::OnCameraRelativeDirectionVertical(float Delta)
{
	this->CameraRelativeDirection.Y = Delta;
}

void UGodComponent::OnCameraRelativeMovementHorizontal(float Delta)
{
	this->CameraRelativeMovement.X = Delta;
}

void UGodComponent::OnCameraRelativeMovementVertical(float Delta)
{
	this->CameraRelativeMovement.Y = Delta;
}
