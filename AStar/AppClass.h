/*----------------------------------------------
Programmer: Alberto Bobadilla (labigm@gmail.com)
Date: 2015/09
----------------------------------------------*/
#ifndef __APPLICATION_H_
#define __APPLICATION_H_

#include "RE\ReEngAppClass.h"
#include <SFML\Graphics.hpp>

using namespace ReEng;
inline int randNext(int max) {
	return rand() % max;
}

typedef struct Point {
	int x, y, z;
} Point;
typedef struct Node {
	Node* Parent;
	float FScore;
	float GScore;
	Point loc;
	vector3 color;
	bool walkable;
	bool closed;
} Node;
struct PriorityQueue {
	std::vector<Node*> queue;
	int index;
	int parent;
	int childLeft;//2i + 1
	int childRight;//2i + 2       
	void Enqueue(Node* data);
	Node* Dequeque();
	Node* Peek();
	bool IsEmpty();
};

inline float Distance(Point a, Point b) {
	return sqrt(pow(a.x-b.x, 2) + pow(a.y-b.y, 2) + pow(a.z-b.z, 2));
}

class AppClass : public ReEngAppClass {
	int sideLength;
	Node *start, *goal, *current;
	Node* data;
	PriorityQueue open;
	bool complete, play;
public:
	typedef ReEngAppClass super;
	inline int at(int x, int y, int z) {
		return z + sideLength * y + sideLength * sideLength * x;
	}
	inline Point randPoint() {
		int x = randNext(sideLength);
		int y = randNext(sideLength);
		int z = randNext(sideLength);
		return{ x, y, z };
	}
	inline int randIndex() {
		int x = randNext(sideLength);
		int y = randNext(sideLength);
		int z = randNext(sideLength);
		return at(x,y,z);
	}

	AppClass(HINSTANCE hInstance, LPWSTR lpCmdLine, int nCmdShow, bool a_bUsingConsole = false) : super(hInstance, lpCmdLine, nCmdShow, a_bUsingConsole) {}
	~AppClass(void) { Release(); }
	void Step();
	void Generate();
	Node** GetNeighbors(Node* subject);
	virtual void InitWindow(String a_sWindowName) final;
	virtual void InitVariables(void) final;
	virtual void Update(void) final;
	virtual void Display(void) final;
	virtual void ProcessKeyboard(void) final;
	virtual void ProcessMouse(void) final;
	virtual void Release(void) final;
};

#endif