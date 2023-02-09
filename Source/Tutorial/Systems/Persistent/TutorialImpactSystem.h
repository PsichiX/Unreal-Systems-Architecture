#pragma once

#include "CoreMinimal.h"

#include "Systems/Public/System.h"

#include "TutorialImpactSystem.generated.h"

class USystemsWorld;

UCLASS(BlueprintType)
class TUTORIAL_API UTutorialImpactSystem : public USystem
{
	GENERATED_BODY()

public:
	virtual void Run(USystemsWorld& Systems) override;

private:
	static void ClearImpact(USystemsWorld& Systems);

	void ApplyImpact(double Scale,
		const APlayerController* PlayerController,
		USystemsWorld& Systems) const;

	UPROPERTY(EditAnywhere)
	FName ImpactId = FName(TEXT("Impact"));

	UPROPERTY(EditAnywhere)
	double ImpactValue = 1000.0;
};