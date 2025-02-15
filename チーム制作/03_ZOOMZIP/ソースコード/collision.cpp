//==================================================================================================
//
//汎用当たり判定処理
//Author:中村陸
//
//==================================================================================================
#include "main.h"
#include "collision.h"
#include "calculation.h"
#include "effect.h"

//==================================================================================================
//円の当たり判定(x.z平面)
//==================================================================================================
bool CollisionCircle(D3DXVECTOR3 posA, D3DXVECTOR3 posB, float length)
{
	//変数宣言
	bool bHit = false;

	if (hypotf(posA.x - posB.x, posA.z - posB.z) < fabsf(length))
	{//円の接触判定
		bHit = true;
	}

	return bHit;
}

//==================================================================================================
//円の当たり判定(2D)
//==================================================================================================
bool CollisionCircle2D(D3DXVECTOR3 posA, D3DXVECTOR3 posB, float length)
{
	//変数宣言
	bool bHit = false;

	if (hypotf(posA.x - posB.x, posA.y - posB.y) < fabsf(length))
	{//円の接触判定
		bHit = true;
	}

	return bHit;
}

//==================================================================================================
//球の当たり判定
//==================================================================================================
bool CollisionSphere(D3DXVECTOR3 posA, D3DXVECTOR3 posB, float length)
{
	//変数宣言
	bool bHit = false;
	D3DXVECTOR3 posDiff;

	//位置の差分計算
	posDiff = posA - posB;

	if (fabsf(D3DXVec3Length(&posDiff)) < fabsf(length))
	{//球の接触判定
		bHit = true;
	}

	return bHit;
}

//==================================================================================================
//楕円の当たり判定(x.z平面)
//==================================================================================================
bool CollisionEllipse(D3DXVECTOR3 posA, D3DXVECTOR3 posB, D3DXVECTOR3 rot, float axisL, float axisS)
{
	//変数宣言
	bool bHit = false;
	D3DXVECTOR3 posFocusA, posFocusB;
	float fAxisTemp;

	if (axisL < axisS)
	{
		fAxisTemp = axisL;
		axisL = axisS;
		axisS = fAxisTemp;
	}

	//焦点の計算
	posFocusA.x = posA.x + (cosf(rot.y) * sqrtf((float)pow(axisL, 2) - (float)pow(axisS, 2)));
	posFocusA.z = posA.x + (sinf(rot.y) * sqrtf((float)pow(axisL, 2) - (float)pow(axisS, 2)));

	posFocusB.x = posA.x + (cosf(rot.y + D3DX_PI) * sqrtf((float)pow(axisL, 2) - (float)pow(axisS, 2)));
	posFocusB.z = posA.x + (sinf(rot.y + D3DX_PI) * sqrtf((float)pow(axisL, 2) - (float)pow(axisS, 2)));

	if (axisL * 2 > hypotf(posFocusA.x - posB.x, posFocusA.z - posB.z) + hypotf(posFocusB.x - posB.x, posFocusB.z - posB.z))
	{//楕円の接触判定
		bHit = true;
	}

	return bHit;
}

//==================================================================================================
//楕円の当たり判定(2D)
//==================================================================================================
bool CollisionEllipse2D(D3DXVECTOR3 posA, D3DXVECTOR3 posB, D3DXVECTOR3 rot, float axisL, float axisS)
{
	//変数宣言
	bool bHit = false;
	D3DXVECTOR3 posFocusA, posFocusB;
	float fAxisTemp;

	if (axisL < axisS)
	{
		fAxisTemp = axisL;
		axisL = axisS;
		axisS = fAxisTemp;
	}

	//焦点の計算
	posFocusA.x = posA.x + (cosf(rot.z) * sqrtf((float)pow(axisL, 2) - (float)pow(axisS, 2)));
	posFocusA.y = posA.y + (sinf(rot.z) * sqrtf((float)pow(axisL, 2) - (float)pow(axisS, 2)));

	posFocusB.x = posA.x + (cosf(rot.z + D3DX_PI) * sqrtf((float)pow(axisL, 2) - (float)pow(axisS, 2)));
	posFocusB.y = posA.y + (sinf(rot.z + D3DX_PI) * sqrtf((float)pow(axisL, 2) - (float)pow(axisS, 2)));
	
	//二つの焦点との距離の和が一定の値以下をとるとき判定する
	if (axisL * 2 > hypotf(posFocusA.x - posB.x, posFocusA.y - posB.y) + hypotf(posFocusB.x - posB.x, posFocusB.y - posB.y))
	{//楕円の接触判定
		bHit = true;
	}

	return bHit;
}

