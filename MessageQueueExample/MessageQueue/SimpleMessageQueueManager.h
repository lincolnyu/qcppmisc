#pragma once

#include "IMessageQueueManager.h"

#include <list>
#include <queue>
#include <unordered_map>

class SimpleMessageQueueManager : public IMessageQueueManager
{
public:
	~SimpleMessageQueueManager() override;

	// Create a Message Queue with the given name.
	//
	// Returns false on failure, or if the given queue already exists
	bool CreateMessageQueue(const std::string& in_queueName) override;

	// Post a message to a message queue.
	// - Posting an empty message is considered a failure case.
	// - Messages should be stored internally until all subscribers have received them via a GetNextMessage call.
	//
	// Returns false on failure, or if the given queue does not exist.
	bool PostMessage(const std::string& in_queueName, const std::string& in_message) override;

	// Create a unique subscription to a given message queue.
	// - Returns a handle to the subscription in out_handle
	// - The subscription must only receive *new* messages created by PostMessage calls that occur *after* the CreateSubscription call
	//
	// Returns false on failure, or if the given queue does not exist
	bool CreateSubscription(const std::string& in_queueName, SubscriptionHandle& out_handle) override;

	// Query for the number of subscribers for the specified queue.
	// - Returns the count of subscribers in out_queueSubscriberCount
	// 
	// Returns false if the given queue does not exist
	bool GetSubscriptionCount(const std::string& in_queueName, int& out_queueSubscriberCount) override;

	// Get the next message from the message queue for a given subscription
	// - Returns the message in out_message, and advances the subscription to the following message
	// - If there are no more messages for the subscription, returns true and out_message is set to an empty string
	// 
	// Returns false on failure 
	bool GetNextMessage(SubscriptionHandle in_handle, std::string& out_message) override;

	// For debug
	bool Debug_GetQueueMessageCount(const std::string& in_queueName, size_t& out_queueMessageCount);

private:
	struct Message
	{
		std::string _content;
		int			_waiting_subscribers_count = 0;		// Number of subscribers that are to read this message next
		bool		_is_dummy = true;					// By default is dummy
	};

	struct Subscriber;

	using SubscriberPtr = std::shared_ptr<Subscriber>;

	struct MessageQueue;
	using MessageQueuePtr = std::shared_ptr<MessageQueue>;
	using MessageList = std::list<Message>;
	using MessageItr = std::list<Message>::iterator;

	struct MessageQueue
	{
		std::string					_name;
		MessageList					_messages;		// The queued messages that are needed
		int							_total_subscriber_count = 0;

		MessageQueue(const std::string& name);
	};

	struct Subscriber
	{
		SubscriptionHandle	_handle;
		MessageQueuePtr		_message_queue;		// The message queue the subscriber subscribed to
		MessageItr			_next_message;		// The iterator to the next message to read from the queue or end() of queue if all existing messages have been read

		Subscriber(SubscriptionHandle handle, MessageQueuePtr message_queue);
	};

private:
	MessageQueuePtr GetQueue(const std::string& in_queueName) const;

private:
	std::unordered_map<std::string, MessageQueuePtr>		_message_queues;				// All the message queues
	std::unordered_map<SubscriptionHandle, SubscriberPtr>	_subscribers;					// All the subscribers
	SubscriptionHandle										_next_subscriber_handle = 1;
};
