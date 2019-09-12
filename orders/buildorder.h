#ifndef BUILDORDER_H
#define BUILDORDER_H

#include <orders/order.h>

class Building;
class Village;
class BuildOrder : public Order
{
	public:
//		std::string buildingName;
		int newLevel;
//		int villageId;
		Building &target; // Building being upgraded
		Building &source; // In case of build order source is HQ building that places order
		BuildOrder ( Building &target, Building &source );
		virtual bool execute () override;
		virtual bool operator== ( const Building &rhs ) const override;
};

#endif // BUILDORDER_H
