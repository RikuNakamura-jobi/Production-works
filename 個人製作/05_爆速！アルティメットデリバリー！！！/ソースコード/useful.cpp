//=====================================
//
// 
// Author:中村陸
//
//=====================================
#include "useful.h"
#include "manager.h"
#include "debugproc.h"

//マクロ定義---------------------------

//列挙型定義---------------------------

//クラス定義---------------------------

//構造体定義---------------------------

//プロトタイプ宣言---------------------

//静的メンバ変数宣言-------------------

D3DXCOLOR useful::HSLtoRGB(float Hue)
{
	D3DXCOLOR colRGB = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	float max = 255.0f;
	float min = 0.0f;

	while (1)
	{
		if (Hue >= 360.0f)
		{
			Hue -= 360.0f;
		}
		else if (Hue < 0.0f)
		{
			Hue += 360.0f;
		}
		else
		{
			break;
		}
	}

	if (Hue >= 300.0f)
	{
		colRGB.r = max;
		colRGB.g = min;
		colRGB.b = max * ((360.0f - Hue) / 60.0f);
	}
	else if (Hue >= 240.0f)
	{
		colRGB.r = max * ((Hue - 240.0f) / 60.0f);
		colRGB.g = min;
		colRGB.b = max;
	}
	else if (Hue >= 180.0f)
	{
		colRGB.r = min;
		colRGB.g = max * ((240.0f - Hue) / 60.0f);
		colRGB.b = max;
	}
	else if (Hue >= 120.0f)
	{
		colRGB.r = min;
		colRGB.g = max;
		colRGB.b = max * ((Hue - 120.0f) / 60.0f);
	}
	else if (Hue >= 60.0f)
	{
		colRGB.r = max * ((120.0f - Hue) / 60.0f);
		colRGB.g = max;
		colRGB.b = min;
	}
	else if (Hue >= 0.0f)
	{
		colRGB.r = max;
		colRGB.g = max * (Hue / 60.0f);
		colRGB.b = min;
	}

	colRGB.r /= max;
	colRGB.g /= max;
	colRGB.b /= max;
	
	return colRGB;
}

//========================================
//3次元空間での行列による回転座標変換関数
//(任意の点からのオフセット位置を角度と距離で変換)
//========================================
D3DXVECTOR3 useful::PosRelativeMtx(D3DXVECTOR3 posO, D3DXVECTOR3 rot, D3DXVECTOR3 offset)
{
	D3DXVECTOR3 posAnswer;
	D3DXMATRIX mtxO, mtxAnswer;
	D3DXMATRIX mtxRot, mtxTrans;		//計算用マトリックス
	D3DXMATRIX mtxRotModel, mtxTransModel, mtxPalent;		//計算用マトリックス

	//パーツのワールドマトリックス初期化
	D3DXMatrixIdentity(&mtxO);

	//向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot,
		rot.y, rot.x, rot.z);
	D3DXMatrixMultiply(&mtxO, &mtxO, &mtxRot);

	//位置を反映
	D3DXMatrixTranslation(&mtxTransModel,
		posO.x, posO.y, posO.z);
	D3DXMatrixMultiply(&mtxO, &mtxO, &mtxTransModel);

	mtxPalent = mtxO;

	//パーツのワールドマトリックス初期化
	D3DXMatrixIdentity(&mtxAnswer);

	//向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot,
		3.14f, 3.14f, 3.14f);
	D3DXMatrixMultiply(&mtxO, &mtxO, &mtxRot);

	//位置を反映
	D3DXMatrixTranslation(&mtxTransModel,
		offset.x, offset.y, offset.z);
	D3DXMatrixMultiply(&mtxAnswer, &mtxAnswer, &mtxTransModel);

	//算出したパーツのワールドマトリックスと親のマトリックスをかけ合わせる
	D3DXMatrixMultiply(&mtxAnswer,
		&mtxAnswer,
		&mtxPalent);

	posAnswer.x = mtxAnswer._41;
	posAnswer.y = mtxAnswer._42;
	posAnswer.z = mtxAnswer._43;

	return posAnswer;
}