#pragma once

#include "CoreMinimal.h"

/// Returns exact number of items that iterator can yield.
///
/// # Example
/// ```snippet
/// iter_count
/// ```
//// [proxy: iter_count, iter, iter_consumers_base, iter_consumers]
//// uint32 Count();
//// [/proxy]
#define ITER_IMPL_COUNT                                \
	uint32 Count()                                     \
	{                                                  \
		return ::IterCount(MoveTempIfPossible(*this)); \
	}

/// Returns Nth item in the iterator.
///
/// # Example
/// ```snippet
/// iter_nth
/// ```
//// [proxy: iter_nth, iter, iter_consumers_base, iter_consumers]
//// TOptional<Item> Nth(uint32 Index);
//// [/proxy]
#define ITER_IMPL_NTH                                       \
	TOptional<Item> Nth(uint32 Index)                       \
	{                                                       \
		return ::IterNth(MoveTempIfPossible(*this), Index); \
	}

/// Folds iterator into single value.
///
/// Folding basically means going through all iterator items and collapsing them
/// into single value. Example of folding can be sum/accumulation, or min/max,
/// or anything like that - although for ones mentioned there are existing
/// optimized iterator consumers: [`struct: $Self$::Sum`]() and
/// [`struct: $Self$::ComparedBy`]().
///
/// # Note
/// > `RESULT` is the returned type, same as `Start` argument used as initial
/// accumulator. `FUNCTOR` should comply to given function signature: `Item(Item
/// Accumulator, I::Item Value)` where `Accumulator` argument holds result of
/// previous iteration, and `Value` holds curren value yielded by iterator.
/// `Start` argument holds value passed to first iteratio `Accumulator`.
///
/// # Example
/// ```snippet
/// iter_fold
/// ```
//// [proxy: iter_fold, iter, iter_consumers_base, iter_consumers]
//// template <typename FUNCTOR>
//// Item Fold(Item Start, FUNCTOR Func);
//// [/proxy]
#define ITER_IMPL_FOLD(FUNCTOR)                                    \
	template <typename RESULT, typename FUNCTOR>                   \
	RESULT Fold(RESULT Start, FUNCTOR Func)                        \
	{                                                              \
		return ::IterFold(MoveTempIfPossible(*this), Start, Func); \
	}

/// Returns sum of values that iterator can yield.
///
/// # Note
/// > Make sure that type of iterator values actually implement `operator+`!
/// Also since iterator can work on non-numerical types, user has to provide
/// initial value, tha makes it work like [`struct: $Self$::Fold`]().
///
/// # Example
/// ```snippet
/// iter_sum
/// ```
/// This is equivalent of:
/// ```snippet
/// iter_fold
/// ```
//// [proxy: iter_sum, iter, iter_consumers]
//// Item Sum(Item InitialValue);
//// [/proxy]
#define ITER_IMPL_SUM                                              \
	Item Sum(Item InitialValue)                                    \
	{                                                              \
		return ::IterSum(MoveTempIfPossible(*this), InitialValue); \
	}

/// Finds and returns value in this iterator that passes `FUNCTOR` predicate.
///
/// # Note
/// > `FUNCTOR` should comply to given function signature: `bool(I::Item
/// Value)` where `Value` holds current value yielded by iterator.
///
/// # Example
/// ```snippet
/// iter_find
/// ```
//// [proxy: iter_find, iter, iter_consumers_base, iter_consumers]
//// template <typename FUNCTOR>
//// TOptional<Item> Find(FUNCTOR Func);
//// [/proxy]
#define ITER_IMPL_FIND(FUNCTOR)                             \
	template <typename FUNCTOR>                             \
	TOptional<Item> Find(FUNCTOR Func)                      \
	{                                                       \
		return ::IterFind(MoveTempIfPossible(*this), Func); \
	}

