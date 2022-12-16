#pragma once

#include "CoreMinimal.h"

#include "Systems/Public/DynamicIterator.h"

#include "DynamicChain.generated.h"

UCLASS(BlueprintType)
class SYSTEMS_API UDynamicChain : public UDynamicIterator
{
	GENERATED_BODY()

public:
	virtual UObject* Next() override;

	virtual int EstimateSizeLeft() const override;

	UPROPERTY(BlueprintReadWrite)
	UDynamicIterator* InnerA = nullptr;

	UPROPERTY(BlueprintReadWrite)
	UDynamicIterator* InnerB = nullptr;
};
