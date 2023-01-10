# Resources

# Table of contents

- [Explanation](#explanation)
  - [Proxy Resources](#proxy-resources)
- [Problems that it solves](#problems-that-it-solves)

# Explanation

Resource in context of Systems Architecture is any global object that all systems should
be able to access (though resources definitely **aren't singletons!** - they share their
lifetime with owning systems world). Objects such as:

- Configuration/settings data
    
    For example systems run criteria that tells the conditions under what certain systems
    can run. Or Data Assets / Data Tables passed from game instance to systems world.

- Cached data shared between systems

    Do you have spatial partitioning system that builds RTree for world actors to speed up
    spatial queries, and other systems in need to find the closest actor to their location?
    You definitely put that data in a resource object and make other systems query and read
    that resource!

- Game managers

    Common Unreal Engine game development pattern is to create work units called "managers"
    that manage certain state. For some solutions, especially when dealing with third-party
    plugins, we have to still have access into these managers in our systems - definitely put
    them in systems world resources. 

```snippet
resources
```

## Proxy Resources

Along the journey you might find yourself in a need for having to use wrapper-like resources
that themselves are not unique types but they wrap unique type resource - wrappers/containers
for some inner data. For that scenario you will use concept called Proxy Resources, which
allows you to install resource wrapper, which of type itself is not used, instead you provide
its inner type used for queries.

First we define proxy resource:
```snippet
proxy_resource_class
```

Then we install it in Systems World:
```snippet
systems_world_install_proxy_resource
```

And finally we access it:
```snippet
systems_world_proxy_resource
```

# Problems that it solves

While in most cases all what you do is you process game world queries, sometimes you require
to access some configuration data, or cache some data for use by many other systems, or you
just need to interact with third-party game managers - these are useful use cases and Systems
Architecture doesn't get rid of them, rather make their use more ergonomic, yet not giving
them global lifetime like with dangerous singletons.
