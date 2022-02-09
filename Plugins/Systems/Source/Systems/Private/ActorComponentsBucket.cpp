#include "Systems/Public/ActorComponentsBucket.h"

#include "Systems/Public/SystemsComponent.h"

void FActorComponentsBucket::Add(UActorComponent* Component)
{
	if (IsValid(Component) && IsValid(Component->GetClass()))
	{
		const auto Id = Component->GetClass()->GetUniqueID();
		this->Removed.Remove(Id);
		this->Added.Add(Component);
	}
}

void FActorComponentsBucket::Remove(UActorComponent* Component)
{
	if (Component != nullptr && IsValid(Component->GetClass()))
	{
		const auto Id = Component->GetClass()->GetUniqueID();
		this->Added.RemoveAllSwap(
			[&](const auto* Item) {
				return IsValid(Item) == false ||
					Item->GetClass()->GetUniqueID() == Id;
			},
			false);
		this->Removed.Add(Id);
	}
}
