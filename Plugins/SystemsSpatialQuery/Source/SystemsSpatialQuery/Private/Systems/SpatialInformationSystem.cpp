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
	if (this->FixedStepFrequency == ESpatialPropagationFixedStepFrequency::None)
	{
		PerformStep(DeltaTime, Information, Graph);
		return;
	}

	const auto FixedDeltaTime = 1.0 / static_cast<double>(static_cast<uint8>(this->FixedStepFrequency));
	this->FixedStepAccumulator += DeltaTime;
	auto Steps = this->FixedStepSubstepsLimit;
	while (this->FixedStepAccumulator >= FixedDeltaTime)
	{
		this->FixedStepAccumulator -= FixedDeltaTime;
		if (Steps > 0)
		{
			PerformStep(FixedDeltaTime, Information, Graph);
			--Steps;
		}
	}
}

const FSpatialPropagationSettings& USpatialInformationSystem::FindPropagationSettings(FName Id) const
{
	const auto* Found = this->SpecializedPropagation.Find(Id);
	return Found == nullptr ? this->DefaultPropagation : *Found;
}

void USpatialInformationSystem::PerformStep(double DeltaTime,
	USpatialInformation* Information,
	const USpatialGraph* Graph) const
{
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
		const auto Distance = FVector::Distance(From->GetActorLocation(), To->GetActorLocation());
		const auto Count = Graph->NeighborsIter(From).Count();
		for (auto& PairFrom : PointFrom->Values)
		{
			const auto& Propagation = FindPropagationSettings(PairFrom.Key);
			auto& ValueTo = PointTo.Values.FindOrAdd(PairFrom.Key);
			const auto Direction = PairFrom.Value.Value - ValueTo.Value;
			const auto Difference = (Direction * Propagation.Speed) / static_cast<double>(Count);
			const auto Change = (Difference / Distance) * DeltaTime;
			PairFrom.Value.Deviation -= Change;
			ValueTo.Deviation += Change;
		}
	}

	for (auto& PointPair : Information->Points)
	{
		for (auto& ValuePair : PointPair.Value.Values)
		{
			ValuePair.Value.Value += ValuePair.Value.Deviation * DeltaTime;

			const auto& Propagation = FindPropagationSettings(ValuePair.Key);
			const auto DampingFactor = Propagation.DampingFactor + ValuePair.Value.DampingFactorBias;
			if (FMath::IsNearlyZero(DampingFactor) == false)
			{
				ValuePair.Value.Value *= FMath::Exp(-DampingFactor * DeltaTime);
			}
		}
	}
}