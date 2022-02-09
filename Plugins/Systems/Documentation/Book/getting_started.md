# Getting started

Before we jump into the meat of Systems Architecture, let me quickly show you a
snippet showcasing how working with Systems looks like in a gist:

<details>
<summary><b><i>Click to unfold code snippet</i></b></summary>

```snippet
book_getting_started
```

</details>

# Table of contents

- [Systems Architecture is not an ECS!](#systems-architecture-is-not-an-ecs)
- [Goals](#goals)
  - [More Data-Oriented-Design approach to game logic](#more-data-oriented-design-approach-to-game-logic)
  - [Flat and deterministic flow of data](#flat-and-deterministic-flow-of-data)
  - [Orthogonal and compact game logic](#orthogonal-and-compact-game-logic)
  - [Modularity and reusability](#modularity-and-reusability)
  - [Conclusions](#conclusions)

# Systems Architecture is not an ECS!

This is very important to say upfront:

> **Although at first glance Systems Architecture API looks somewhat like ECS, it
definitely is not one.**

There are similarities in both, but ECS do more than treat game as database, and
its design choices revolve around best performance possible for massive worlds and
making improved productivity and reduction of complexity is a side-effect of that,
while Systems Architecture doesn't change much to the Unreal Engine API, rather
resuses its parts (like actors and actor components) and only adds an API to enable
accessing and processing game world in database-like fashion, making Systems
Architecture focusing mostly on ergonomics and reducement of overall complexity.

That doesn't mean Systems Architecture doesn't care about performance - it already
makes its internals do as less work as possible, and more optimizations are gonna
arrive in further development, but the grand goal is and will be always making
Unreal Engine game developers life as easier as possible.

That being said, Systems Architecture took great amount of inspiration from ECS,
mostly from Rust ECS-based libraries and game engines that proven over time that
database-like DOD approach to game logic is not only viable, but also highly
beneficial for faster and easier game development process:
- [**`hecs`**](https://github.com/Ralith/hecs)
- [**`Oxygengine`**](https://github.com/PsichiX/Oxygengine)
- [**`Bevy`**](https://github.com/bevyengine/bevy)

As well as Rust [**`lazy-iterators`**](https://doc.rust-lang.org/stable/std/iter/index.html)
and [**`channel`**](https://doc.rust-lang.org/stable/std/sync/mpsc/index.html) APIs.

# Goals

## More Data-Oriented-Design approach to game logic

Most infromation you can find prasing DOD focuses too much on its benefits to game performance
with huge worlds. I would safely say that this actually shouldn't be the main thing DOD should
be recognized by, there is even more important benefit of thinking in terms of data and data
processing rather than communication between objects (OOP) - **avoiding common pitfalls when
trying to model game systems around _Real World Things <sup>TM</sup>_**.

> All game is, is just a bunch of data in memory that gets transformed from one form to another.

When we realize that, we can better and easier solve problems by designing data and game logic
that works best together and do as less work as possible, instead of adding lots of usually
unnecessary abstraction layers just because we might have a false intuition that representing data
and logic in human-readable real world representation somehow will make us understand it better
and quicker, while after year of development on each game, we find ourselves struggling with
adding, removing or just changing how certain parts of game logic works, slapping hacky solutions
here and there just to meet deadlines (we can't remove deadlines but we can reduce complexity).

We all have been through that, and we all promise ourselves **we are not gonna make same mistakes
again in next project**, we just need to "better design how our objects communicate", while making
totally new and even more problematic mistakes along the way - we can all agree that this next
project is never actually free from mistakes we did last time, they all share same source.

> That being said DOD isn't a miracle cure that solves all our problems, it brings its own to the
table.

But all these problems revolve around us still trying to slap our (false) intuition about objects
communication into problems that do not require these in the first place, us not being able to
easily switch our brains from thinking in terms of object communication into thinking in terms of
data transformations.

Yes, this takes time to appreciate data, but it pays off eventually with us being able to solve
problems right way, not creating workarounds here ant there just to follow tons of abstraction
layers requirements.

**I hope this book will do its best to speed up switching from OOP to more DOD approach, by showing
how to simplify problems without using tons of design patterns.**

## Flat and deterministic flow of data

Events-based logic approach commonly used in Unreal Engine is great for triggering foreign actions
(a.k.a. objects messaging) but has one big flaw - **it's not scalable** as project grows. This happens
because user have very limited control over order of actions happening when using events, and it
grows into even bigger problem as dependency graph between parts of event-based logic grows.

> Dependency graph in events-based game logic is a set of hidden rules of what action requires what
other action to run before and after this one, to make sure objects being communicated have valid and
expected state at each point of action execution. Hidden, because these rules emerge over time without
user noticing them.

Basically any situation where feature A to run properly, depends on feature B to have a certain data,
but that data is provided by feature C and so on, and there is no clear way to determine the order of
actions that prepare data for any other feature in all code points related to this problem.

Sometimes it gets even worse when features triggers events that receivers triggers their events and
their receivers triggers their events and so on - you can see where i'm going with it, this quickly
turns into _**The Fuck Cascade <sup>TM</sup>**_ of tons of state recalculation along the way in a
single point in time, just to keep all objects always have valid state that depends on other game
features state. This is clearly unmanageable and requires a lot of refactoring to either remove
dependencies between them or to defer operations in time. This is something user usually do not put
much thought into, until a *really weird bug happen in some place unrelated to the problem*.

**Ok, so what we can do about it?**

Well, **flatten dependency graph** and put execution of game features in line as much as possible.
With that we reduce cognitive complexity to bare minimum, and user can easily manage exact order of
actions to make sure state is always valid and do not need to recalculate on each change causing
cascade of instant changes in features that depend on it - **TL;DR: user can see exact flow of data
in one place and that makes it easy to reason about, no need for juggling around the whole codebase**.

In case user still wants to send data with events, but keep deterministic events execution, Systems
Architecture provides feature called [channels](/book/architecture/channels.md) which uses polling
to get received messages and execute them at the exact moment system needs to, avoiding immediate
message execution when broadcasting messages.

There is also another benefit we gain from flat dependency graph, that we will discuss next:

## Orthogonal and compact game logic

Quote from Eric Steven Raymond in
[Art of UNIX programming](http://www.catb.org/~esr/writings/taoup/html/ch04s02.html#orthogonality):
> Orthogonality is one of the most important properties that can help make even complex designs
compact. In a purely orthogonal design, operations do not have side effects; each action (whether
it's an API call, a macro invocation, or a language operation) changes just one thing without
affecting others. There is one and only one way to change each property of whatever system you are
controlling.

What intrests us game developers from this quote is that when we express game logic in a set of
simple work units we can easier reduce (or even remove) dependencies between parts of game logic and
that allows us to easily reason about them (small code) and add/change/remove them without much time
spend on refactoring (next to none dependencies between game logic parts).

> Orthogonal game systems are ones that do not require each other to work (like you remove one and
your game still runs), but they together make up bigger feature.

**How Systems Architecture helps to achieve that?**

Well, it helps by forcing user to think about data transformations instead of object communication.
**When we do not have to deal with abstractions, what's left is data and how we process that data.**
That allows to focus on solving actual problem instead of trying to fit it into some design pattern.

Because systems are all about data transformations, and System Architecture provides an ergonomic
way for performing these (such as queries over game world and iterators), each system by the nature
of data it works on, will keep itself doing only the work it needs and not causing hidden cascades of
changes in the world, **making every change explicit and intentional**.

Simple example would be: instead of having per unit movement that handles input, position change and
physics at once, you create separate systems:
- input system that collects player input and turns it into input data stored either in a global
    resource or in some actor component, it depends on what you want to do with this data later.

    > You should always design your data around what it's gonna be used for, not the other way around!
- velocity resolve system that reads input data available for given actor and converts these inputs
    into velocity change and stores it in velocity component.
- position change system that reads velocity and applies it to unit position.

You might think: "ok, but this particular example seems to be an easy task that _can_ be just executed
all in one place" - and you couldn't be wrong more.

Look at the bigger picture - few months later you'll need to extend or replace physics, or you would
need to add another physics resolvers that work on unit velocity and at that point this code gets bigger.
You obviously split it into smaller methods, but now designers wants you to let them parameterize or even
disable some of its parts at will - well, hello **Refactor Tractor <sup>TM</sup>** my old friend.
If you split these into smaller batched work units, you gain both easy way to toggle each stage of
movement and even be able to quickly change or even remove/replace with brand new ones, not to mention
being able to quickly add parameterization for designers in matter of minutes to hour, not days.

Like i said before:
> Keeping systems small makes it easy to reason about them and that is a one huge productivity booster
that you'll love to still have after year of development on your game.

## Modularity and reusability

Modularity, which means we can build complex features from smaller building blocks, gives us an ability
to reuse or even share these modules with other developers (free or paid, on github or marketplace).
In companies it is even more important to share and reuse certain modules between different teams and
products, as demand for making more by doing less grows as company structure and portfolio grows.

Systems Architecture, by its design, allows to put generalized systems and components into separate
modules, with none (or sometimes next to none) dependencies between other parts of the game. Are you
making a simulation game with NPCs wandering around on the map in one project, then in next one you need
to make them too? Put these in a module or just migrate them to another project. Code that doesn't
compile? Fear no more, you have got decoupled systems at the moment you've started making them, there is
next to none work needed after migration, just register systems into pipeline and slap these components
on actors and run the game.

**For indies and Unreal Engine plugin creators there are also great news!**

Systems Architecture core design, when treated as platform/framework, enables Unreal Engine plugin
developers to finally **create an ecosystem of plugins trully compatible with each other**. Ecosystem
of compatible plugins that do not require a hussle of installation steps, makes plugins more accessible
for users that want game features that **Just Works <sup>TM</sup>** and creates opportunity for more
sales for plugin devs - this is a big win for both worlds, users and creators.

And as a bonus:
> **System Architecture is and will always remain free** under permissive licenses, for anyone, accepting
reasonable contributions that improve its ergonomics, quality and performance.

# Conclusions

If you should take anything from this, remember:
> **When making games, you're basically just processing data and you don't need fancy
distraction-abstractions for that.**

- Make it **flat**
- Make it **small**
- Make it **compact and disconnected from other systems**

Then you'll keep your sanity after year of development on a single game title.
