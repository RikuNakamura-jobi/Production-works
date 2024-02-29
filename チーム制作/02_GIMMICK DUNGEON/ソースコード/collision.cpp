//==================================================================================================
//
//collision.cpp
//Author:髙山桃也
//
//==================================================================================================

#include "main.h"

//==================================================================================================
//　球の当たり判定
//==================================================================================================
bool SphereCollision(D3DXVECTOR3 pos, D3DXVECTOR3 posTarget, float width, float TargetWidth)
{
	//変数宣言
	bool bHit = false;
	D3DXVECTOR3 posDiff;
	float fLengthBet;	//差分の長さ
	float fLengthWidth = width + TargetWidth;	//幅分の長さ

	//目標の値と現在の値の差分を算出
	posDiff = posTarget - pos;

	//差分を直線距離に変換
	fLengthBet = D3DXVec3Length(&posDiff);

	//プラス・マイナスの補正
	fLengthBet = fLengthBet * fLengthBet;
	fLengthWidth = fLengthWidth * fLengthWidth;

	if (fLengthBet <= fLengthWidth)
	{// 球が接触しているかの判定
		bHit = true;
	}

	return bHit;
}