/// Finds and returns value in this iterator that passes `FUNCTOR` predicate,
/// mapped to another type.
///
/// # Note
/// > `FUNCTOR` should comply to given function signature:
/// `TOptional<Item>(I::Item Value)` where `Value` holds current value
/// yielded by iterator. `RESULT` is the returned type and use should always
/// return `TOptional<RESULT>` in the predicate where some value means "found"
/// an none means "not found".
///
/// # Example
/// ```snippet
/// iter_find_map
/// ```
//// [proxy: iter_find_map, iter, iter_consumers_base, iter_consumers]
//// template <typename RESULT, typename FUNCTOR>
//// TOptional<RESULT> FindMap(FUNCTOR Func);
//// [/proxy]
#define ITER_IMPL_FIND_MAP(RESULT, FUNCTOR)                            \
	template <typename RESULT, typename FUNCTOR>                       \
	TOptional<RESULT> FindMap(FUNCTOR Func)                            \
	{                                                                  \
		return ::IterFindMap<RESULT>(MoveTempIfPossible(*this), Func); \
	}

/// Returns first item in the iterator.
///
/// This is an equivalent of calling [`struct: $Self$::Next`]().
///
/// # Example
/// ```snippet
/// iter_first
/// ```
//// [proxy: iter_first, iter, iter_consumers_base, iter_consumers]
//// TOptional<Item> First();
//// [/proxy]
#define ITER_IMPL_FIRST                                \
	TOptional<Item> First()                            \
	{                                                  \
		return ::IterFirst(MoveTempIfPossible(*this)); \
	}

/// Returns last item in the iterator.
///
/// # Example
/// ```snippet
/// iter_last
/// ```
//// [proxy: iter_last, iter, iter_consumers_base, iter_consumers]
//// TOptional<Item> Last();
//// [/proxy]
#define ITER_IMPL_LAST                                \
	TOptional<Item> Last()                            \
	{                                                 \
		return ::IterLast(MoveTempIfPossible(*this)); \
	}

/// Consumes iterator and yields its values for user to process.
///
/// This is equivalent of:
/// ```snippet
/// iter_while
/// ```
///
/// # Note
/// > `FUNCTOR` should comply to given function signature: `void(I::Item
/// Value)` where `Value` holds current value yielded by iterator.
///
/// # Example
/// ```snippet
/// iter_for_each
/// ```
//// [proxy: iter_for_each, iter, iter_consumers_base, iter_consumers]
//// template <typename FUNCTOR>
//// void ForEach(FUNCTOR Func);
//// [/proxy]
#define ITER_IMPL_FOR_EACH(FUNCTOR)                     \
	template <typename FUNCTOR>                         \
	void ForEach(FUNCTOR Func)                          \
	{                                                   \
		::IterForEach(MoveTempIfPossible(*this), Func); \
	}

/// Checks if any value yielded by this iterator passes predicate.
///
/// # Note
/// > `FUNCTOR` should comply to given function signature: `bool(I::Item
/// Value)` where `Value` holds current value yielded by iterator.
///
/// # Example
/// ```snippet
/// iter_any
/// ```
//// [proxy: iter_any, iter, iter_consumers_base, iter_consumers]
//// template <typename FUNCTOR>
//// bool Any(FUNCTOR Func);
//// [/proxy]
#define ITER_IMPL_ANY(FUNCTOR)                             \
	template <typename FUNCTOR>                            \
	bool Any(FUNCTOR Func)                                 \
	{                                                      \
		return ::IterAny(MoveTempIfPossible(*this), Func); \
	}

/// Checks if all values yielded by this iterator passes predicate.
///
/// # Note
/// > `FUNCTOR` should comply to given function signature: `bool(I::Item
/// Value)` where `Value` holds current value yielded by iterator.
///
/// # Example
/// ```snippet
/// iter_all
/// ```
//// [proxy: iter_all, iter, iter_consumers_base, iter_consumers]
//// template <typename FUNCTOR>
//// bool All(FUNCTOR Func);
//// [/proxy]
#define ITER_IMPL_ALL(FUNCTOR)                             \
	template <typename FUNCTOR>                            \
	bool All(FUNCTOR Func)                                 \
	{                                                      \
		return ::IterAll(MoveTempIfPossible(*this), Func); \
	}

