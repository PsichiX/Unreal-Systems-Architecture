#include "Systems/Public/ArchetypeSignature.h"

FArchetypeSignature::FArchetypeSignature()
{
	for (auto Index = 0; Index < SYSTEMS_ARCHETYPE_BUCKETS_COUNT; ++Index)
	{
		this->Buckets[Index] = 0;
	}
}

FArchetypeSignature::FArchetypeSignature(const FArchetypeSignature& Other)
{
	for (auto Index = 0; Index < SYSTEMS_ARCHETYPE_BUCKETS_COUNT; ++Index)
	{
		this->Buckets[Index] = Other.Buckets[Index];
	}
}

FArchetypeSignature::FArchetypeSignature(uint32 Bit)
{
	EnableBit(Bit);
}

void FArchetypeSignature::Clear()
{
	for (auto Index = 0; Index < SYSTEMS_ARCHETYPE_BUCKETS_COUNT; ++Index)
	{
		this->Buckets[Index] = 0;
	}
}

bool FArchetypeSignature::IsEmpty() const
{
	for (auto Index = 0; Index < SYSTEMS_ARCHETYPE_BUCKETS_COUNT; ++Index)
	{
		if (this->Buckets[Index] > 0)
		{
			return false;
		}
	}
	return true;
}

void FArchetypeSignature::EnableBit(uint32 Index)
{
	const auto Bucket = Index / 32;
	const auto Bit = Index % 32;
	if (Bucket < SYSTEMS_ARCHETYPE_BUCKETS_COUNT)
	{
		this->Buckets[Bucket] |= 1 << Bit;
	}
}

void FArchetypeSignature::DisableBit(uint32 Index)
{
	const auto Bucket = Index / 32;
	const auto Bit = Index % 32;
	if (Bucket < SYSTEMS_ARCHETYPE_BUCKETS_COUNT)
	{
		this->Buckets[Bucket] &= ~(1 << Bit);
	}
}

bool FArchetypeSignature::HasBit(uint32 Index) const
{
	const auto Bucket = Index / 32;
	const auto Bit = Index % 32;
	return Bucket < SYSTEMS_ARCHETYPE_BUCKETS_COUNT &&
		(this->Buckets[Bucket] & 1 << Bit) > 0;
}

bool FArchetypeSignature::ContainsAll(const FArchetypeSignature& Other) const
{
	for (auto Index = 0; Index < SYSTEMS_ARCHETYPE_BUCKETS_COUNT; ++Index)
	{
		if ((this->Buckets[Index] & Other.Buckets[Index]) !=
			Other.Buckets[Index])
		{
			return false;
		}
	}
	return true;
}

bool FArchetypeSignature::ContainsAny(const FArchetypeSignature& Other) const
{
	for (auto Index = 0; Index < SYSTEMS_ARCHETYPE_BUCKETS_COUNT; ++Index)
	{
		if ((this->Buckets[Index] & Other.Buckets[Index]) != 0)
		{
			return true;
		}
	}
	return false;
}

FArchetypeSignature FArchetypeSignature::Include(
	const FArchetypeSignature& Other) const
{
	FArchetypeSignature Result;
	for (auto Index = 0; Index < SYSTEMS_ARCHETYPE_BUCKETS_COUNT; ++Index)
	{
		Result.Buckets[Index] = this->Buckets[Index] | Other.Buckets[Index];
	}
	return Result;
}

FArchetypeSignature FArchetypeSignature::Exclude(
	const FArchetypeSignature& Other) const
{
	FArchetypeSignature Result;
	for (auto Index = 0; Index < SYSTEMS_ARCHETYPE_BUCKETS_COUNT; ++Index)
	{
		Result.Buckets[Index] = this->Buckets[Index] & ~Other.Buckets[Index];
	}
	return Result;
}

FArchetypeSignature FArchetypeSignature::Union(
	const FArchetypeSignature& Other) const
{
	FArchetypeSignature Result;
	for (auto Index = 0; Index < SYSTEMS_ARCHETYPE_BUCKETS_COUNT; ++Index)
	{
		Result.Buckets[Index] = this->Buckets[Index] & Other.Buckets[Index];
	}
	return Result;
}

FArchetypeSignature FArchetypeSignature::Difference(
	const FArchetypeSignature& Other) const
{
	FArchetypeSignature Result;
	for (auto Index = 0; Index < SYSTEMS_ARCHETYPE_BUCKETS_COUNT; ++Index)
	{
		Result.Buckets[Index] = this->Buckets[Index] ^ Other.Buckets[Index];
	}
	return Result;
}

uint32 FArchetypeSignature::CountBits() const
{
	auto Result = 0;
	for (auto Index = 0; Index < SYSTEMS_ARCHETYPE_BUCKETS_COUNT; ++Index)
	{
		auto Bucket = this->Buckets[Index];
		Bucket = Bucket - ((Bucket >> 1) & 0x55555555);
		Bucket = (Bucket & 0x33333333) + ((Bucket >> 2) & 0x33333333);
		Bucket = (Bucket + (Bucket >> 4)) & 0x0F0F0F0F;
		Result += (Bucket * 0x01010101) >> 24;
	}
	return Result;
}

bool FArchetypeSignature::operator==(const FArchetypeSignature& Other) const
{
	return Equals(Other);
}

bool FArchetypeSignature::Equals(const FArchetypeSignature& Other) const
{
	for (auto Index = 0; Index < SYSTEMS_ARCHETYPE_BUCKETS_COUNT; ++Index)
	{
		if (this->Buckets[Index] != Other.Buckets[Index])
		{
			return false;
		}
	}
	return true;
}

uint32 GetTypeHash(const FArchetypeSignature& Signature)
{
	return FCrc::MemCrc32(&Signature, sizeof(FArchetypeSignature));
}
