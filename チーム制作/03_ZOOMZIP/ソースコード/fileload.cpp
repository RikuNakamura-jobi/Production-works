//==========================================================
//
//外部ファイル読み込み処理 [fileload.cpp]
//Author Ibuki Okusada
//
//==========================================================
#include "fileload.h"
#include <stdio.h>
#include <string.h>
#include "texture.h"
#include "meshfield.h"
#include "meshwall.h"
#include "model.h"
#include "result.h"

//==========================================================
//マクロ定義
//==========================================================
#define MAX_FILESTRING	(256)	//ファイルの文字最大
#define GAME_TXTFILE		"data\\TXT\\game.txt"		//ゲーム画面で使用する読み込み用ファイル
#define TUTORIAL_TXTFILE	"data\\TXT\\tutorial.txt"	//チュートリアル画面で使用する読み込み用ファイル
#define CHECKTXT			"data\\TXT\\check.txt"		//デバッグ用確認マップ
#define TITLE_TXTFILE		"data\\TXT\\title.txt"		//タイトル画面用読み込みファイル
#define GAMEOVERTXTFILE		"data\\TXT\\gameover.txt"	//ゲームオーバー読み込みファイル
#define GAMECLEARTXTFILE	"data\\TXT\\result.txt"	//ゲームクリア読み込みファイル
#define RANKINGTXTFILE		"data\\TXT\\ranking.txt"	//ランキング画面用読み込みファイル

#define CHACKTXT_NUM	(0)		//テキストファイル切り替え。[0]がゲーム用,[1]がチェック用

//==========================================================
//プロトタイプ宣言
//==========================================================
void LoadTxtFileData(FILE *pFile);
void LoadTextureNum(FILE *pFile);
void LoadTextureFileName(FILE *pFile);
void LoadModelNum(FILE *pFile);
void LoadModelFileName(FILE *pFile);

//==========================================================
//グローバル変数宣言
//==========================================================
char g_aTextureFileName[MAX_TEXTUREFILE][MAX_FILESTRING];	//テクスチャデータのファイル名格納用
char g_aModelFileName[MAX_MODELFILE][MAX_FILESTRING];		//モデルデータのファイル名格納用
int g_nTextureNum;		//読み込んだテクスチャ数を保持
int g_nModelNum;		//読み込んだモデル数を保持

//==========================================================
//ファイル読み込み初期化処理
//==========================================================
void InitFileLoad(void)
{
	//テクスチャファイル名初期化
	memset(&g_aTextureFileName, '\0', sizeof(g_aTextureFileName));

	//モデルファイル名初期化
	memset(&g_aModelFileName, '\0', sizeof(g_aModelFileName));

	//読み込み数初期化
	g_nTextureNum = 0;		//テクスチャ数
	g_nModelNum = 0;		//モデル数
}

//==========================================================
//ファイル読み込み終了処理
//==========================================================
void UninitFileLoad(void)
{

}

//==========================================================
//ファイル読み込み更新処理
//==========================================================
void UpdateFileLoad(void)
{

}

//==========================================================
//配置テキストファイル読み込み処理
//==========================================================
void SetFileLoad(void)
{

}

//==========================================================
//ゲームモード用読み込みファイル
//==========================================================
void GametxtFileLoad(void)
{
	FILE *pFile;

	switch (GetMode())
	{
	case MODE_TITLE:	//タイトル画面
		pFile = fopen(TITLE_TXTFILE, "r");
		break;
	case MODE_TUTORIAL:	//チュートリアル画面
		pFile = fopen(TUTORIAL_TXTFILE, "r");
		break;
	case MODE_RESULT:	//リザルト画面
	
			pFile = fopen(GAMECLEARTXTFILE, "r");
		break;
	case MODE_RANKING:	//ランキング画面
		pFile = fopen(RANKINGTXTFILE, "r");
		break;
	default:	//ゲーム画面
#if CHACKTXT_NUM
		pFile = fopen(CHECKTXT, "r");
#else
		pFile = fopen(GAME_TXTFILE, "r");
#endif 
		break;
	}

	if (pFile != NULL)
	{//ファイルが開けた場合
		char aStr[256];

		//開始文字まで読み込む
		while (1)
		{
			//テキスト読み込み
			int nResult = fscanf(pFile, "%s", &aStr[0]);

			if (strcmp(&aStr[0], START_SCRIPTTXT) == 0)
			{//スクリプト開始の文字が確認できた場合
			 //各データの読み込み開始
				LoadTxtFileData(pFile);

				break;
			}
			else if (nResult == EOF)
			{//ファイルの最後まで読み込んでしまった場合
				break;
			}
		}
		//ファイルを閉じる
		fclose(pFile);
	}
	else
	{//ファイルが開けなかった場合
		return;
	}

	//メッシュフィールドバッファ生成
	CreateFieldBuff();

	//メッシュウォールバッファ生成
	CreateWallBuff();

	//モデルファイル読み込み
	LoadModelFile(g_nModelNum);

}

