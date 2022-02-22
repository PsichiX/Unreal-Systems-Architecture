#pragma once

#include "CoreMinimal.h"

#include "Blueprint/UserWidget.h"
#include "Engine/Level.h"

#include "WidgetGameSelectionMenu.generated.h"

class UListView;
class UButton;

USTRUCT(BLueprintType)
struct FGameItem
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	FText GameName = FText();

	UPROPERTY(EditAnywhere)
	FName MapName = FName();
};

UCLASS()
class SHARED_API UWidgetGameSelectionMenu : public UUserWidget
{
	GENERATED_BODY()

private:
	virtual void NativeConstruct() override;

	virtual void NativeDestruct() override;

	UFUNCTION()
	void OnExitGame();

	UPROPERTY(EditAnywhere)
	TArray<FGameItem> GamesList = {};

	UPROPERTY(Meta = (BindWidget))
	UListView* GamesListView = nullptr;

	UPROPERTY(Meta = (BindWidget))
	UButton* ExitGameButton = nullptr;
};
