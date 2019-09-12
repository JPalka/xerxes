#ifndef SCAVENGEORDER_H
#define SCAVENGEORDER_H

#include <orders/order.h>

class Place;
class ScavengeOrder : public Order {
	public:
		Place &source;
		std::vector<std::pair<std::string, int>> units;
		Resources loot;
		int level;
		virtual bool execute () override;
		virtual bool operator== ( const Building &rhs ) const override;
		ScavengeOrder ( Place &source, std::vector<std::pair<std::string, int>> unitsSend, Resources loot, int level );
};

#endif // SCAVENGEORDER_H
