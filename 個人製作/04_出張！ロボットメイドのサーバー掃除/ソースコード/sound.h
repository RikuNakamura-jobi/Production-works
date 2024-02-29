//=============================================================================
//
// サウンド処理 [sound.h]
// Author :中村陸
//
//=============================================================================
#ifndef _SOUND_H_
#define _SOUND_H_

#include "main.h"

//サウンドクラス
class CSound
{
public:

	// サウンド一覧
	typedef enum
	{
		SOUND_LABEL_BGM000 = 0,			// BGM0
		SOUND_LABEL_BGM001,				// BGM0
		SOUND_LABEL_BGM002,				// BGM0
		SOUND_LABEL_BGM003,				// BGM0
		SOUND_LABEL_BGM004,				// BGM0
		SOUND_LABEL_BGM005,				// BGM0
		SOUND_LABEL_SE_SHOT000,			// 弾発射音1
		SOUND_LABEL_SE_SCORE,			// 弾発射音1
		SOUND_LABEL_SE_YES,				// 弾発射音1
		SOUND_LABEL_SE_DAMAGE,			// 弾発射音1
		SOUND_LABEL_SE_HIT,				// 弾発射音1
		SOUND_LABEL_SE_DASH,			// 弾発射音1
		SOUND_LABEL_SE_THUNDER,			// 弾発射音1
		SOUND_LABEL_MAX,
	} SOUND_LABEL;

	// サウンド情報の構造体定義
	typedef struct
	{
		char *pFilename;	// ファイル名
		int nCntLoop;		// ループカウント
	} SOUNDINFO;

	CSound();
	~CSound();

	//メンバ関数
	HRESULT Init(HWND hWnd);
	void Uninit(void);

	//静的メンバ関数
	static HRESULT PlaySound(SOUND_LABEL label);
	static void StopSound(SOUND_LABEL label);
	static void StopSound(void);

private:

	

	//メンバ関数
	HRESULT CheckChunk(HANDLE hFile, DWORD format, DWORD *pChunkSize, DWORD *pChunkDataPosition);
	HRESULT ReadChunkData(HANDLE hFile, void *pBuffer, DWORD dwBuffersize, DWORD dwBufferoffset);

	//メンバ変数
	IXAudio2 *m_pXAudio2;								// XAudio2オブジェクトへのインターフェイス
	IXAudio2MasteringVoice *m_pMasteringVoice;			// マスターボイス

	//静的メンバ変数
	static SOUNDINFO m_aSoundInfo[SOUND_LABEL_MAX];
	static IXAudio2SourceVoice *m_apSourceVoice[SOUND_LABEL_MAX];	// ソースボイス
	static BYTE *m_apDataAudio[SOUND_LABEL_MAX];					// オーディオデータ
	static DWORD m_aSizeAudio[SOUND_LABEL_MAX];					// オーディオデータサイズ
};

#endif
