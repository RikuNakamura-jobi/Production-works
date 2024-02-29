//===========================================
//
// 当たり判定のメイン処理[collision.cpp]
// Author 小原立暉
//
//===========================================
//*******************************************
// インクルードファイル
//*******************************************
#include "game.h"
#include "manager.h"
#include "renderer.h"
#include "collision.h"
#include "shadowCircle.h"
#include "objectElevation.h"
#include "elevation_manager.h"
#include "useful.h"

//===============================
// マクロ定義
//===============================
#define COLLISION_ADD_DIFF_LENGTH		(0.01f)			// 僅かな誤差を埋めるためのマクロ定義(突っかかり防止)

//===============================
// 丸影の当たり判定処理
//===============================
void collision::ShadowCollision(const D3DXVECTOR3& pos, int nIdx)
{
	// ローカル変数宣言
	CShadowCircle* pShadow = CShadowCircle::GetShadow(nIdx);	// 影のポインタ
	D3DXVECTOR3 posPlayer = pos;				// プレイヤーの位置
	D3DXVECTOR3 posCand = NONE_D3DXVECTOR3;		// 影の位置の候補

	if (pShadow != nullptr)
	{ // 影のポインタが NULL じゃなかった場合

		// 影の位置を取得する
		posCand = D3DXVECTOR3(posPlayer.x, pShadow->GetPos().y, posPlayer.z);

		// 丸影の起伏地面の当たり判定
		ShadowElevCollision(posCand);

		// 位置を設定する
		pShadow->SetPos(posCand);
	}
}

//===============================
// 丸影と起伏地面の当たり判定
//===============================
void collision::ShadowElevCollision(D3DXVECTOR3& pos)
{
	// ローカル変数宣言
	CElevation* pMesh = CElevationManager::Get()->GetTop();		// メッシュのポインタを取得する

	while (pMesh != nullptr)
	{ // オブジェクトのが NULL じゃない限り回す

		// 影の位置を設定する
		pos.y = pMesh->ElevationCollision(pos);

		// 次のオブジェクトを代入する
		pMesh = pMesh->GetNext();
	}
}

//===============================
// 起伏地面の範囲外の当たり判定
//===============================
bool collision::ElevOutRangeCollision(D3DXVECTOR3* pPos, const D3DXVECTOR3& posOld, const float fWidth)
{
	// ローカル変数宣言
	CElevation* pElev = CElevationManager::Get()->GetTop();		// 先頭の起伏地面を取得する
	bool bCollision = false;									// 当たり判定状況

	while (pElev != nullptr)
	{ // ブロックの情報が NULL じゃない場合

		if (pElev->GetPos().z + (pElev->GetSize().z * 0.5f) >= pPos->z &&
			pElev->GetPos().z - (pElev->GetSize().z * 0.5f) <= pPos->z &&
			pElev->GetPos().y > pPos->y)
		{ // 起伏地面より下にいる場合

			if (posOld.x + fWidth <= pElev->GetPos().x - (pElev->GetSize().x * 0.5f) &&
				pPos->x + fWidth >= pElev->GetPos().x - (pElev->GetSize().x * 0.5f))
			{ // 左の当たり判定

				// 位置を設定する
				pPos->x = pElev->GetPos().x - (pElev->GetSize().x * 0.5f) - fWidth;

				// 当たり判定状況を true にする
				bCollision = true;
			}
			else if (posOld.x - fWidth >= pElev->GetPos().x + (pElev->GetSize().x * 0.5f) &&
				pPos->x - fWidth <= pElev->GetPos().x + (pElev->GetSize().x * 0.5f))
			{ // 右の当たり判定

				// 位置を設定する
				pPos->x = pElev->GetPos().x + (pElev->GetSize().x * 0.5f) + fWidth;

				// 当たり判定状況を true にする
				bCollision = true;
			}
		}

		// 次のオブジェクトを代入する
		pElev = pElev->GetNext();
	}

	// 当たり判定状況を返す
	return bCollision;
}