/// Finds iterator value that compared to other items gets greater "score".
///
/// Headline is rather vague, but what it actually does is user can do finding
/// min/max value with this iterator consumer.
///
/// # Note
/// > `FUNCTOR` should comply to given function signature: `bool(I::Item A,
/// I::Item B)` where `A` holds current value yielded by iterator and `B` is
/// the one that has best "score" s far.
///
/// # Example
/// ```snippet
/// iter_all
/// ```
//// [proxy: iter_compared_by, iter, iter_consumers_base, iter_consumers]
//// template <typename FUNCTOR>
//// TOptional<Item> ComparedBy(FUNCTOR Func);
//// [/proxy]
#define ITER_IMPL_COMPARED_BY(FUNCTOR)                            \
	template <typename FUNCTOR>                                   \
	TOptional<Item> ComparedBy(FUNCTOR Func)                      \
	{                                                             \
		return ::IterComparedBy(MoveTempIfPossible(*this), Func); \
	}

/// Consumes iterator and stores its values into provided array.
///
/// # Example
/// ```snippet
/// iter_collect_into_array
/// ```
//// [proxy: iter_collect_into_array, iter, iter_collectors]
//// void CollectIntoArray(TArray<Item>& Result);
//// [/proxy]
#define ITER_IMPL_COLLECT_INTO_ARRAY                               \
	void CollectIntoArray(TArray<Item>& Result)                    \
	{                                                              \
		::IterCollectIntoArray(MoveTempIfPossible(*this), Result); \
	}

/// Consumes iterator and returns an array with its values.
///
/// # Example
/// ```snippet
/// iter_collect_array
/// ```
//// [proxy: iter_collect_array, iter, iter_collectors]
//// TArray<Item> CollectArray();
//// [/proxy]
#define ITER_IMPL_COLLECT_ARRAY                               \
	TArray<Item> CollectArray()                               \
	{                                                         \
		return ::IterCollectArray(MoveTempIfPossible(*this)); \
	}

/// Consumes iterator and stores its values into provided set.
///
/// # Example
/// ```snippet
/// iter_collect_into_set
/// ```
//// [proxy: iter_collect_into_set, iter, iter_collectors]
//// void CollectIntoSet(TSet<Item>& Result);
//// [/proxy]
#define ITER_IMPL_COLLECT_INTO_SET                               \
	void CollectIntoSet(TSet<Item>& Result)                      \
	{                                                            \
		::IterCollectIntoSet(MoveTempIfPossible(*this), Result); \
	}

/// Consumes iterator and returns a set with its values.
///
/// # Example
/// ```snippet
/// iter_collect_array
/// ```
//// [proxy: iter_collect_set, iter, iter_collectors]
//// TSet<Item> CollectSet();
//// [/proxy]
#define ITER_IMPL_COLLECT_SET                               \
	TSet<Item> CollectSet()                                 \
	{                                                       \
		return ::IterCollectSet(MoveTempIfPossible(*this)); \
	}

#define ITER_IMPL_COLLECT_INTO_MAP(KEY, VALUE)                               \
	void CollectIntoMap(TMap<KEY, VALUE>& Result)                            \
	{                                                                        \
		::IterCollectIntoMap<KEY, VALUE>(MoveTempIfPossible(*this), Result); \
	}

#define ITER_IMPL_COLLECT_MAP(KEY, VALUE)                               \
	TMap<KEY, VALUE> CollectMap()                                       \
	{                                                                   \
		return ::IterCollectMap<KEY, VALUE>(MoveTempIfPossible(*this)); \
	}