//==========================================================
//ファイル内容読み込み処理
//==========================================================
void LoadTxtFileData(FILE *pFile)
{
	char aStr[256];	//余分な文章読み込み用

	while (1)
	{
		int nResult = fscanf(pFile, "%s", &aStr[0]);

		//読み込み内容確認
		if (strcmp(&aStr[0], TEXTURENUM_TXT) == 0)
		{//使用するテクスチャ枚数の場合
			LoadTextureNum(pFile);
		}
		else if (strcmp(&aStr[0], MODELNUM_TXT) == 0)
		{//使用するモデル数の場合
			LoadModelNum(pFile);
		}

		//ファイル名確認
		if (strcmp(&aStr[0], TEXTUREFILENAME_TXT) == 0)
		{//テクスチャファイル名の場合
			LoadTextureFileName(pFile);
		}
		else if (strcmp(&aStr[0], MODELFILENAME_TXT) == 0)
		{//モデルファイル名の場合
			LoadModelFileName(pFile);
		}
		else if (strcmp(&aStr[0], MODELFILE_TXT) == 0)
		{//モデルファイル名の場合
			LoadModelFileData(pFile);
		}

		//配置情報確認
		if (strcmp(&aStr[0], WALLSET_TXT) == 0)
		{//壁配置の場合
			LoadMeshWallData(pFile);
		}
		else if (strcmp(&aStr[0], FIELDSET_TXT) == 0)
		{//床配置の場合
			LoadMeshFieldData(pFile);
		}
		else if (strcmp(&aStr[0], MODELSET_TXT) == 0)
		{//モデル配置の場合
			LoadModelData(pFile);
		}
		else if (strcmp(&aStr[0], ITEMSET_TXT) == 0)
		{//パーツ配置の場合
			//LoadItemData(pFile);
		}
		else if (strcmp(&aStr[0], PARTSSET_TXT) == 0)
		{//パーツ配置の場合
			//LoadPartsData(pFile);
		}


		//終了確認
		if (strcmp(&aStr[0], END_SCRIPTTXT) == 0)
		{//スクリプト開始の文字が確認できた場合
			break;
		}
		else if (nResult == EOF)
		{//ファイルの最後まで読み込んでしまった場合
			break;
		}
	}
}

//==========================================================
//テクスチャ枚数取得
//==========================================================
void LoadTextureNum(FILE *pFile)
{
	char aStr[4];	//余分な文章読み込み用

	fscanf(pFile, "%s", &aStr[0]);	//(=)読み込み
	fscanf(pFile, "%d", &g_nTextureNum);	//使用枚数読み込み
}

//==========================================================
//テクスチャファイル名取得
//==========================================================
void LoadTextureFileName(FILE *pFile)
{
	char aStr[4];	//余分な文章読み込み用

	fscanf(pFile, "%s", &aStr[0]);	//(=)読み込み

	for (int nCntTexture = 0; nCntTexture < g_nTextureNum; nCntTexture++)
	{
		if (g_aTextureFileName[nCntTexture][0] == '\0')
		{//ファイル名を入れる場所が使われていない場合
			fscanf(pFile, "%s", &g_aTextureFileName[nCntTexture][0]);	//(=)読み込み
			break;
		}
	}
}

//==========================================================
//モデル数取得
//==========================================================
void LoadModelNum(FILE *pFile)
{
	char aStr[4];	//余分な文章読み込み用

	fscanf(pFile, "%s", &aStr[0]);	//(=)読み込み
	fscanf(pFile, "%d", &g_nModelNum);	//使用数読み込み
}

//==========================================================
//モデルファイル名取得
//==========================================================
void LoadModelFileName(FILE *pFile)
{
	char aStr[4];	//余分な文章読み込み用

	fscanf(pFile, "%s", &aStr[0]);	//(=)読み込み

	for (int nCntModel = 0; nCntModel < g_nModelNum; nCntModel++)
	{
		if (g_aModelFileName[nCntModel][0] == '\0')
		{//ファイル名を入れる場所が使われていない場合
			fscanf(pFile, "%s", &g_aModelFileName[nCntModel][0]);	//ファイル名読み込み
			break;
		}
	}
}

//==========================================================
//テクスチャ読み込み数取得
//==========================================================
int GetTextureCnt(void)
{
	return g_nTextureNum;
}

//==========================================================
//テクスチャファイル名取得
//==========================================================
const char *pTextureFileName(int nCntTxture)
{
	return &g_aTextureFileName[nCntTxture][0];
}

//==========================================================
//モデル読み込み数取得
//==========================================================
int GetModelCnt(void)
{
	return g_nModelNum;
}

//==========================================================
//モデルファイル名取得
//==========================================================
const char *pModelFileName(int nCntModel)
{
	return &g_aModelFileName[nCntModel][0];
}