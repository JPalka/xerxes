#ifndef ORDERPROCESSOR_H
#define ORDERPROCESSOR_H

#include <boost/heap/priority_queue.hpp>
#include <orders/order.h>

class OrderProcessor {
	private:
		boost::heap::priority_queue<std::unique_ptr<Order>, boost::heap::compare<Order::OrderByDueTime>> ordersQueue;
	public:
		bool addOrder ( std::unique_ptr<Order> newOrder );
		int processOrders ( long int timestamp ); // Executes orders that are due in timestamp
		std::vector<Order*> getOrders ( Building &source );
		Order &top ();
		bool empty ();
		OrderProcessor();
};

#endif // ORDERPROCESSOR_H