/// Injects custom iterator adapter into the chain of iteration.
///
/// Useful only for really custom/advanced solutions that cannot be solved with
/// regular iterators.
///
/// # Note
/// > `ADAPTER` should implement iterator adapter methods. Yielded values share
/// same type wit iterator that wraps this adapter.
///
/// # Example
/// ```snippet
/// iter_adapt
/// ```
/// ```snippet
/// iter_adapter
/// ```
//// [proxy: iter_adapt, iter, iter_converters]
//// template <typename ADAPTER>
//// TIterAdapt<Self, ADAPTER> Adapt(ADAPTER Adapter);
//// [/proxy]
#define ITER_IMPL_ADAPT(ADAPTER)                                \
	template <typename ADAPTER>                                 \
	TIterAdapt<Self, ADAPTER> Adapt(ADAPTER Adapter)            \
	{                                                           \
		return ::IterAdapt(MoveTempIfPossible(*this), Adapter); \
	}

/// Casts yielded values to another type.
///
/// Commonly used as a shourtcut for mapping between types using target type
/// constructor.
///
/// # Note
/// > `RESULT` is type that this iterator will yield after casting.
///
/// # Example
/// ```snippet
/// iter_cast
/// ```
//// [proxy: iter_map, iter, iter_converters]
//// template <typename RESULT>
//// TIterCast<typename RESULT, Self> Cast();
//// [/proxy]
#define ITER_IMPL_CAST(RESULT)                                \
	template <typename RESULT>                                \
	TIterCast<typename RESULT, Self> Cast()                   \
	{                                                         \
		return ::IterCast<RESULT>(MoveTempIfPossible(*this)); \
	}

/// Appends another iterator at the end of this iterator.
///
/// Useful for combining results of different iterators that yield same value
/// type.
///
/// # Note
/// > `ITER` should implement iterator methods. Yielded values share same type
/// with this iterato value type.
///
/// # Example
/// ```snippet
/// iter_chain
/// ```
//// [proxy: iter_chain, iter, iter_converters]
//// template <typename ITER>
//// TIterChain<Self, ITER> Chain(ITER&& Iter);
//// [/proxy]
#define ITER_IMPL_CHAIN(ITER)                                     \
	template <typename ITER>                                      \
	TIterChain<Self, ITER> Chain(ITER&& Iter)                     \
	{                                                             \
		return ::IterChain(                                       \
			MoveTempIfPossible(*this), MoveTempIfPossible(Iter)); \
	}

/// Enumerates values in this iterator.
///
/// Useful for reading index of element/iteration.
///
/// # Note
/// > Yielded values have type: `TTuple<uint32, Item>`, which means this
/// iterator yields tuple o index-value pair.
///
/// # Example
/// ```snippet
/// iter_enumerate
/// ```
//// [proxy: iter_enumerate, iter, iter_converters]
//// TIterEnumerate<Self> Enumerate();
//// [/proxy]
#define ITER_IMPL_ENUMERATE                                \
	TIterEnumerate<Self> Enumerate()                       \
	{                                                      \
		return ::IterEnumerate(MoveTempIfPossible(*this)); \
	}

/// Filters values in the iterator by predicate.
///
/// # Note
/// > `FUNCTOR` should comply to this function signature: `bool(const I::Item&
/// Value)`.
///
/// # Example
/// ```snippet
/// iter_filter
/// ```
//// [proxy: iter_filter, iter, iter_converters]
//// template <typename FUNCTOR>
//// TIterFilter<Self, typename FUNCTOR> Filter(FUNCTOR Func);
//// [/proxy]
#define ITER_IMPL_FILTER(FUNCTOR)                             \
	template <typename FUNCTOR>                               \
	TIterFilter<Self, typename FUNCTOR> Filter(FUNCTOR Func)  \
	{                                                         \
		return ::IterFilter(MoveTempIfPossible(*this), Func); \
	}

