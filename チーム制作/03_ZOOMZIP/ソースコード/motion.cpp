//===================================================
//
//階層構造によるモーション処理 [motion.cpp]
//Author Ibuki Okusada
//
//===================================================
#include "motion.h"
#include "fileload.h"

//===================================================
//モーションの初期化処理
//===================================================
void InitMotion(void)
{

}

//===================================================
//モーションの終了処理
//===================================================
void UninitMotion(Body *pBody)
{
	for (int nCntParts = 0; nCntParts < MAX_PARTS; nCntParts++)
	{
		//メッシュの廃棄
		if (pBody->FileData.aParts[nCntParts].FileData.pMesh != NULL)
		{
			pBody->FileData.aParts[nCntParts].FileData.pMesh->Release();
			pBody->FileData.aParts[nCntParts].FileData.pMesh = NULL;
		}

		//マテリアルの廃棄
		if (pBody->FileData.aParts[nCntParts].FileData.pBuffMat != NULL)
		{
			pBody->FileData.aParts[nCntParts].FileData.pBuffMat->Release();
			pBody->FileData.aParts[nCntParts].FileData.pBuffMat = NULL;
		}

		//テクスチャの廃棄
		for (int nCntTexture = 0; nCntTexture < MAX_MODELTEX; nCntTexture++)
		{
			if (pBody->FileData.aParts[nCntParts].FileData.apTexture[nCntTexture] != NULL)
			{
				pBody->FileData.aParts[nCntParts].FileData.apTexture[nCntTexture]->Release();
				pBody->FileData.aParts[nCntParts].FileData.apTexture[nCntTexture] = NULL;
			}
		}
	}
}

