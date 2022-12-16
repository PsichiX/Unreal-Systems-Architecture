#pragma once

#include "CoreMinimal.h"

#include "Systems/Public/DynamicIterator.h"

#include "DynamicTake.generated.h"

UCLASS(BlueprintType)
class SYSTEMS_API UDynamicTake : public UDynamicIterator
{
	GENERATED_BODY()

public:
	virtual UObject* Next() override;

	virtual int EstimateSizeLeft() const override;

	UPROPERTY(BlueprintReadWrite)
	UDynamicIterator* Inner = nullptr;

	UPROPERTY(BlueprintReadWrite)
	int Num = 0;
};