//============================================================
// 箱の当たり判定
//============================================================
bool collision::Square
( // 引数
	D3DXVECTOR3 posSquare,			//箱の位置
	D3DXVECTOR3 *posTarget,			//衝突した物の位置
	D3DXVECTOR3 posTargetOld,		//衝突した物の過去位置
	D3DXVECTOR3 rotSquare,			//箱の角度
	D3DXVECTOR3 offsetMax,			//箱の最大位置オフセット
	D3DXVECTOR3 offsetMin			//箱の最小位置オフセット
)
{
	D3DXVECTOR3 posCorner[8] = {};									//箱の角
	D3DXVECTOR3 posPlaneCenter[6] = {};								//箱の各面の中心
	D3DXVECTOR3 vecIntersect = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//箱との衝突点
	D3DXVECTOR3 vecNorPlaneCenter = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//平面の法線ベクトル
	D3DXPLANE plane = D3DXPLANE(0.0f, 0.0f, 0.0f, 0.0f);			//各面の平面
	float lengthX, lengthY, lengthZ;								//各方向軸の大きさ

																	//各方向軸で大きさを図る
	lengthX = fabsf(offsetMax.x - offsetMin.x) * 0.5f;
	lengthY = fabsf(offsetMax.y - offsetMin.y) * 0.5f;
	lengthZ = fabsf(offsetMax.z - offsetMin.z) * 0.5f;

	//箱の各面の中心を求める
	posPlaneCenter[0] = PosRelativeMtx(posSquare, rotSquare, D3DXVECTOR3(lengthX, 0.0f, 0.0f));
	posPlaneCenter[1] = PosRelativeMtx(posSquare, rotSquare, D3DXVECTOR3(-lengthX, 0.0f, 0.0f));
	posPlaneCenter[2] = PosRelativeMtx(posSquare, rotSquare, D3DXVECTOR3(0.0f, lengthY, 0.0f));
	posPlaneCenter[3] = PosRelativeMtx(posSquare, rotSquare, D3DXVECTOR3(0.0f, -lengthY, 0.0f));
	posPlaneCenter[4] = PosRelativeMtx(posSquare, rotSquare, D3DXVECTOR3(0.0f, 0.0f, lengthZ));
	posPlaneCenter[5] = PosRelativeMtx(posSquare, rotSquare, D3DXVECTOR3(0.0f, 0.0f, -lengthZ));

	//posが箱の中に入っている
	if (SquareTrigger(posSquare, *posTarget, rotSquare, offsetMax, offsetMin) == true)
	{
		//6面全てで確かめる
		for (int nCnt = 0; nCnt < 6; nCnt++)
		{
			//各面の法線ベクトルを計算する
			vecNorPlaneCenter = posSquare - posPlaneCenter[nCnt];
			D3DXVec3Normalize(&vecNorPlaneCenter, &vecNorPlaneCenter);

			//法線ベクトルから平面の式を計算する
			D3DXPlaneFromPointNormal(&plane, &posPlaneCenter[nCnt], &vecNorPlaneCenter);

			//平面の式と点から
			if (D3DXPlaneDotCoord(&plane, posTarget) > 0.0f && D3DXPlaneDotCoord(&plane, &posTargetOld) <= 0.0f)
			{
				//平面と位置過去位置から衝突点を算出
				D3DXPlaneIntersectLine(&vecIntersect, &plane, posTarget, &posTargetOld);

				//衝突点を修正
				if (posPlaneCenter[nCnt].x == 0.0f)
				{
					vecIntersect.x = 0.0f;
				}

				if (posPlaneCenter[nCnt].y == 0.0f)
				{
					vecIntersect.y = 0.0f;
				}

				if (posPlaneCenter[nCnt].z == 0.0f)
				{
					vecIntersect.z = 0.0f;
				}

				//位置修正
				*posTarget = vecIntersect - (vecNorPlaneCenter * 7.0f);

				return true;
			}
		}
	}

	return false;
}

