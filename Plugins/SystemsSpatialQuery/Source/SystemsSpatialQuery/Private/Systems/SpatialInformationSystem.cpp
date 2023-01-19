#include "SystemsSpatialQuery/Public/Systems/SpatialInformationSystem.h"

#include "Systems/Public/SystemsWorld.h"
#include "SystemsSpatialQuery/Public/Resources/SpatialGraph.h"
#include "SystemsSpatialQuery/Public/Resources/SpatialInformation.h"

void USpatialInformationSystem::Run(USystemsWorld& Systems)
{
	auto* Information = Systems.Resource<USpatialInformation>();
	const auto* Graph = Cast<USpatialGraph>(Systems.ResourceRaw(this->ResourceType.Get()));
	if (IsValid(Information) == false || IsValid(Graph) == false)
	{
		return;
	}

	const auto DeltaTime = GetWorld()->GetDeltaSeconds();

	for (const auto& FromTo : Graph->ConnectionsIter())
	{
		const auto& From = FromTo.Get<0>();
		const auto& To = FromTo.Get<1>();
		auto* PointFrom = Information->Points.Find(From);
		if (PointFrom == nullptr)
		{
			continue;
		}
		auto& PointTo = Information->Points.FindOrAdd(To);
		for (auto& PairFrom : PointFrom->Values)
		{
			auto& ValueTo = PointTo.Values.FindOrAdd(PairFrom.Key);
			const auto Difference = PairFrom.Value.Value - ValueTo.Value;
			PairFrom.Value.Deviation -= Difference;
			ValueTo.Deviation += Difference;
		}
	}

	for (auto& PointPair : Information->Points)
	{
		for (auto& ValuePair : PointPair.Value.Values)
		{
			const auto& Propagation = FindPropagationSettings(ValuePair.Key);
			const auto Count = Graph->NeighborsIter(PointPair.Key).Count();
			ValuePair.Value.Value += ValuePair.Value.Deviation / static_cast<double>(Count) * DeltaTime;

			if (FMath::IsNearlyZero(Propagation.DampingFactor) == false)
			{
				ValuePair.Value.Value *= FMath::Exp(-Propagation.DampingFactor * DeltaTime);
			}
		}
	}
}

const FSpatialPropagationSettings& USpatialInformationSystem::FindPropagationSettings(FName Id) const
{
	const auto* Found = this->SpecializedPropagation.Find(Id);
	return Found == nullptr ? this->DefaultPropagation : *Found;
}