//============================================================
//
// Xファイルヘッダー [xfile.h]
// Author：小原立暉
//
//============================================================
#ifndef _XFILE_H_	// このマクロ定義がされていない場合
#define _XFILE_H_	// 二重インクルード防止のマクロを定義する

//------------------------------------------------------------
// マクロ定義
//------------------------------------------------------------
#define INIT_VTX_MIN	(D3DXVECTOR3( 9999.0f,  9999.0f,  9999.0f))		// モデルの最小の頂点座標の初期値
#define INIT_VTX_MAX	(D3DXVECTOR3(-9999.0f, -9999.0f, -9999.0f))		// モデルの最大の頂点座標の初期値
#define INIT_SIZE		(D3DXVECTOR3(0.0f, 0.0f, 0.0f))					// モデルサイズの初期値
#define INIT_PLAYER		(CXFile::TYPE_PLAYER_HIP)						// プレイヤーのモデルの初期値

//------------------------------------------------------------
// クラス定義(Xファイル)
//------------------------------------------------------------
class CXFile
{
public:			// 誰でもアクセスできる

	//************************************************************
	//	列挙型定義 (TYPE)
	//************************************************************
	enum TYPE
	{
		// 演出モデル
		TYPE_FRAC_SCREW,		// ネジ(破片)
		TYPE_FRAC_GEAR,			// 歯車(破片)
		TYPE_FRAC_RING,			// 輪(破片)
		TYPE_FRAC_WOOD,			// 木片(破片)
		TYPE_RIPPLE,			// 波紋

		// 通常モデル
		TYPE_FLAG,				// 旗
		TYPE_SANDBAG,			// 土嚢
		TYPE_TREE,				// 木
		TYPE_CASTLE,			// 城
		TYPE_SNOWBALL,			// 雪玉

		// プレイヤーモデル
		TYPE_PLAYER_HIP,				//腰
		TYPE_PLAYER_BODY,				//体
		TYPE_PLAYER_DEFHEAD,			//防御頭
		TYPE_PLAYER_HEAD,				//頭
		TYPE_PLAYER_RSHORDER,			//右肩
		TYPE_PLAYER_RARM,				//右腕
		TYPE_PLAYER_RHAND,				//右手
		TYPE_PLAYER_LSHORDER,			//左肩
		TYPE_PLAYER_LARM,				//左腕
		TYPE_PLAYER_LHAND,				//左手
		TYPE_PLAYER_RTHIGH,				//右腿
		TYPE_PLAYER_RFOOT,				//右脛
		TYPE_PLAYER_RLEG,				//右足
		TYPE_PLAYER_LTHIGH,				//左腿
		TYPE_PLAYER_LFOOT,				//左脛
		TYPE_PLAYER_LLEG,				//左足


		TYPE_MAX,				// 全モデルの総数
	};

	//************************************************************
	//	構造体定義 (SXFile)
	//************************************************************
	struct SXFile
	{
		LPD3DXMESH	 pMesh;						// メッシュ (頂点情報) へのポインタ
		LPD3DXBUFFER pBuffMat;					// マテリアルへのポインタ
		DWORD		 dwNumMat;					// マテリアルの数
		D3DXVECTOR3	 vtxMin;					// 最小の頂点座標
		D3DXVECTOR3	 vtxMax;					// 最大の頂点座標
		D3DXVECTOR3	 collsize;					// 当たり判定のサイズ
		float		 fRadius;					// 半径
		int			 m_nTexIdx[MAX_MATERIAL];	// テクスチャのインデックス
	};

	CXFile();			// コンストラクタ
	~CXFile();			// デストラクタ

	// メンバ関数
	static HRESULT Init(void);		// Xファイルの初期化処理
	static void Uninit(void);		// Xファイルの終了処理

	// セット・ゲット関数
	static SXFile GetXFile(TYPE type);	// Xファイルの取得処理

private:

	// メンバ関数
	static HRESULT LoadXFile(void);		// xファイルの読み込み
	static void SetCollision(void);		// 当たり判定の作成
	static HRESULT LoadTexture(void);	// テクスチャの読み込み

	// 静的メンバ変数
	static const char *c_apModelData[CXFile::TYPE_MAX];			// モデルの名前
	static SXFile m_apModel[CXFile::TYPE_MAX];					// モデルの情報
};

#endif