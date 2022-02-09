# Iterators

# Table of contents

- [Explanation](#explanation)
- [Types of iterators](#types-of-iterators)
- [Creating custom iterators](#creating-custom-iterators)
    - [Anatomy of iterators](#anatomy-of-iterators)
    - [Iterator adapters](#iterator-adapters)

# Explanation

Iterators, or more precisely lazy iterators, are an useful tool for more efficient and
ergonomic processing of collections and procedural generators. Lazy iterators are lazy
because on their own they do not yield any values until consumed.

```snippet
iter_map
```

This lazyness allows us to produce more elaborated processing over data without making
unnecessary caching between iteration stages, and that also makes iteration code cleaner
and easier to reason about.

# Types of iterators

- Consumers

    Consumers are final iterators that consume yielded values and produce some result out
    of it, usually single value.

    ```snippet
    iter_find
    ```

    - [`struct: TQuery::Count`]()
    - [`struct: TQuery::Nth`]()
    - [`struct: TQuery::Fold`]()
    - `IterSum`
    - [`struct: TQuery::Nth`]()
    - [`struct: TQuery::Find`]()
    - [`struct: TQuery::FindMap`]()
    - [`struct: TQuery::First`]()
    - [`struct: TQuery::Last`]()
    - [`struct: TQuery::ForEach`]()
    - [`struct: TQuery::Any`]()
    - [`struct: TQuery::All`]()
    - [`struct: TQuery::ComparedBy`]()

- Colectors

    Collectors are special kind of consumer iterators that are an explicit way to collect
    all yielded values into commonly used collections such as `TArray`, `TSet` and `TMap`.

    ```snippet
    iter_collect_array
    ```

    - [`struct: TQuery::CollectArray`]()
    - [`struct: TQuery::CollectIntoArray`]()
    - [`struct: TQuery::CollectSet`]()
    - [`struct: TQuery::CollectIntoSet`]()
    - `IterCollectMap`
    - `IterCollectIntoMap`

- Converters

    Converters are stages of iteration that converts values yielded by previous iteration
    stages into some new values.

    ```snippet
    iter_map
    ```

    - [`struct: TQuery::Adapt`]()
    - [`struct: TQuery::Chain`]()
    - [`struct: TQuery::Enumerate`]()
    - [`struct: TQuery::Filter`]()
    - [`struct: TQuery::FilterMap`]()
    - [`struct: TQuery::Flatten`]()
    - [`struct: TQuery::Inspect`]()
    - [`struct: TQuery::Map`]()
    - [`struct: TQuery::Skip`]()
    - [`struct: TQuery::Take`]()
    - [`struct: TQuery::Zip`]()

# Creating custom iterators

While built-in iterators provided by SystemsArchitecture should be enough for common usecases,
sometimes for more advanced solutions you might find yourself in a need of creating a custom
iterator that cannot be solved using built-in ones.

## Anatomy if iterators

Let's take a look at Repeat iterator:

```snippet
iterator_anatomy
```

- `Self` type alias

    Iterators chaining is done by wrapping consecutive iterators in one another so when consumer
    calls `Next`, it internally calls `Next` of iterator it wraps, and so on. To make them easily
    injectable with macros, they use `Self` as an alias for their type.

- `Item` type alias

    Type alias for type of value that this iterator yields. Not stores, not takes as an input -
    exactly one that it yields. This is again used for iterators wrapping purposes so converter
    iterators that wraps other iterators can identify value type of previous one in chain by
    `typename ITERATOR::Item`.

- `Next` method

    Every iterator should implement `TOptional<Item> Next()` method. This method does the actual
    job of yielding a value.

    See [`struct: TQuery::Next`]().

- `SizeHint` method.

    Size hints are used by for example collector iterators to estimate the capacity of collection
    where yielded data will be stored. This at most removes reallocations when adding every next
    value into that collection, and at least reduces it to some reasonable number.

    Finite iterators will give lower bounds and upper bounds set, infinite iterators will give
    only lower bounds.

    See [`struct: TQuery::SizeHint`](), [`struct: IterSizeHint`]().

- `ITER_IMPL` macro

    This macro injects other iterators as methods of this one. Without it user would be left with
    ugly iterators chains made by passing next stages as argument to iterator functions/constructors.

## Iterator adapters

In some rare cases you might find yourself struggling to express your data processing with built-in
iterators, or you must to make some optimizations that cannot be solved using provided ones. For
this advanced usecase you can create custom iterator adapters that works basically the same way as
converter iterators.

```snippet
iter_adapter
```

```snippet
iter_adapt
```

They differ from regular iterators in a way that they do not neet type aliases and their `Next` and
`SizeHint` methods require reference to previous stage iterator so they both consume and process its
yielded values.
