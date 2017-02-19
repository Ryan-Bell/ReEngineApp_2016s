#include "MyPrimitive.h"

MyPrimitive::MyPrimitive() { }
MyPrimitive::MyPrimitive(const MyPrimitive& other) { }
MyPrimitive& MyPrimitive::operator=(const MyPrimitive& other) { return *this; }
MyPrimitive::~MyPrimitive(void) { super::Release(); }
void MyPrimitive::CompileObject(vector3 a_v3Color)
{
	m_uVertexCount = static_cast<int> (m_lVertexPos.size());
	for (uint i = 0; i < m_uVertexCount; i++)
	{
		AddVertexColor(a_v3Color);
	}
	
	CompleteTriangleInfo(true);
	CompileOpenGL3X();

}

void MyPrimitive::AddTri(vector3 a_vBottomLeft, vector3 a_vBottomRight, vector3 a_vTop)
{
	AddVertexPosition(a_vBottomLeft);
	AddVertexPosition(a_vBottomRight);
	AddVertexPosition(a_vTop);
}

//C--D
//|\ |
//| \|
//A--B
//This will make the triang A->B->C and then the triang C->B->D
void MyPrimitive::AddQuad(vector3 a_vBottomLeft, vector3 a_vBottomRight, vector3 a_vTopLeft, vector3 a_vTopRight)
{
	AddTri(a_vBottomLeft, a_vBottomRight, a_vTopLeft);
	AddTri(a_vTopLeft, a_vBottomRight, a_vTopRight);
}

