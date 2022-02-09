#pragma once

#include "CoreMinimal.h"

#include "Systems/Public/SystemsComponent.h"

#include "PersonalityComponent.generated.h"

UENUM(BlueprintType)
enum class EPersonalityKind : uint8
{
	Conscientiousness,
	Agreeableness,
	Neuroticism,
	Openness,
	Extraversion
};

USTRUCT(BlueprintType)
struct SYSTEMS_WORKSPACE_API FPersonalityTraits
{
	GENERATED_BODY()

public:
	FPersonalityTraits();

	FPersonalityTraits(float All);

	float Get(const EPersonalityKind Kind) const;

	UPROPERTY(EditAnywhere, Meta = (ClampMin = -1, ClampMax = 1))
	float Conscientiousness = 0;

	UPROPERTY(EditAnywhere, Meta = (ClampMin = -1, ClampMax = 1))
	float Agreeableness = 0;

	UPROPERTY(EditAnywhere, Meta = (ClampMin = -1, ClampMax = 1))
	float Neuroticism = 0;

	UPROPERTY(EditAnywhere, Meta = (ClampMin = -1, ClampMax = 1))
	float Openness = 0;

	UPROPERTY(EditAnywhere, Meta = (ClampMin = -1, ClampMax = 1))
	float Extraversion = 0;
};

UCLASS(BlueprintType, Blueprintable, Meta = (BlueprintSpawnableComponent))
class SYSTEMS_WORKSPACE_API UPersonalityComponent : public USystemsActorComponent
{
	GENERATED_BODY()

public:
	UPROPERTY()
	FPersonalityTraits Scales = FPersonalityTraits();

private:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly)
	FPersonalityTraits RandomMin = FPersonalityTraits(-1);

	UPROPERTY(EditDefaultsOnly)
	FPersonalityTraits RandomMax = FPersonalityTraits(1);
};
