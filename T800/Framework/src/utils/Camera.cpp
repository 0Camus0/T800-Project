/*********************************************************
* Copyright (C) 2017 Daniel Enriquez (camus_mm@hotmail.com)
* All Rights Reserved
*
* You may use, distribute and modify this code under the
* following terms:
* ** Do not claim that you wrote this software
* ** A mention would be appreciated but not needed
* ** I do not and will not provide support, this software is "as is"
* ** Enjoy, learn and share.
*********************************************************/

#include <utils\Camera.h>

const	XVECTOR3	Camera::LookConstCameraSpace = XVECTOR3(0.0f, 0.0f, 1.0f);
const	XVECTOR3	Camera::RightConstCameraSpace = XVECTOR3(1.0f, 0.0f, 0.0f);
const	XVECTOR3	Camera::UpConstCameraSpace = XVECTOR3(0.0f, 1.0f, 0.0f);

Camera::Camera() {
	Reset();
}

void	Camera::Init(XVECTOR3 position, float fov, float ratio, float np, float fp, bool lf) {
	this->Fov = fov;
	this->AspectRatio = ratio;
	this->NPlane = np;
	this->FPlane = fp;
	this->Eye = position;
	this->LeftHanded = lf;
	CreatePojection();
}

void	Camera::CreatePojection() {
	if(LeftHanded)
		XMatPerspectiveLH(Projection, Fov, AspectRatio, NPlane, FPlane);
	else
		XMatPerspectiveRH(Projection, Fov, AspectRatio, NPlane, FPlane);
}

void	Camera::SetLookAt(XVECTOR3 v) {
	Look = v - Eye;
	Look.Normalize();
	Pitch = atan2f(-Look.y, -Look.z);
	Yaw = atan2f(-Look.x, Look.z);

	Update(1.0f/60.0f);
}

void	Camera::MoveForward(float dt) {
	Velocity.z += Speed * dt;
}

void	Camera::MoveBackward(float dt) {
	Velocity.z -= Speed * dt;
}

void	Camera::StrafeLeft(float dt) {
	Velocity.x -= Speed * dt;
}

void	Camera::StrafeRight(float dt) {
	Velocity.x += Speed * dt;
}

void	Camera::SetFov(float f) {
	this->Fov = f;
	CreatePojection();
}

void	Camera::SetRatio(float r) {
	this->AspectRatio = r;
	CreatePojection();
}

void	Camera::SetPlanes(float n, float f) {
	this->NPlane = n;
	this->FPlane = f;
	CreatePojection();
}

void	Camera::MoveYaw(float f) {
	if (MaxYaw != 0.0) {
		if ((Yaw + f) > MaxYaw || (Yaw + f) < -MaxYaw)
			return;
	}
	Yaw += f;
}

void	Camera::MovePitch(float f) {
	if (MaxPitch != 0.0) {
		if ((Pitch + f) > MaxPitch || (Pitch + f) < -MaxPitch)
			return;
	}
	Pitch += f;
}

void	Camera::MoveRoll(float f) {
	if (MaxRoll != 0.0) {
		if ((Roll + f) > MaxRoll || (Roll + f) < -MaxRoll)
			return;
	}
	Roll += f;
}

void	Camera::Update(float dt) {
	XMATRIX44	X_, Y_, Z_, T_;
	XMatRotationX(X_, -Pitch);
	XMatRotationY(Y_, -Yaw);
	XMatRotationZ(Z_, -Roll);
	View = Z_*Y_*X_;

	XMATRIX44 transpose;
	XMatTranspose(transpose, View);
	XVecTransformNormal(Look, LookConstCameraSpace, transpose);
	XVecTransformNormal(Up, UpConstCameraSpace, transpose);
	XVecTransformNormal(Right, RightConstCameraSpace, transpose);

	Look.Normalize();
	Up.Normalize();
	Right.Normalize();

	XVECTOR3 currentvelocity = Velocity.x*Right + Velocity.y*Up + Velocity.z*Look;
	Velocity -= Velocity*Friction;
	Eye += currentvelocity*dt;

	XMatTranslation(T_, (-Eye));
	View = T_*View;
	VP = View*Projection;
}

void	Camera::Reset() {
	Eye = XVECTOR3(0.0f, 0.0f, 0.0f);
	Velocity = XVECTOR3(0.0f, 0.0f, 0.0f);
	Fov = Deg2Rad(45.0f);
	NPlane = 0.01f;
	FPlane = 1000.0f;
	AspectRatio = 1.0f;
	Speed = 5000.0;
	Yaw = 0.0f;
	Pitch = 0.0f;
	Roll = 0.0f;
	Friction = 0.01f;
	MaxRoll = 0.0f;
	MaxPitch = Deg2Rad(89.0f);
	MaxYaw = 0.0f;
	LeftHanded = true;
}