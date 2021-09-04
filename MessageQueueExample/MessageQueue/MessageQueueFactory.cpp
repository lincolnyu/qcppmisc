#include "IMessageQueueManager.h"

#include "SimpleMessageQueueManager.h"

std::unique_ptr<class IMessageQueueManager> CreateMessageQueueManager()
{
    return std::make_unique<SimpleMessageQueueManager>();
}
