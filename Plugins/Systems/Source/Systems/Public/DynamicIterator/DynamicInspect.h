#pragma once

#include "CoreMinimal.h"

#include "Systems/Public/DynamicIterator.h"
#include "Systems/Public/DynamicIterator/DynamicInspectEvent.h"

#include "DynamicInspect.generated.h"

UCLASS(BlueprintType)
class SYSTEMS_API UDynamicInspect : public UDynamicIterator
{
	GENERATED_BODY()

public:
	virtual UObject* Next() override;

	virtual int EstimateSizeLeft() const override;

	UPROPERTY(BlueprintReadWrite)
	UDynamicIterator* Inner = nullptr;

	UPROPERTY(BlueprintReadWrite)
	FDynamicInspectEvent Event = {};
};
