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

#ifndef UAD_PRIMITIVE_INSTANCE_H
#define UAD_PRIMITIVE_INSTANCE_H

#include <Config.h>

#include <scene\PrimitiveBase.h>
#include <utils\xMaths.h>

class PrimitiveInst {
	public:
		void	CreateInstance(PrimitiveBase *pPrim, XMATRIX44 *pVP) {
			pBase = pPrim;
			pViewProj = pVP;
			XMatIdentity(Position);
			XMatIdentity(Scale);
			XMatIdentity(RotationX);
			XMatIdentity(RotationY);
			XMatIdentity(RotationZ);
			XMatIdentity(RotationZ);
			XMatIdentity(Final);
			Visible = true;
		}

		void	TranslateAbsolute(float x, float y, float z);
		void	RotateXAbsolute(float ang);
		void	RotateYAbsolute(float ang);
		void	RotateZAbsolute(float ang);
		void	ScaleAbsolute(float sc);

		void	TranslateRelative(float x, float y, float z);
		void	RotateXRelative(float ang);
		void	RotateYRelative(float ang);
		void	RotateZRelative(float ang);
		void	ScaleRelative(float sc);

		void	Update();
		void	Draw();

		void ToogleVisible() { Visible = !Visible; }
		void SetVisible(bool f) { Visible = f; }
		bool Visible;

		XMATRIX44		Position;
		XMATRIX44		Scale;
		XMATRIX44		RotationX;
		XMATRIX44		RotationY;
		XMATRIX44		RotationZ;
		XMATRIX44		Final;

		XMATRIX44		*pViewProj;

		PrimitiveBase	*pBase;
};


#endif