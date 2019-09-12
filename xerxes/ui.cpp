#include "ui.h"

//#include <menu.h>
//#include <form.h>
//#include <panel.h>
#include <string>
#include <cstring>
#include <map>
#include <vector>
#include <math.h>
#include <iomanip>
#include <glib.h>

#include <game/village.h>
#include <buildings/buildingfactory.h>
#include <units/unitfactory.h>
#include <misc/utils.h>
#include <misc/resources.h>

void print_in_middle(WINDOW *win, int starty, int startx, int width, const char *string, chtype color)
{
	int length, x, y;
	float temp;
	if(win == NULL)
		win = stdscr;
	getyx(win, y, x);
	if(startx != 0)
		x = startx;
	if(starty != 0)
		y = starty;
	if(width == 0)
		width = 80;

	length = strlen(string);
	temp = (width - length)/ 2;
	x = startx + (int)temp;
	wattron(win, color);
	mvwprintw(win, y, x, "%s", string);
	wattroff(win, color);
	refresh();
}


UI::UI() {
	//init ncurses jazz
//	setlocale (LC_ALL, "");
	initscr();
	cbreak();
	noecho();
//	curs_set ( 0 );

	start_color ();

	this->windows.push_back ( newwin(LINES-2, COLS-2, 1, 1) ); //main window
//	box ( stdscr, ACS_DARROW, ACS_DIAMOND );
	box ( stdscr, 0, 0 );
	keypad ( this->windows[0], TRUE);
	idlok ( this->windows[0], TRUE );
	scrollok ( this->windows[0], TRUE );

	/* Attach a panel to each window */ 	/* Order is bottom up */
	this->panels.push_back ( new_panel(this->windows[0]) );

	/* Update the stacking order. 2nd panel will be on top */
	update_panels();

	/* Show it on the screen */
	doupdate();

}



void UI::printMessage ( std::string message ) {
//	waddch ( this->windows[0], ACS_DIAMOND );
	wprintw ( this->windows[0], message.c_str () );
//	wrefresh ( this->windows[0] );
	update_panels ();
	doupdate ();
}
//TODO: Fix bar when there is not enough remaining space in a row to fit at least "[]100%"
void UI::printProgressBar ( int count, int total ) {
	int winx = 0;
	winx = getmaxx ( this->windows[0] );
	int colOffset = getcurx ( this->windows[0] );
	double percentageDone = ( (double)count / total ) * 100;
//	wmove ( this->windows[0], getcurx (this->windows[0]), getcury (this->windows[0]) );
	wrefresh ( this->windows[0] );
	waddch ( this->windows[0], '[' );
	//Print #'s that correspond to completed part and finish with _'s
	for ( int i = 0; i < winx-6-colOffset; i++ ) {
		if ( (double)i / (winx-6-colOffset) * 100 <= percentageDone ) {
			waddch ( this->windows[0], '#' );
		}
		else {
			waddch ( this->windows[0], '_' );
		}
	}
	waddch ( this->windows[0], ']' );
	std::string percentageString = std::to_string ((int)percentageDone);
	//Pad string with ' ' to be always 3 chars wide
	while ( percentageString.length () < 3 ) {
		percentageString = " " + percentageString;
	}
	waddstr ( this->windows[0], percentageString.c_str () );
	waddch ( this->windows[0], '%' );
	int cury = getcury (this->windows[0]);
	//return cursor to the colOffset it is on IF its not last print
	if ( count != total ) {
		wmove ( this->windows[0], cury-1, colOffset );
	}
	wrefresh ( this->windows[0] );
	doupdate();
}

