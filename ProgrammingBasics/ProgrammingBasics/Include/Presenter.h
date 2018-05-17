#define SINGLE_SELECTION
#define POLY_SELECTION

#ifndef __PRESENTER

#include "IView.h"
#include "Model.h"
#include "Mode.h"

class Presenter {
private:
	static Mode* mode;
	static Model* model;
	static IView* view;
public:
	static void drawScene();

	//static void DrawSelected(const Array<ID>&){}

	static void Initializer(IView* _view);
	
	/* using for creating figures
	 points, segments, arcs, circles .. */
	static ID CreateObject(object_type, const Array<double>&);
	bool GetObject(double, double, const ID&);

	/* trying to impose requirements
	on selected object */

	static bool CreateRequirement(object_type, const Array<ID>&, const Array<double>&);
	static bool GetObject(double, double, ID&) { return true; }

	/*void DeletePrimitives();

	void DeleteRequirement(const ID&);

	void ChangeParamRequirement(const ID&, const double);

	void ScaleObjects(const double);

	void MoveObject(const Vector2&);*/

	/*static void ChangeObject(){}*/

	static void GetRequirements(const ID&, Array<ID>&);

	//function for viewFLTK
	
	static statusCreate status;
	static Array<Vector2> posClicks;
	static Array<double> params;

	static void changeStatusCreate(const statusCreate newStatus);

	static void clearScene();

	static void clickOnScene(double x, double y);
};
#endif