//===================================================
//モーションの更新処理
//===================================================
void UpdateMotion(Body *pBody)
{
	if (pBody->FileData.aMotionInfo[pBody->nNowMotion].nNumKey > 0)
	{//キー数が存在している場合
		int nNextkey = (pBody->nNowKey + 1) % pBody->FileData.aMotionInfo[pBody->nNowMotion].nNumKey;	//次のキー

		for (int nCntParts = 0; nCntParts < pBody->FileData.nNumParts; nCntParts++)
		{
			//差分を算出
			float fPosDiffX = pBody->FileData.aMotionInfo[pBody->nNowMotion].aKeyInfo[nNextkey].aKey[nCntParts].fPosX	//X座標
				- pBody->FileData.aMotionInfo[pBody->nNowMotion].aKeyInfo[pBody->nNowKey].aKey[nCntParts].fPosX;
			float fPosDiffY = pBody->FileData.aMotionInfo[pBody->nNowMotion].aKeyInfo[nNextkey].aKey[nCntParts].fPosY	//Y座標
				- pBody->FileData.aMotionInfo[pBody->nNowMotion].aKeyInfo[pBody->nNowKey].aKey[nCntParts].fPosY;
			float fPosDiffZ = pBody->FileData.aMotionInfo[pBody->nNowMotion].aKeyInfo[nNextkey].aKey[nCntParts].fPosZ	//Z座標
				- pBody->FileData.aMotionInfo[pBody->nNowMotion].aKeyInfo[pBody->nNowKey].aKey[nCntParts].fPosZ;
			float fRotDiffX = pBody->FileData.aMotionInfo[pBody->nNowMotion].aKeyInfo[nNextkey].aKey[nCntParts].fRotX	//X向き
				- pBody->FileData.aMotionInfo[pBody->nNowMotion].aKeyInfo[pBody->nNowKey].aKey[nCntParts].fRotX;
			float fRotDiffY = pBody->FileData.aMotionInfo[pBody->nNowMotion].aKeyInfo[nNextkey].aKey[nCntParts].fRotY	//Y向き
				- pBody->FileData.aMotionInfo[pBody->nNowMotion].aKeyInfo[pBody->nNowKey].aKey[nCntParts].fRotY;
			float fRotDiffZ = pBody->FileData.aMotionInfo[pBody->nNowMotion].aKeyInfo[nNextkey].aKey[nCntParts].fRotZ	//Z向き
				- pBody->FileData.aMotionInfo[pBody->nNowMotion].aKeyInfo[pBody->nNowKey].aKey[nCntParts].fRotZ;

			//現在のフレームの座標を求める
			pBody->aSet[nCntParts].fPosX = pBody->FileData.aParts[nCntParts].SetPos.x + pBody->FileData.aMotionInfo[pBody->nNowMotion].aKeyInfo[pBody->nNowKey].aKey[nCntParts].fPosX	//X座標
				+ fPosDiffX * ((float)pBody->nNowFrame / (float)pBody->FileData.aMotionInfo[pBody->nNowMotion].aKeyInfo[pBody->nNowKey].nFrame);
			pBody->aSet[nCntParts].fPosY = pBody->FileData.aParts[nCntParts].SetPos.y + pBody->FileData.aMotionInfo[pBody->nNowMotion].aKeyInfo[pBody->nNowKey].aKey[nCntParts].fPosY	//Y座標
				+ fPosDiffY * ((float)pBody->nNowFrame / (float)pBody->FileData.aMotionInfo[pBody->nNowMotion].aKeyInfo[pBody->nNowKey].nFrame);
			pBody->aSet[nCntParts].fPosZ = pBody->FileData.aParts[nCntParts].SetPos.z + pBody->FileData.aMotionInfo[pBody->nNowMotion].aKeyInfo[pBody->nNowKey].aKey[nCntParts].fPosZ	//Z座標
				+ fPosDiffZ * ((float)pBody->nNowFrame / (float)pBody->FileData.aMotionInfo[pBody->nNowMotion].aKeyInfo[pBody->nNowKey].nFrame);

			//現在のフレームの向きを求める
			pBody->aSet[nCntParts].fRotX = pBody->FileData.aParts[nCntParts].SetRot.x + pBody->FileData.aMotionInfo[pBody->nNowMotion].aKeyInfo[pBody->nNowKey].aKey[nCntParts].fRotX	//X向き
				+ fRotDiffX * ((float)pBody->nNowFrame / (float)pBody->FileData.aMotionInfo[pBody->nNowMotion].aKeyInfo[pBody->nNowKey].nFrame);
			pBody->aSet[nCntParts].fRotY = pBody->FileData.aParts[nCntParts].SetRot.y + pBody->FileData.aMotionInfo[pBody->nNowMotion].aKeyInfo[pBody->nNowKey].aKey[nCntParts].fRotY	//Y向き
				+ fRotDiffY * ((float)pBody->nNowFrame / (float)pBody->FileData.aMotionInfo[pBody->nNowMotion].aKeyInfo[pBody->nNowKey].nFrame);
			pBody->aSet[nCntParts].fRotZ = pBody->FileData.aParts[nCntParts].SetRot.z + pBody->FileData.aMotionInfo[pBody->nNowMotion].aKeyInfo[pBody->nNowKey].aKey[nCntParts].fRotZ	//Z向き
				+ fRotDiffZ * ((float)pBody->nNowFrame / (float)pBody->FileData.aMotionInfo[pBody->nNowMotion].aKeyInfo[pBody->nNowKey].nFrame);
		}

		pBody->nNowFrame++;

		if (pBody->FileData.aMotionInfo[pBody->nNowMotion].aKeyInfo[pBody->nNowKey].nFrame != 0)
		{//フレームが0ではない場合
			if (pBody->nNowFrame % pBody->FileData.aMotionInfo[pBody->nNowMotion].aKeyInfo[pBody->nNowKey].nFrame == 0)
			{//現在のフレーム数が到達した場合
				pBody->nNowKey = (pBody->nNowKey + pBody->FileData.aMotionInfo[pBody->nNowMotion].nNumKey - 1) % pBody->FileData.aMotionInfo[pBody->nNowMotion].nNumKey;	//次のキーに移動

				if (pBody->nNowKey == pBody->FileData.aMotionInfo[pBody->nNowMotion].nNumKey - 1
					&& pBody->FileData.aMotionInfo[pBody->nNowMotion].bLoop == false)
				{//ループオフの場合
					pBody->nNowMotion = MOTIONTYPE_NEUTRAL;
					pBody->nNowKey = 0;
					pBody->nNowFrame = 0;
				}
				else
				{
					pBody->nNowFrame = 0;
				}
			}
		}
		else
		{//0の場合
			pBody->nNowKey = 0;
			pBody->nNowFrame = 0;
		}
	}
}

