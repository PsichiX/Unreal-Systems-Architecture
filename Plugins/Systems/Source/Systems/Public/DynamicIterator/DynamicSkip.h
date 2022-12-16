#pragma once

#include "CoreMinimal.h"

#include "Systems/Public/DynamicIterator.h"

#include "DynamicSkip.generated.h"

UCLASS(BlueprintType)
class SYSTEMS_API UDynamicSkip : public UDynamicIterator
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
