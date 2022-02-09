#pragma once

#include "CoreMinimal.h"

#include "Systems/Public/Constants.h"

#include "ArchetypeSignature.generated.h"

/// Signature of actor components set.
///
/// More in depth about archetype signatures in [this architecture book
/// section](/book/architecture/world.md).
///
/// Signatures are used internally by [`class: USystemsWorld`]() to allow
/// identification of to which buckets (archetypes) given set of components
/// belongs to - that allows for querying iterators over only these actor
/// components that contain all requested components and omiting ones that
/// doesn't, which improves performance of querying systems world (which is the
/// most frequently used operation in systems game architecture, hence the
/// importance of making it fast).
///
/// # Note
/// > Although constructing signatures has no real value for common usecases
/// while developing games this API can be useful for users that needs to
/// perform more advanced operations.
///
/// # Example
/// ```snippet
/// systems_world_count_raw
/// ```
USTRUCT()
struct SYSTEMS_API FArchetypeSignature
{
	GENERATED_BODY()

public:
	/// Constructs signature of empty components set.
	FArchetypeSignature();

	/// Copies signature from another one.
	FArchetypeSignature(
		/// Other signature to copy components set from.
		const FArchetypeSignature& Other);

	/// Constructs signature with single component bit enabled.
	FArchetypeSignature(
		/// Index of component bit to enable.
		uint32 Bit);

	/// Clears components set.
	void Clear();

	/// Checks if components set is empty.
	bool IsEmpty() const;

	/// Enable component bit at given index.
	void EnableBit(
		/// Index of component bit to enable.
		uint32 Index);

	/// Disable component bit at given index.
	void DisableBit(
		/// Index of component bit to disable.
		uint32 Index);

	/// Checks if component bit at given index is enabled.
	bool HasBit(
		/// Index of component bit to check.
		uint32 Index) const;

	/// Checks if this signature fully contain component set of another
	/// signature.
	///
	/// Used by [`class: USystemsWorld::Query`]() to filter archetypes that are
	/// valid for iteration.
	bool ContainsAll(
		/// Other signature to compare with.
		const FArchetypeSignature& Other) const;

	/// Checks if this signature at least partially contain component set of
	/// another signature.
	bool ContainsAny(
		/// Other signature to compare with.
		const FArchetypeSignature& Other) const;

	/// Include components set bits from ones of another signature.
	FArchetypeSignature Include(
		/// Other signature to include from.
		const FArchetypeSignature& Other) const;

	/// Exclude components set bits from ones of another signature.
	FArchetypeSignature Exclude(
		/// Other signature to exclude from.
		const FArchetypeSignature& Other) const;

	/// Returns signature with only component set bits enabled by both
	/// signatures.
	FArchetypeSignature Union(
		/// Other signature to test with.
		const FArchetypeSignature& Other) const;

	/// Returns signature with only component set bits that differs between this
	/// and another signature.
	FArchetypeSignature Difference(
		/// Other signature to test with.
		const FArchetypeSignature& Other) const;

	/// Counts bits of enabled components in set.
	uint32 CountBits() const;

	bool operator==(const FArchetypeSignature& Other) const;

	bool Equals(const FArchetypeSignature& Other) const;

private:
	UPROPERTY()
	uint32 Buckets[SYSTEMS_ARCHETYPE_BUCKETS_COUNT] = {};
};

uint32 SYSTEMS_API GetTypeHash(const FArchetypeSignature& Signature);
