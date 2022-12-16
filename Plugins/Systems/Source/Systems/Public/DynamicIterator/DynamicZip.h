#pragma once

#include "CoreMinimal.h"

#include "Systems/Public/DynamicIterator.h"

#include "DynamicZip.generated.h"

UCLASS(BlueprintType)
class SYSTEMS_API UDynamicZipValue : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite)
	UObject* First = nullptr;

	UPROPERTY(BlueprintReadWrite)
	UObject* Second = nullptr;
};

UCLASS(BlueprintType)
class SYSTEMS_API UDynamicZip : public UDynamicIterator
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
