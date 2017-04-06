#include "Camera.hpp"

matrix4 Camera::GetView(void) {
	return glm::lookAt(position, position + forward, up);
}

matrix4 Camera::GetProjection(bool bOrthographic) {
	return bOrthographic ? glm::ortho(0.0f, 400.0f, 0.0f, 300.0f, 0.1f, 100.0f) :
		glm::perspective(45.0f, 4.0f / 3.0f, 0.1f, 100.0f);
}

void Camera::SetPosition(vector3 pos) {
	position = pos;
}

void Camera::SetTarget(vector3 v3Target) {
	//hacks
	rotation = quaternion(glm::lookAt(position, v3Target, up));
	recalc();
}

void Camera::SetUp(vector3 v3Up) {
	up = v3Up;
}
void Camera::MoveForward(float fIncrement) {
	position += forward * fIncrement;
}
void Camera::MoveSideways(float fIncrement) {
	position += right * fIncrement;
}
void Camera::MoveVertical(float fIncrement) {
	position += up * fIncrement;
}
void Camera::ChangePitch(float fIncrement) {
	rotation = glm::rotate(rotation, fIncrement, vector3(1, 0, 0));
	recalc();
}
void Camera::ChangeRoll(float fIncrement) {
	rotation = glm::rotate(rotation, fIncrement, vector3(0, 0, 1));
	recalc();
}
void Camera::ChangeYaw(float fIncrement) {
	rotation = glm::rotate(rotation, fIncrement, vector3(0, 1, 0));
	recalc();
}
void Camera::recalc() {
	up = rotation * vector3(0, 1, 0);
	right = rotation * vector3(1, 0, 0);
	forward = rotation * vector3(0, 0, 1);
}