/// Filters values in the iterator by predicate and maps them to another type.
///
/// # Note
/// > `RESULT` is a type of values yielded by this iterator that maps into.
/// `FUNCTOR` should comply to this function signature:
/// `TOptional<RESULT>(const I::Item& Value)` Returning some value means
/// pasing it to next iterator, returning none means we omit thi value.
///
/// # Example
/// ```snippet
/// iter_filter_map
/// ```
//// [proxy: iter_filter_map, iter, iter_converters]
//// template <typename RESULT, typename FUNCTOR>
//// TIterFilterMap<typename RESULT, Self, typename FUNCTOR>
//// FilterMap(FUNCTOR Func);
//// [/proxy]
#define ITER_IMPL_FILTER_MAP(RESULT, FUNCTOR)                            \
	template <typename RESULT, typename FUNCTOR>                         \
	TIterFilterMap<typename RESULT, Self, typename FUNCTOR> FilterMap(   \
		FUNCTOR Func)                                                    \
	{                                                                    \
		return ::IterFilterMap<RESULT>(MoveTempIfPossible(*this), Func); \
	}

/// Flattens nested iterators.
///
/// Imagine you have an iterator that yields another iterators, such as arrays
/// of arrays.
///
/// # Note
/// > `RESULT` is a type of values yielded by this iterator - it should be the
/// same as neste iterator value type (c++ won't accept that syntax, hence we
/// have to provide a `RESULT` typ and ensure it's the same) `FUNCTOR`
/// should comply to this function signature: `RESULT(const I::Item& Value)`.
///
/// # Example
/// ```snippet
/// iter_flatten
/// ```
//// [proxy: iter_flatten, iter, iter_converters]
//// template <typename RESULT>
//// TIterFlatten<typename RESULT, Self> Flatten();
//// [/proxy]
#define ITER_IMPL_FLATTEN(RESULT)                                \
	template <typename RESULT>                                   \
	TIterFlatten<typename RESULT, Self> Flatten()                \
	{                                                            \
		return ::IterFlatten<RESULT>(MoveTempIfPossible(*this)); \
	}

/// Inspects yielded values.
///
/// Useful when debugging iterators to for example log what values are yielded
/// at which iterator chain stage.
///
/// # Note
/// > `FUNCTOR` should comply to this function signature: `void(const I::Item&
/// Value)`.
///
/// # Example
/// ```snippet
/// iter_inspect
/// ```
//// [proxy: iter_inspect, iter, iter_converters]
//// template <typename FUNCTOR>
//// TIterInspect<Self, typename FUNCTOR> Inspect(FUNCTOR Func);
//// [/proxy]
#define ITER_IMPL_INSPECT(FUNCTOR)                             \
	template <typename FUNCTOR>                                \
	TIterInspect<Self, typename FUNCTOR> Inspect(FUNCTOR Func) \
	{                                                          \
		return ::IterInspect(MoveTempIfPossible(*this), Func); \
	}

/// Maps yielded values to another type.
///
/// Commonly used for data transformations for use in later stages of iteration.
///
/// # Note
/// > `RESULT` is type that this iterator will yield after data transformations.
/// `FUNCTOR` should comply to this function signature: `RESULT(const I::Item&
/// Value)`.
///
/// # Example
/// ```snippet
/// iter_map
/// ```
//// [proxy: iter_map, iter, iter_converters]
//// template <typename RESULT, typename FUNCTOR>
//// TIterMap<typename RESULT, Self, typename FUNCTOR> Map(FUNCTOR Func);
//// [/proxy]
#define ITER_IMPL_MAP(RESULT, FUNCTOR)                                  \
	template <typename RESULT, typename FUNCTOR>                        \
	TIterMap<typename RESULT, Self, typename FUNCTOR> Map(FUNCTOR Func) \
	{                                                                   \
		return ::IterMap<RESULT>(MoveTempIfPossible(*this), Func);      \
	}

/// Skips iteration by number of elements.
///
/// It's worth noting that this is one-shot skip, not repeated one, which means
/// if we yield iterator of 10 elements and we skip 2 iterations, then it will
/// skip just 2 values and yield rest 8.
///
/// # Example
/// ```snippet
/// iter_skip_take
/// ```
//// [proxy: iter_skip, iter, iter_converters]
//// TIterSkip<Self> Skip(uint32 Count);
//// [/proxy]
#define ITER_IMPL_SKIP                                       \
	TIterSkip<Self> Skip(uint32 Count)                       \
	{                                                        \
		return ::IterSkip(MoveTempIfPossible(*this), Count); \
	}