//===================================================
//モーションの設定処理
//===================================================
void SetMotion(Body *pBody, int nMotionType)
{
	if (pBody->nNowMotion != nMotionType)
	{//指定のモーションと現在のモーションが違う場合
		pBody->nNowMotion = nMotionType;	//モーションタイプを変更
		pBody->nNowFrame = 0;	//フレーム数リセット
		pBody->nNowKey = 0;		//現在のキーをリセット
	}
}

//==========================================================
//パーツのXファイル情報読み込み
//==========================================================
void LoadPartsFileData(Parts *pParts, const char *pFileName)
{
	int nNumVtx;		//頂点数
	DWORD dwSizeFVF;	//頂点フォーマットのサイズ
	BYTE *pVtxBuff;		//頂点バッファのポインタ
	LPDIRECT3DDEVICE9 pDevice = GetDevice();		//デバイスへのポインタ

	//Xファイルの読み込み
	D3DXLoadMeshFromX(pFileName,
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&pParts->FileData.pBuffMat,
		NULL,
		&pParts->FileData.dwNumMat,
		&pParts->FileData.pMesh);

	//頂点数を取得
	nNumVtx = pParts->FileData.pMesh->GetNumVertices();

	//頂点フォーマットのサイズを取得
	dwSizeFVF = D3DXGetFVFVertexSize(pParts->FileData.pMesh->GetFVF());

	//頂点バッファをロック
	pParts->FileData.pMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

	for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
	{
		D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVtxBuff;	//頂点座標の代入

		if (vtx.x < pParts->FileData.vtxMin.x)
		{//最小値よりも値が小さい場合
			pParts->FileData.vtxMin.x = vtx.x;
		}
		else if (vtx.x > pParts->FileData.vtxMax.x)
		{//最大値よりも値が大きい場合
			pParts->FileData.vtxMax.x = vtx.x;
		}

		if (vtx.z < pParts->FileData.vtxMin.z)
		{//最小値よりも値が小さい場合
			pParts->FileData.vtxMin.z = vtx.z;
		}
		else if (vtx.z > pParts->FileData.vtxMax.z)
		{//最大値よりも値が大きい場合
			pParts->FileData.vtxMax.z = vtx.z;
		}

		if (vtx.y < pParts->FileData.vtxMin.y)
		{//最小値よりも値が小さい場合
			pParts->FileData.vtxMin.y = vtx.y;
		}
		else if (vtx.y > pParts->FileData.vtxMax.y)
		{//最大値よりも値が大きい場合
			pParts->FileData.vtxMax.y = vtx.y;
		}

		pVtxBuff += dwSizeFVF;	//頂点フォーマットのサイズ分ポインタを進める
	}

	//頂点バッファをアンロック
	pParts->FileData.pMesh->UnlockVertexBuffer();

	D3DXMATERIAL *pMat;		//マテリアルデータへのポインタ

	//マテリアルデータへのポインタを取得
	pMat = (D3DXMATERIAL*)pParts->FileData.pBuffMat->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)pParts->FileData.dwNumMat; nCntMat++)
	{
		if (pMat[nCntMat].pTextureFilename != NULL)
		{//テクスチャファイルが存在している
		 //テクスチャの読み込み
			D3DXCreateTextureFromFile(pDevice, pMat[nCntMat].pTextureFilename, &pParts->FileData.apTexture[nCntMat]);
		}
	}

	//ポインタを指定
	pParts->model.pModelFiledata = &pParts->FileData;
}

//==========================================================
//パーツの情報テキスト読み込み
//==========================================================
void OpenBodyFile(const char *pFileData, BodyFileData *pBody)
{
	FILE *pFile;

	pFile = fopen(pFileData, "r");

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
				LoadBodyFile(pFile, pBody);
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
}

