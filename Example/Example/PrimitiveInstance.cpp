#include "PrimitiveInstance.h"

void PrimitiveInst::TranslateAbsolute(float x, float y, float z){
	D3DXMatrixTranslation(&Position, x, y, z);
}

void PrimitiveInst::RotateXAbsolute(float ang) {
	D3DXMatrixRotationX(&RotationX,D3DXToRadian(ang));
}

void PrimitiveInst::RotateYAbsolute(float ang) {
	D3DXMatrixRotationY(&RotationY, D3DXToRadian(ang));
}

void PrimitiveInst::RotateZAbsolute(float ang) {
	D3DXMatrixRotationZ(&RotationZ, D3DXToRadian(ang));
}

void PrimitiveInst::ScaleAbsolute(float sc) {
	D3DXMatrixScaling(&Scale,sc,sc,sc);
}

void PrimitiveInst::TranslateRelative(float x, float y, float z) {
	D3DXMATRIX tmp;
	D3DXMatrixTranslation(&tmp, x, y, z);
	Position *= tmp;
}

void PrimitiveInst::RotateXRelative(float ang) {
	D3DXMATRIX tmp;
	D3DXMatrixRotationX(&tmp, D3DXToRadian(ang));
	RotationX *= tmp;
}

void PrimitiveInst::RotateYRelative(float ang) {
	D3DXMATRIX tmp;
	D3DXMatrixRotationY(&tmp, D3DXToRadian(ang));
	RotationY *= tmp;
}

void PrimitiveInst::RotateZRelative(float ang) {
	D3DXMATRIX tmp;
	D3DXMatrixRotationZ(&tmp, D3DXToRadian(ang));
	RotationZ *= tmp;
}

void PrimitiveInst::ScaleRelative(float sc) {
	D3DXMATRIX tmp;
	D3DXMatrixScaling(&tmp, sc, sc, sc);
	Scale *= tmp;
}

void PrimitiveInst::Update() {
	Final = Scale*RotationX*RotationY*RotationZ*Position;
}

void PrimitiveInst::Draw(){
	pBase->Draw(&Final.m[0][0],&(*pViewProj).m[0][0]);
}
