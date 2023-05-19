#include "Multiplayer/UI/WidgetMultiplayerLobby.h"

#include "Components/Button.h"
#include "Components/ListView.h"
#include "Components/WidgetSwitcher.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "Kismet/GameplayStatics.h"
#include "Online/OnlineSessionNames.h"
#include "OnlineSubsystemUtils.h"
#include "Systems/Public/Iterator.h"

#include "Multiplayer/Constants.h"
#include "Multiplayer/UI/WidgetMultiplayerSessionButton.h"

void UWidgetMultiplayerLobby::NativeConstruct()
{
	Super::NativeConstruct();

	if (IsValid(this->HostSessionButton))
	{
		this->HostSessionButton->OnClicked.AddUniqueDynamic(this, &ThisClass::OnHostSession);
	}

	if (IsValid(this->JoinSessionButton))
	{
		this->JoinSessionButton->OnClicked.AddUniqueDynamic(this, &ThisClass::OnJoinSession);
	}

	if (IsValid(this->CreateSessionButton))
	{
		this->CreateSessionButton->OnClicked.AddUniqueDynamic(this, &ThisClass::OnCreateSession);
	}
}

void UWidgetMultiplayerLobby::NativeDestruct()
{
	Super::NativeDestruct();

	if (IsValid(this->HostSessionButton))
	{
		this->HostSessionButton->OnClicked.RemoveAll(this);
	}

	if (IsValid(this->JoinSessionButton))
	{
		this->JoinSessionButton->OnClicked.RemoveAll(this);
	}

	if (IsValid(this->CreateSessionButton))
	{
		this->CreateSessionButton->OnClicked.RemoveAll(this);
	}
}

void UWidgetMultiplayerLobby::OnHostSession()
{
	if (IsValid(this->ModeSwitcher) == false)
	{
		return;
	}

	this->ModeSwitcher->SetActiveWidgetIndex(0);

	auto Sessions = Online::GetSessionInterface(GetWorld());
	if (Sessions.IsValid())
	{
		Sessions->CancelFindSessions();
	}
}

void UWidgetMultiplayerLobby::OnJoinSession()
{
	if (this->bCreateSessionInProgress || IsValid(this->ModeSwitcher) == false ||
		IsValid(this->SessionsListView) == false || IsValid(this->FindModeSwitcher) == false)
	{
		return;
	}

	this->ModeSwitcher->SetActiveWidgetIndex(1);
	this->SessionsListView->ClearListItems();
	this->SessionSearch = MakeShared<FOnlineSessionSearch>(FOnlineSessionSearch());

	const auto* LocalPlayer = GetWorld()->GetFirstLocalPlayerFromController();
	auto Sessions = Online::GetSessionInterface(GetWorld());
	if (Sessions.IsValid() && IsValid(LocalPlayer))
	{
		this->FindModeSwitcher->SetActiveWidgetIndex(0);
		Sessions->CancelFindSessions();
		Sessions->OnFindSessionsCompleteDelegates.AddUObject(this, &ThisClass::OnSessionsFound);

		const auto Id = LocalPlayer->GetPreferredUniqueNetId();
		Sessions->FindSessions(*Id, this->SessionSearch.ToSharedRef());
	}
}

void UWidgetMultiplayerLobby::OnCreateSession()
{
	if (this->bCreateSessionInProgress || IsValid(this->CreateSessionButton) == false)
	{
		return;
	}

	const auto* LocalPlayer = GetWorld()->GetFirstLocalPlayerFromController();
	auto Sessions = Online::GetSessionInterface(GetWorld());
	if (Sessions.IsValid() && IsValid(LocalPlayer))
	{
		Sessions->CancelFindSessions();
		Sessions->OnCreateSessionCompleteDelegates.AddUObject(this, &ThisClass::OnSessionCreated);

		auto Settings = FOnlineSessionSettings();
		Settings.bAllowInvites = true;
		Settings.bAllowJoinInProgress = true;
		Settings.bAllowJoinViaPresence = true;
		Settings.bIsDedicated = false;
		Settings.bIsLANMatch = true;
		Settings.bShouldAdvertise = true;
		Settings.bUsesPresence = true;
		Settings.NumPublicConnections = 2;
		Settings.Set(
			SETTING_MAPNAME, LEVEL_NAME.ToString(), EOnlineDataAdvertisementType::ViaOnlineService);

		const auto Id = LocalPlayer->GetPreferredUniqueNetId();
		this->bCreateSessionInProgress = Sessions->CreateSession(*Id, NAME_GameSession, Settings);
	}
}

void UWidgetMultiplayerLobby::OnSessionCreated(FName Name, bool bSuccess)
{
	this->bCreateSessionInProgress = false;

	auto Sessions = Online::GetSessionInterface(GetWorld());
	if (Sessions.IsValid())
	{
		Sessions->OnCreateSessionCompleteDelegates.RemoveAll(this);

		if (bSuccess)
		{
			UE_LOG(LogTemp, Warning, TEXT("=== SESSION CREATED"));
			Sessions->OnStartSessionCompleteDelegates.AddUObject(
				this, &ThisClass::OnSessionStarted);

			Sessions->StartSession(NAME_GameSession);
		}
	}
}

void UWidgetMultiplayerLobby::OnSessionStarted(FName Name, bool bSuccess)
{
	auto Sessions = Online::GetSessionInterface(GetWorld());
	if (Sessions.IsValid())
	{
		Sessions->OnStartSessionCompleteDelegates.RemoveAll(this);

		if (bSuccess)
		{
			UE_LOG(LogTemp, Warning, TEXT("=== SESSION STARTED"));

			UGameplayStatics::OpenLevel(GetWorld(), LEVEL_NAME, true, "listen");
		}
	}
}

void UWidgetMultiplayerLobby::OnSessionsFound(bool bSuccess)
{
	if (IsValid(this->FindModeSwitcher))
	{
		this->FindModeSwitcher->SetActiveWidgetIndex(1);
	}

	auto Sessions = Online::GetSessionInterface(GetWorld());
	if (Sessions.IsValid())
	{
		Sessions->OnFindSessionsCompleteDelegates.RemoveAll(this);
	}

	if (bSuccess && this->SessionSearch.IsValid() && IsValid(this->SessionsListView))
	{
		const auto Items = IterStdConst(this->SessionSearch->SearchResults)
							   .FilterMap<UObject*>(
								   [&](const auto& Result)
								   {
									   if (Result.IsValid())
									   {
										   auto* Info = NewObject<UWidgetMultiplayerSessionInfo>(
											   this, UWidgetMultiplayerSessionInfo::StaticClass());
										   Info->Result = Result;

										   return TOptional<UObject*>(Info);
									   }
									   else
									   {
										   return TOptional<UObject*>();
									   }
								   })
							   .CollectArray();

		this->SessionsListView->SetListItems(Items);
		this->SessionSearch.Reset();
	}
}