//==========================================================
//パーツテキスト情報読み込み
//==========================================================
void LoadBodyFile(FILE *pFile, BodyFileData *pBodyFile)
{
	char aStr[256];
	int nMotion = 0;	//読み込みモーション番号

	//情報読み込み開始
	while (1)
	{
		int nResult = fscanf(pFile, "%s", &aStr[0]);

		if (strcmp(&aStr[0], MODELNUM_TXT) == 0)
		{//使用するモデル数の場合
			fscanf(pFile, "%s", &aStr[0]); //=
			fscanf(pFile, "%d", &pBodyFile->nNumParts); //パーツ数取得

			//ファイル名取得
			LoadBodyXFileName(pFile, pBodyFile);
		}
		else if (strcmp(&aStr[0], CHARACTERSET_TXT) == 0)
		{//キャラクター情報の場合
			LoadPartsSetData(pFile, pBodyFile);
		}
		else if (strcmp(&aStr[0], MOTIONSET_TXT) == 0)
		{//モーション情報の場合
			LoadMotionData(pFile, pBodyFile, nMotion);
			nMotion++;	//モーション番号を進める
		}
		else if (strcmp(&aStr[0], END_SCRIPTTXT) == 0)
		{//スクリプト終了の文字が確認できた場合
			break;
		}
		else if (nResult == EOF)
		{//ファイルの最後まで読み込んでしまった場合
			break;
		}
	}
}

//==========================================================
//パーツモデルファイル名読み込み
//==========================================================
void LoadBodyXFileName(FILE *pFile, BodyFileData *pBodyFile)
{
	int nParts = 0;		//現在のパーツ番号
	char aStr[128] = {};	//配置数文字格納用

	while (1)
	{
		if (nParts >= pBodyFile->nNumParts)
		{//パーツ総数まで到達した場合
			break;
		}
		else
		{//到達していない場合
		 //テキスト読み込み
			int nResult = fscanf(pFile, "%s", &aStr[0]);

			if (strcmp(&aStr[0], MODELFILENAME_TXT) == 0)
			{//モデル情報読み込み
				fscanf(pFile, "%s", &aStr[0]); //=
				fscanf(pFile, "%s", &aStr[0]); //ファイル名

				//Xファイル情報を読み込み
				LoadPartsFileData(&pBodyFile->aParts[nParts], &aStr[0]);
				nParts++;
			}
			if (strcmp(&aStr[0], END_SCRIPTTXT) == 0)
			{//スクリプト終了の文字が確認できた場合
				break;
			}
			else if (nResult == EOF)
			{//ファイルの最後まで読み込んでしまった場合
				break;
			}
		}
	}
}

//==========================================================
//パーツの初期設置情報読み込み
//==========================================================
void LoadPartsSetData(FILE *pFile, BodyFileData *pBodyFile)
{
	char aStr[128] = {};	//配置数文字格納用
	int nCntParts = 0;
	while (1)
	{
		//テキスト読み込み
		int nResult = fscanf(pFile, "%s", &aStr[0]);

		if (strcmp(&aStr[0], PARTSSET_TXT) == 0)
		{//パーツ情報確認文字あった場合
			while (1)
			{
				//読み込み
				fscanf(pFile, "%s", &aStr[0]);

				if (nCntParts >= pBodyFile->nNumParts)
				{//パーツ総数に到達した場合
					break;
				}
				if (strcmp(&aStr[0], LOAD_POS) == 0)
				{//座標
					fscanf(pFile, "%s", &aStr[0]);	//(=)読み込み
					fscanf(pFile, "%f", &pBodyFile->aParts[nCntParts].SetPos.x);	//x座標読み込み
					fscanf(pFile, "%f", &pBodyFile->aParts[nCntParts].SetPos.y);	//y座標読み込み
					fscanf(pFile, "%f", &pBodyFile->aParts[nCntParts].SetPos.z);	//z座標読み込み
					pBodyFile->aParts[nCntParts].model.pos = pBodyFile->aParts[nCntParts].SetPos;
				}
				else if (strcmp(&aStr[0], LOAD_ROT) == 0)
				{//向き
					fscanf(pFile, "%s", &aStr[0]);	//(=)読み込み
					fscanf(pFile, "%f", &pBodyFile->aParts[nCntParts].SetRot.x);	//x座標読み込み
					fscanf(pFile, "%f", &pBodyFile->aParts[nCntParts].SetRot.y);	//y座標読み込み
					fscanf(pFile, "%f", &pBodyFile->aParts[nCntParts].SetRot.z);	//z座標読み込み
					pBodyFile->aParts[nCntParts].model.rot = pBodyFile->aParts[nCntParts].SetRot;
				}
				else if (strcmp(&aStr[0], LOAD_INDEX) == 0)
				{//パーツ番号
					fscanf(pFile, "%s", &aStr[0]);	//(=)読み込み
					fscanf(pFile, "%d", &pBodyFile->aParts[nCntParts].nIndex);	//読み込み
				}
				else if (strcmp(&aStr[0], LOAD_PARENT) == 0)
				{//親番号
					fscanf(pFile, "%s", &aStr[0]);	//(=)読み込み
					fscanf(pFile, "%d", &pBodyFile->aParts[nCntParts].nIdexParent);	//読み込み
				}
				else if (strcmp(&aStr[0], ENDPARTSSET_TXT) == 0)
				{//パーツ情報終了文字確認できた場合
					nCntParts++;
				}
			}


		}
		if (strcmp(&aStr[0], ENDCHARACTERSET_TXT) == 0)
		{//キャラクター情報終了文字確認できた場合
			return;	//なにもせず返す
		}
	}
}

