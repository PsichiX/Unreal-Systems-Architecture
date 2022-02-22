#pragma once

#include "CoreMinimal.h"

#include "Systems/Public/SystemsComponent.h"

#include "SpeciesComponent.generated.h"

UENUM(Meta = (Bitflags, UseEnumValuesAsMaskValuesInEditor = "true"))
enum class ESpeciesKind : uint32
{
	None = 0,
	Human = 1 << 0,
	Chicken = 1 << 1,
	Goose = 1 << 2,
	Turtle = 1 << 3,
	Sheep = 1 << 4,
	Cow = 1 << 5,
	Animal = 0b111110 UMETA(Hidden),
	ConiferTree = 1 << 6,
	SmallTree = 1 << 7,
	RegularTree = 1 << 8,
	StrongTree = 1 << 9,
	Tree = 0b1111000000 UMETA(Hidden),
};

UCLASS(BlueprintType, Blueprintable, Meta = (BlueprintSpawnableComponent))
class LIFE_API USpeciesComponent : public USystemsActorComponent
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	ESpeciesKind Kind = ESpeciesKind::None;
};
