#include "Systems_Workspace/Boids/UI/WidgetRunCriteria.h"

#include "ReactiveUserInterfaceSystems/Public/Resources/UiChangeDetection.h"
#include "Systems/Public/SystemsStatics.h"
#include "Systems/Public/SystemsWorld.h"

#include "Systems_Workspace/Boids/BoidsGameMode.h"
#include "Systems_Workspace/Boids/Resources/BoidsSystemsRunCriteria.h"
#include "Systems_Workspace/Shared/UI/WidgetCheckbox.h"

void UWidgetRunCriteria::NativeConstruct()
{
	Super::NativeConstruct();

	const auto* RunCriteria = USystemsStatics::GetResource<UBoidsSystemsRunCriteria>(
		ABoidsGameMode::SYSTEMS_WORLD, GetWorld());

	if (IsValid(this->RunPopulationControl))
	{
		this->RunPopulationControl->OnChanged().AddUniqueDynamic(
			this, &ThisClass::OnRunPopulationControlChanged);
	}
	if (IsValid(this->RunSeparation))
	{
		this->RunSeparation->OnChanged().AddUniqueDynamic(this, &ThisClass::OnRunSeparationChanged);
	}
	if (IsValid(this->RunAlignment))
	{
		this->RunAlignment->OnChanged().AddUniqueDynamic(this, &ThisClass::OnRunAlignmentChanged);
	}
	if (IsValid(this->RunCohesion))
	{
		this->RunCohesion->OnChanged().AddUniqueDynamic(this, &ThisClass::OnRunCohesionChanged);
	}
	if (IsValid(this->RunHunt))
	{
		this->RunHunt->OnChanged().AddUniqueDynamic(this, &ThisClass::OnRunHuntChanged);
	}
	if (IsValid(this->RunHunger))
	{
		this->RunHunger->OnChanged().AddUniqueDynamic(this, &ThisClass::OnRunHungerChanged);
	}
	if (IsValid(this->RunEat))
	{
		this->RunEat->OnChanged().AddUniqueDynamic(this, &ThisClass::OnRunEatChanged);
	}
	if (IsValid(this->RunLimitVelocity))
	{
		this->RunLimitVelocity->OnChanged().AddUniqueDynamic(
			this, &ThisClass::OnRunLimitVelocityChanged);
	}
	if (IsValid(this->RunKeepInSpaceBounds))
	{
		this->RunKeepInSpaceBounds->OnChanged().AddUniqueDynamic(
			this, &ThisClass::OnRunKeepInSpaceBoundsChanged);
	}
	if (IsValid(this->RunApplyImpulse))
	{
		this->RunApplyImpulse->OnChanged().AddUniqueDynamic(
			this, &ThisClass::OnRunApplyImpulseChanged);
	}
	if (IsValid(this->RunMovement))
	{
		this->RunMovement->OnChanged().AddUniqueDynamic(this, &ThisClass::OnRunMovementChanged);
	}
	if (IsValid(this->RunFaceDirection))
	{
		this->RunFaceDirection->OnChanged().AddUniqueDynamic(
			this, &ThisClass::OnRunFaceDirectionChanged);
	}
	if (IsValid(this->RunProximityColor))
	{
		this->RunProximityColor->OnChanged().AddUniqueDynamic(
			this, &ThisClass::OnRunProximityColorChanged);
	}
	if (IsValid(this->RunDebugDraw))
	{
		this->RunDebugDraw->OnChanged().AddUniqueDynamic(this, &ThisClass::OnRunDebugDrawChanged);
	}

	auto* ChangeDetection =
		USystemsStatics::GetResource<UUiChangeDetection>(ABoidsGameMode::SYSTEMS_WORLD, GetWorld());
	if (IsValid(ChangeDetection))
	{
		auto Signature = FUiChangeSignature();
		Signature.Resource<UBoidsSystemsRunCriteria>();
		ChangeDetection->Subscribe(Signature, this, &ThisClass::OnRunCriteriaChanged);
	}
}

