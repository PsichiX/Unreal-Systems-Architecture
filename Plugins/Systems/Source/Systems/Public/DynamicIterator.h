#pragma once

#include "CoreMinimal.h"

#include "DynamicIterator.generated.h"

UENUM(BlueprintType)
enum class EDynamicQueryBranch : uint8
{
	Continue,
	End,
};

UCLASS(BlueprintType)
class SYSTEMS_API UDynamicIterator : public UObject
{
	GENERATED_BODY()

public:
	/// Performs iteration and returns yielded value.
	///
	/// Null means no object left to iterate.
	UFUNCTION(BlueprintCallable, Category = Systems)
	virtual UObject* Next()
	{
		//// [ignore]
		return nullptr;
		//// [/ignore]
	}

	/// Handy wrapper for [`class: Self::Next`]() for use in blueprints, where iteration can branch to `yielded` and
	/// `completed` execution nodes.
	UFUNCTION(BlueprintCallable, Category = Systems, Meta = (ExpandEnumAsExecs = "Branches"))
	UObject* NextBranched(EDynamicQueryBranch& Branches);

	/// Calculates (usually minimal) number of items this iterator stage can yield.
	UFUNCTION(BlueprintCallable, Category = Systems)
	virtual int EstimateSizeLeft() const
	{
		//// [ignore]
		return 0;
		//// [/ignore]
	}
};
