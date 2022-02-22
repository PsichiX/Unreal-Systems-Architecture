#pragma once

#include "CoreMinimal.h"

#include "Blueprint/UserWidget.h"

#include "WidgetMultiplayerMainMenu.generated.h"

class UButton;
class UEditableTextBox;

UCLASS()
class MULTIPLAYER_API UWidgetMultiplayerMainMenu : public UUserWidget
{
	GENERATED_BODY()

private:
	virtual void NativeConstruct() override;

	virtual void NativeDestruct() override;

	UFUNCTION()
	void OnHostGame();

	UFUNCTION()
	void OnJoinGame();

	UFUNCTION()
	void OnExitGame();

	UPROPERTY(EditAnywhere)
	FName ExitToAnotherMap = FName();

	UPROPERTY(Meta = (BindWidget))
	UButton* HostGameButton = nullptr;

	UPROPERTY(Meta = (BindWidget))
	UButton* JoinGameButton = nullptr;

	UPROPERTY(Meta = (BindWidget))
	UEditableTextBox* ServerAddressTextBox = nullptr;

	UPROPERTY(Meta = (BindWidget))
	UButton* ExitGameButton = nullptr;
};
