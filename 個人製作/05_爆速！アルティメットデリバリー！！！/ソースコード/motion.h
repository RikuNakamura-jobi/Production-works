//=====================================
//
// modelヘッダー
// Author:中村　陸
//
//=====================================
#ifndef _MOTION_H_
#define _MOTION_H_

#include "main.h"
#include "object.h"

//マクロ定義---------------------------
#define MAX_KEY (32)
#define MAX_KEY_INFO (16)
#define MAX_MOTION (16)

//列挙型定義---------------------------

//クラス定義---------------------------
class CModel;
class CMotion
{
public:				//外部からアクセス可能

	//プレイヤーの構造体
	struct Key
	{
		D3DXVECTOR3 pos;
		D3DXVECTOR3 rot;
	};

	//プレイヤーの構造体
	struct KeyInfo
	{
		Key aKey[MAX_KEY];
		int nFrame;
	};

	struct Info
	{
		KeyInfo aKeyInfo[MAX_KEY_INFO];
		int nLoop;
		int nNumKey;
	};

	//コンストラクタ・デストラクタ
	CMotion();
	~CMotion();

	//メンバ関数
	HRESULT Init(void);
	void Update(void);

	void Set(int nType);
	void SetInfo(Info Info);
	void SetModel(CModel **ppModel, int nNumModel);

	int GetType(void) { return m_nType; }
	bool IsFinish(void) { return m_bFinish; }

	int GetKey(void) { return m_nKey; }
	int GetFrame(void) { return m_nNumCounter; }

	void ResetFrame(void) { m_nNumCounter = 0; }

protected:			//子ならアクセス可能(使わない)

private:			//外部からアクセス不可能

	//メンバ関数

	//メンバ変数
	Info m_aInfo[MAX_MOTION];
	KeyInfo aKeyInfoBlend;
	int m_nNumInfo;

	int m_nType;
	bool m_bLoop;
	int m_nNumKey;
	int m_nKey;
	int m_nNumCounter;
	bool m_bFinish;

	CModel **m_ppModel;
	int m_nNumModel;
	//静的メンバ変数

};

//構造体定義---------------------------

//プロトタイプ宣言---------------------

#endif // !_MOTION_H_