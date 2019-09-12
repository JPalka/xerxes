TEMPLATE = app
CONFIG += console c++17
CONFIG -= app_bundle
CONFIG -= qt
CONFIG += c++17

INCLUDEPATH += /home/jony/boost_1_70_0
INCLUDEPATH += /usr/local/include/mongocxx/v_noabi \
	       /usr/local/include/bsoncxx/v_noabi \
	       /usr/local/include/libxml++-3.0 \
	       /usr/include/libxml2 \
	       /usr/include/glibmm-2.4 \
	       /usr/include/glib-2.0 \
	       /usr/include/sigc++-2.0 \
	       /usr/local/lib/libxml++-3.0/include \
	       /usr/lib/x86_64-linux-gnu/glibmm-2.4/include \
	       /usr/lib/x86_64-linux-gnu/glib-2.0/include \
	       /usr/lib/x86_64-linux-gnu/sigc++-2.0/include \

LIBS += -L/usr/local/lib -lmongocxx -lbsoncxx -lxml++-3.0 -lxml2 -lglibmm-2.4 -lgobject-2.0 -lglib-2.0 -lsigc-2.0
LIBS += "-L/usr/local/lib"
LIBS += -lpthread
LIBS += -lcrypt
LIBS += -lcrypto
LIBS += -lssl
#LIBS += "-L/home/jony/boost_1_70_0/stage/lib"
#LIBS += -lcpprest
LIBS += -lcurl
LIBS += -lcurlpp
LIBS += -lmongocxx
LIBS += -lbsoncxx
LIBS += -lz
LIBS += -lboost_iostreams
LIBS += -lmenuw
LIBS += -lformw
LIBS += -lpanelw
LIBS += -lncursesw

SOURCES += main.cpp \
    buildings/building.cpp \
    buildings/watchtower.cpp \
    buildings/market.cpp \
    buildings/wood.cpp \
    buildings/stone.cpp \
    buildings/iron.cpp \
    buildings/smith.cpp \
    buildings/snob.cpp \
    buildings/farm.cpp \
    buildings/statue.cpp \
    buildings/storage.cpp \
    buildings/hide.cpp \
    buildings/wall.cpp \
    buildings/barracks.cpp \
    buildings/church.cpp \
    buildings/garage.cpp \
    buildings/stable.cpp \
    buildings/place.cpp \
    buildings/headquarters.cpp \
    buildings/buildingfactory.cpp \
    units/unit.cpp \
    units/infantry.cpp \
    units/cavalry.cpp \
    units/archer.cpp \
    units/spear.cpp \
    units/sword.cpp \
    units/axe.cpp \
    units/ram.cpp \
    units/catapult.cpp \
    units/militia.cpp \
    units/paladin.cpp \
    units/light.cpp \
    units/heavy.cpp \
    units/farcher.cpp \
    units/marcher.cpp \
    units/snobunit.cpp \
    units/spy.cpp \
    units/unitfactory.cpp \
    misc/coordinate.cpp \
    misc/resources.cpp \
    game/game.cpp \
    game/worldlist.cpp \
    game/world.cpp \
    game/player.cpp \
    game/village.cpp \
    game/worldsettings.cpp \
    game/onlineclient.cpp \
    game/account.cpp \
    buildings/university.cpp \
    client/client.cpp \
    orders/order.cpp \
    orders/buildorder.cpp \
    misc/utils.cpp \
    orders/recruitorder.cpp \
    orders/scavengeorder.cpp \
    xerxes/xerxes.cpp \
    xerxes/ui.cpp \
    game/orderprocessor.cpp


HEADERS += \
    buildings/building.h \
    buildings/watchtower.h \
    buildings/market.h \
    buildings/wood.h \
    buildings/stone.h \
    buildings/iron.h \
    buildings/smith.h \
    buildings/snob.h \
    buildings/farm.h \
    buildings/statue.h \
    buildings/storage.h \
    buildings/hide.h \
    buildings/wall.h \
    buildings/barracks.h \
    buildings/church.h \
    buildings/garage.h \
    buildings/stable.h \
    buildings/place.h \
    buildings/headquarters.h \
    buildings/buildingfactory.h \
    units/unit.h \
    units/infantry.h \
    units/cavalry.h \
    units/archer.h \
    units/spear.h \
    units/sword.h \
    units/axe.h \
    units/ram.h \
    units/catapult.h \
    units/militia.h \
    units/paladin.h \
    units/light.h \
    units/heavy.h \
    units/farcher.h \
    units/marcher.h \
    units/snobunit.h \
    units/spy.h \
    units/unitfactory.h \
    misc/coordinate.h \
    misc/resources.h \
    game/game.h \
    game/worldlist.h \
    game/world.h \
    game/player.h \
    game/village.h \
    game/worldsettings.h \
    game/onlineclient.h \
    game/account.h \
    buildings/university.h \
    client/client.h \
    orders/order.h \
    orders/buildorder.h \
    misc/utils.h \
    orders/recruitorder.h \
    orders/scavengeorder.h \
    xerxes/xerxes.h \
    xerxes/ui.h \
    exceptions/gameexception.h \
    exceptions/wrongcredentials.h \
    exceptions/worldinactive.h \
    game/orderprocessor.h \
    exceptions/insufficentunits.h
