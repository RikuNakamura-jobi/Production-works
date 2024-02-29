//==========================================
//
//  マップクラス(map.h)
//  Author : Tomoya Kanazaki
//
//==========================================
#ifndef _MAP_H_
#define _MAP_H_

//==========================================
//  前方宣言
//==========================================
class CSpot;

//==========================================
//  クラス定義
//==========================================
class CMap
{
public:

	//メンバ関数
	CMap();
	~CMap();

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	float GetWorldMove(void) { return m_vecMove; } //世界移動量の取得
	D3DXVECTOR3 GetHeightMin(void); //一番低い地点の取得
	D3DXVECTOR3 GetHeightNext(void); //二番目に低い地点の取得
	void DeleteMin(void); //一番低い奴を削除
	float GetWorldSpeed(void) { return m_fSpeed; }

private:

	//差分設定用構造体
	struct DIFF
	{
		int nMinY; //Y最小値
		int nMaxY; //Y最大値
		int nMinX; //X最小値
		int nMaxX; //X最大値
	};

	//定数
	const static DIFF m_Diff;
	static float m_vecMove; //世界の移動量
#define NUM (10) //データ数

	//メンバ変数
	CSpot* m_pSpot[NUM]; //移動地点
	int m_nCntAdd;
	float m_fSpeed;

	//メンバ関数
	void SetSpot(void);

};

#endif
