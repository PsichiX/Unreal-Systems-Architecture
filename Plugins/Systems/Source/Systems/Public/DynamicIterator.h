#pragma once

#include "CoreMinimal.h"

#include "Systems/Public/DynamicIterator/DynamicAllEvent.h"
#include "Systems/Public/DynamicIterator/DynamicAnyEvent.h"
#include "Systems/Public/DynamicIterator/DynamicComparedByEvent.h"
#include "Systems/Public/DynamicIterator/DynamicFilterEvent.h"
#include "Systems/Public/DynamicIterator/DynamicFilterMapEvent.h"
#include "Systems/Public/DynamicIterator/DynamicFindEvent.h"
#include "Systems/Public/DynamicIterator/DynamicFindMapEvent.h"
#include "Systems/Public/DynamicIterator/DynamicForEachEvent.h"
#include "Systems/Public/DynamicIterator/DynamicInspectEvent.h"
#include "Systems/Public/DynamicIterator/DynamicMapEvent.h"

#include "DynamicIterator.generated.h"

UENUM(BlueprintType)
enum class EDynamicIteratorNextBranch : uint8
{
	Continue,
	End,
};

UENUM(BlueprintType)
enum class EDynamicIteratorStatusBranch : uint8
{
	Success,
	Failure,
};

UCLASS(BlueprintType)
class SYSTEMS_API UDynamicIterator : public UObject
{
	GENERATED_BODY()

public:
	/// Performs iteration and returns yielded value.
	///
	/// Null means no object left to iterate.
	UFUNCTION(BlueprintCallable, Category = Systems, Meta = (DevelopmentOnly))
	virtual UObject* Next()
	{
		//// [ignore]
		return nullptr;
		//// [/ignore]
	}

	/// Handy wrapper for [`class: Self::Next`]() for use in blueprints, where iteration can branch to `yielded` and
	/// `completed` execution nodes.
	UFUNCTION(BlueprintCallable, Category = Systems, Meta = (ExpandEnumAsExecs = "Branches", DevelopmentOnly))
	UObject* NextBranched(EDynamicIteratorNextBranch& Branches);

	/// Calculates (usually minimal) number of items this iterator stage can yield.
	UFUNCTION(BlueprintCallable, Category = Systems, Meta = (DevelopmentOnly))
	virtual int EstimateSizeLeft() const
	{
		//// [ignore]
		return 0;
		//// [/ignore]
	}

	UFUNCTION(BlueprintCallable,
		Category = "Systems|Iterator|Collector",
		Meta = (DeterminesOutputType = "Type", DynamicOutputParam = "Result", DevelopmentOnly))
	void CollectArray(TSubclassOf<UObject> Type, TArray<UObject*>& Result);

	UFUNCTION(BlueprintCallable,
		Category = "Systems|Iterator|Collector",
		Meta = (DeterminesOutputType = "Type", DynamicOutputParam = "Result", DevelopmentOnly))
	void CollectSet(TSubclassOf<UObject> Type, TSet<UObject*>& Result);

	UFUNCTION(BlueprintCallable, Category = "Systems|Iterator|Consumer", Meta = (DevelopmentOnly))
	int Count();

	UFUNCTION(BlueprintCallable,
		Category = "Systems|Iterator|Consumer",
		Meta = (ExpandEnumAsExecs = "Branches",
			DeterminesOutputType = "Type",
			DynamicOutputParam = "Result",
			DevelopmentOnly))
	void Nth(TSubclassOf<UObject> Type, int Index, UObject*& Result, EDynamicIteratorStatusBranch& Branches);

	// UFUNCTION(BlueprintCallable,
	// 	Category = "Systems|Iterator|Consumer",
	// 	Meta = (ExpandEnumAsExecs = "Branches",
	// 		DeterminesOutputType = "Type",
	// 		DynamicOutputParam = "Result",
	// 		DevelopmentOnly))
	// void Fold(TSubclassOf<UObject> Type,
	// 	UObject* StartValue,
	// 	FDynamicFoldEvent OnFold,
	// 	UObject*& Result,
	// 	EDynamicIteratorStatusBranch& Branches);

