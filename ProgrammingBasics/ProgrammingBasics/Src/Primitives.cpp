#include "Primitives.h"
#include <stdexcept>

#pragma region Primitive
Primitive::Primitive(object_type _type, const Array<double>& _params, const Array<ID>& _children)
	: Object(_type, _params, children),
	doubleParams(_params.GetSize()),
	isActivated(true)
{
	for (int i = 0; i < doubleParams.GetSize(); ++i) {
		doubleParams[i] = new double(params[i]);
	}
}

Primitive::~Primitive() {
	for (int i = 0; i < doubleParams.GetSize(); ++i) {
		delete doubleParams[i];
	}

}

Array<double*> Primitive::GetDoubleParamsAsPointers() {
	return doubleParams;
}

void Primitive::ApplyDoubleParams() {
	for (int i = 0; i < params.GetSize(); ++i) {
		params[i] = *doubleParams[i];
	}
}

bool Primitive::IsActivated() {
	return isActivated;
}
void Primitive::Activate() {
	isActivated = true;
}
void Primitive::Deactivate() {
	isActivated = false;
}

#pragma endregion

#pragma region Point
Point::Point(const Vector2& pos) :
	Primitive(ot_point, CreateArr(pos.x, pos.y), Array<ID>(0))
{
	this->x = &params[0];
	this->y = &params[1];
}
Point::Point(double _x, double _y) :
	Primitive(ot_point, CreateArr(_x, _y), Array<ID>(0))
{
	this->x = &params[0];
	this->y = &params[1];
}
Point::Point(const Point& _p) :
	Primitive(ot_point, CreateArr(*_p.x, *_p.y), Array<ID>(0))
{
	this->x = &params[0];
	this->y = &params[1];
}
double Point::GetDist(const Vector2& point) const {
	return sqrt((*x - point.x)*(*x - point.x) +
		(*y - point.y)*(*y - point.y));
}
Vector2 Point::GetPos() const {
	return Vector2(*x, *y);
}
void Point::SetPos(const Vector2& _pos) {
	*x = _pos.x;
	*y = _pos.y;
}
void Point::SetPos(double _x, double _y) {
	*x = _x;
	*y = _y;
}
#pragma endregion

#pragma region Segment
Segment::Segment(Point* _p1, Point* _p2) :
	Primitive(ot_segment, Array<double>(0), CreateArr(_p1->GetID(), _p2->GetID()))
{
	if (_p1 == nullptr || _p2 == nullptr) {
		throw std::invalid_argument("Segment::Segment::parameters was nullptr");
	}

	point1 = _p1;
	point2 = _p2;
}
double Segment::GetLength() const {
	return (point1->GetPos() - point2->GetPos()).GetLength();
}
Vector2 Segment::GetPointPos1() const {
	return point1->GetPos();
}
Vector2 Segment::GetPointPos2() const {
	return point2->GetPos();
}
void Segment::SetPointPos1(const Vector2& _pos) {
	point1->SetPos(_pos);
}
void Segment::SetPointPos2(const Vector2& _pos) {
	point2->SetPos(_pos);
}
void Segment::SetPointPos1(const double x, const double y) {
	point1->SetPos(x, y);
}
void Segment::SetPointPos2(const double x, const double y) {
	point2->SetPos(x, y);
}
ID Segment::GetPointID1() const {
	return point1->GetID();
}
ID Segment::GetPointID2() const {
	return point2->GetID();
}
double Segment::Inequality(Vector2 vector) {
	Equation* equation = this->CreateEquation();
	double answer = equation->a * vector.x + equation->b * vector.y + equation->c;
	delete equation;
	return answer;
}
double Segment::GetDist(const Vector2& point) const {
	double dotProduct1 = 0.0;
	double dotProduct2 = 0.0;
	Vector2 point1 = this->point1->GetPos();
	Vector2 point2 = this->point2->GetPos();

	Vector2 pointTo1 = point - point1;
	Vector2 pointTo2 = point - point2;
	Vector2 segment = point2 - point1;
	dotProduct1 = Vector2::Dot(pointTo1, segment);
	dotProduct2 = Vector2::Dot(pointTo2, segment);
	dotProduct2 *= -1;
	double answer = 0;
	if (dotProduct1 >= 0 && dotProduct2 >= 0) {
		answer = abs(Vector2::Cross(pointTo1, segment));
		answer /= 2;
		answer /= segment.GetLength();
	}
	else {
		answer = pointTo1.GetLength();
		if (answer > pointTo2.GetLength()) {
			answer = pointTo2.GetLength();
		}
	}
	return answer;
}
#pragma endregion

#pragma region Arc
Arc::Arc(Point* _p1, Point* _p2, double _angle) :
	Primitive(ot_arc, CreateArr(_angle), CreateArr(_p1->GetID(), _p2->GetID()))
{
	if (_p1 == nullptr || _p2 == nullptr) {
		throw std::invalid_argument("Arc::Arc::parameters was nullptr");
	}

	point1 = _p1;
	point2 = _p2;

	angle = &params[0];
	RestoreCenter();
}

