#pragma once

#include "CoreMinimal.h"

#include "Blueprint/UserWidget.h"
#include "OnlineSessionSettings.h"

#include "WidgetMultiplayerLobby.generated.h"

class UButton;
class UEditableTextBox;
class UWidgetSwitcher;
class UListView;

/// NOTE:
/// Best practice is to put session related functionality into a separate subsystem - we will do it
/// in some later update, for now we only care about doing as less as possible, no need for fancy
/// abstractions at this point.
UCLASS()
class MULTIPLAYER_API UWidgetMultiplayerLobby : public UUserWidget
{
	GENERATED_BODY()

private:
	virtual void NativeConstruct() override;

	virtual void NativeDestruct() override;

	UFUNCTION()
	void OnHostSession();

	UFUNCTION()
	void OnJoinSession();

	UFUNCTION()
	void OnCreateSession();

	UFUNCTION()
	void OnSessionCreated(FName Name, bool bSuccess);

	UFUNCTION()
	void OnSessionStarted(FName Name, bool bSuccess);

	UFUNCTION()
	void OnSessionsFound(bool bSuccess);

	UPROPERTY(Meta = (BindWidget))
	UButton* HostSessionButton = nullptr;

	UPROPERTY(Meta = (BindWidget))
	UButton* JoinSessionButton = nullptr;

	UPROPERTY(Meta = (BindWidget))
	UWidgetSwitcher* ModeSwitcher = nullptr;

	UPROPERTY(Meta = (BindWidget))
	UEditableTextBox* SessionNameTextBox = nullptr;

	UPROPERTY(Meta = (BindWidget))
	UButton* CreateSessionButton = nullptr;

	UPROPERTY(Meta = (BindWidget))
	UWidgetSwitcher* FindModeSwitcher = nullptr;

	UPROPERTY(Meta = (BindWidget))
	UListView* SessionsListView = nullptr;

	UPROPERTY()
	bool bCreateSessionInProgress = false;

	TSharedPtr<FOnlineSessionSearch> SessionSearch = {};
};
