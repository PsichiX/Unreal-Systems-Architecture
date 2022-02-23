#pragma once

#include "CoreMinimal.h"

#include "Blueprint/IUserObjectListEntry.h"
#include "Blueprint/UserWidget.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "OnlineSessionSettings.h"

#include "WidgetMultiplayerSessionButton.generated.h"

class UButton;
class UTextBlock;

UCLASS()
class MULTIPLAYER_API UWidgetMultiplayerSessionInfo : public UObject
{
	GENERATED_BODY()

public:
	FOnlineSessionSearchResult Result = {};
};

UCLASS()
class MULTIPLAYER_API UWidgetMultiplayerSessionButton : public UUserWidget,
														public IUserObjectListEntry
{
	GENERATED_BODY()

private:
	virtual void NativeConstruct() override;

	virtual void NativeDestruct() override;

	virtual void NativeOnListItemObjectSet(UObject* ListItemObject) override;

	UFUNCTION()
	void OnClicked();

	void OnSessionJoined(FName Name, EOnJoinSessionCompleteResult::Type Result);

	UPROPERTY(Meta = (BindWidget))
	UButton* Button = nullptr;

	UPROPERTY(Meta = (BindWidget))
	UTextBlock* Text = nullptr;

	UPROPERTY()
	UWidgetMultiplayerSessionInfo* Info = nullptr;

	FDelegateHandle JoinSessionCompleteDelegateHandle;
};
