//============================================================
//
//	スコア処理 [score.cpp]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	インクルードファイル
//************************************************************
#include "score.h"
#include "manager.h"
#include "renderer.h"
#include "value.h"
#include "texture.h"

//************************************************************
//	マクロ定義
//************************************************************
#define SCORE_PRIO	(6)	// スコアの優先順位

#define SCO_NUMMIN	(0)			// 最少スコア
#define SCO_NUMMAX	(99999999)	// 最大スコア

//************************************************************
//	子クラス [CScore] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CScore::CScore() : CObject(CObject::LABEL_NUMBER, DEFAULT_PRIO)
{
	// メンバ変数をクリア
	memset(&m_apValue[0], 0, sizeof(m_apValue));	// 数値の情報
	m_pUnit	= NULL;				// 単位の情報
	m_pos	= VEC3_ZERO;		// 位置
	m_sizeValue = VEC3_ZERO;	// 数字の大きさ
	m_sizeUnit = VEC3_ZERO;		// 単位の大きさ
	m_spaceValue = VEC3_ZERO;	// 数字の空白
	m_spaceUnit = VEC3_ZERO;	// 単位の空白
	m_nNum	= 0;				// スコア
}

//============================================================
//	デストラクタ
//============================================================
CScore::~CScore()
{

}

