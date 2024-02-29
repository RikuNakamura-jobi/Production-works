//===========================================
//
// 攻撃側復活地点ヘッダー[off_respawn.h]
// Author 小原立暉
//
//===========================================
#ifndef _OFF_RESPAWN_H_
#define _OFF_RESPAWN_H_

//-------------------------------------------
// インクルードファイル
//-------------------------------------------
#include "object.h"

//-------------------------------------------
// クラス定義(攻撃側復活地点)
//-------------------------------------------
class COffRespawn : public CObject
{
public:		// 誰でもアクセスできる

	COffRespawn();					// コンストラクタ
	~COffRespawn();			// デストラクタ

	// メンバ関数
	HRESULT Init(void);	// 初期化処理
	void Uninit(void);	// 終了処理
	void Update(void);	// 更新処理
	void Draw(void);	// 描画処理

	void SetData(const D3DXVECTOR3 pos);				// 情報の設定処理

	// セット・ゲット関係
	void SetPos(const D3DXVECTOR3& pos);		// 位置の設定処理
	D3DXVECTOR3 GetPos(void) const;				// 位置の取得処理

	// 静的メンバ関数
	static COffRespawn* Create(const D3DXVECTOR3 pos);	// 生成処理

private:		// 自分だけアクセスできる

	// メンバ変数
	D3DXVECTOR3 m_pos;		// 位置
};

#endif