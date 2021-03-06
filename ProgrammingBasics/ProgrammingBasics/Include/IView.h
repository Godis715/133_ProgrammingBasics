#ifndef __IVIEW
#define __IVIEW
#include "Vector2.h"
#include "IWidjet.h"
#include "Colors.h"

enum typeDrawing{dot, dash, solid, null};
enum statusCreate { drawPoint, drawSegment, drawArc, drawCircle };
enum typeWidjet{displayParamPrim, displayParamReq, creatingToolbar, requirementInput, drawMode};

class IView
{
private:

public:
	virtual void DrawLine(const Array<double>&) = 0;
	virtual void DrawArc(const Array<double>&) = 0;
	virtual void DrawPoint(const Array<double>&) = 0;
	virtual void DrawCircle(const Array<double>&) = 0;
	virtual void DrawCurve(const Array<double>&) = 0;

	virtual void SetStyleDrawing(const rgbColor, const typeDrawing = null) = 0;
	virtual int Run() = 0;
	virtual void Update() = 0;
	virtual void TranslateScene(const Vector2&) = 0;
	virtual void ScaleScene(const double&) = 0;
	virtual void RotateScene(const double&) = 0;
	virtual IWidjet* GetWidjet(const typeWidjet) = 0;
};

#endif // !__IVIEW