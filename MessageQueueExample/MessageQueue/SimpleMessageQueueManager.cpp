#include "SimpleMessageQueueManager.h"

#include <cassert>

SimpleMessageQueueManager::MessageQueue::MessageQueue(const std::string& name)
    : _name(name)
{
    _messages.emplace_back();   // dummy message at the tail
}

SimpleMessageQueueManager::Subscriber::Subscriber(SubscriptionHandle handle, MessageQueuePtr message_queue)
    : _handle(handle), _message_queue(message_queue)
{
}

SimpleMessageQueueManager::~SimpleMessageQueueManager()
{
}

bool SimpleMessageQueueManager::CreateMessageQueue(const std::string& in_queueName)
{
    if (GetQueue(in_queueName))
    {
        return false;
    }

    _message_queues.insert(std::make_pair(in_queueName, std::make_shared<MessageQueue>(in_queueName)));

    return true;
}

bool SimpleMessageQueueManager::PostMessage(const std::string& in_queueName, const std::string& in_message)
{
    if (in_message == "")
    {
        return false;
    }

    auto queue = GetQueue(in_queueName);
    if (!queue)
    {
        return false;
    }

    auto it_last = queue->_messages.end();
    --it_last;
    it_last->_content = in_message;         // turn the dummy message to normal message
    it_last->_is_dummy = false;
    queue->_messages.emplace_back();        // add dummy message to tail

    return true;
}

bool SimpleMessageQueueManager::CreateSubscription(const std::string& in_queueName, SubscriptionHandle& out_handle)
{
    auto queue = GetQueue(in_queueName);
    if (!queue)
    {
        return false;
    }

    auto new_handle = _next_subscriber_handle++;

    auto subscriber = std::make_shared<Subscriber>(new_handle, queue);
    subscriber->_next_message = queue->_messages.end();
    --subscriber->_next_message;    // point to the dummy message at the tail

    ++(queue->_total_subscriber_count);

    // TODO Assert. (Guaranteed to not exist though)
    _subscribers[new_handle] = subscriber;

    out_handle = new_handle;
    return true;
}

bool SimpleMessageQueueManager::GetSubscriptionCount(const std::string& in_queueName, int& out_queueSubscriberCount)
{
    auto queue = GetQueue(in_queueName);
    if (!queue)
    {
        return false;
    }
    out_queueSubscriberCount = queue->_total_subscriber_count;
    return true;
}

bool SimpleMessageQueueManager::GetNextMessage(SubscriptionHandle in_handle, std::string& out_message)
{
    auto it = _subscribers.find(in_handle);
    if (it == _subscribers.end())
    {
        return false;
    }
    auto& subscriber = it->second;

    auto& queue = subscriber->_message_queue;

    auto& next_msg_itr = subscriber->_next_message;

    // Consume the message and move to next
    out_message = next_msg_itr->_content; // Message may be destroyed, read the content now

    if (next_msg_itr->_is_dummy)
    {
        return true;
    }

    --(next_msg_itr->_waiting_subscribers_count);
    if (next_msg_itr->_waiting_subscribers_count == 0 && subscriber->_next_message == queue->_messages.begin())
    {
        // This message has been read by all subscribers
        queue->_messages.pop_front();
        next_msg_itr = queue->_messages.begin();
    }
    else
    {
        ++next_msg_itr;
    }

    ++(next_msg_itr->_waiting_subscribers_count);

    return true;
}

SimpleMessageQueueManager::MessageQueuePtr SimpleMessageQueueManager::GetQueue(const std::string& in_queueName) const
{
    auto it = _message_queues.find(in_queueName);
    if (it == _message_queues.end())
    {
        return nullptr;
    }
    return it->second;
}

bool SimpleMessageQueueManager::Debug_GetQueueMessageCount(const std::string& in_queueName, size_t& out_queueMessageCount)
{
    auto queue = GetQueue(in_queueName);
    if (!queue)
    {
        return false;
    }
    out_queueMessageCount = queue->_messages.size();
    return true;
}
