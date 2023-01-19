#include "SystemsSpatialQuery/Public/Systems/DebugSpatialDiscretizationSystem.h"

#include "DrawDebugHelpers.h"
#include "Systems/Public/SystemsWorld.h"
#include "SystemsSpatialQuery/Public/Resources/SpatialDiscretization.h"

void UDebugSpatialDiscretizationSystem::Run(USystemsWorld& Systems)
{
	if (IsValid(this->ResourceType.Get()) == false)
	{
		return;
	}
	const auto* Discretization = Cast<USpatialDiscretization>(Systems.ResourceRaw(this->ResourceType.Get()));
	if (IsValid(Discretization) == false)
	{
		return;
	}
	TArray Vertices = {FVector(0), FVector(0), FVector(0)};
	const TArray Indices = {0, 1, 2};
	for (const auto& Triangle : Discretization->TrianglesIter())
	{
		Vertices[0] = Triangle.A->GetActorLocation();
		Vertices[1] = Triangle.B->GetActorLocation();
		Vertices[2] = Triangle.C->GetActorLocation();
		DrawDebugMesh(GetWorld(), Vertices, Indices, this->TrianglesColor);
	}
}