#include "orderprocessor.h"

#include <orders/order.h>
#include <orders/recruitorder.h>
#include <orders/buildorder.h>
#include <orders/scavengeorder.h>

bool OrderProcessor::addOrder( std::unique_ptr<Order> newOrder) {
	this->ordersQueue.emplace ( std::move ( newOrder ) );
//	this->ordersQueue.add ( );
	return true;
}

int OrderProcessor::processOrders ( long int timestamp ) {
	int executedCount = 0;
	while ( !this->ordersQueue.empty () && this->ordersQueue.top ()->dueTime <= timestamp ) {
		this->ordersQueue.top ()->execute(); // Execute
		this->ordersQueue.pop (); // Yeet order from queue. Perhaps add it to some sort of log
		executedCount++;
	}
	return executedCount;
}
/*
 * Get orders that originated from given building
 */
std::vector<Order*> OrderProcessor::getOrders ( Building &source ) {
	std::vector<Order*> result;
	for ( auto it = this->ordersQueue.begin (); it != this->ordersQueue.end (); it++ ) {
		if ( **it == source ) {
			result.push_back ( (*it).get() );
		}
	}
	std::sort ( result.begin (), result.end (), [](Order* lhs, Order* rhs) {
		return *lhs > *rhs;
	});
	result.shrink_to_fit ();
	return result;
}

Order &OrderProcessor::top() {
	return *this->ordersQueue.top ();
}

bool OrderProcessor::empty () {
	return this->ordersQueue.empty ();
}
OrderProcessor::OrderProcessor() {

}
