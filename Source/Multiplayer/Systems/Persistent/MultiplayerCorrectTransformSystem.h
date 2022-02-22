#pragma once

#include "CoreMinimal.h"

class USystemsWorld;

/// This system exists only for the time until proper client-side prediction and server-side
/// reconciliation gets implemented - until that happen, server has to correct clients about their
/// objects positions because client-side extrapolation cannot be precise enough to allow clients to
/// extrapolate to exact server state, hence we correct clients about their transforms.
///
/// Note that this obviously will produce delayed response due to networking lag - client-side
/// prediction and server side reconciliation will solve that problem when gets implemented into
/// this demo!
///
/// Additional note: since actor's replicated movement does not replicate to client that onws that
/// actor (stilll cannot figure out why), transform correction is used to correct these actors
/// transforms first and foremost.
UFUNCTION()
void MultiplayerCorrectTransformSystem(USystemsWorld& Systems);
