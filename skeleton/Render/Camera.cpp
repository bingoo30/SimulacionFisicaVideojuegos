// This code contains NVIDIA Confidential Information and is disclosed to you
// under a form of NVIDIA software license agreement provided separately to you.
//
// Notice
// NVIDIA Corporation and its licensors retain all intellectual property and
// proprietary rights in and to this software and related documentation and
// any modifications thereto. Any use, reproduction, disclosure, or
// distribution of this software and related documentation without an express
// license agreement from NVIDIA Corporation is strictly prohibited.
//
// ALL NVIDIA DESIGN SPECIFICATIONS, CODE ARE PROVIDED "AS IS.". NVIDIA MAKES
// NO WARRANTIES, EXPRESSED, IMPLIED, STATUTORY, OR OTHERWISE WITH RESPECT TO
// THE MATERIALS, AND EXPRESSLY DISCLAIMS ALL IMPLIED WARRANTIES OF NONINFRINGEMENT,
// MERCHANTABILITY, AND FITNESS FOR A PARTICULAR PURPOSE.
//
// Information and code furnished is believed to be accurate and reliable.
// However, NVIDIA Corporation assumes no responsibility for the consequences of use of such
// information or for any infringement of patents or other rights of third parties that may
// result from its use. No license is granted by implication or otherwise under any patent
// or patent rights of NVIDIA Corporation. Details are subject to change without notice.
// This code supersedes and replaces all information previously supplied.
// NVIDIA Corporation products are not authorized for use as critical
// components in life support devices or systems without express written approval of
// NVIDIA Corporation.
//
// Copyright (c) 2008-2018 NVIDIA Corporation. All rights reserved.
// Copyright (c) 2004-2008 AGEIA Technologies, Inc. All rights reserved.
// Copyright (c) 2001-2004 NovodeX AG. All rights reserved.  



#include "Camera.h"
#include <ctype.h>
#include <iostream>
#include "../Particle.h"
#include "foundation/PxMat33.h"

using namespace physx;

namespace Snippets
{

	Camera::Camera(const PxVec3& eye, const PxVec3& dir)
	{
		mEye = eye;
		mDir = dir.getNormalized();
		mMouseX = 0;
		mMouseY = 0;
	}

	void Camera::handleMouse(int button, int state, int x, int y)
	{
		PX_UNUSED(state);
		PX_UNUSED(button);
		mMouseX = x;
		mMouseY = y;
	}

	bool Camera::handleKey(unsigned char key, int x, int y, float speed)
	{
		PX_UNUSED(x);
		PX_UNUSED(y);

		PxVec3 viewY = mDir.cross(PxVec3(0, 1, 0)).getNormalized();
		switch (toupper(key))
		{
		case 'W':	mEye += mDir * 2.0f * speed;		break;
		case 'S':	mEye -= mDir * 2.0f * speed;		break;
		case 'A':	mEye -= viewY * 2.0f * speed;		break;
		case 'D':	mEye += viewY * 2.0f * speed;		break;
		default:							return false;
		}
		return true;
	}

	void Camera::handleAnalogMove(float x, float y)
	{
		PxVec3 viewY = mDir.cross(PxVec3(0, 1, 0)).getNormalized();
		mEye += mDir * y;
		mEye += viewY * x;
	}

	void Camera::handleMotion(int x, int y)
	{
		int dx = mMouseX - x;
		int dy = mMouseY - y;

		PxVec3 viewY = mDir.cross(PxVec3(0, 1, 0)).getNormalized();

		PxQuat qx(PxPi * dx / 180.0f, PxVec3(0, 1, 0));
		mDir = qx.rotate(mDir);
		PxQuat qy(PxPi * dy / 180.0f, viewY);
		mDir = qy.rotate(mDir);

		mDir.normalize();

		mMouseX = x;
		mMouseY = y;
	}

	PxTransform Camera::getTransform() const
	{
		PxVec3 viewY = mDir.cross(PxVec3(0, 1, 0));

		if (viewY.normalize() < 1e-6f)
			return PxTransform(mEye);

		PxMat33 m(mDir.cross(viewY), viewY, -mDir);
		return PxTransform(mEye, PxQuat(m));
	}

	PxVec3 Camera::getEye() const
	{
		return mEye;
	}

	PxVec3 Camera::getDir() const
	{
		return mDir;
	}

	void Camera::resetCamera()
	{
		mEye = PxVec3(0.0, 45.0, 65.0);
		mDir = PxVec3(0.0, -0.25, -1.0);
	}

	// ============ IMPLEMENTACIÓN DE MÉTODOS DE SEGUIMIENTO ============

	void Camera::setFollowTarget(Particle* target)
	{
		mFollowTarget = target;
		mFollowEnabled = (target != nullptr);

		if (mFollowEnabled) {
			std::cout << "Cámara: Modo seguimiento ACTIVADO" << std::endl;
		}
		else {
			std::cout << "Cámara: Modo seguimiento DESACTIVADO" << std::endl;
		}
	}

	void Camera::setFollowOffset(const PxVec3& offset)
	{
		mFollowOffset = offset;
	}

	void Camera::setFollowDistance(float distance)
	{
		mFollowDistance = distance;
		std::cout << "Cámara: Distancia de seguimiento = " << distance << std::endl;
	}

	void Camera::setFollowHeight(float height)
	{
		mFollowHeight = height;
		std::cout << "Cámara: Altura de seguimiento = " << height << std::endl;
	}

	void Camera::setFollowEnabled(bool enabled)
	{
		mFollowEnabled = enabled;
		std::cout << "Cámara: Seguimiento " << (enabled ? "ACTIVADO" : "DESACTIVADO") << std::endl;
	}

	void Camera::updateFollow(float dt)
	{
		if (!mFollowEnabled || !mFollowTarget) {
			return;
		}

		PxVec3 targetPos = mFollowTarget->getPosition();

		// Calcular la posición deseada de la cámara
		// Detrás y arriba del objetivo
		PxVec3 desiredEye;

		// Si el objetivo está en una posición válida
		if (!targetPos.isZero()) {
			// Posición detrás del objetivo (en el eje Z negativo)
			desiredEye.x = targetPos.x + mFollowOffset.x;
			desiredEye.y = targetPos.y + mFollowHeight;     // Altura fija
			desiredEye.z = targetPos.z + mFollowDistance;   // Distancia detrás

			// Suavizar el movimiento (interpolación lineal)
			float smoothSpeed = 5.0f;  // Velocidad de suavizado
			mEye = mEye * (1.0f - smoothSpeed * dt) + desiredEye * (smoothSpeed * dt);

			// Calcular dirección de la cámara (mirar al objetivo)
			PxVec3 desiredLookAt = targetPos;
			desiredLookAt.y += 2.0f;  // Mirar un poco arriba del suelo

			// Calcular vector dirección
			mDir = (desiredLookAt - mEye).getNormalized();
		}
	}

}