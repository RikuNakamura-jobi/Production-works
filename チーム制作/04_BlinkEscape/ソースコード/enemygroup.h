//==========================================
//
//  エネミーの集団(enemygroup.h)
//  Author : Tomoya Kanazaki
//
//==========================================
#ifndef _ENEMYGROUP_H_
#define _ENEMYGROUP_H_
#include "object.h"

//==========================================
//  前方宣言
//==========================================
class CEnemy;

//==========================================
//  クラス定義
//==========================================
class CEnemyGroup : public CObject
{
public:

	//メンバ関数
	CEnemyGroup();
	~CEnemyGroup();

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void SetPos(D3DXVECTOR3 pos) { m_pos = pos; }

	//静的メンバ関数
	static CEnemyGroup* Create(D3DXVECTOR3 pos, float fLength);

private:

#define NUM (4) //総数

	//メンバ変数
	D3DXVECTOR3 m_pos;
	D3DXVECTOR3 m_rot;
	int m_nNum;
	CEnemy* m_pEnemy[NUM];
	float m_fAngle;
	float m_fLength;
};

#endif
