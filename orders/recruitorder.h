#ifndef RECRUITORDER_H
#define RECRUITORDER_H

#include <orders/order.h>

class Unit;
class Building;
class RecruitOrder : public Order {
	public:
		Unit &unit;
		Building &source;
		RecruitOrder ( Unit &unit, Building &source );
		virtual bool execute () override;
		virtual bool operator== ( const Building &rhs ) const override;
};

#endif // RECRUITORDER_H
