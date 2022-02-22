#pragma once

#include "CoreMinimal.h"

#include "Blueprint/IUserObjectListEntry.h"
#include "Blueprint/UserWidget.h"

#include "Life/Resources/LifeSpawner.h"

#include "WidgetSpawnerIconButton.generated.h"

class UButton;
class UImage;
class UBorder;
class USystemsWorld;

UCLASS()
class LIFE_API UWidgetSpawnerIconButton : public UUserWidget,
													   public IUserObjectListEntry
{
	GENERATED_BODY()

private:
	virtual void NativeConstruct() override;

	virtual void NativeDestruct() override;

	virtual void NativeOnListItemObjectSet(UObject* ListItemObject) override;

	void OnLifeSpawnerChanged(USystemsWorld& Systems);

	UFUNCTION()
	void OnClicked();

	UPROPERTY(EditAnywhere)
	UClass* Spawnable = nullptr;

	UPROPERTY(Meta = (BindWidget))
	UButton* Button = nullptr;

	UPROPERTY(Meta = (BindWidget))
	UImage* Icon = nullptr;

	UPROPERTY(Meta = (BindWidget))
	UBorder* Border = nullptr;
};
