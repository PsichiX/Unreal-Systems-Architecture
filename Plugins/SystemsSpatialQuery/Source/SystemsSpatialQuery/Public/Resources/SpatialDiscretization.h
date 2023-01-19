#pragma once

#include "CoreMinimal.h"

#include "Systems/Public/Iterator.h"

#include "SpatialDiscretization.generated.h"

USTRUCT(BlueprintType)
struct SYSTEMSSPATIALQUERY_API FSpatialDiscretizationTriangle
{
	GENERATED_BODY()

	FSpatialDiscretizationTriangle()
	{
	}

	FSpatialDiscretizationTriangle(AActor* InA, AActor* InB, AActor* InC);

	bool ContainsPoint(const FVector2D& Point) const;

	FVector ToBarycentric(const FVector2D& Point) const;

	FVector2D FromBarycentric(const FVector& Barycentric) const;

	TTuple<FVector2D, double> Circumcircle() const;

	FBox2d BoundingBox() const;

	bool Equals(const FSpatialDiscretizationTriangle& Other) const;

	bool operator==(const FSpatialDiscretizationTriangle& Other) const;

	UPROPERTY()
	TObjectPtr<AActor> A = {};

	UPROPERTY()
	TObjectPtr<AActor> B = {};

	UPROPERTY()
	TObjectPtr<AActor> C = {};
};

uint32 GetTypeHash(const FSpatialDiscretizationTriangle& Value);

UCLASS(BlueprintType)
class SYSTEMSSPATIALQUERY_API USpatialDiscretization : public UObject
{
	GENERATED_BODY()

public:
	void Reset();

	void AddTriangle(FSpatialDiscretizationTriangle Triangle);

	bool HasTriangle(const FSpatialDiscretizationTriangle& Triangle) const;

	auto TrianglesIter() const
	{
		return IterStdConst(this->Triangles);
	}

	auto TrianglesContainingPointIter(const FVector2D& Point) const
	{
		return IterStdConst(this->Triangles)
			.Filter([&](const auto& Triangle) { return Triangle.ContainsPoint(Point); });
	}

	TArray<TTuple<TObjectPtr<AActor>, double>> TriangleActorWeightsForPoint(const FVector2D& Point) const;

private:
	UPROPERTY()
	TSet<FSpatialDiscretizationTriangle> Triangles = {};
};