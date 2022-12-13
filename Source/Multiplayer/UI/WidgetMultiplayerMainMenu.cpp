#include "Multiplayer/UI/WidgetMultiplayerMainMenu.h"

#include "Components/Button.h"
#include "Components/EditableTextBox.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"

#include "Multiplayer/Constants.h"

void UWidgetMultiplayerMainMenu::NativeConstruct()
{
	Super::NativeConstruct();

	if (IsValid(this->HostGameButton))
	{
		this->HostGameButton->OnClicked.AddUniqueDynamic(this, &ThisClass::OnHostGame);
	}

	if (IsValid(this->JoinGameButton))
	{
		this->JoinGameButton->OnClicked.AddUniqueDynamic(this, &ThisClass::OnJoinGame);
	}

	if (IsValid(this->ExitGameButton))
	{
		this->ExitGameButton->OnClicked.AddUniqueDynamic(this, &ThisClass::OnExitGame);
	}
}

void UWidgetMultiplayerMainMenu::NativeDestruct()
{
	Super::NativeDestruct();

	if (IsValid(this->HostGameButton))
	{
		this->HostGameButton->OnClicked.RemoveAll(this);
	}

	if (IsValid(this->JoinGameButton))
	{
		this->JoinGameButton->OnClicked.RemoveAll(this);
	}

	if (IsValid(this->ExitGameButton))
	{
		this->ExitGameButton->OnClicked.RemoveAll(this);
	}
}

void UWidgetMultiplayerMainMenu::OnHostGame()
{
	UGameplayStatics::OpenLevel(GetWorld(), LEVEL_NAME, true, "listen");
}

void UWidgetMultiplayerMainMenu::OnJoinGame()
{
	if (IsValid(this->ServerAddressTextBox))
	{
		const auto ServerAddress = this->ServerAddressTextBox->GetText().ToString();
		UGameplayStatics::OpenLevel(GetWorld(), FName(ServerAddress));
	}
}

void UWidgetMultiplayerMainMenu::OnExitGame()
{
	if (this->ExitToAnotherMap.IsNone())
	{
		UKismetSystemLibrary::QuitGame(GetWorld(), nullptr, EQuitPreference::Type::Quit, false);
	}
	else
	{
		UGameplayStatics::OpenLevel(GetWorld(), ExitToAnotherMap);
	}
}
