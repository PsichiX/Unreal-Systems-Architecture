# Resources

# Table of contents

- [Explanation](#explanation)
- [Problems that it solves](#problems-that-it-solves)

# Explanation

Resource in context of Systems Architecture is any global object that all systems should
be able to access (though resources definitely **aren't singletons!** - they share their
lifetime with owning systems world). Objects such as:

- Configuration/settings data
    
    For example systems run criteria that tells the conditions under what certain systems
    can run. Or Data Assets / Data Tables passed from game instance to systems world.

- Cached data shared between systems

    Do you have spatialization system that builds RTree for world actors to speed up spatial
    queries, and other systems in need to find the closest actor to their location? You
    definitely put that data in a resource object and make other systems query and read
    that resource!

- Game managers

    Common Unreal Engine game development pattern is to create work units called "managers"
    that manage certain state. For some solutions, epecially when dealing with third-party
    plugins, we have to still have access into these managers in our systems - definitely put
    them in systems world resources. 

```snippet
resources
```

# Problems that it solves

While in most cases all what you do is you process game world queries, sometimes you require
to access some configuration data, or cache some data for use by many other systems, or you
just need to interact with third-party game managers - these are useful usecases and Systems
Architecture doesn't get rid of them, rather make their use more ergonomic, yet not giving
them global lifetime like with dangerous singletons.