void UWidgetRunCriteria::NativeDestruct()
{
	Super::NativeDestruct();

	if (IsValid(this->RunPopulationControl))
	{
		this->RunPopulationControl->OnChanged().RemoveAll(this);
	}
	if (IsValid(this->RunSeparation))
	{
		this->RunSeparation->OnChanged().RemoveAll(this);
	}
	if (IsValid(this->RunAlignment))
	{
		this->RunAlignment->OnChanged().RemoveAll(this);
	}
	if (IsValid(this->RunCohesion))
	{
		this->RunCohesion->OnChanged().RemoveAll(this);
	}
	if (IsValid(this->RunHunt))
	{
		this->RunHunt->OnChanged().RemoveAll(this);
	}
	if (IsValid(this->RunHunger))
	{
		this->RunHunger->OnChanged().RemoveAll(this);
	}
	if (IsValid(this->RunEat))
	{
		this->RunEat->OnChanged().RemoveAll(this);
	}
	if (IsValid(this->RunLimitVelocity))
	{
		this->RunLimitVelocity->OnChanged().RemoveAll(this);
	}
	if (IsValid(this->RunKeepInSpaceBounds))
	{
		this->RunKeepInSpaceBounds->OnChanged().RemoveAll(this);
	}
	if (IsValid(this->RunApplyImpulse))
	{
		this->RunApplyImpulse->OnChanged().RemoveAll(this);
	}
	if (IsValid(this->RunMovement))
	{
		this->RunMovement->OnChanged().RemoveAll(this);
	}
	if (IsValid(this->RunFaceDirection))
	{
		this->RunFaceDirection->OnChanged().RemoveAll(this);
	}
	if (IsValid(this->RunProximityColor))
	{
		this->RunProximityColor->OnChanged().RemoveAll(this);
	}
	if (IsValid(this->RunDebugDraw))
	{
		this->RunDebugDraw->OnChanged().RemoveAll(this);
	}

	auto* ChangeDetection =
		USystemsStatics::GetResource<UUiChangeDetection>(ABoidsGameMode::SYSTEMS_WORLD, GetWorld());
	if (IsValid(ChangeDetection))
	{
		ChangeDetection->UnsubscribeAll(this);
	}
}

void UWidgetRunCriteria::OnRunCriteriaChanged(USystemsWorld& Systems)
{
	const auto* RunCriteria = Systems.Resource<UBoidsSystemsRunCriteria>();

	if (IsValid(RunCriteria) == false)
	{
		return;
	}

	if (IsValid(this->RunPopulationControl))
	{
		this->RunPopulationControl->SetChecked(RunCriteria->bRunPopulationControl);
	}
	if (IsValid(this->RunSeparation))
	{
		this->RunSeparation->SetChecked(RunCriteria->bRunSeparation);
	}
	if (IsValid(this->RunAlignment))
	{
		this->RunAlignment->SetChecked(RunCriteria->bRunAlignment);
	}
	if (IsValid(this->RunCohesion))
	{
		this->RunCohesion->SetChecked(RunCriteria->bRunCohesion);
	}
	if (IsValid(this->RunHunt))
	{
		this->RunHunt->SetChecked(RunCriteria->bRunHunt);
	}
	if (IsValid(this->RunHunger))
	{
		this->RunHunger->SetChecked(RunCriteria->bRunHunger);
	}
	if (IsValid(this->RunEat))
	{
		this->RunEat->SetChecked(RunCriteria->bRunEat);
	}
	if (IsValid(this->RunLimitVelocity))
	{
		this->RunLimitVelocity->SetChecked(RunCriteria->bRunLimitVelocity);
	}
	if (IsValid(this->RunKeepInSpaceBounds))
	{
		this->RunKeepInSpaceBounds->SetChecked(RunCriteria->bRunKeepInSpaceBounds);
	}
	if (IsValid(this->RunApplyImpulse))
	{
		this->RunApplyImpulse->SetChecked(RunCriteria->bRunApplyImpulse);
	}
	if (IsValid(this->RunMovement))
	{
		this->RunMovement->SetChecked(RunCriteria->bRunMovement);
	}
	if (IsValid(this->RunFaceDirection))
	{
		this->RunFaceDirection->SetChecked(RunCriteria->bRunFaceDirection);
	}
	if (IsValid(this->RunProximityColor))
	{
		this->RunProximityColor->SetChecked(RunCriteria->bRunProximityColor);
	}
	if (IsValid(this->RunDebugDraw))
	{
		this->RunDebugDraw->SetChecked(RunCriteria->bRunDebugDraw);
	}
}

