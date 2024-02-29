//============================================
//
// 兵士プレイヤーヘッダー[soldier_player.h]
// Author：小原立暉
//
//============================================
#ifndef _SOLDIER_PLAYER_H_					// このマクロ定義がされていなかったら
#define _SOLDIER_PLAYER_H_					// 2重インクルード防止のマクロを定義する

//********************************************
// インクルードファイル
//********************************************
#include "soldier.h"

//--------------------------------------------
// クラス(兵士プレイヤークラス)
//--------------------------------------------
class CSoldierPlayer : public CSoldier
{
public:			// 誰でもアクセスできる

	CSoldierPlayer();			// コンストラクタ
	~CSoldierPlayer();			// デストラクタ

	// メンバ関数
	HRESULT Init(void);		// 初期化処理
	void Uninit(void);		// 終了処理
	void Update(void);		// 更新処理
	void Draw(void);		// 描画処理

	void SetData(const D3DXVECTOR3& pos, const TYPE type, const BATTLE battle);		// 情報の設定処理

private:		// 自分だけアクセスできる

	// メンバ関数
	void Control(void);				// 操作処理
	void MoveControl(void);			// 移動コントロール
	void JumpControl(void);			// ジャンプコントロール
	void ThrowControl(void);		// 投げのコントロール
	void CameraControl(void);		// カメラのコントロール

	// メンバ変数
	D3DXVECTOR3 m_CameraRot;		// カメラの向き
};

#endif