void MyPrimitive::GeneratePlane(float a_fSize, vector3 a_v3Color)
{
	if (a_fSize < 0.01f)
		a_fSize = 0.01f;

	Release();
	Init();

	float fValue = 0.5f * a_fSize;

	vector3 pointA(-fValue, -fValue, 0.0f); //0
	vector3 pointB(fValue, -fValue, 0.0f); //1
	vector3 pointC(fValue, fValue, 0.0f); //2
	vector3 pointD(-fValue, fValue, 0.0f); //3

	vector3 pointE(fValue, -fValue, -0.001f); //1
	vector3 pointF(-fValue, -fValue, -0.001f); //0
	vector3 pointG(fValue, fValue, -0.001f); //2
	vector3 pointH(-fValue, fValue, -0.001f); //3

											  //F
	AddQuad(pointA, pointB, pointD, pointC);
	//Double sided
	AddQuad(pointE, pointF, pointG, pointH);

	CompileObject(a_v3Color);
}
void MyPrimitive::GenerateCube(float a_fSize, vector3 a_v3Color)
{
	if (a_fSize < 0.01f)
		a_fSize = 0.01f;

	Release();
	Init();

	float fValue = 0.5f * a_fSize;
	//3--2
	//|  |
	//0--1
	vector3 point0(-fValue, -fValue, fValue); //0
	vector3 point1(fValue, -fValue, fValue); //1
	vector3 point2(fValue, fValue, fValue); //2
	vector3 point3(-fValue, fValue, fValue); //3

	vector3 point4(-fValue, -fValue, -fValue); //4
	vector3 point5(fValue, -fValue, -fValue); //5
	vector3 point6(fValue, fValue, -fValue); //6
	vector3 point7(-fValue, fValue, -fValue); //7

											  //F
	AddQuad(point0, point1, point3, point2);

	//B
	AddQuad(point5, point4, point6, point7);

	//L
	AddQuad(point4, point0, point7, point3);

	//R
	AddQuad(point1, point5, point2, point6);

	//U
	AddQuad(point3, point2, point7, point6);

	//D
	AddQuad(point4, point5, point0, point1);

	CompileObject(a_v3Color);
}
void MyPrimitive::GenerateCone(float a_fRadius, float a_fHeight, int a_nSubdivisions, vector3 a_v3Color)
{
	if (a_nSubdivisions < 3)
		a_nSubdivisions = 3;
	if (a_nSubdivisions > 360)
		a_nSubdivisions = 360;

	Release();
	Init();

	float degree_step = 2 * PI / a_nSubdivisions;
	float degrees = 0.0f;

	vector3 origin(0, 0, 0);
	vector3 topPoint(0, 0, a_fHeight);

	vector3 left, right;
	for (int i = 0; i < a_nSubdivisions; i++) {
		left = vector3(a_fRadius * cos(degrees), a_fRadius * sin(degrees), 0);
		degrees += degree_step;
		right = vector3(a_fRadius * cos(degrees), a_fRadius * sin(degrees), 0);
		AddTri(left, right, topPoint);
		AddTri(left, origin, right);
	}

	CompileObject(a_v3Color);
}
void MyPrimitive::GenerateCylinder(float a_fRadius, float a_fHeight, int a_nSubdivisions, vector3 a_v3Color)
{
	if (a_nSubdivisions < 3)
		a_nSubdivisions = 3;
	if (a_nSubdivisions > 360)
		a_nSubdivisions = 360;

	Release();
	Init();

	float degree_step = 2 * PI / a_nSubdivisions;
	float degrees = 0.0f;

	vector3 origin(0, 0, 0);
	vector3 topPoint(0, 0, a_fHeight);

	vector3 bottomLeft, bottomRight, topLeft, topRight;
	for (int i = 0; i < a_nSubdivisions; i++) {
		bottomLeft = vector3(a_fRadius * cos(degrees), a_fRadius * sin(degrees), 0);
		topLeft = vector3(a_fRadius * cos(degrees), a_fRadius * sin(degrees), a_fHeight);
		degrees += degree_step;
		bottomRight = vector3(a_fRadius * cos(degrees), a_fRadius * sin(degrees), 0);
		topRight = vector3(a_fRadius * cos(degrees), a_fRadius * sin(degrees), a_fHeight);
		AddTri(bottomLeft, origin, bottomRight);
		AddTri(topLeft, topRight, topPoint);
		AddQuad(bottomLeft, bottomRight, topLeft, topRight);
	}
		
	CompileObject(a_v3Color);
}
void MyPrimitive::GenerateTube(float a_fOuterRadius, float a_fInnerRadius, float a_fHeight, int a_nSubdivisions, vector3 a_v3Color)
{
	if (a_nSubdivisions < 3)
		a_nSubdivisions = 3;
	if (a_nSubdivisions > 360)
		a_nSubdivisions = 360;

	Release();
	Init();

	float degree_step = 2 * PI / a_nSubdivisions;
	float degrees = 0.0f;

	vector3 origin(0, 0, 0);
	vector3 topPoint(0, 0, a_fHeight);

	vector3 bottomLeft, bottomRight, topLeft, topRight;
	vector3 bottomInnerLeft, bottomInnerRight, topInnerLeft, topInnerRight;
	for (int i = 0; i < a_nSubdivisions; i++) {
		bottomLeft = vector3(a_fOuterRadius * cos(degrees), a_fOuterRadius * sin(degrees), 0);
		topLeft = vector3(a_fOuterRadius * cos(degrees), a_fOuterRadius * sin(degrees), a_fHeight);

		bottomInnerLeft = vector3(a_fInnerRadius * cos(degrees), a_fInnerRadius * sin(degrees), 0);
		topInnerLeft = vector3(a_fInnerRadius * cos(degrees), a_fInnerRadius * sin(degrees), a_fHeight);

		degrees += degree_step;
		
		bottomRight = vector3(a_fOuterRadius * cos(degrees), a_fOuterRadius * sin(degrees), 0);
		topRight = vector3(a_fOuterRadius * cos(degrees), a_fOuterRadius * sin(degrees), a_fHeight);

		bottomInnerRight = vector3(a_fInnerRadius * cos(degrees), a_fInnerRadius * sin(degrees), 0);
		topInnerRight = vector3(a_fInnerRadius * cos(degrees), a_fInnerRadius * sin(degrees), a_fHeight);
		

		AddQuad(bottomLeft, bottomInnerLeft, bottomRight, bottomInnerRight);
		AddQuad(topLeft, topRight, topInnerLeft, topInnerRight);
		AddQuad(bottomLeft, bottomRight, topLeft, topRight);
		AddQuad(bottomInnerLeft, topInnerLeft, bottomInnerRight, topInnerRight);
	}

	CompileObject(a_v3Color);
}
void MyPrimitive::GenerateTorus(float a_fOuterRadius, float a_fInnerRadius, int a_nSubdivisionsA, int a_nSubdivisionsB, vector3 a_v3Color)
{
	if (a_fOuterRadius <= a_fInnerRadius + 0.1f)
		return;

	if (a_nSubdivisionsA < 3)
		a_nSubdivisionsA = 3;
	if (a_nSubdivisionsA > 25)
		a_nSubdivisionsA = 25;

	if (a_nSubdivisionsB < 3)
		a_nSubdivisionsB = 3;
	if (a_nSubdivisionsB > 25)
		a_nSubdivisionsB = 25;

	Release();
	Init();

	CompileObject(a_v3Color);
}
void MyPrimitive::GenerateSphere(float a_fRadius, int a_nSubdivisions, vector3 a_v3Color, Sphere type)
{
	//Sets minimum and maximum of subdivisions
	if (a_nSubdivisions < 1)
	{
		GenerateCube(a_fRadius * 2, a_v3Color);
		return;
	}
	if (a_nSubdivisions > 6)
		a_nSubdivisions = 6;

	Release();
	Init();

	switch (type)
	{
	case MyPrimitive::icoshpere:
		GenerateIcosphere(a_fRadius, a_nSubdivisions);
		break;
	default:
		break;
	}



	CompileObject(a_v3Color);
}