void UWidgetRunCriteria::OnRunPopulationControlChanged(bool bIsChecked)
{
	auto* Systems = USystemsStatics::GetSystemsWorld(ABoidsGameMode::SYSTEMS_WORLD, GetWorld());
	if (IsValid(Systems))
	{
		auto* RunCriteria = Systems->Resource<UBoidsSystemsRunCriteria>();
		if (IsValid(RunCriteria))
		{
			RunCriteria->bRunPopulationControl = bIsChecked;
			Systems->MarkResourceChanged(RunCriteria);
		}
	}
}

void UWidgetRunCriteria::OnRunSeparationChanged(bool bIsChecked)
{
	auto* Systems = USystemsStatics::GetSystemsWorld(ABoidsGameMode::SYSTEMS_WORLD, GetWorld());
	if (IsValid(Systems))
	{
		auto* RunCriteria = Systems->Resource<UBoidsSystemsRunCriteria>();
		if (IsValid(RunCriteria))
		{
			RunCriteria->bRunSeparation = bIsChecked;
			Systems->MarkResourceChanged(RunCriteria);
		}
	}
}

void UWidgetRunCriteria::OnRunAlignmentChanged(bool bIsChecked)
{
	auto* Systems = USystemsStatics::GetSystemsWorld(ABoidsGameMode::SYSTEMS_WORLD, GetWorld());
	if (IsValid(Systems))
	{
		auto* RunCriteria = Systems->Resource<UBoidsSystemsRunCriteria>();
		if (IsValid(RunCriteria))
		{
			RunCriteria->bRunAlignment = bIsChecked;
			Systems->MarkResourceChanged(RunCriteria);
		}
	}
}

void UWidgetRunCriteria::OnRunCohesionChanged(bool bIsChecked)
{
	auto* Systems = USystemsStatics::GetSystemsWorld(ABoidsGameMode::SYSTEMS_WORLD, GetWorld());
	if (IsValid(Systems))
	{
		auto* RunCriteria = Systems->Resource<UBoidsSystemsRunCriteria>();
		if (IsValid(RunCriteria))
		{
			RunCriteria->bRunCohesion = bIsChecked;
			Systems->MarkResourceChanged(RunCriteria);
		}
	}
}

void UWidgetRunCriteria::OnRunHuntChanged(bool bIsChecked)
{
	auto* Systems = USystemsStatics::GetSystemsWorld(ABoidsGameMode::SYSTEMS_WORLD, GetWorld());
	if (IsValid(Systems))
	{
		auto* RunCriteria = Systems->Resource<UBoidsSystemsRunCriteria>();
		if (IsValid(RunCriteria))
		{
			RunCriteria->bRunHunt = bIsChecked;
			Systems->MarkResourceChanged(RunCriteria);
		}
	}
}

void UWidgetRunCriteria::OnRunHungerChanged(bool bIsChecked)
{
	auto* Systems = USystemsStatics::GetSystemsWorld(ABoidsGameMode::SYSTEMS_WORLD, GetWorld());
	if (IsValid(Systems))
	{
		auto* RunCriteria = Systems->Resource<UBoidsSystemsRunCriteria>();
		if (IsValid(RunCriteria))
		{
			RunCriteria->bRunHunger = bIsChecked;
			Systems->MarkResourceChanged(RunCriteria);
		}
	}
}

void UWidgetRunCriteria::OnRunEatChanged(bool bIsChecked)
{
	auto* Systems = USystemsStatics::GetSystemsWorld(ABoidsGameMode::SYSTEMS_WORLD, GetWorld());
	if (IsValid(Systems))
	{
		auto* RunCriteria = Systems->Resource<UBoidsSystemsRunCriteria>();
		if (IsValid(RunCriteria))
		{
			RunCriteria->bRunEat = bIsChecked;
			Systems->MarkResourceChanged(RunCriteria);
		}
	}
}

