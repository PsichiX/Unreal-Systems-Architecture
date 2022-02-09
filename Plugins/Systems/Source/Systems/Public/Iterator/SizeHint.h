#pragma once

#include "CoreMinimal.h"

/// Hint about minimum and optionally maximum number of items iterator can
/// yield.
///
/// See [`struct: TQuery::SizeHint`]().
///
/// # Example
/// ```snippet
/// query_size_hint
/// ```
struct IterSizeHint
{
	/// Minimum number of items iterator can yield.
	uint32 Minimum = 0;
	/// Maximum number of items iterator can yield.
	TOptional<uint32> Maximum = {};
};
