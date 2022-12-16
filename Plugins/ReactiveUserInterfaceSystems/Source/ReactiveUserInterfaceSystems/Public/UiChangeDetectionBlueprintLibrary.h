#pragma once

#include "CoreMinimal.h"

#include "Kismet/BlueprintFunctionLibrary.h"

#include "UiChangeDetectionBlueprintLibrary.generated.h"

class USystemsWorld;
struct FUiChangeSignature;

UCLASS()
class REACTIVEUSERINTERFACESYSTEMS_API UUiChangeDetectionBlueprintLibrary
	: public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure,
		Meta = (AutoCreateRefTerm = "Components,Resources"))
	static FUiChangeSignature NewUiChangeSignature(const USystemsWorld* Systems,
		const TArray<UClass*>& Components,
		const TArray<UClass*>& Resources);
};