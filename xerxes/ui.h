#ifndef UI_H
#define UI_H

#include <string>
#include <map>
#include <vector>

#include <ncursesw/curses.h>
#include <ncursesw/menu.h>
#include <ncursesw/form.h>
#include <ncursesw/panel.h>

class Village;
//Makeshift User Interface
class UI {
		std::vector<PANEL*> panels;
		std::vector<WINDOW*> windows;
		WINDOW *mainWindow;
		std::vector<std::string> logLines;
	public:
		enum Action { OK_ACTION, CANCEL_ACTION };
		UI();
		void printMessage ( std::string message );
		void printProgressBar ( int count, int total );
		int createLoginForm(std::pair<std::string, std::string> &credentials);
		std::string createWorldSelectionMenu(std::map<std::string, std::string> worlds);
		~UI();
		UI( const UI &src ) = delete;
		int createVillageOverview ( Village *village );
};

#endif // UI_H
