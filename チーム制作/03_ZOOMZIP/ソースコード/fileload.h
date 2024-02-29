//==========================================================
//
//外部ファイル読み込み処理 [fileload.h]
//Author Ibuki Okusada
//
//==========================================================
#ifndef _FILELOAD_H_
#define _FILELOAD_H_

#include "main.h"			//main.hで定義しているものが必要なためinclude

//==========================================================
//マクロ定義
//==========================================================
//確認文字
#define START_SCRIPTTXT	"SCRIPT"		//スクリプト開始確認文字
#define END_SCRIPTTXT	"END_SCRIPT"	//スクリプト終了確認文字
#define TEXTURENUM_TXT	"NUM_TEXTURE"	//テクスチャ使用枚数確認文字
#define TEXTUREFILENAME_TXT	"TEXTURE_FILENAME"	//テクスチャファイル名確認文字
#define MODELNUM_TXT	"NUM_MODEL"				//モデル使用確認文字
#define MODELFILENAME_TXT	"MODEL_FILENAME"	//モデルファイル名確認文字
#define FIELDSET_TXT	"FIELDSET"		//メッシュフィールド設置確認文字
#define WALLSET_TXT		"WALLSET"		//メッシュウォール設置確認文字
#define MODELSET_TXT	"MODELSET"		//モデル配置確認文字
#define MOTIONSET_TXT	"MOTIONSET"		//モーション確認文字
#define NUMKEY_TXT		"NUM_KEY"		//数読み込み確認文字
#define NUMLOOP_TXT		"LOOP"			//パーツ数読み込み確認文字
#define NUMFRAME_TXT	"FRAME"			//フレーム読み込み確認文字
#define SETMOTION_TXT	"MOTIONSET"		//モーション情報確認文字
#define KEYSET_TXT		"KEYSET"		//キーセット情報確認文字
#define KEY_TXT			"KEY"			//キー確認文字
#define LOAD_POS		"POS"			//座標
#define LOAD_ROT		"ROT"			//向き
#define LOAD_TEXTYPE	"TEXTYPE"		//テクスチャ番号
#define LOAD_MODELTYPE	"TYPE"			//モデル番号
#define LOAD_BLOCK		"BLOCK"			//枚数
#define LOAD_SIZE		"SIZE"			//サイズ
#define LOAD_SHADOW		"SHADOW"		//サイズ
#define ENDFIELDSET_TXT	"END_FIELDSET"	//床読み込み終了
#define ENDWALLSET_TXT	"END_WALLSET"	//壁読み込み終了
#define ENDMODELSET_TXT	"END_MODELSET"	//モデル読み込み終了
#define ENDMOTIONSET_TXT	"END_MOTIONSET"	//モーション情報読み込み終了
#define ENDKEYSET_TXT	"END_KEYSET"	//キーセット情報読み込み終了
#define ENDKEY_TXT		"END_KEY"		//キー読み込み終了
#define CHARACTERSET_TXT	"CHARACTERSET"	//キャラクター情報読み込み確認文字
#define ENDCHARACTERSET_TXT	"END_CHARACTERSET"	//キャラクター情報読み込み終了文字
#define PARTSSET_TXT	"PARTSSET"		//パーツ配置情報読み込み開始
#define ENDPARTSSET_TXT	"END_PARTSSET"	//パーツ情報読み込み終了
#define LOAD_INDEX		"INDEX"			//パーツ番号
#define LOAD_PARENT		"PARENT"		//親番号
#define MOTIONSET_TXT	"MOTIONSET"		//モーション情報確認文字
#define ENDMOTIONSET_TXT	"END_MOTIONSET"	//モーション情報読み込み終了文字
#define LOAD_BLOCKTYPE	"BLOCKTYPE"		//モーション情報確認文字
#define MODELFILE_TXT	"MODELFILESET"	//ファイル情報確認
#define END_MODELFILE_TXT	"END_MODELFILESET"	//ファイル情報確認
#define ITEMTYPE_TXT	"ITEMTYPE"		//アイテム種類確認文字
#define ITEMSET_TXT		"ITEMSET"		//アイテム配置情報確認文字
#define END_ITEMSETTXT	"END_ITEMSET"	//アイテム配置情報読み込み終了
#define PARTSSET_TXT	"PARTSSET"		//パーツ配置情報確認文字
#define END_PARTSSETTXT	"END_PARTSSET"	//パーツ配置情報読み込み終了
#define BODYTYPE_TXT	"BODYTYPE"		//半身確認文字
#define AREASET_TXT		"AREASET"		//Tipsエリア情報確認文字
#define END_AREASETTXT	"END_AREASET"	//Tipsエリア情報読み込み終了
#define NUM_AREATXT		"NUM_AREA"		//Tipsエリア情報数確認文字
#define CHACKPOS_TXT	"CHACKPOS"		//Tipsチェックポイント確認文字
#define PARTSFILETXT		"PARTSFILESET"		//読み込み確認
#define END_PARTSFILETXT	"END_PARTSFILESET"	//終了確認
#define PARTSTYPE_TXT		"PARTSTYPE"			//パーツ種類
#define PARTSFILENAME_TXT	"PARTS_FILENAME"	//パーツファイル名確認

//==========================================================
//プロトタイプ宣言
//==========================================================
void InitFileLoad(void);
void UninitFileLoad(void);
void UpdateFileLoad(void);
void SetFileLoad(void);

//各読み込みファイル別
void GametxtFileLoad(void);
int GetTextureCnt(void);
int GetModelCnt(void);
const char *pTextureFileName(int nCntTxture);
const char *pModelFileName(int nCntModel);

#endif