#pragma once

#include "CoreMinimal.h"

#include "Kismet/BlueprintFunctionLibrary.h"

#include "SystemsChangeDetectionBlueprintLibrary.generated.h"

class USystemsWorld;
struct FSystemsChangeSignature;

UCLASS()
class REACTIVESYSTEMS_API USystemsChangeDetectionBlueprintLibrary
	: public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure,
		Meta = (AutoCreateRefTerm = "Components,Resources"))
	static FSystemsChangeSignature NewChangeSignature(
		const USystemsWorld* Systems,
		const TArray<UClass*>& Components,
		const TArray<UClass*>& Resources);
};