int UI::createVillageOverview ( Village *village ) {
	int x = COLS-2, y = 21;
	//Create main window for village overview and add panel
	this->windows.push_back ( newwin(y, x, 1, 1 ) );
	this->panels.push_back ( new_panel(this->windows.back ()) );
	WINDOW *titleWin = derwin ( this->windows.back (), 3, x, 0, 0 );
	box ( this->windows.back (), 0, 0 );
	wborder ( titleWin, 0, 0, 0, 0, 0, 0, ACS_LTEE, ACS_RTEE );
	std::string titleString = village->name + " " + village->location.toString () + " K"
	+ std::to_string ( village->location.getContinent () );
	mvwprintw ( titleWin, 1, 1, titleString.c_str () );

	MENU *buildingsMenu;
	std::vector<ITEM*> buildingsMenuItems;
	//Fill buildingsMenuItems
	std::vector<std::pair<std::string,std::string>> buildingsInfo;
	for ( size_t i = 0; i < village->buildings.size (); i++ ) {
		std::pair<std::string,std::string> temp;
		temp.first = village->buildings[i]->name;
		temp.second = std::to_string (village->buildings[i]->level) + "/" + std::to_string (village->buildings[i]->maxLevel);
		buildingsInfo.push_back ( temp );
	}
	for ( size_t i = 0; i < buildingsInfo.size (); i++ ) {
		buildingsMenuItems.push_back ( new_item ( buildingsInfo[i].first.c_str (), buildingsInfo[i].second.c_str () ) );
	}
	buildingsMenuItems.push_back ( new_item ( (char*)NULL, (char*)NULL ) );
	buildingsMenuItems.shrink_to_fit ();
	//Menu creation
	int menuCols=19, menuRows = 19; //minus one because of null menu item
	buildingsMenu = new_menu ( buildingsMenuItems.data () );

	//Building menu window creation

	//ACS_VLINE, ACS_VLINE,ACS_HLINE,ACS_HLINE,ACS_ULCORNER,ACS_URCORNER,ACS_LLCORNER,ACS_LRCORNER.
	/*
	ls - left side, rs - right side, ts - top side, bs - bottom side, tl - top left-hand corner, tr - top right-hand corner,
	bl - bottom left-hand corner, and
	br - bottom right-hand corner.
	*/
	WINDOW *buildingsWin = derwin (this->windows.back (), menuRows, menuCols, 2, 0);
	wborder ( buildingsWin, 0, 0, 0, 0, ACS_LTEE, ACS_TTEE, 0, ACS_BTEE );
	WINDOW *buildingsMenuWin = derwin (buildingsWin, menuRows-2, menuCols-2, 2, 1);
	mvwprintw ( buildingsWin, 1, 1, "BUILDINGS" );
	/* Set main window and sub window */
	set_menu_win ( buildingsMenu, buildingsMenuWin );
	set_menu_sub ( buildingsMenu, derwin (buildingsMenuWin, menuRows-5, menuCols-2, 3, 1) );
	set_menu_mark ( buildingsMenu, (char*)NULL );
	post_menu ( buildingsMenu );
	wrefresh ( buildingsMenuWin );

	//Building information window creation
	WINDOW *infoWin = derwin (this->windows.back (), 15, 58, 5, 19 );
//	wborder ( infoWin, 0, 0, 0, 0, ACS_LTEE, 0, ACS_BTEE, 0 );
	wrefresh ( infoWin );

	//Storage display window
	WINDOW *storageWin = derwin (this->windows.back (), 4, 25, 2, x-25 );
	wborder ( storageWin, 0, 0, 0, 0, ACS_TTEE, ACS_RTEE, ACS_BTEE, ACS_RTEE );
	mvwprintw ( storageWin, 1, 1, "WOOD |STONE|IRON |MAX  ");

	keypad(stdscr, TRUE);
	update_panels ();
	doupdate ();
	int c;
	//instead of sleep 1 inside loop just wait for key for 1 second.
	//That way screen is updated every time user presses key or every second
	timeout (1000);
	bool visibleVillage = true; //Ugly and temporary
	while ( (c = getch()) != 'q' ) {
		//Update time window
		std::string dateTime = Utils::timestampToString ( Utils::getTimestamp () );
		mvwprintw ( titleWin, 1, x-dateTime.length ()-1, dateTime.c_str () );


		/*******************************BUILDINGS UPDATE******************************/
		switch (c) {
			case KEY_UP:
				menu_driver ( buildingsMenu, REQ_UP_ITEM );
				break;
			case KEY_DOWN:
				menu_driver ( buildingsMenu, REQ_DOWN_ITEM );
				break;
			case 9: //TAB key
				//If village panel is visible hide it and show log panel or vice versa
				if ( visibleVillage ) {
					top_panel ( panel_below ( this->panels.at(1) ) );
					visibleVillage = false;
				}
				else {
					top_panel ( this->panels.at(1) );
					visibleVillage = true;
				}
				break;
		}
		//TODO: UPDATE BUILDINGS LABELS
		/*******************************END BUILDINGS UPDATE***************************/
		/*******************************BUILDING INFO UPDATE***************************/
		werase ( infoWin );
//		mvwprintw ( infoWin, 1, 1, item_name ( current_item (buildingsMenu) ) );
		Building &buildRef = village->getBuilding ( item_name (current_item (buildingsMenu)) );
		mvwaddnstr ( infoWin, 0, 1, buildRef.toString ().c_str (), -1 );
//		wborder ( infoWin, 0, 0, 0, 0, ACS_LTEE, 0, ACS_BTEE, 0 );
		/*******************************END BUILDING INFO UPDATE***********************/
		/*******************************STORAGE UPDATE******************************/
		std::string wood, stone, iron, max;
		Resources storage = static_cast<Storage&>(village->getBuilding ("storage")).getStorage ();
		Resources maxRes = static_cast<Storage&>(village->getBuilding ("storage")).getCapacity ();
		wood = (storage.wood < 100000) ? std::to_string ((int)storage.wood) : std::to_string ( (int)storage.wood/1000 )+"K";
		stone = (storage.stone < 100000) ? std::to_string ((int)storage.stone) : std::to_string ( (int)storage.stone/1000 )+"K";
		iron = (storage.iron < 100000) ? std::to_string ((int)storage.iron) : std::to_string ( (int)storage.iron/1000 )+"K";
		max = (maxRes.iron < 100000) ? std::to_string ((int)maxRes.iron) : std::to_string ( (int)maxRes.iron/1000 )+"K";
		//Pad strings to be 5 chars wide
		while (wood.length () < 5) {
			wood.push_back ( ' ' );
		}
		while (stone.length () < 5) {
			stone.push_back ( ' ' );
		}
		while (iron.length () < 5) {
			iron.push_back ( ' ' );
		}
		while (max.length () < 5) {
			max.push_back ( ' ' );
		}
		std::string finalStorage = wood + '|' + stone + '|' + iron + '|' + max;
		//Print storage line
		mvwaddstr ( storageWin, 2, 1, finalStorage.c_str () );
		wborder ( storageWin, 0, 0, 0, 0, ACS_TTEE, ACS_RTEE, ACS_BTEE, ACS_RTEE );

		/*******************************END STORAGE UPDATE****************************/
		if ( visibleVillage ) {
			wrefresh ( buildingsMenuWin );
			wrefresh ( infoWin );
			wrefresh ( titleWin );
			wrefresh ( storageWin );
		}
		update_panels ();
		doupdate();
		refresh ();
//		sleep (1);
	}
	unpost_menu(buildingsMenu);
	free_menu(buildingsMenu);
	for ( auto &item : buildingsMenuItems ) {
		free_item ( item );
	}
	delwin ( titleWin );
	delwin ( this->windows.back () );
	del_panel ( this->panels.back () );
	this->panels.pop_back ();
	this->windows.pop_back ();
	timeout ( 0 ); //Set back to nonblocking getch()
	return 0;
}

