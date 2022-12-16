#pragma once

#include "CoreMinimal.h"

#include "Systems/Public/DynamicIterator.h"

#include "DynamicEnumerate.generated.h"

UCLASS(BlueprintType)
class SYSTEMS_API UDynamicEnumerateValue : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite)
	int Index = 0;

	UPROPERTY(BlueprintReadWrite)
	UObject* Value = nullptr;
};

UCLASS(BlueprintType)
class SYSTEMS_API UDynamicEnumerate : public UDynamicIterator
{
	GENERATED_BODY()

public:
	virtual UObject* Next() override;

	virtual int EstimateSizeLeft() const override;

	UPROPERTY(BlueprintReadWrite)
	UDynamicIterator* Inner = nullptr;

private:
	int Index = 0;
};