void MyPrimitive::GenerateIcosphere(float a_fRadius, int a_nSubdivisions) {

	//the points in a simple dodecahedron
	vector3 top(0, 0, a_fRadius);
	vector3 bottom(0, 0, -a_fRadius);

	float shortRadius = sqrt((a_fRadius * a_fRadius) / 2);
	vector3 b1(-shortRadius, -shortRadius, 0);
	vector3 b2(-shortRadius, shortRadius, 0);
	vector3 b3(shortRadius, shortRadius, 0);
	vector3 b4(shortRadius, -shortRadius, 0);
	vector3 corners[] = {b1, b2, b3, b4};
	
	std::vector<vector3> points, newPoints;
	for (int i = 0; i < 4; i++) {
		newPoints = SubDivide(corners[i], top, corners[(i+1) % 4], a_nSubdivisions);
		points.insert(points.end(), newPoints.begin(), newPoints.end());
		newPoints = SubDivide(corners[i], corners[(i + 1) % 4], bottom, a_nSubdivisions);
		points.insert(points.end(), newPoints.begin(), newPoints.end());
	}
	
	for (int i = 0; i < points.size(); i++) {
		points[i] = glm::normalize(points[i]);
	}
	for (int i = 0; i < points.size(); i += 3) {
		AddTri(points[i], points[(i + 1) % points.size()], points[(i + 2) % points.size()]);
	}
}

std::vector<vector3> MyPrimitive::SubDivide(vector3 point1, vector3 point2, vector3 point3, int level) {
	std::vector<vector3> points;
	
	if (level == 0) {
		points.push_back(point1);
		points.push_back(point2);
		points.push_back(point3);
	}
	else {
		vector3 mid1 = (point1 + point2) / 2.0f;
		vector3 mid2 = (point2 + point3) / 2.0f;
		vector3 mid3 = (point3 + point1) / 2.0f;

		std::vector<vector3> newPoints;

		newPoints = SubDivide(point1, mid1, mid3, level - 1);
		points.insert(points.end(), newPoints.begin(), newPoints.end());

		newPoints = SubDivide(mid1, point2, mid2, level - 1);
		points.insert(points.end(), newPoints.begin(), newPoints.end());

		newPoints = SubDivide(mid3, mid2, point3, level - 1);
		points.insert(points.end(), newPoints.begin(), newPoints.end());

		newPoints = SubDivide(mid1, mid2, mid3, level - 1);
		points.insert(points.end(), newPoints.begin(), newPoints.end());
	}
	return points;
}
