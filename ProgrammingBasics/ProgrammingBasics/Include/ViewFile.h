#ifndef __VIEW_FILE
#define __VIEW_FILE

#include "ViewHeaders.h"
#include "ViewLog.h"

class ViewFile : public DisplayWidjet
{
private:
	static ViewLog* viewLog;
	
	static Color* colorThemes;

	const int positionX = 10;
	const int positionY = 0;
	const int indentX = 10;
	const int indentY = 0;
	
	const int hBut = 30;
	const int wSave_b = 50;
	const int wDownland_b = 70;
	const int wScript_b = 50;
	const int wTheme_m = 40;

	Fl_Group* group;
	Fl_Menu_Item* themes;
	Fl_Menu_Button* theme_b;
	Fl_Button* execute_script_b;
	Fl_Button* save_b;
	Fl_Button* downland_b;
	void Initializer();

	static void cl_theme_b(Fl_Widget*, void*);

	static void cl_execute_script_b(Fl_Widget*, void*);

	static void cl_SaveProject(Fl_Widget*, void*);

	static void cl_DownloadFile(Fl_Widget*, void*);
public:
	ViewFile();

	~ViewFile();
};

#endif __VIEW_FILE
