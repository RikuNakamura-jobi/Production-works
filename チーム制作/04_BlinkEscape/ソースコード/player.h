//==========================================
//
//  プレイヤークラス(player.h)
//  Author : Tomoya Kanazaki
//
//==========================================
#ifndef _PLAYER_H_
#define _PLAYER_H_
#include "object2D.h"

//==========================================
//	前方宣言
//==========================================
class CObjectOrbit;	// オブジェクト軌跡クラス

//==========================================
//  クラス定義
//==========================================
class CPlayer : public CObject2D
{
public:

	//メンバ関数
	CPlayer();
	~CPlayer();

	HRESULT Init(void) override;
	void Uninit(void) override;
	void Update(void) override;
	void Draw(void) override;

	void GetVecInfo(D3DXVECTOR3* posCurrent, D3DXVECTOR3* posNext, D3DXVECTOR3* vecMove); //移動情報の取得
	float GetMove(void) { return m_vecMove.y; } //Y方向の移動を取得
	bool GetMiss(void) { return m_bMiss; }
	void SetMiss(void) { m_bMiss = true; }
	bool IsMove(void) { return m_bMove; }

	//静的メンバ関数
	static CPlayer* Create(const D3DXVECTOR3& rPos, const D3DXVECTOR3& rSize = VEC3_ONE, const D3DXVECTOR3& rRot = VEC3_ZERO, const D3DXCOLOR& rCol = XCOL_WHITE);

private:

	//メンバ関数
	void Move(D3DXVECTOR3 pos); //移動
	void Rotation(void); //回転

	//メンバ変数
	CObjectOrbit *m_pOrbit;	// オブジェクト軌跡クラス
	D3DXVECTOR3 m_posNext;	// 次座標
	D3DXVECTOR3 m_vecMove;	// 移動ベクトル
	bool m_bMiss; //失敗
	bool m_bMove;

};

#endif
