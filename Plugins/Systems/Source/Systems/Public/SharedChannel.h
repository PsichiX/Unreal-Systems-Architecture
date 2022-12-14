#pragma once

#include "CoreMinimal.h"

#include "Containers/RingBuffer.h"

template <typename T>
struct TReceiverChannel;

/// SPMC (single-prodicer, multiple-consumer) sender channel.
///
/// More in-depth explanation of channels can be found in [this architecture
/// book page](/book/architecture/channels.md).
///
/// Channels are similar to Unreal events, the difference is while channels
/// sends data that gets stored immediatelly on receiver queue, user have to
/// consume received messages by polling (intentionally asking for next message
/// in queue).
///
/// The benefit of polling over pushing (Unreal events) is that user has full
/// controll over when and where incoming messages are processed and executed,
/// and that makes user avoid random/undefined data flow which is common pitfall
/// when using events - with channels we bring determinism into the
/// communication between parts of the game.
///
/// # Note
/// > User can freely send messages across multiple separate threads, for
/// example if user spawn system on another thread to process some big chunk
/// of data and that system has to send it processing results back to game
/// thread.
///
/// > User might notice there is method for creating bound receiver, but there
/// is none fo destroying it - the reason is receivers are weakly connected
/// so they will automaticall unbound from sender channel as soon as they get
/// destroyed in their scope, and sender will tr to send any message to that
/// already gone receiver.
///
/// # Example
/// ```snippet
/// shared_channel
/// ```
template <typename T>
struct TSenderChannel
{
public:
	/// Sends message to all registered receivers.
	void Send(
		/// Data representation of given message to send.
		const T& Data)
	{
		//// [ignore]
		this->Queues.RemoveAllSwap(
			[Data](auto& Weak)
			{
				if (auto Strong = Weak.Pin())
				{
					Strong->Add(Data);
					return false;
				}
				return true;
			},
			false);
		//// [/ignore]
	}

	/// Creates receiver channel bound to this sender channel.
	TReceiverChannel<T> Receiver(
		/// Initial capacity of messages queue for created receiver channel.
		uint32 Capacity = 0)
	{
		//// [ignore]
		auto Queue = TRingBuffer<T>();
		Queue.Reserve(Capacity);
		const auto Strong = MakeShared<TRingBuffer<T>, ESPMode::ThreadSafe>(Queue);
		const auto Weak = TWeakPtr<TRingBuffer<T>, ESPMode::ThreadSafe>(Strong);
		this->Queues.Add(Weak);
		return TReceiverChannel<T>(Strong);
		//// [/ignore]
	}

	/// Returns number of actively bound receivers.
	uint32 Receivers() const
	{
		//// [ignore]
		return this->Queues.num();
		//// [/ignore]
	}

private:
	using Pointer = TWeakPtr<TRingBuffer<T>, ESPMode::ThreadSafe>;

	TArray<Pointer> Queues = {};
};

/// SPMC (single-prodicer, multiple-consumer) receiver channel.
///
/// See [`struct: TSenderChannel`]().
///
/// # Example
/// ```snippet
/// shared_channel
/// ```
// TODO: figure out if and how receiver should implement lazy-iterators.
template <typename T>
struct TReceiverChannel
{
	friend struct TSenderChannel<T>;

public:
	TReceiverChannel() : Queue()
	{
	}

	/// Checks if there are incoming messages in queue.
	bool IsEmpty() const
	{
		//// [ignore]
		return Queue.IsValid() && Queue->IsEmpty();
		//// [/ignore]
	}

	/// Clears internal queue.
	///
	/// User would rather want to read and conume stored messages but sometimes
	/// user might have valid reasons to discard all messages without reading
	/// them.
	void Clear()
	{
		//// [ignore]
		if (Queue.IsValid())
		{
			Queue->Reset();
		}
		//// [/ignore]
	}

	/// Unbind from sender.
	///
	/// This forcefully unbinds reciever from sender and clears internal queue,
	/// though user do not have to call this method when its lifetime ends -
	/// receivers unbind themselves as soon as they get destroyed. This method
	/// exists only in case user would want to stop receiving further messages
	/// on demand, useful in more advanced scenarios.
	void Unbind()
	{
		//// [ignore]
		if (Queue.IsValid())
		{
			Queue->Reset();
		}
		//// [/ignore]
	}

	/// Consumes and returns one message from internal queue.
	///
	/// # Example
	/// ```snippet
	/// shared_channel
	/// ```
	TOptional<T> Receive()
	{
		//// [ignore]
		return Queue.IsValid() && Queue->IsEmpty() == false ? TOptional<T>(Queue->PopFrontValue()) : TOptional<T>();
		//// [/ignore]
	}

private:
	using Pointer = TSharedPtr<TRingBuffer<T>, ESPMode::ThreadSafe>;

	TReceiverChannel(Pointer Strong) : Queue(Strong)
	{
	}

	Pointer Queue = {};
};
