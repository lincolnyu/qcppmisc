#include "ReferenceMessageQueueManager.h"

bool ReferenceMessageQueueManager::CreateMessageQueue(const std::string& in_queueName)
{
    return false;
}

bool ReferenceMessageQueueManager::PostMessage(const std::string& in_queueName, const std::string& in_message)
{
    return false;
}

bool ReferenceMessageQueueManager::CreateSubscription(const std::string& in_queueName, SubscriptionHandle& out_handle)
{
    return false;
}

bool ReferenceMessageQueueManager::GetSubscriptionCount(const std::string& in_queueName, int& out_queueSubscriberCount)
{
    return false;
}

bool ReferenceMessageQueueManager::GetNextMessage(SubscriptionHandle in_handle, std::string& out_message)
{
    return false;
}
