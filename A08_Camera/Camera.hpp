#pragma once
#include "RE/ReEng.h"

struct Camera {
	quaternion rotation;
	vector3 position;
	vector3 up;
	vector3 right;
	vector3 forward;

	matrix4 GetView(void);
	matrix4 GetProjection(bool bOrthographic);
	void SetPosition(vector3 v3Position);
	void SetTarget(vector3 v3Target);
	void SetUp(vector3 v3Up);
	void MoveForward(float fIncrement);
	void MoveSideways(float fIncrement);
	void MoveVertical(float fIncrement);
	void ChangePitch(float fIncrement);
	void ChangeRoll(float fIncrement);
	void ChangeYaw(float fIncrement);
	void recalc();
};