//==================================================================================================
//楕円球の当たり判定(未完成)
//==================================================================================================
bool CollisionOval(D3DXVECTOR3 posA, D3DXVECTOR3 posB, D3DXVECTOR3 rot, float axisL, float axisS)
{
	//変数宣言
	bool bHit = false;
	D3DXVECTOR3 posFocusA, posFocusB;

	//焦点の計算
	posFocusA.x = posA.x + (cosf(rot.y) * sqrtf((float)pow(axisL, 2) - (float)pow(axisS, 2)));
	posFocusA.z = posA.x + (sinf(rot.y) * sqrtf((float)pow(axisL, 2) - (float)pow(axisS, 2)));

	posFocusB.x = posA.x - (cosf(rot.y) * sqrtf((float)pow(axisL, 2) - (float)pow(axisS, 2)));
	posFocusB.z = posA.x - (sinf(rot.y) * sqrtf((float)pow(axisL, 2) - (float)pow(axisS, 2)));

	if (hypotf(posA.x - posB.x, posA.z - posB.z) < hypotf(posFocusA.x - posB.x, posFocusA.z - posB.z) + hypotf(posFocusB.x - posB.x, posFocusB.z - posB.z))
	{//球の接触判定
		bHit = true;
	}

	return bHit;
}

//==================================================================================================
//矩形の当たり判定
//==================================================================================================
bool CollisionRectangle(D3DXVECTOR3 posA, D3DXVECTOR3 posB, D3DXVECTOR3 rot, float height, float width)
{
	bool bRec;
	bRec = false;

	return bRec;
}

//==================================================================================================
//矩形の当たり判定(2D)
//==================================================================================================
bool CollisionRectangle2D(D3DXVECTOR3 posA, D3DXVECTOR3 posB, D3DXVECTOR3 rot, float height, float width)
{
	bool bRec;
	bRec = false;

	D3DXVECTOR3 point[4];
	float fAngle, fLength;
	int nRec = 0;

	//対角線の長さを算出する
	fLength = hypotf(width, height) * 0.5f;

	//対角線の角度を算出する
	fAngle = atan2f(width, height);

	//頂点座標の設定
	point[0].x = posA.x + (sinf(rot.z - D3DX_PI + fAngle) * fLength);
	point[0].y = posA.y + (cosf(rot.z - D3DX_PI + fAngle) * fLength);
	point[0].z = 0.0f;

	point[1].x = posA.x + (sinf(rot.z - fAngle) * fLength);
	point[1].y = posA.y + (cosf(rot.z - fAngle) * fLength);
	point[1].z = 0.0f;

	point[2].x = posA.x + (sinf(rot.z + fAngle) * fLength);
	point[2].y = posA.y + (cosf(rot.z + fAngle) * fLength);
	point[2].z = 0.0f;

	point[3].x = posA.x + (sinf(rot.z + D3DX_PI - fAngle) * fLength);
	point[3].y = posA.y + (cosf(rot.z + D3DX_PI - fAngle) * fLength);
	point[3].z = 0.0f;

	for (int nCntRectangle = 0; nCntRectangle < 4; nCntRectangle++)
	{
		if (nCntRectangle == 3)
		{
			if (OuterProduct2D(point[nCntRectangle], posB, point[0]) > 0.0f)
			{
				nRec++;
			}
		}
		else
		{
			if (OuterProduct2D(point[nCntRectangle], posB, point[nCntRectangle + 1]) > 0.0f)
			{
				nRec++;
			}
		}
	}

	if (nRec == 2)
	{
		bRec = true;
	}

	return bRec;
}

//==================================================================================================
//箱の当たり判定(未完成)
//==================================================================================================
bool CollisionSquare(D3DXVECTOR3 posA, D3DXVECTOR3 posB, D3DXVECTOR3 rot, float axisL, float axisS)
{
	bool bSquare;
	bSquare = false;

	return bSquare;
}