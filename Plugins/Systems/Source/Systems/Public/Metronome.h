#pragma once

#include "CoreMinimal.h"

#include "Metronome.generated.h"

/// Metronome for ticking optimizations.
///
/// Useful for spreading tickable work across time.
/// User should use it to limit number of heavy ticking operations that do not
/// need to perform on every tick, rather can be executed with uniform random
/// time offset for each instance of a group. Good usecase would be AI tasks.
///
/// # Example
/// ```snippet
/// metronome
/// ```
USTRUCT(BlueprintType, Blueprintable)
struct SYSTEMS_API FMetronome
{
	GENERATED_BODY()

public:
	/// Tries to uniformly randomize [`struct: FMetronome::Accumulator`]() value
	/// in range from 0 to [`struct: FMetronome::Limit`]().
	void Randomize();

	/// Performs [`struct: FMetronome::Accumulator`]() accumulation by
	/// `DeltaTime` value.
	void Progress(float DeltaTime);

	/// Chechs if metronome ticked and consumes [`struct:FMetronome::bTicked`]()
	/// (sets it to `false`).
	bool ConsumeTicked();

	/// Tells the current time phase.
	UPROPERTY(EditAnywhere, Category = "Systems|Metronome")
	float Accumulator = 0;

	/// Tells the time limit that marks time of ticking.
	UPROPERTY(EditAnywhere, Category = "Systems|Metronome")
	float Limit = 1;

	/// Tells if metronome has ticked.
	UPROPERTY(EditAnywhere, Category = "Systems|Metronome")
	bool bTicked = true;
};