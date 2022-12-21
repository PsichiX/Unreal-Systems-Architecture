#pragma once

#include "CoreMinimal.h"

#include "Systems/Public/DynamicIterator.h"
#include "Systems/Public/DynamicIterator/DynamicMapEvent.h"

#include "DynamicMap.generated.h"

UCLASS(BlueprintType)
class SYSTEMS_API UDynamicMap : public UDynamicIterator
{
	GENERATED_BODY()

public:
	virtual UObject* Next() override;

	virtual int EstimateSizeLeft() const override;

	UPROPERTY(BlueprintReadWrite)
	UDynamicIterator* Inner = nullptr;

	UPROPERTY(BlueprintReadWrite)
	FDynamicMapEvent Event = {};
};