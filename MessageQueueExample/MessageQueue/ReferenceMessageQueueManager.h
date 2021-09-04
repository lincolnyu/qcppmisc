#pragma once

#include "IMessageQueueManager.h"

#include <map>

class ReferenceMessageQueueManager : public IMessageQueueManager
{
    // Inherited via IMessageQueueManager
    virtual bool CreateMessageQueue(const std::string& in_queueName) override;
    virtual bool PostMessage(const std::string& in_queueName, const std::string& in_message) override;
    virtual bool CreateSubscription(const std::string& in_queueName, SubscriptionHandle& out_handle) override;
    virtual bool GetSubscriptionCount(const std::string& in_queueName, int& out_queueSubscriberCount) override;
    virtual bool GetNextMessage(SubscriptionHandle in_handle, std::string& out_message) override;
};

