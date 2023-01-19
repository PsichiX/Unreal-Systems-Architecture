#include "SystemsSpatialQuery/Public/Resources/SpatialDiscretization.h"

double TriangleArea(FVector2D A, FVector2D B, FVector2D C)
{
	return FVector2D::CrossProduct(B - A, C - A) * 0.5;
}

FSpatialDiscretizationTriangle::FSpatialDiscretizationTriangle(AActor* InA, AActor* InB, AActor* InC)
{
	TArray<AActor*, TInlineAllocator<3>> Inputs = {InA, InB, InC};
	Inputs.Sort();
	this->A = Inputs[0];
	this->B = Inputs[1];
	this->C = Inputs[2];
}

bool FSpatialDiscretizationTriangle::ContainsPoint(const FVector2D& Point) const
{
	if (BoundingBox().IsInside(Point) == false)
	{
		return false;
	}
	const auto Barycentric = ToBarycentric(Point);
	return Barycentric.X >= 0.0 && Barycentric.X <= 1.0 && Barycentric.Y >= 0.0 && Barycentric.Y <= 1.0 &&
		Barycentric.Z >= 0.0 && Barycentric.Z <= 1.0;
}

FVector FSpatialDiscretizationTriangle::ToBarycentric(const FVector2D& Point) const
{
	const auto PosA = FVector2D(this->A->GetActorLocation());
	const auto PosB = FVector2D(this->B->GetActorLocation());
	const auto PosC = FVector2D(this->C->GetActorLocation());
	const auto MainArea = TriangleArea(PosA, PosB, PosC);
	const auto Alpha = TriangleArea(Point, PosB, PosC) / MainArea;
	const auto Beta = TriangleArea(Point, PosC, PosA) / MainArea;
	const auto Gamma = 1.0 - Alpha - Beta;
	return {Alpha, Beta, Gamma};
}

FVector2D FSpatialDiscretizationTriangle::FromBarycentric(const FVector& Barycentric) const
{
	const auto PosA = FVector2D(this->A->GetActorLocation());
	const auto PosB = FVector2D(this->B->GetActorLocation());
	const auto PosC = FVector2D(this->C->GetActorLocation());
	const auto X = Barycentric.X * PosA.X + Barycentric.Y * PosB.X + Barycentric.Z * PosC.X;
	const auto Y = Barycentric.X * PosA.Y + Barycentric.Y * PosB.Y + Barycentric.Z * PosC.Y;
	return {X, Y};
}

TTuple<FVector2D, double> FSpatialDiscretizationTriangle::Circumcircle() const
{
	const auto PosA = FVector2D(this->A->GetActorLocation());
	const auto PosB = FVector2D(this->B->GetActorLocation());
	const auto PosC = FVector2D(this->C->GetActorLocation());
	const auto Center = (PosA + PosB + PosC) / 3.0;
	const auto Radius = FVector2D::Distance(Center, PosA);
	return MakeTuple(Center, Radius);
}

FBox2d FSpatialDiscretizationTriangle::BoundingBox() const
{
	const auto PosA = FVector2D(this->A->GetActorLocation());
	const auto PosB = FVector2D(this->B->GetActorLocation());
	const auto PosC = FVector2D(this->C->GetActorLocation());
	return FBox2d(PosA, PosA) + PosB + PosC;
}

bool FSpatialDiscretizationTriangle::Equals(const FSpatialDiscretizationTriangle& Other) const
{
	return this->A == Other.A && this->B == Other.B && this->C && Other.C;
}

bool FSpatialDiscretizationTriangle::operator==(const FSpatialDiscretizationTriangle& Other) const
{
	return Equals(Other);
}

uint32 GetTypeHash(const FSpatialDiscretizationTriangle& Value)
{
	return HashCombine(GetTypeHash(Value.A), HashCombine(GetTypeHash(Value.B), GetTypeHash(Value.C)));
}

void USpatialDiscretization::Reset()
{
	this->Triangles.Reset();
}

void USpatialDiscretization::AddTriangle(FSpatialDiscretizationTriangle Triangle)
{
	this->Triangles.Add(Triangle);
}

bool USpatialDiscretization::HasTriangle(const FSpatialDiscretizationTriangle& Triangle) const
{
	return this->Triangles.Contains(Triangle);
}

TArray<TTuple<TObjectPtr<AActor>, double>> USpatialDiscretization::TriangleActorWeightsForPoint(
	const FVector2D& Point) const
{
	const auto Found =
		IterStdConst(this->Triangles).Find([&](const auto& Triangle) { return Triangle.ContainsPoint(Point); });
	if (Found.IsSet())
	{
		const auto& Triangle = Found.GetValue();
		const auto Barycentric = Triangle.ToBarycentric(Point);
		return {MakeTuple(Triangle.A, Barycentric.X),
			MakeTuple(Triangle.B, Barycentric.Y),
			MakeTuple(Triangle.C, Barycentric.Z)};
	}
	return {};
}