/// Limits iterator to at most number of iterations.
///
/// If we create an iterator that yields 10 values and we tell it to take 5,
/// then it will stop iterating after next 5 values (or less, depends if there
/// is enough values left in iterator).
///
/// # Example
/// ```snippet
/// iter_skip_take
/// ```
//// [proxy: iter_take, iter, iter_converters]
//// TIterTake<Self> Take(uint32 Count);
//// [/proxy]
#define ITER_IMPL_TAKE                                       \
	TIterTake<Self> Take(uint32 Count)                       \
	{                                                        \
		return ::IterTake(MoveTempIfPossible(*this), Count); \
	}

/// Yields sequences of consecutive views over set of values.
///
/// If we create an iterator that yields 5 integer values and we tell it to view
/// 2 at the same time, then it will yield `TArrayView` with values: `[0, 1],
/// [1, 2], [2, 3], [3, 4]`.
///
/// # Example
/// ```snippet
/// iter_views
/// ```
//// [proxy: iter_take, iter, iter_converters]
//// template <uint32 COUNT>
//// TIterViews<Self, COUNT> Views();
//// [/proxy]
#define ITER_IMPL_VIEWS(COUNT)                                      \
	template <uint32 COUNT>                                         \
	TIterViews<Self, COUNT> Views()                                 \
	{                                                               \
		return ::IterViews<Self, COUNT>(MoveTempIfPossible(*this)); \
	}

/// Combines two iterators to make one that yields pair of both iterator values
/// at the same time.
///
/// # Example
/// ```snippet
/// iter_zip
/// ```
//// [proxy: iter_zipe, iter, iter_converters]
//// template <typename ITER>
//// TIterZip<Self, ITER> Zip(ITER&& Iter);
//// [/proxy]
#define ITER_IMPL_ZIP(ITER)                                                    \
	template <typename ITER>                                                   \
	TIterZip<Self, ITER> Zip(ITER&& Iter)                                      \
	{                                                                          \
		return ::IterZip(MoveTempIfPossible(*this), MoveTempIfPossible(Iter)); \
	}

#define ITER_IMPL_CONVERTERS   \
	ITER_IMPL_ADAPT(A)         \
	ITER_IMPL_CAST(R)          \
	ITER_IMPL_CHAIN(J)         \
	ITER_IMPL_ENUMERATE        \
	ITER_IMPL_FILTER(G)        \
	ITER_IMPL_FILTER_MAP(R, G) \
	ITER_IMPL_FLATTEN(R)       \
	ITER_IMPL_INSPECT(G)       \
	ITER_IMPL_MAP(R, G)        \
	ITER_IMPL_SKIP             \
	ITER_IMPL_TAKE             \
	ITER_IMPL_VIEWS(C)         \
	ITER_IMPL_ZIP(J)

#define ITER_IMPL_CONSUMERS_BASE \
	ITER_IMPL_COUNT              \
	ITER_IMPL_NTH                \
	ITER_IMPL_FOLD(G)            \
	ITER_IMPL_FIND(G)            \
	ITER_IMPL_FIND_MAP(R, G)     \
	ITER_IMPL_FIRST              \
	ITER_IMPL_LAST               \
	ITER_IMPL_FOR_EACH(G)        \
	ITER_IMPL_ANY(G)             \
	ITER_IMPL_ALL(G)             \
	ITER_IMPL_COMPARED_BY(G)

#define ITER_IMPL_CONSUMERS  \
	ITER_IMPL_CONSUMERS_BASE \
	ITER_IMPL_SUM

#define ITER_IMPL_COLLECTORS     \
	ITER_IMPL_COLLECT_INTO_ARRAY \
	ITER_IMPL_COLLECT_ARRAY      \
	ITER_IMPL_COLLECT_INTO_SET   \
	ITER_IMPL_COLLECT_SET

#define ITER_IMPL        \
	ITER_IMPL_CONVERTERS \
	ITER_IMPL_CONSUMERS  \
	ITER_IMPL_COLLECTORS
