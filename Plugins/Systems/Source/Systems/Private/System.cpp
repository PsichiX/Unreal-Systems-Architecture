#include "Systems/Public/System.h"

#include "Systems/Public/SystemsReflection.h"
#include "Systems/Public/SystemsWorld.h"

void UScriptableSystem::OnInit_Implementation(USystemsWorld* Systems)
{
}

void UScriptableSystem::Init(USystemsWorld& Systems)
{
	OnInit(&Systems);
}

void UScriptableSystem::OnRun_Implementation(USystemsWorld* Systems, float DeltaSeconds)
{
}

void UScriptableSystem::Run(USystemsWorld& Systems)
{
	OnRun(&Systems, GetWorld()->GetDeltaSeconds());
}

AActor* UScriptableSystem::SpawnActorFromClass(TSubclassOf<AActor> Class,
	const FTransform& SpawnTransform,
	AActor* Owner)
{
	FActorSpawnParameters Parameters = {};
	Parameters.Owner = Owner;
	return GetWorld()->SpawnActor<AActor>(Class.Get(), SpawnTransform, Parameters);
}

void ULambdaSystem::Bind(TFunction<ThisClass::FunctorType>&& Func)
{
	this->Functor.Emplace(MoveTempIfPossible(Func));
}

void ULambdaSystem::Unbind()
{
	this->Functor.Reset();
}

void ULambdaSystem::Init(USystemsWorld& Systems)
{
	Super::Init(Systems);

	if (this->FunctionName.IsNone() == false)
	{
		const auto Callback = FSystemsReflection::Get().FindByName(this->FunctionName);
		if (Callback.IsSet())
		{
			this->Functor = CopyTemp(Callback.GetValue());
		}
	}
}

void ULambdaSystem::Run(USystemsWorld& Systems)
{
	Super::Run(Systems);

	if (this->Functor.IsSet())
	{
		this->Functor.GetValue()(Systems);
	}
}
