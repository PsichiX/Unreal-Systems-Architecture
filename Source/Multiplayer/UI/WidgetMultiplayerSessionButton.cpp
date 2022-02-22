#include "Multiplayer/UI/WidgetMultiplayerSessionButton.h"

#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "OnlineSubsystem.h"
#include "OnlineSubsystemUtils.h"

void UWidgetMultiplayerSessionButton::NativeConstruct()
{
	Super::NativeConstruct();

	if (IsValid(this->Button))
	{
		this->Button->OnClicked.AddUniqueDynamic(this, &ThisClass::OnClicked);
	}
}

void UWidgetMultiplayerSessionButton::NativeDestruct()
{
	Super::NativeDestruct();

	if (IsValid(this->Button))
	{
		this->Button->OnClicked.RemoveAll(this);
	}
}

void UWidgetMultiplayerSessionButton::NativeOnListItemObjectSet(UObject* ListItemObject)
{
	this->Info = Cast<UWidgetMultiplayerSessionInfo>(ListItemObject);
	if (IsValid(this->Info) == false || this->Info->Result.IsValid() == false)
	{
		return;
	}

	const auto DisplayName = this->Info->Result.Session.OwningUserName;
	const auto Ping = this->Info->Result.PingInMs;
	const auto Content = FString::Printf(TEXT("%s (%i ms)"), *DisplayName, Ping);

	this->Text->SetText(FText::AsCultureInvariant(Content));
}

void UWidgetMultiplayerSessionButton::OnClicked()
{
	if (IsValid(this->Info) == false || this->Info->Result.IsValid() == false)
	{
		return;
	}

	const auto* LocalPlayer = GetWorld()->GetFirstLocalPlayerFromController();
	auto Sessions = Online::GetSessionInterface(GetWorld());
	if (Sessions.IsValid() && IsValid(LocalPlayer))
	{
		this->JoinSessionCompleteDelegateHandle = Sessions->AddOnJoinSessionCompleteDelegate_Handle(
			FOnJoinSessionCompleteDelegate::CreateUObject(this, &ThisClass::OnSessionJoined));

		const auto Id = LocalPlayer->GetPreferredUniqueNetId();
		Sessions->JoinSession(*Id, NAME_GameSession, this->Info->Result);
	}
}

void UWidgetMultiplayerSessionButton::OnSessionJoined(FName Name,
	EOnJoinSessionCompleteResult::Type Result)
{
	if (IsValid(this->Info) == false || this->Info->Result.IsValid() == false)
	{
		return;
	}

	auto Sessions = Online::GetSessionInterface(GetWorld());
	if (Sessions.IsValid() == false)
	{
		return;
	}

	Sessions->ClearOnJoinSessionCompleteDelegate_Handle(this->JoinSessionCompleteDelegateHandle);

	auto* PlayerController = GetWorld()->GetFirstPlayerController();
	if (Result == EOnJoinSessionCompleteResult::Success && IsValid(PlayerController))
	{
		UE_LOG(LogTemp, Warning, TEXT("=== JOIN SUCCESS"));
		auto Connection = FString();
		if (Sessions->GetResolvedConnectString(this->Info->Result, NAME_GamePort, Connection))
		{
			UE_LOG(LogTemp, Warning, TEXT("=== JOIN CONNECTION: %s"), *Connection);
			PlayerController->ClientTravel(Connection, TRAVEL_Absolute);
		}
	}
}