	UFUNCTION(BlueprintCallable,
		Category = "Systems|Iterator|Consumer",
		Meta = (ExpandEnumAsExecs = "Branches",
			DeterminesOutputType = "Type",
			DynamicOutputParam = "Result",
			DevelopmentOnly))
	void Find(TSubclassOf<UObject> Type,
		FDynamicFindEvent OnFind,
		UObject*& Result,
		EDynamicIteratorStatusBranch& Branches);

	UFUNCTION(BlueprintCallable,
		Category = "Systems|Iterator|Consumer",
		Meta = (ExpandEnumAsExecs = "Branches",
			DeterminesOutputType = "Type",
			DynamicOutputParam = "Result",
			DevelopmentOnly))
	void FindMap(TSubclassOf<UObject> Type,
		FDynamicFindMapEvent OnFind,
		UObject*& Result,
		EDynamicIteratorStatusBranch& Branches);

	UFUNCTION(BlueprintCallable,
		Category = "Systems|Iterator|Consumer",
		Meta = (ExpandEnumAsExecs = "Branches",
			DeterminesOutputType = "Type",
			DynamicOutputParam = "Result",
			DevelopmentOnly))
	void First(TSubclassOf<UObject> Type, UObject*& Result, EDynamicIteratorStatusBranch& Branches);

	UFUNCTION(BlueprintCallable,
		Category = "Systems|Iterator|Consumer",
		Meta = (ExpandEnumAsExecs = "Branches",
			DeterminesOutputType = "Type",
			DynamicOutputParam = "Result",
			DevelopmentOnly))
	void Last(TSubclassOf<UObject> Type, UObject*& Result, EDynamicIteratorStatusBranch& Branches);

	UFUNCTION(BlueprintCallable, Category = "Systems|Iterator|Consumer", Meta = (DevelopmentOnly))
	void ForEach(FDynamicForEachEvent OnForEach);

	UFUNCTION(BlueprintCallable, Category = "Systems|Iterator|Consumer", Meta = (DevelopmentOnly))
	bool Any(FDynamicAnyEvent OnAny);

	UFUNCTION(BlueprintCallable, Category = "Systems|Iterator|Consumer", Meta = (DevelopmentOnly))
	bool All(FDynamicAllEvent OnAll);

	UFUNCTION(BlueprintCallable,
		Category = "Systems|Iterator|Consumer",
		Meta = (ExpandEnumAsExecs = "Branches",
			DeterminesOutputType = "Type",
			DynamicOutputParam = "Result",
			DevelopmentOnly))
	void ComparedBy(TSubclassOf<UObject> Type,
		FDynamicComparedByEvent OnComparedBy,
		UObject*& Result,
		EDynamicIteratorStatusBranch& Branches);

	UFUNCTION(BlueprintCallable, Category = "Systems|Iterator|Converter", Meta = (DevelopmentOnly))
	UDynamicChain* Chain(UDynamicIterator* Other);

	UFUNCTION(BlueprintCallable, Category = "Systems|Iterator|Converter", Meta = (DevelopmentOnly))
	UDynamicFilter* Filter(FDynamicFilterEvent OnFilter);

	UFUNCTION(BlueprintCallable, Category = "Systems|Iterator|Converter", Meta = (DevelopmentOnly))
	UDynamicMap* Map(FDynamicMapEvent OnMap);

	UFUNCTION(BlueprintCallable, Category = "Systems|Iterator|Converter", Meta = (DevelopmentOnly))
	UDynamicFilterMap* FilterMap(FDynamicFilterMapEvent OnFilterMap);

	UFUNCTION(BlueprintCallable, Category = "Systems|Iterator|Converter", Meta = (DevelopmentOnly))
	UDynamicInspect* Inspect(FDynamicInspectEvent OnInspect);

	UFUNCTION(BlueprintCallable, Category = "Systems|Iterator|Converter", Meta = (DevelopmentOnly))
	UDynamicSkip* Skip(int Count);

	UFUNCTION(BlueprintCallable, Category = "Systems|Iterator|Converter", Meta = (DevelopmentOnly))
	UDynamicTake* Take(int Count);

	UFUNCTION(BlueprintCallable, Category = "Systems|Iterator|Converter", Meta = (DevelopmentOnly))
	UDynamicEnumerate* Enumerate();

	UFUNCTION(BlueprintCallable, Category = "Systems|Iterator|Converter", Meta = (DevelopmentOnly))
	UDynamicZip* Zip(UDynamicIterator* Other);
};