//==========================================================
//モーション情報読み込み
//==========================================================
void LoadMotionData(FILE *pFile, BodyFileData *pBodyFile, int nMotion)
{
	char aStr[128] = {};	//配置数文字格納用
	int nCntParts = 0;
	int nCntKey = 0;
	bool bKey = false;
	while (1)
	{
		//テキスト読み込み
		int nResult = fscanf(pFile, "%s", &aStr[0]);

		if (nCntKey >= pBodyFile->aMotionInfo[nMotion].nNumKey && bKey == true)
		{//キー数に来た場合
			break;
		}
		if (strcmp(&aStr[0], NUMLOOP_TXT) == 0)
		{//ループ確認
			int nLoop = 0;
			fscanf(pFile, "%s", &aStr[0]);	//(=)読み込み
			fscanf(pFile, "%d", &nLoop);	//(=)読み込み

			if (nLoop == 0)
			{//ループしない場合
				pBodyFile->aMotionInfo[nMotion].bLoop = false;	//ループしないようにする
			}
			else
			{
				pBodyFile->aMotionInfo[nMotion].bLoop = true;	//ループするようにする
			}
		}
		else if (strcmp(&aStr[0], NUMKEY_TXT) == 0)
		{//キー数
			fscanf(pFile, "%s", &aStr[0]);	//(=)読み込み
			fscanf(pFile, "%d", &pBodyFile->aMotionInfo[nMotion].nNumKey);	//x座標読み込み
			bKey = true;	//キー数を取得した状態にする
		}
		else if (strcmp(&aStr[0], KEYSET_TXT) == 0)
		{//パーツ情報確認文字あった場合
			bool bFrame = false;
			nCntParts = 0;
			while (1)
			{
				//読み込み
				fscanf(pFile, "%s", &aStr[0]);

				if (nCntParts >= pBodyFile->nNumParts)
				{//パーツ総数に到達した場合
					break;
				}
				else if (strcmp(&aStr[0], NUMFRAME_TXT) == 0)
				{//フレーム数
					fscanf(pFile, "%s", &aStr[0]);	//(=)読み込み
					fscanf(pFile, "%d", &pBodyFile->aMotionInfo[nMotion].aKeyInfo[nCntKey].nFrame);	//読み込み
					bFrame = true;
				}
				else if (strcmp(&aStr[0], KEY_TXT) == 0 && bFrame == true)
				{//親番号
					while (1)
					{
						//読み込み
						fscanf(pFile, "%s", &aStr[0]);

						if (strcmp(&aStr[0], ENDKEY_TXT) == 0)
						{//フレーム数
							break;
						}
						else if (strcmp(&aStr[0], LOAD_POS) == 0)
						{//フレーム数
							fscanf(pFile, "%s", &aStr[0]);	//(=)読み込み
							fscanf(pFile, "%f", &pBodyFile->aMotionInfo[nMotion].aKeyInfo[nCntKey].aKey[nCntParts].fPosX);	//x座標読み込み
							fscanf(pFile, "%f", &pBodyFile->aMotionInfo[nMotion].aKeyInfo[nCntKey].aKey[nCntParts].fPosY);	//y座標読み込み
							fscanf(pFile, "%f", &pBodyFile->aMotionInfo[nMotion].aKeyInfo[nCntKey].aKey[nCntParts].fPosZ);	//z座標読み込み
						}
						else if (strcmp(&aStr[0], LOAD_ROT) == 0)
						{//フレーム数
							fscanf(pFile, "%s", &aStr[0]);	//(=)読み込み
							fscanf(pFile, "%f", &pBodyFile->aMotionInfo[nMotion].aKeyInfo[nCntKey].aKey[nCntParts].fRotX);	//x座標読み込み
							fscanf(pFile, "%f", &pBodyFile->aMotionInfo[nMotion].aKeyInfo[nCntKey].aKey[nCntParts].fRotY);	//y座標読み込み
							fscanf(pFile, "%f", &pBodyFile->aMotionInfo[nMotion].aKeyInfo[nCntKey].aKey[nCntParts].fRotZ);	//z座標読み込み
						}
					}
					nCntParts++;
				}
				else if (strcmp(&aStr[0], ENDKEYSET_TXT) == 0)
				{//パーツ情報終了文字確認できた場合
					break;
				}
			}
			nCntKey++;	//キーを移動
		}
		if (strcmp(&aStr[0], ENDMOTIONSET_TXT) == 0)
		{//キャラクター情報終了文字確認できた場合
			return;	//なにもせず返す
		}
	}
}