// write this function
double Arc::GetDist(const Vector2& _point) const {
	Vector2 center(cx, cy);
	Vector2 r1 = point1->GetPos() - center;
	Vector2 r2 = point2->GetPos() - center;
	Vector2 vec = _point - center;

	bool inSector = true;

	double cross1 = Vector2::Cross(r1, vec);
	double cross2 = Vector2::Cross(vec, r2);

	if (*angle < PI) {
		inSector = (cross1 > 0 && cross2 > 0);
	}
	else {
		inSector = (cross1 > 0 || cross2 > 0);
	}
	if (inSector) {
		return abs(vec.GetLength() - r1.GetLength());
	}
	else {
		double dist1 = (r1 - vec).GetLength();
		double dist2 = (r2 - vec).GetLength();
		return (dist1 > dist2) ? dist1 : dist2;
	}
}

Vector2 Arc::GetCenter() const {
	return Vector2(cx, cy);
}

void Arc::RestoreCenter() {
	Vector2 center;
	Vector2 point1Pos = point1->GetPos();
	Vector2 point2Pos = point2->GetPos();

	Vector2 base = point2Pos - point1Pos;
	double baseLength = base.GetLength();

	double H = (baseLength / 2.0) / tan(*angle / 2.0);

	Vector2 ortH(-base.y / baseLength, base.x / baseLength);
	Vector2 midBase = (point1Pos + point2Pos) / 2.0;

	center = midBase + (ortH * H);

	point1Pos = point1Pos - center;
	point2Pos = point2Pos - center;

	if (Vector2::Cross(point1Pos, point2Pos) * (*angle - PI) > 0) {
		center = midBase - (ortH * H);
	}
	cx = center.x;
	cy = center.y;
}

ID Arc::GetPointID1() const {
	return point1->GetID();
}
ID Arc::GetPointID2() const {
	return point2->GetID();
}
Vector2 Arc::GetPointPos1() const {
	return point1->GetPos();
}
Vector2 Arc::GetPointPos2() const {
	return point2->GetPos();
}
void Arc::SetPointPos1(const Vector2& _pos) {
	point1->SetPos(_pos);
}
void Arc::SetPointPos2(const Vector2& _pos) {
	point2->SetPos(_pos);
}
void Arc::SetPointPos1(const double x, const double y) {
	point1->SetPos(x, y);
}
void Arc::SetPointPos2(const double x, const double y) {
	point2->SetPos(x, y);
}
double Arc::GetAngle() const {
	return *angle;
}

void Arc::SetAngle(double newAngle) {
	if (newAngle > 2 * PI) {
		*angle = newAngle - (double)(int)(newAngle / (2 * PI)) * 2 * PI;
		return;
	}
	if (newAngle < 0.0) {
		*angle = newAngle + ((double)((int)(abs(newAngle) / (2 * PI)) + 1) * 2 * PI);
		return;
	}
	*angle = newAngle;
}
#pragma endregion

#pragma region Circle
Circle::Circle(Point* _center,  double _radius) :
	Primitive(ot_circle, CreateArr(_radius), CreateArr(_center->GetID()))
{
	if (_center == nullptr) {
		throw std::invalid_argument("Circle::Circle::_center was nullptr");
	}
	if (_radius < 0) {
		throw std::invalid_argument("Circle::Circle::_radius was negative");
	}

	center = _center;

	radius = &params[0];
}

// write this function
double Circle::GetDist(const Vector2& _point) const {
	return abs(*radius - (_point - Vector2(center->GetPos().x, center->GetPos().y)).GetLength());
}

Vector2 Circle::GetCenter() const {
	return center->GetPos();
}

ID Circle::GetCenterID() const {
	return children[0];
}

void Circle::SetCenterPos(const Vector2& _pos) {
	center->SetPos(_pos);
}
void Circle::SetCenterPos(const double x, const double y) {
	center->SetPos(x, y);
}

double Circle::GetRadius() const {
	return *radius;
}

void Circle::SetRadius(const double _radius)
{
	*radius = _radius;
}
#pragma endregion

#pragma region Curve
Curve::Curve(Point* p1, Point* p2, Point* p3, Point* p4) :
	Primitive(ot_circle, 0, 
		CreateArr(p1->GetID(), p2->GetID(), p3->GetID(), p4->GetID()))
{
	point1 = p1;
	point2 = p2;
	point3 = p3;
	point4 = p4;
}

double Curve::GetDist(const Vector2&) const {
	return 500.0;
}

Array<ID> Curve::GetPointIDs() const {
	return CreateArr(point1->GetID(), point2->GetID(), point3->GetID(), point4->GetID());
}
Array<Vector2> Curve::GetPointPositions() const {
	return CreateArr(point1->GetPos(), point2->GetPos(), point3->GetPos(), point4->GetPos());
}
Array<double> Curve::GetPointDoubles() const {
	auto v1 = point1->GetPos();
	auto v2 = point2->GetPos();
	auto v3 = point3->GetPos();
	auto v4 = point4->GetPos();

	return CreateArr(v1.x, v1.y, v2.x, v2.y, v3.x, v3.y, v4.x, v4.y);
}
void Curve::SetPointPositions(const Array<Vector2>& vectors) {
	if (vectors.GetSize() != 4) {
		throw std::invalid_argument("Curve::invalid size");
	}
	point1->SetPos(vectors[0]);
	point2->SetPos(vectors[1]);
	point3->SetPos(vectors[2]);
	point4->SetPos(vectors[3]);
}
void Curve::SetPointPositions(const Array<double> params) {
	if (params.GetSize() != 8) {
		throw std::invalid_argument("Curve::invalid size");
	}
	point1->SetPos(params[0], params[1]);
	point2->SetPos(params[2], params[3]);
	point3->SetPos(params[4], params[5]);
	point4->SetPos(params[6], params[7]);
}
#pragma endregion