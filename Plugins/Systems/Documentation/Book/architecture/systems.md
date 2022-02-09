# Systems

# Table of contents

- [Explanation](#explanation)
- [Types of systems](#types-of-systems)
    - [Stateless systems](#stateless-systems)
    - [Stateful systems](#stateful-systems)
- [Problems that it solves](#problems-that-it-solves)

# Explanation

Systems are small but precise units of work, work that's mainly about processing game world data
using queries. These queries are processed using [iterators](/book/architecture/iterators.md),
and that makes queries the whole point of systems ergonomics since they simplify both computational
and comprehensive complexity and make that part of game logic really easy to reason about,
which in turn makes adding, removing and changing game logic easy and quicker than with monolithic
code or features highly dependent on each other, scattered around the codebase.

# Types of systems

There are few types of systems, but the ones that you will mostly use are stateless (lambdas/functions)
and stateful (classes) systems.

## Stateless systems

These are the most common ones. Stateless systems are just lambdas/functions that expect reference
to [`class: USystemsWorld`]() to operate on it. Stateless systems do not require any internal state
to operate.

```snippet
stateless_system
```

```snippet
install_stateless_system
```

## Stateful systems

Stateful systems are useful in cases where system requires some internal state that cannot be
shared with other systems via resources (for example internal cache that is used to track changes
in the system across multiple game ticks).

```snippet
system
```

```snippet
install_stateful_system
```

# Problems that it solves

There is very common (bad) practice in the industry that we make a game prototype, then **we build
actual game on top of that prototype codebase**, instead of treating it purely as a reference for
an actual, properly implemented solutions in actual project - this can and will quickly lead to
poisoned codebase months and years later. It is really hard to break that habit, mostly in
professional game development because time budget and famous last words such as ["we will refactor
that later"](https://www.youtube.com/watch?v=SETnK2ny1R0), but that "later" do not come as soon as
we would like, making us having a weak foundation and we build new features on top of that weak
foundation, **basically playing Jenga with our codebase**, codebase which parts of will fall later
as the project grows, causing **The Fuck Cascade <sup>TM.</sup>** and frustration later on - we can
do better than that.

Systems Architecture helps with that by forcing simple and modular codebase from the get go,
making switching from prototype to actual implementation a matter of just swapping prototype
systems with actual properly implemented systems, sometimes not even having to change them at all.
