# Components

# Table of contents

- [Explanation](#explanation)
- [Problems that it solves](#problems-that-it-solves)

# Explanation

See [`class: USystemsActorComponent`]() and [`class: USystemsSceneComponent`]().

Systems Architecture treats game world as database that user can query. That enables us to
perform same operations on a batch of actors and their requested components in-place.

```snippet
systems_world_query
```

And all you have to do is to add or remove components on your actors of interest to enable/disable
them for certain systems, instead of juggling or even duplicating code around many classes,
growing your hidden dependency graph. That simplifies the game logic and greatly improves
iteration times on game systems.

> Keeping data decoupled from game logic enables you to easily perform different logic on same
set of data, avoiding duplication of both logic and data, making it easier to iterate on them.

Of course sometimes keeping some logic in components is reasonable - good practice is to add
into actor components only these methods that do not produce any side effects outside this
component scope, which means logic that only serves to recalculate its internal state.

# Problems that it solves

When working with Unreal Engine, mostly as a beginner, a little bit less as an advanced user,
you find yourself slapping a lot of code into actors. The general thought wandering around
your mind at that time usually goes like this: "This code is something that only this actor
does, so it's obvious that this code belongs to this actor class, duh" - at that time, while
there is still not much work put in there you _might_ be right, but in that case your intuition
is wrong once again.

Across next months you slap bits of logic there and its complexity grows. It doesn't help that
you've split it into several methods, all of that effort gets invalidated once you have to add
a second, and a third actor that does similar things, and you suddenly find yourself
redesigning proper decoupling or inheritance tree or even doing massive refactoring because
this one problem touches many seemingly unrelated places, that were part of complex hidden
dependency tree.

All of that could be avoided with splitting monolithic work per components, but we can do even
better and **completely decouple data from logic that we put in small dedicated systems**.
