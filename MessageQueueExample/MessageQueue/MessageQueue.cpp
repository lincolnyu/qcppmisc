#include "IMessageQueueManager.h"

#include "SimpleMessageQueueManager.h"

#include <assert.h>

void myassert(bool b)
{
	if (!b)
	{
		assert(false);
		exit(1);
	}
}

int main()
{
	auto manager = CreateMessageQueueManager();

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Add unit test code here to demonstrate that your manager works correctly

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	bool res = manager->CreateMessageQueue("Message Queue 1");
	myassert(res);
	res = manager->CreateMessageQueue("Message Queue 1");
	myassert(!res);
	res = manager->CreateMessageQueue("Message Queue 2");
	myassert(res);
	res = manager->CreateMessageQueue("Message Queue 3");
	myassert(res);
	res = manager->CreateMessageQueue("Message Queue 2");
	myassert(!res);

	SubscriptionHandle bad_handle;
	res = manager->CreateSubscription("Message Queue A", bad_handle);
	myassert(!res);

	SubscriptionHandle sub_q1_1;
	res = manager->CreateSubscription("Message Queue 1", sub_q1_1);
	myassert(res);

	SubscriptionHandle sub_q1_2;
	res = manager->CreateSubscription("Message Queue 1", sub_q1_2);
	myassert(res);

	SubscriptionHandle sub_q1_3;
	res = manager->CreateSubscription("Message Queue 1", sub_q1_3);
	myassert(res);

	SubscriptionHandle sub_q2_1;
	res = manager->CreateSubscription("Message Queue 2", sub_q2_1);
	myassert(res);

	SubscriptionHandle sub_q2_2;
	res = manager->CreateSubscription("Message Queue 2", sub_q2_2);
	myassert(res);

	SubscriptionHandle sub_q3_1;
	res = manager->CreateSubscription("Message Queue 3", sub_q3_1);
	myassert(res);

	int subscriber_count;
	res = manager->GetSubscriptionCount("Message Queue C", subscriber_count);
	myassert(!res);

	res = manager->GetSubscriptionCount("Message Queue 1", subscriber_count);
	myassert(res);
	myassert(subscriber_count == 3);

	res = manager->PostMessage("Message Queue B", "Sorry");
	myassert(!res);

	res = manager->PostMessage("Message Queue 1", "Hello");
	myassert(res);

	res = manager->PostMessage("Message Queue 1", "This is a");
	myassert(res);

	std::string out_message;
	res = manager->GetNextMessage(sub_q1_1, out_message);
	myassert(res);
	myassert(out_message == "Hello");

	res = manager->PostMessage("Message Queue 1", "Message to");
	myassert(res);

	res = manager->PostMessage("Message Queue 2", "Bonjour");
	myassert(res);

	res = manager->PostMessage("Message Queue 1", "");
	myassert(!res);

	res = manager->GetNextMessage(sub_q1_1, out_message);
	myassert(res);
	myassert(out_message == "This is a");

	res = manager->PostMessage("Message Queue 1", "all Queue 1 subscribers");
	myassert(res);

	res = manager->GetNextMessage(sub_q1_1, out_message);
	myassert(res);
	myassert(out_message == "Message to");

	res = manager->GetNextMessage(sub_q1_3, out_message);
	myassert(res);
	myassert(out_message == "Hello");

	res = manager->GetNextMessage(sub_q1_1, out_message);
	myassert(res);
	myassert(out_message == "all Queue 1 subscribers");

	SubscriptionHandle sub_q1_4;
	res = manager->CreateSubscription("Message Queue 1", sub_q1_4);
	myassert(res);

	res = manager->GetNextMessage(sub_q1_4, out_message);
	myassert(res);
	myassert(out_message == "");

	res = manager->GetNextMessage(sub_q1_2, out_message);
	myassert(res);
	myassert(out_message == "Hello");

	res = manager->GetNextMessage(sub_q1_1, out_message);
	myassert(res);
	myassert(out_message == "");

	SubscriptionHandle sub_q1_5;
	res = manager->CreateSubscription("Message Queue 1", sub_q1_5);
	myassert(res);

	res = manager->GetNextMessage(sub_q1_5, out_message);
	myassert(res);
	myassert(out_message == "");

	res = manager->GetNextMessage(sub_q2_1, out_message);
	myassert(res);
	myassert(out_message == "Bonjour");

	res = manager->GetNextMessage(sub_q2_1, out_message);
	myassert(res);
	myassert(out_message == "");

	res = manager->GetSubscriptionCount("Message Queue 2", subscriber_count);
	myassert(res);
	myassert(subscriber_count == 2);

	res = manager->GetNextMessage(sub_q3_1, out_message);
	myassert(res);
	myassert(out_message == "");

	res = manager->GetNextMessage(sub_q1_2, out_message);
	myassert(res);
	myassert(out_message == "This is a");
	res = manager->GetNextMessage(sub_q1_3, out_message);
	myassert(res);
	myassert(out_message == "This is a");
	res = manager->GetNextMessage(sub_q1_4, out_message);
	myassert(res);
	myassert(out_message == "");

	res = manager->GetNextMessage(sub_q1_2, out_message);
	myassert(res);
	myassert(out_message == "Message to");
	res = manager->GetNextMessage(sub_q1_3, out_message);
	myassert(res);
	myassert(out_message == "Message to");
	res = manager->GetNextMessage(sub_q1_4, out_message);
	myassert(res);
	myassert(out_message == "");
	res = manager->GetNextMessage(sub_q1_5, out_message);
	myassert(res);
	myassert(out_message == "");

	res = manager->GetNextMessage(sub_q1_1, out_message);
	myassert(res);
	myassert(out_message == "");

	res = manager->GetNextMessage(sub_q1_2, out_message);
	myassert(res);
	myassert(out_message == "all Queue 1 subscribers");
	res = manager->GetNextMessage(sub_q1_3, out_message);
	myassert(res);
	myassert(out_message == "all Queue 1 subscribers");
	res = manager->GetNextMessage(sub_q1_4, out_message);
	myassert(res);
	myassert(out_message == "");
	res = manager->GetNextMessage(sub_q1_5, out_message);
	myassert(res);
	myassert(out_message == "");

	res = manager->GetNextMessage(sub_q1_2, out_message);
	myassert(res);
	myassert(out_message == "");
	res = manager->GetNextMessage(sub_q1_3, out_message);
	myassert(res);
	myassert(out_message == "");
	res = manager->GetNextMessage(sub_q1_4, out_message);
	myassert(res);
	myassert(out_message == "");
	res = manager->GetNextMessage(sub_q1_5, out_message);
	myassert(res);
	myassert(out_message == "");

	/*auto simple_manager = dynamic_cast<SimpleMessageQueueManager*>(manager.get());
	if (simple_manager)
	{
		int messageCount = 0;
		myassert(simple_manager->Debug_GetQueueMessageCount("Message Queue 1", messageCount));
		myassert(messageCount == 0);
	}*/

	manager->PostMessage("Message Queue 1", "Bye");
	res = manager->GetNextMessage(sub_q1_1, out_message);
	myassert(res);
	myassert(out_message == "Bye");
	res = manager->GetNextMessage(sub_q1_2, out_message);
	myassert(res);
	myassert(out_message == "Bye");
	res = manager->GetNextMessage(sub_q1_3, out_message);
	myassert(res);
	myassert(out_message == "Bye");
	res = manager->GetNextMessage(sub_q1_4, out_message);
	myassert(res);
	myassert(out_message == "Bye");
	res = manager->GetNextMessage(sub_q1_5, out_message);
	myassert(res);
	myassert(out_message == "Bye");

	//// stress test
	//{
	//	std::string queue_names[] = {
	//	"Message Queue 1",
	//	"Message Queue 2",
	//	"Message Queue 3",
	//	"Message Queue 4",
	//	};

	//	SubscriptionHandle handles[] = {
	//		sub_q1_1,
	//		sub_q1_2,
	//		sub_q1_3,
	//		sub_q1_4,
	//		sub_q1_5,
	//		sub_q2_1,
	//		sub_q2_2,
	//		sub_q3_1
	//	};

	//	const int num_handles = sizeof(handles) / sizeof(SubscriptionHandle);

	//	for (int i = 0; i < 10000; i++)
	//	{
	//		int ip = rand() % 10;
	//		if (ip > 4)
	//		{
	//			auto iqn = rand() % 4;
	//			auto qname = queue_names[iqn];
	//			char c = rand() % 256;
	//			std::string msg{ c };
	//			bool res = manager->PostMessage(qname, msg);
	//			myassert(res == (iqn < 3));
	//		}
	//		else
	//		{
	//			auto ihandle = rand() % num_handles;
	//			std::string msg;
	//			auto handle = handles[ihandle];
	//			bool res = manager->GetNextMessage(handle, msg);
	//			myassert(res);
	//		}
	//	}

	//	for (int i = 0; i < 10000; i++)
	//	{
	//		for (int j = 0; j < num_handles; j++)
	//		{
	//			std::string msg;
	//			auto handle = handles[j];
	//			bool res = manager->GetNextMessage(handle, msg);
	//			myassert(res);
	//		}
	//	}
	//	for (int j = 0; j < num_handles; j++)
	//	{
	//		std::string msg;
	//		auto handle = handles[j];
	//		bool res = manager->GetNextMessage(handle, msg);
	//		myassert(res);
	//		myassert(msg == "");
	//	}
	//}

	return 0;
}
