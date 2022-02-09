# World

# Table of contents

- [Explanation](#explanation)
    - [Queries](#queries)
    - [Archetypes](#archetypes)
- [Problems that it solves](#problems-that-it-solves)

# Explanation

[`class: USystemsWorld`]() is a registry that stores [systems](/book/architecture/systems.md),
[resources](/book/architecture/resources.md) and [actor components](/book/architecture/components.md).
The first thing you do is you setup your systems world by installing systems, resources and
registering actor components that will be used in systems queries to process and do something
useful with resources and actor components.

```snippet
subsystem_game_instance
```

In example above we register systems world in game instance. We can of course create systems
worlds at will without registering it into [`class: USystemsSubsystem`](), this global registry
is only a more convenient way to access and automate systems world management globally.

Once per tick (when using subsystem approach) all systems gets to run in order they were
installed (system installation options can provide additional hints about more precise order
when working with dependencies between systems).

## Queries

The whole point of Systems Architecture is to process game world in a database-like fashion,
which means to perform same operations on a batched set of actor components, instead of putting
that work into these objects. That makes us completely decouple data from behaviors, making it
easier to add, remove and change them at will, even reuse without dealing with solving hidden
dependency tree problems between seemingly unrelated parts of codebase.

```snippet
systems_world_query
```

Queries are [iterators](/book/architecture/iterators.md) that always yields a tuple of actor
components and can be chained with other iterators to produce more elaborated, yet easy to
reason about data transformations. For example when you do `Systems.Query<A, B, C>`, values
yielded by query iterator will be of type `TTuple<AActor*, A*, B*, C*>`. it's also important
to note that user should never cache query objects, rather consume queries in place.

See [`struct: TQuery`]().

## Archetypes

Internally all actor components are stored in buckets called archetypes. Given actor archetype
is determined by its [`struct: FArchetypeSignature`](), which is calculated from the set of
components given actor owns. When actor registers its components, systems world searches through
already existing archetypes and if it finds one, actor gets registered in that bucket, if not
new archetype bucket gets created.

The point of storing actors in groups per archetype is to make queries iteration as fast as
possible by iterating over only these actors that own all requested components in a linear
fashion without need for skipping over actors that we don't need to work on.

Archetype signatures are basically bitsets of 256 bits capacity, each bit represents registered
actor component. Whenever signature gets calculated for given query, it makes empty signature
and enables bits for requested component, then it compares this signature with each archetype
bucket signature and stores in query object a list of archetypes to iterate on.

# Problems that it solves

Usually when working with game managers it's a common practice to cache managers in other managers
to not perform costly searches in game world. This is unnecessary work and Systems Architecture
solves that by storing resources in single systems world, easily accessible from any system
without any need for caching any used resources in systems itself.

Another common problem is usually hierarchy of game logic gets growing in depth, like when game
logic is performed by game objects themselves, they reference one another, growing hidden
dependency tree and by that comprehensive complexity that requires sometimes hours to jump around
whole codebase just to understand how data flows between certain objects. Systems Architecture
forces user to make flat game logic with explicit order of execution, additionally making units
of work as small and as precise as possible to make it easy to reason about how game data gets
processed.