void UWidgetRunCriteria::OnRunLimitVelocityChanged(bool bIsChecked)
{
	auto* Systems = USystemsStatics::GetSystemsWorld(ABoidsGameMode::SYSTEMS_WORLD, GetWorld());
	if (IsValid(Systems))
	{
		auto* RunCriteria = Systems->Resource<UBoidsSystemsRunCriteria>();
		if (IsValid(RunCriteria))
		{
			RunCriteria->bRunLimitVelocity = bIsChecked;
			Systems->MarkResourceChanged(RunCriteria);
		}
	}
}

void UWidgetRunCriteria::OnRunKeepInSpaceBoundsChanged(bool bIsChecked)
{
	auto* Systems = USystemsStatics::GetSystemsWorld(ABoidsGameMode::SYSTEMS_WORLD, GetWorld());
	if (IsValid(Systems))
	{
		auto* RunCriteria = Systems->Resource<UBoidsSystemsRunCriteria>();
		if (IsValid(RunCriteria))
		{
			RunCriteria->bRunKeepInSpaceBounds = bIsChecked;
			Systems->MarkResourceChanged(RunCriteria);
		}
	}
}

void UWidgetRunCriteria::OnRunApplyImpulseChanged(bool bIsChecked)
{
	auto* Systems = USystemsStatics::GetSystemsWorld(ABoidsGameMode::SYSTEMS_WORLD, GetWorld());
	if (IsValid(Systems))
	{
		auto* RunCriteria = Systems->Resource<UBoidsSystemsRunCriteria>();
		if (IsValid(RunCriteria))
		{
			RunCriteria->bRunApplyImpulse = bIsChecked;
			Systems->MarkResourceChanged(RunCriteria);
		}
	}
}

void UWidgetRunCriteria::OnRunMovementChanged(bool bIsChecked)
{
	auto* Systems = USystemsStatics::GetSystemsWorld(ABoidsGameMode::SYSTEMS_WORLD, GetWorld());
	if (IsValid(Systems))
	{
		auto* RunCriteria = Systems->Resource<UBoidsSystemsRunCriteria>();
		if (IsValid(RunCriteria))
		{
			RunCriteria->bRunMovement = bIsChecked;
			Systems->MarkResourceChanged(RunCriteria);
		}
	}
}

void UWidgetRunCriteria::OnRunFaceDirectionChanged(bool bIsChecked)
{
	auto* Systems = USystemsStatics::GetSystemsWorld(ABoidsGameMode::SYSTEMS_WORLD, GetWorld());
	if (IsValid(Systems))
	{
		auto* RunCriteria = Systems->Resource<UBoidsSystemsRunCriteria>();
		if (IsValid(RunCriteria))
		{
			RunCriteria->bRunFaceDirection = bIsChecked;
			Systems->MarkResourceChanged(RunCriteria);
		}
	}
}

void UWidgetRunCriteria::OnRunProximityColorChanged(bool bIsChecked)
{
	auto* Systems = USystemsStatics::GetSystemsWorld(ABoidsGameMode::SYSTEMS_WORLD, GetWorld());
	if (IsValid(Systems))
	{
		auto* RunCriteria = Systems->Resource<UBoidsSystemsRunCriteria>();
		if (IsValid(RunCriteria))
		{
			RunCriteria->bRunProximityColor = bIsChecked;
			Systems->MarkResourceChanged(RunCriteria);
		}
	}
}

void UWidgetRunCriteria::OnRunDebugDrawChanged(bool bIsChecked)
{
	auto* Systems = USystemsStatics::GetSystemsWorld(ABoidsGameMode::SYSTEMS_WORLD, GetWorld());
	if (IsValid(Systems))
	{
		auto* RunCriteria = Systems->Resource<UBoidsSystemsRunCriteria>();
		if (IsValid(RunCriteria))
		{
			RunCriteria->bRunDebugDraw = bIsChecked;
			Systems->MarkResourceChanged(RunCriteria);
		}
	}
}