//============================================================
//	初期化処理
//============================================================
HRESULT CScore::Init(void)
{
	// メンバ変数を初期化
	memset(&m_apValue[0], 0, sizeof(m_apValue));	// 数値の情報
	m_pUnit	= NULL;				// 単位の情報
	m_pos	= VEC3_ZERO;		// 位置
	m_sizeValue = VEC3_ZERO;	// 数字の大きさ
	m_sizeUnit = VEC3_ZERO;		// 単位の大きさ
	m_spaceValue = VEC3_ZERO;	// 数字の空白
	m_spaceUnit = VEC3_ZERO;	// 単位の空白
	m_nNum	= 0;				// スコア

	for (int nCntScore = 0; nCntScore < MAX_SCORE; nCntScore++)
	{ // スコアの桁数分繰り返す

		// 数字の生成
		m_apValue[nCntScore] = CValue::Create(CValue::TEXTURE_MAGIC_GREEN);
		if (m_apValue[nCntScore] == NULL)
		{ // 生成に失敗した場合

			// 失敗を返す
			assert(false);
			return E_FAIL;
		}

		// 優先順位を設定
		m_apValue[nCntScore]->SetPriority(SCORE_PRIO);
	}

	// 単位の生成
	m_pUnit = CObject2D::Create(VEC3_ZERO);
	if (m_pUnit == NULL)
	{ // 生成に失敗した場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// 優先順位を設定
	m_pUnit->SetPriority(SCORE_PRIO);

	m_pUnit->BindTexture(CManager::GetTexture()->Regist("data\\TEXTURE\\unit000.png"));

	// 成功を返す
	return S_OK;
}

//============================================================
//	終了処理
//============================================================
void CScore::Uninit(void)
{
	// 数字オブジェクトを破棄
	for (int nCntScore = 0; nCntScore < MAX_SCORE; nCntScore++)
	{ // スコアの桁数分繰り返す

		// 数字の終了
		m_apValue[nCntScore]->Uninit();
	}

	// 単位の終了
	m_pUnit->Uninit();

	// 自身のオブジェクトを破棄
	Release();
}

//============================================================
//	更新処理
//============================================================
void CScore::Update(void)
{
	for (int nCntScore = 0; nCntScore < MAX_SCORE; nCntScore++)
	{ // スコアの桁数分繰り返す

		// 数字の更新
		m_apValue[nCntScore]->Update();
	}

	// 単位の更新
	m_pUnit->Update();
}

//============================================================
//	描画処理
//============================================================
void CScore::Draw(void)
{

}

//============================================================
//	生成処理
//============================================================
CScore *CScore::Create
(
	const D3DXVECTOR3& rPos,		// 位置
	const D3DXVECTOR3& rSizeValue,	// 数字の大きさ
	const D3DXVECTOR3& rSizeUnit,	// 単位の大きさ
	const D3DXVECTOR3& rSpaceValue,	// 数字の空白
	const D3DXVECTOR3& rSpaceUnit	// 単位の空白
)
{
	// ポインタを宣言
	CScore *pScore = NULL;		// スコア生成用

	if (pScore == NULL)
	{ // 使用されていない場合

		// メモリ確保
		pScore = new CScore;	// スコア
	}
	else { assert(false); return NULL; }	// 使用中

	if (pScore != NULL)
	{ // 確保に成功している場合

		// スコアの初期化
		if (FAILED(pScore->Init()))
		{ // 初期化に失敗した場合

			// メモリ開放
			delete pScore;
			pScore = NULL;

			// 失敗を返す
			return NULL;
		}

		// 位置を設定
		pScore->SetPosition(rPos);

		// 大きさを設定
		pScore->SetScalingValue(rSizeValue);
		pScore->SetScalingUnit(rSizeUnit);

		// 空白を設定
		pScore->SetSpaceValue(rSpaceValue);
		pScore->SetSpaceUnit(rSpaceUnit);

		// 確保したアドレスを返す
		return pScore;
	}
	else { assert(false); return NULL; }	// 確保失敗
}

//============================================================
//	加算処理
//============================================================
void CScore::Add(const int nNum)
{
	// スコアを引数分加算
	m_nNum += nNum;

	// スコアの補正
	useful::LimitNum(m_nNum, SCO_NUMMIN, SCO_NUMMAX);

	// 数字のテクスチャ座標の設定
	SetTexNum();
}

//============================================================
//	設定処理
//============================================================
void CScore::Set(const int nNum)
{
	// スコアを引数の値に設定
	m_nNum = nNum;

	// スコアの補正
	useful::LimitNum(m_nNum, SCO_NUMMIN, SCO_NUMMAX);

	// 数字のテクスチャ座標の設定
	SetTexNum();
}

//============================================================
//	取得処理
//============================================================
int CScore::Get(void)
{
	// スコアの値を返す
	return m_nNum;
}

//============================================================
//	位置の設定処理
//============================================================
void CScore::SetPosition(const D3DXVECTOR3& rPos)
{
	// 引数の位置を設定
	m_pos = rPos;

	// 数字の表示設定
	SetDrawValue();

	// 単位の表示設定
	SetDrawUnit();
}

//============================================================
//	数字の大きさ設定処理
//============================================================
void CScore::SetScalingValue(const D3DXVECTOR3& rSize)
{
	// 引数の大きさを設定
	m_sizeValue = rSize;

	// 数字の表示設定
	SetDrawValue();
}

//============================================================
//	単位の大きさ設定処理
//============================================================
void CScore::SetScalingUnit(const D3DXVECTOR3& rSize)
{
	// 引数の大きさを設定
	m_sizeUnit = rSize;

	// 単位の表示設定
	SetDrawUnit();
}

//============================================================
//	色の設定処理
//============================================================
void CScore::SetColor(const D3DXCOLOR& rCol)
{
	for (int nCntScore = 0; nCntScore < MAX_SCORE; nCntScore++)
	{ // 数字の総数分繰り返す

		// 数字の色を設定
		m_apValue[nCntScore]->SetColor(rCol);
	}
}

//============================================================
//	数字の空白の設定処理
//============================================================
void CScore::SetSpaceValue(const D3DXVECTOR3& rSpace)
{
	// 引数の空白を設定
	m_spaceValue = rSpace;

	// 数字の表示設定
	SetDrawValue();
}

//============================================================
//	単位の空白の設定処理
//============================================================
void CScore::SetSpaceUnit(const D3DXVECTOR3& rSpace)
{
	// 引数の空白を設定
	m_spaceUnit = rSpace;

	// 単位の表示設定
	SetDrawUnit();
}

//============================================================
//	優先順位の設定処理
//============================================================
void CScore::SetPriority(const int nPriority)
{
	// 自身の優先順位を設定
	CObject::SetPriority(nPriority);

	if (m_apValue[0] != NULL)
	{ // スコアの先頭の数値が使用されている場合

		// 数字オブジェクトの優先順位を設定
		for (int nCntScore = 0; nCntScore < MAX_SCORE; nCntScore++)
		{ // スコアの桁数分繰り返す

			m_apValue[nCntScore]->SetPriority(nPriority);
		}
	}
	else { assert(false); }	// 非使用中
}

//============================================================
//	更新状況の設定処理
//============================================================
void CScore::SetEnableUpdate(const bool bUpdate)
{
	// 自身の更新状況を設定
	CObject::SetEnableUpdate(bUpdate);

	if (m_apValue[0] != NULL)
	{ // スコアの先頭の数値が使用されている場合

		// 数字オブジェクトの更新状況を設定
		for (int nCntScore = 0; nCntScore < MAX_SCORE; nCntScore++)
		{ // スコアの桁数分繰り返す

			m_apValue[nCntScore]->SetEnableUpdate(bUpdate);
		}
	}
	else { assert(false); }	// 非使用中
}

//============================================================
//	描画状況の設定処理
//============================================================
void CScore::SetEnableDraw(const bool bDraw)
{
	// 自身の描画状況を設定
	CObject::SetEnableDraw(bDraw);

	if (m_apValue[0] != NULL)
	{ // スコアの先頭の数値が使用されている場合

		// 数字オブジェクトの描画状況を設定
		for (int nCntScore = 0; nCntScore < MAX_SCORE; nCntScore++)
		{ // スコアの桁数分繰り返す

			m_apValue[nCntScore]->SetEnableDraw(bDraw);
		}
	}
	else { assert(false); }	// 非使用中

	m_pUnit->SetEnableDraw(bDraw);
}

//============================================================
//	位置取得処理
//============================================================
D3DXVECTOR3 CScore::GetPosition(void) const
{
	// 位置を返す
	return m_pos;
}

//============================================================
//	大きさ取得処理
//============================================================
D3DXVECTOR3 CScore::GetScaling(void) const
{
	// 大きさを返す
	return m_sizeValue;
}

//============================================================
//	空白取得処理
//============================================================
D3DXVECTOR3 CScore::GetSpace(void) const
{
	// 空白を返す
	return m_spaceValue;
}

//============================================================
//	単位の表示設定処理
//============================================================
void CScore::SetDrawUnit(void)
{
	if (m_apValue[0] != NULL)
	{ // スコアの先頭の数値が使用されている場合

		// 単位の位置を設定
		m_pUnit->SetPosition(m_pos + m_spaceUnit);

		// 単位の大きさを設定
		m_pUnit->SetScaling(m_sizeUnit);
	}
	else { assert(false); }	// 非使用中
}

//============================================================
//	数字の表示設定処理
//============================================================
void CScore::SetDrawValue(void)
{
	if (m_apValue[0] != NULL)
	{ // スコアの先頭の数値が使用されている場合

		for (int nCntScore = 0; nCntScore < MAX_SCORE; nCntScore++)
		{ // スコアの桁数分繰り返す

			// 数字の位置を設定
			m_apValue[nCntScore]->SetPosition(m_pos + (m_spaceValue * (float)nCntScore));

			// 数字の大きさを設定
			m_apValue[nCntScore]->SetScaling(m_sizeValue);
		}
	}
	else { assert(false); }	// 非使用中
}

//============================================================
//	数字のテクスチャ座標の設定処理
//============================================================
void CScore::SetTexNum(void)
{
	// 変数を宣言
	int aNumDivide[MAX_SCORE];	// 桁数ごとの分解用

	// スコアを桁数ごとに分解
	useful::DivideDigitNum
	( // 引数
		&aNumDivide[0],	// 分解結果の格納配列
		m_nNum,			// 分解する数値
		MAX_SCORE		// 分解する数字の桁数
	);

	for (int nCntScore = 0; nCntScore < MAX_SCORE; nCntScore++)
	{ // スコアの桁数分繰り返す

		// 数字の設定
		m_apValue[nCntScore]->SetNumber(aNumDivide[nCntScore]);
	}
}
