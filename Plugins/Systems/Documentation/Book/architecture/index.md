# Architecture

This section will explain in-depth how Systems Architecture exactly works and how to abuse
it to solve your problems the right way.

Systems Architecture adds a database-like interface over game world to allow ergonomic
queries for game world data processing.

- ## Actor components should only store data
    
    Or no data, if they are considered tags, used as markers over actors. Tags such as
    `UEnemyComponent` that enables this actor to be queried in systems that works on enemies.

    And sometimes methods, but only these that do not have side effects reaching outside of
    this actor component scope - the point is: it's systems job to perform game logic, actor
    component methods should at most contain methods that recalculate their internal state,
    and these are operations called in more than one place.

    This rule gets invalidated for situations like actor component being a bridge to some
    Unreal Engine built-in components or third-party code to which handles are stored in
    actor components. Just make sure your own actor components do as less work as possible.

- ## Systems do most if not all game logic

    This means whenever you want to put some commonly used piece of logic in actor component
    or (gods forbid) in actor, make a system for it.

    It will pay off months later by still having small orthogonal game systems with as less
    dependencies as possible, systems that can be easily toggled, changed or even
    removed/replaced - no more hours or days of careful code refactoring just because you need
    to change or remove some bit of game feature.

- ## Your game world is just a database

    Systems Architecture gives you ergonomic tools to ease work on batched sets of actor
    components using game world queries.

    ```snippet
    system_run
    ```

    With queries you have all this system logic in one place - that greatly reduces comprehensive
    complexity, no more need to jump around whole codebase just to understand how system works,
    and **we make ourselves keep systems small, doing only only the work they need and no other**.
