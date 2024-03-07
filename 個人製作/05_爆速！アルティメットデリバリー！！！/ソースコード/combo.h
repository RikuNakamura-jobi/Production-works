//=====================================
//
// comboヘッダー
// Author:中村　陸
//
//=====================================
#ifndef _COMBO_H_
#define _COMBO_H_

#include "main.h"
#include "object.h"

//マクロ定義---------------------------
#define MAX_PLACE_COMBO (3)			//多重スクロール用背景の最大数

//列挙型定義---------------------------

//クラス定義---------------------------
class CNumber;
class CObject;
class CCombo : public CObject
{
public:				//外部からアクセス可能

	//コンストラクタ・デストラクタ
	CCombo(int nPriority);				//デフォルト
	virtual ~CCombo();

	//メンバ関数
	virtual HRESULT Init(void);
	virtual void Uninit(void);
	virtual void Update(void);
	virtual void Draw(void);

	void Set(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float fWidth, float fHeight);

	void SetPos(D3DXVECTOR3 pos){ m_pos = pos; }
	D3DXVECTOR3 GetPos(void) { return m_pos; }
	void SetPosOld(D3DXVECTOR3 posOld) { m_posOld = posOld; }
	D3DXVECTOR3 GetPosOld(void) { return m_posOld; }
	void SetMove(D3DXVECTOR3 move) { m_move = move; }
	D3DXVECTOR3 GetMove(void) { return m_move; }
	void SetRot(D3DXVECTOR3 rot) { m_rot = rot; }
	D3DXVECTOR3 GetRot(void) { return m_rot; }
	void SetWidth(float fWidth) { m_fWidth = fWidth; }
	float GetWidth(void) { return m_fWidth; }
	void SetHeight(float fHeight) { m_fHeight = fHeight; }
	float GetHeight(void) { return m_fHeight; }

	void AddCombo(int nCombo) { m_nCombo += nCombo; }
	void SetCombo(int nCombo) { m_nCombo = nCombo; }
	int GetCombo(void) { return m_nCombo; }

	//静的メンバ関数
	static CCombo *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float fWidth, float fHeight);

	static int GetComboResult(void) { return m_nComboResult; }
	static void SetComboResult(int Combo) { m_nComboResult = Combo; }

protected:			//子ならアクセス可能(使わない)

private:			//外部からアクセス不可能

	//メンバ関数

	//メンバ変数
	CNumber *m_apObject2D[MAX_PLACE_COMBO];
	D3DXVECTOR3 m_pos;
	D3DXVECTOR3 m_posOld;
	D3DXVECTOR3 m_move;
	D3DXVECTOR3 m_rot;
	float m_fWidth;
	float m_fHeight;
	float m_fHue;
	int m_nCombo;

	//静的メンバ変数
	static int m_nComboResult;
};

//構造体定義---------------------------

//プロトタイプ宣言---------------------

#endif // !_COMBO_H_