//==========================================================
//半身初期化処理
//==========================================================
void ResetBodyData(Body * pBody)
{
	pBody->pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//位置
	pBody->rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//向き
	pBody->nNowFrame = 0;	//フレーム
	pBody->nNowKey = 0;		//キー数
}

//==========================================================
//半身ファイル情報初期化処理
//==========================================================
void ResetBodyFileData(Body * pBody)
{
	pBody->FileData.nNumParts = 0;

	//パーツ情報
	for (int nCntParts = 0; nCntParts < MAX_PARTS; nCntParts++)
	{
		pBody->FileData.aParts[nCntParts].FileData.nType = 0;
		pBody->FileData.aParts[nCntParts].FileData.dwNumMat = 0;
		pBody->FileData.aParts[nCntParts].FileData.vtxMax = D3DXVECTOR3(-100000.0f, -100000.0f, -100000.0f);
		pBody->FileData.aParts[nCntParts].FileData.vtxMin = D3DXVECTOR3(100000.0f, 100000.0f, 100000.0f);
		pBody->FileData.aParts[nCntParts].FileData.pBuffMat = NULL;
		pBody->FileData.aParts[nCntParts].FileData.pMesh = NULL;
		pBody->FileData.aParts[nCntParts].model.bUse = false;
		pBody->FileData.aParts[nCntParts].model.fMoveY = 0.0f;
		pBody->FileData.aParts[nCntParts].model.nShadow = -1;
		pBody->FileData.aParts[nCntParts].model.nType = 0;
		pBody->FileData.aParts[nCntParts].model.pModelFiledata = NULL;
		pBody->FileData.aParts[nCntParts].model.pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pBody->FileData.aParts[nCntParts].model.posOld = pBody->FileData.aParts[nCntParts].model.pos;
	}

	//モーション情報
	for (int nCntMotion = 0; nCntMotion < MAX_MOTION; nCntMotion++)
	{
		pBody->FileData.aMotionInfo[nCntMotion].bLoop = false;
		pBody->FileData.aMotionInfo[nCntMotion].nNumKey = 0;
		
		for (int nCntKeyInfo = 0; nCntKeyInfo < MAX_KEY; nCntKeyInfo++)
		{
			pBody->FileData.aMotionInfo[nCntMotion].aKeyInfo[nCntKeyInfo].nFrame = 0;

			for (int nCntKey = 0; nCntKey < MAX_KEY; nCntKey++)
			{
				pBody->FileData.aMotionInfo[nCntMotion].aKeyInfo[nCntKeyInfo].aKey[nCntKey].fPosX = 0.0f;
				pBody->FileData.aMotionInfo[nCntMotion].aKeyInfo[nCntKeyInfo].aKey[nCntKey].fPosY = 0.0f;
				pBody->FileData.aMotionInfo[nCntMotion].aKeyInfo[nCntKeyInfo].aKey[nCntKey].fPosZ = 0.0f;
				pBody->FileData.aMotionInfo[nCntMotion].aKeyInfo[nCntKeyInfo].aKey[nCntKey].fRotX = 0.0f;
				pBody->FileData.aMotionInfo[nCntMotion].aKeyInfo[nCntKeyInfo].aKey[nCntKey].fRotY = 0.0f;
				pBody->FileData.aMotionInfo[nCntMotion].aKeyInfo[nCntKeyInfo].aKey[nCntKey].fRotZ = 0.0f;
			}
		}
	}
}
