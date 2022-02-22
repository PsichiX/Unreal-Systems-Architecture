#pragma once

#include "CoreMinimal.h"

#include "Components/SphereComponent.h"
#include "Systems/Public/ArchetypeSignature.h"

#include "MultiplayerHitComponent.generated.h"

class USystemsWorld;

UCLASS(BlueprintType, Blueprintable, Meta = (BlueprintSpawnableComponent))
class MULTIPLAYER_API UMultiplayerHitComponent : public USphereComponent
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

private:
	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult);

	UPROPERTY(EditAnywhere)
	TSet<TSubclassOf<UActorComponent>> RequiredComponents = {};

	UPROPERTY()
	FArchetypeSignature RequiredSignature = {};
};
