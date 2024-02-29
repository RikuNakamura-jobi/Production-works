//==================================================================================================
//
//calculation.h
//Author:������
//
//==================================================================================================

#ifndef _CALCULATION_H_
#define _CALCULATION_H_

#include "main.h"

//================================
// �v���g�^�C�v�錾
//================================
float OuterProduct(D3DXVECTOR3 posA, D3DXVECTOR3 posB, D3DXVECTOR3 posC);
float OuterProduct2D(D3DXVECTOR3 posA, D3DXVECTOR3 posB, D3DXVECTOR3 posC);
D3DXVECTOR3 PosRelative(D3DXVECTOR3 posO, D3DXVECTOR3 rot, float length);
float LengthPToL(D3DXVECTOR3 point, D3DXVECTOR3 linePoint1, D3DXVECTOR3 linePoint2);

#endif