std::string UI::createWorldSelectionMenu ( std::map<std::string, std::string> worlds ) {
	int c;
	MENU *my_menu;
	WINDOW *my_menu_win;
	int cols, rows;
	//	init_pair(1, COLOR_RED, COLOR_BLACK);

	/* Create items */
	//	n_choices = ARRAY_SIZE(choices);
	std::vector<ITEM *> items;
	//	my_items = (ITEM **)calloc(n_choices, sizeof(ITEM *));
	for ( auto const &world : worlds ) {
		items.push_back ( new_item (world.first.c_str(), world.second.c_str()) );
	}
	items.push_back ( new_item ( (char*)NULL, (char*)NULL ) );
	items.shrink_to_fit ();

	/* Crate menu */
	my_menu = new_menu(items.data ());
	menu_opts_off ( my_menu, O_SHOWDESC );
	menu_format ( my_menu, &cols, &rows );
//	set_menu_format ( my_menu, 20, 2 );

	/* Create the window to be associated with the menu */
	my_menu_win = newwin(7, 19, (LINES - 7) / 2, (COLS - 19) / 2);
	keypad(my_menu_win, TRUE);

	/* Set main window and sub window */
	set_menu_win(my_menu, my_menu_win);
	set_menu_sub(my_menu, derwin(my_menu_win, 3, 17, 3, 1) );
	set_menu_mark(my_menu, (char*)NULL );
	/* Set menu mark to the string " * " */

	menu_opts_off ( my_menu, O_NONCYCLIC );
	set_menu_format ( my_menu, 10, 3 );
	menu_format ( my_menu, &cols, &rows );


	/* Print a border around the main window and print a title */
	box(my_menu_win, 0, 0);
	mvwhline ( my_menu_win, 2, 1, ACS_HLINE, 17 );
	print_in_middle ( my_menu_win, 1, 0, 20, "Select world", 0 );

	//	mvwaddch(my_menu_win, 2, 0, ACS_LTEE);
	//	mvwhline(my_menu_win, 2, 1, ACS_HLINE, 38);
	//	mvwaddch(my_menu_win, 2, 39, ACS_RTEE);
	refresh();

	/* Post the menu */
	post_menu(my_menu);
	wrefresh(my_menu_win);

	std::string result;
	Action ok;
	while( ok != OK_ACTION ) {
		c = wgetch(my_menu_win);
		switch(c) {
			case 10:
				result = item_name ( current_item (my_menu) );
				ok = OK_ACTION;
				break;
			case KEY_DOWN:
				menu_driver(my_menu, REQ_DOWN_ITEM);
				break;
			case KEY_UP:
				menu_driver(my_menu, REQ_UP_ITEM);
				break;
			case KEY_LEFT:
				menu_driver(my_menu, REQ_LEFT_ITEM);
				break;
			case KEY_RIGHT:
				menu_driver(my_menu, REQ_RIGHT_ITEM);
				break;
		}
		wrefresh(my_menu_win);
		refresh ();
	}

	/* Unpost and free all the memory taken up */
	unpost_menu(my_menu);
	free_menu(my_menu);
	for ( auto &item : items ) {
		free_item ( item );
	}
	return result;
}

