//===================================
//
// 雪玉ヘッダー[snowball.h]
// Author 小原立暉
//
//===================================
#ifndef _SNOWBALL_H_
#define _SNOWBALL_H_

//***********************************
// インクルードファイル
//***********************************
#include "model.h"
#include "soldier.h"

//-----------------------------------
// クラス定義(雪玉)
//-----------------------------------
class CSnowBall : public CModel
{
public:			// 誰でもアクセスできる

	CSnowBall();			// コンストラクタ
	~CSnowBall();			// デストラクタ

	// リスト構造関係
	void SetPrev(CSnowBall* pPrev);	// 前のポインタの設定処理
	void SetNext(CSnowBall* pNext);	// 後のポインタの設定処理
	CSnowBall* GetPrev(void) const;	// 前のポインタの設定処理
	CSnowBall* GetNext(void) const;	// 次のポインタの設定処理

	// メンバ関数
	HRESULT Init(void);		// 初期化処理
	void Uninit(void);		// 終了処理
	void Update(void);		// 更新処理
	void Draw(void);		// 描画処理

	void SetData(const D3DXVECTOR3& pos, const D3DXVECTOR3& move, const CSoldier::BATTLE battle);				// 情報の設定処理

	// 静的メンバ関数
	static CSnowBall* Create(const D3DXVECTOR3& pos, const D3DXVECTOR3& move, const CSoldier::BATTLE battle);	// 生成処理

private:		// 自分だけアクセスできる

	// メンバ関数
	bool Colision();			//当たり判定

	// メンバ変数
	D3DXVECTOR3 m_move;			// 移動量
	CSoldier::BATTLE m_type;	// 攻守の種類
	int m_nLife;				// 寿命

	// リスト構造関係
	CSnowBall* m_pPrev;	// 前へのポインタ
	CSnowBall* m_pNext;	// 次へのポインタ
};

#endif