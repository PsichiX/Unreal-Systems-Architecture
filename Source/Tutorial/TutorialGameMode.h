#pragma once

#include "CoreMinimal.h"

#include "GameFramework/GameModeBase.h"
#include "Systems/Public/SystemsGameMode.h"

#include "TutorialGameMode.generated.h"

UCLASS()
class TUTORIAL_API ATutorialGameMode : public ASystemsGameMode
{
	GENERATED_BODY()

private:
	virtual void BeginPlay() override;
};