//============================================================
//箱の当たり判定(内外判定のみ)
//============================================================
bool collision::SquareTrigger
(// 引数
	D3DXVECTOR3 posSquare,		//箱の位置
	D3DXVECTOR3 posTarget,		//衝突した物の位置
	D3DXVECTOR3 rotSquare,		//箱の角度
	D3DXVECTOR3 offsetMax,		//箱の最大位置オフセット
	D3DXVECTOR3 offsetMin		//箱の最小位置オフセット
)
{
	bool bSquare;							//判定用bool
	bSquare = false;

	D3DXVECTOR3 posCorner[8];			//箱の角
	D3DXVECTOR3 posPlaneCenter[6];		//箱の各面の中心
	D3DXVECTOR3 vecNorPlaneCenter[6];	//平面の法線ベクトル
	D3DXPLANE plane[6];					//各面の平面
	int nCheckCollision = 0;			//面で当たったかどうか確認用変数

										//箱の各面の中心を求める
	posPlaneCenter[0] = PosRelativeMtx(posSquare, rotSquare, D3DXVECTOR3(offsetMax.x, 0.0f, 0.0f));
	posPlaneCenter[1] = PosRelativeMtx(posSquare, rotSquare, D3DXVECTOR3(offsetMin.x, 0.0f, 0.0f));
	posPlaneCenter[2] = PosRelativeMtx(posSquare, rotSquare, D3DXVECTOR3(0.0f, offsetMax.y, 0.0f));
	posPlaneCenter[3] = PosRelativeMtx(posSquare, rotSquare, D3DXVECTOR3(0.0f, offsetMin.y, 0.0f));
	posPlaneCenter[4] = PosRelativeMtx(posSquare, rotSquare, D3DXVECTOR3(0.0f, 0.0f, offsetMax.z));
	posPlaneCenter[5] = PosRelativeMtx(posSquare, rotSquare, D3DXVECTOR3(0.0f, 0.0f, offsetMin.z));

	for (int nCnt = 0; nCnt < 6; nCnt++)
	{
		//各面の法線ベクトルを計算する
		vecNorPlaneCenter[nCnt] = posSquare - posPlaneCenter[nCnt];
		D3DXVec3Normalize(&vecNorPlaneCenter[nCnt], &vecNorPlaneCenter[nCnt]);

		//法線ベクトルから平面の式を計算する
		D3DXPlaneFromPointNormal(&plane[nCnt], &posPlaneCenter[nCnt], &vecNorPlaneCenter[nCnt]);

		//平面の式と点から
		if (D3DXPlaneDotCoord(&plane[nCnt], &posTarget) > 0.0f)
		{
			nCheckCollision++;
		}
	}

	//全ての面で内側だったら
	if (nCheckCollision == 6)
	{
		bSquare = true;
	}

	return bSquare;
}

//========================================
//3次元空間での行列による回転座標変換関数
//(任意の点からのオフセット位置を角度と距離で変換)
//========================================
D3DXVECTOR3 collision::PosRelativeMtx
(
	D3DXVECTOR3 posO,		//回転中心点
	D3DXVECTOR3 rot,		//角度
	D3DXVECTOR3 offset		//オフセット位置
)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::Get()->GetRenderer()->GetDevice();			// デバイスのポインタ
	D3DXVECTOR3 posAnswer;																//返答用変数
	D3DXMATRIX mtxO, mtxAnswer;															//計算用マトリックス
	D3DXMATRIX mtxRot, mtxTrans;														//計算用マトリックス
	D3DXMATRIX mtxRotModel, mtxTransModel, mtxPalent;									//計算用マトリックス

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

	//ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &mtxO);

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

	//ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &mtxAnswer);

	//変数用変数に代入
	posAnswer.x = mtxAnswer._41;
	posAnswer.y = mtxAnswer._42;
	posAnswer.z = mtxAnswer._43;

	return posAnswer;
}