//===================================
//
// 当たり判定ヘッダー[collision.h]
// Author 小原立暉
//
//===================================
#ifndef _COLLISION_H_			//このマクロ定義がされていなかったら
#define _COLLISION_H_			//2重インクルード防止のマクロを定義する

//***********************************
// インクルードファイル
//***********************************
#include "main.h"

//-----------------------------------
// 名前空間(当たり判定)
//-----------------------------------
namespace collision
{
	// 丸影の当たり判定
	void ShadowCollision(const D3DXVECTOR3& pos, int nIdx);							// 丸影の当たり判定
	void ShadowElevCollision(D3DXVECTOR3& pos);										// 丸影と起伏地面の当たり判定

	// 起伏地面の当たり判定
	bool ElevOutRangeCollision(D3DXVECTOR3* pPos, const D3DXVECTOR3& posOld, const float fWidth);			// 起伏地面の範囲外の当たり判定

	// 箱の当たり判定
	bool Square
	( // 引数
		D3DXVECTOR3 posSquare,		//箱の位置
		D3DXVECTOR3 *posTarget,		//衝突した物の位置
		D3DXVECTOR3 posTargetOld,	//衝突した物の過去位置
		D3DXVECTOR3 rotSquare,		//箱の角度
		D3DXVECTOR3 offsetMax,		//箱の最大位置オフセット
		D3DXVECTOR3 offsetMin		//箱の最小位置オフセット
	);
	bool SquareTrigger		//箱の当たり判定(内外判定のみ)
	(// 引数
		D3DXVECTOR3 posSquare,		//箱の位置
		D3DXVECTOR3 posTarget,		//衝突した物の位置
		D3DXVECTOR3 rotSquare,		//箱の角度
		D3DXVECTOR3 offsetMax,		//箱の最大位置オフセット
		D3DXVECTOR3 offsetMin		//箱の最小位置オフセット
	);
	D3DXVECTOR3 PosRelativeMtx	//3次元空間での行列による回転座標変換関数
	(
		D3DXVECTOR3 posO,			//回転中心点
		D3DXVECTOR3 rot,			//角度
		D3DXVECTOR3 offset			//オフセット位置
	);
}

#endif