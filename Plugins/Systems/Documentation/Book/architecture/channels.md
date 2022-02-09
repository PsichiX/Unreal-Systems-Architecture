# Channels

# Table of contents

- [Explanation](#explanation)
- [Problems that it solves](#problems-that-it-solves)

# Explanation

See [`struct: TSenderChannel`](), [`struct: TReceiverChannel`]().

Shared channels are Single-Produce, Multiple-Consumer events that instead
being executed ad the time of sending message (pushing), they are executed
at the time of receiving, which is an on-demand operation - user decides
when to read messages received in event.

```snippet
shared_channel
```

Internally channels work as a ring buffer behind thread-safe shared pointer
in receiver channel, and sender channel only holds a weak pointer to list
of registered receivers. When you want to subscribe to a channel, you create
a receiver and store that receiver somewhere.

> It's important to note that receiver is bound to a sender channel as long
as it exists, so there is no need for intentional unbinds, it is all
automated by its design.

After you have sender and receivers made, when you tell sender channel to
send message, it goes through all weak pointers pointing to receiver
channels, and write cloned data into each of their queues. **Messages are
stored, but not yet executed**.

When you want to receive and execute a message, just make receiver read
sequence of next messages in queue and do something useful with them.
That's all, simple isn't it?

# Problems that it solves

There is only one but big problem that it solves - makind more
deterministic and ordered data flow between parts of game logic. That way
we avoid situations where objects communicating with each other are
dependend on proper order of messages being sent and received but we can't
ensure that order. We also avoid event cycles, where there is set of event
points that suddenly gets connected in a way that introduces a cycle of
passing messages, leading to game freezes/crashes due to stack overflow, or
simply just situations where some data gets passed too early and
invalidates dependent object state causing unnecessary state recalculations
later by other objects trying to get some data out of it.
