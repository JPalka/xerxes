#ifndef ORDER_H
#define ORDER_H

#include <game/village.h>
#include <buildings/building.h>

class Order {
	public:
		int id;
		long int dueTime;
		long int startTime;
		std::string cancelSlug;
		std::string text; //Text that describes Order in some hooman fashion. Not actually used for anything
		Order();
		virtual bool execute () = 0;
		virtual bool operator== ( const Building &rhs ) const = 0;
		virtual bool operator< ( const Order &rhs ) const {
			return this->dueTime < rhs.dueTime;
		}
		virtual bool operator> ( const Order &rhs ) const {
			return this->dueTime > rhs.dueTime;
		}
		class OrderByDueTime {
			public:
				bool operator () ( const std::unique_ptr<Order> &lhs, const std::unique_ptr<Order> &rhs ) const {
					return lhs > rhs;
				}
		};
};

#endif // ORDER_H