int UI::createLoginForm ( std::pair<std::string, std::string> &credentials ) {
	FIELD *field[5];
	FORM  *my_form;
	int ch = 0, rows, cols;
	/* Initialize few color pairs */
	init_pair(1, COLOR_WHITE, COLOR_BLACK);
	init_pair(2, COLOR_BLACK, COLOR_WHITE);
	/* Initialize the fields */
	field[0] = new_field(1, 16, 0, 12, 0, 0); // username field
	field[1] = new_field(1, 16, 2, 12, 0, 0); // password field
	field[2] = new_field(1, 2, 4, 6, 0, 0); //OK button
	field[3] = new_field(1, 6, 4, 18, 0, 0); //Cancel button
	field[4] = NULL;

	/* Set field options */
//	set_field_back(field[0], A_UNDERLINE);
//	set_field_fore(field[0], COLOR_PAIR(2));
	field_opts_off(field[0], O_AUTOSKIP);

//	set_field_back(field[1], A_UNDERLINE);
	field_opts_off(field[1], O_AUTOSKIP);

//	init_pair(1, COLOR_WHITE, COLOR_GREEN);

	field_opts_off ( field[2], O_EDIT );
	set_field_buffer ( field[2], 0, "OK" );

	field_opts_off ( field[3], O_EDIT );
	set_field_buffer ( field[3], 0, "Cancel" );

	/* Create the form and post it */
	my_form = new_form(field);

	/* Calculate the area required for the form */
	scale_form(my_form, &rows, &cols);

	/* Create the window to be associated with the form */
	this->windows.push_back ( newwin(9, 30, (LINES - 7) / 2, (COLS - 30) / 2) );
	keypad(this->windows.back (), TRUE);

	this->panels.push_back ( new_panel(this->windows[1]) );
	/* Set main window and sub window */
	set_form_win(my_form, this->windows.back ());
	set_form_sub(my_form, derwin(this->windows.back (), rows, cols, 3, 0));

	post_form(my_form);
	wrefresh(this->windows.back ());

	/* Update the stacking order. 2nd panel will be on top */
	update_panels();

	/* Show it on the screen */
	doupdate();

	/* Print a border around the main window and print a title */
	box(this->windows.back (), 0, 0);
	print_in_middle ( this->windows.back (), 1, 0, 30, "Enter credentials", 0 );
	mvwprintw ( this->windows.back (), 3, 2, "Login:" );
	mvwprintw ( this->windows.back (), 5, 2, "Password:" );
	wrefresh ( this->windows.back () );
//	wrefresh ( my_form_win );
//	mvprintw(LINES - 2, 0, "Use UP, DOWN arrow keys to switch between fields");
	refresh( );
	set_current_field ( my_form, field[0] );
	set_field_back( field[0], COLOR_PAIR(2));

	// Force refresh on first form element on init
	form_driver(my_form, REQ_NEXT_FIELD);
	form_driver(my_form, REQ_PREV_FIELD);

	std::string action = "";
	/* Loop through to get user requests */
	while( action == "" ) {
		ch = wgetch(this->windows.back ());
		switch(ch) {
			case 10:
				form_driver(my_form, REQ_NEXT_FIELD);
				form_driver(my_form, REQ_PREV_FIELD);
				if ( field_buffer ( current_field (my_form), 0) == std::string("OK") ) {
					credentials.first = g_strstrip ( field_buffer ( field[0], 0 ) );
					credentials.second = g_strstrip ( field_buffer ( field[1], 0 ) );
					action = field_buffer ( current_field (my_form), 0);
				}
				if ( field_buffer ( current_field (my_form), 0) == std::string("Cancel") ) {
					action = field_buffer ( current_field (my_form), 0);
				}
				else {
					set_field_back(current_field ( my_form ), COLOR_PAIR(1));
					form_driver(my_form, REQ_NEXT_FIELD);
					set_field_back(current_field ( my_form ), COLOR_PAIR(2));
				}
				break;
			case KEY_DOWN:
				/* Go to next field */
				set_field_back(current_field ( my_form ), COLOR_PAIR(1));
				form_driver(my_form, REQ_NEXT_FIELD);
				set_field_back(current_field ( my_form ), COLOR_PAIR(2));
				/* Go to the end of the present buffer */
				/* Leaves nicely at the last character */
				form_driver(my_form, REQ_END_LINE);
				break;
			case KEY_UP:
				/* Go to previous field */
				set_field_back(current_field ( my_form ), COLOR_PAIR(1));
				form_driver(my_form, REQ_PREV_FIELD);
				set_field_back(current_field ( my_form ), COLOR_PAIR(2));
				form_driver(my_form, REQ_END_LINE);
				break;
			case KEY_BACKSPACE:
				// remove character
//				form_driver ( my_form, REQ_PREV_CHAR );
				form_driver ( my_form, REQ_DEL_PREV );
				break;
			case KEY_LEFT:
				form_driver ( my_form, REQ_PREV_CHAR );
				break;
			case KEY_RIGHT:
				form_driver ( my_form, REQ_NEXT_CHAR );
				break;
			default:
				/* If this is a normal character, it gets */
				/* Printed				  */
				form_driver(my_form, ch);
				break;
		}
	}

	/* Un post form and free the memory */
	unpost_form(my_form);
	free_form(my_form);
	free_field(field[0]);
	free_field(field[1]);
	free_field(field[2]);
	free_field(field[3]);
	delwin ( this->windows.back () );
	del_panel ( this->panels.back () );
	this->panels.pop_back ();
	this->windows.pop_back ();
	update_panels ();
	doupdate ();
	return (action == "OK") ? OK_ACTION : CANCEL_ACTION;
}

//Wait for any key and end curses mode
UI::~UI() {
	getch();
	endwin();
}
