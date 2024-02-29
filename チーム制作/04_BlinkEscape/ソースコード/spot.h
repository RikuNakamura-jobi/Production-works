//==========================================
//
//  地点クラス(spot.h)
//  Author : Tomoya Kanazaki
//
//==========================================
#ifndef _SPOT_H_
#define _SPOT_H_
#include "object2D.h"

class CEnemyGroup;

//==========================================
//  クラス定義
//==========================================
class CSpot : public CObject2D
{
public:

	//メンバ関数
	CSpot();
	~CSpot();

	HRESULT Init(void) override;
	void Uninit(void) override;
	void Update(void) override;
	void Draw(void) override;

	float GetPosHeight(void) { return GetPosition().y; } //高さの取得

	//静的メンバ関数
	static CSpot* Create(D3DXVECTOR3 pos, D3DXVECTOR3 size);

private:

	CEnemyGroup *m_pGroup;

};

#endif
