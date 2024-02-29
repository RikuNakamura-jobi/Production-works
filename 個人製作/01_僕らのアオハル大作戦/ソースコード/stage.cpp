//===========================
//
//ダイレクトX.stageファイル
//Author:中村　陸
//
//===========================
#include "main.h"
#include "stage.h"
#include "block.h"
#include "start.h"
#include "goal.h"
#include "enemy.h"
#include "coin.h"
#include <stdio.h>

//グローバル変数宣言
STAGESTATE g_stageState;
int g_stageType;

//===========================
//ゲーム画面の初期化処理
//===========================
void InitStage(void)
{
	int nStageNum;
	int stageType;
	int nBlock[16];

	FILE *pFile = fopen("data\\BLOCK\\BlockData.csv", "r");

	if (pFile == NULL)
	{
		return;
	}

	for (int nCnt = 0; nCnt < STAGESTATE_MAX - 1; nCnt++)
	{
		fscanf(pFile, "%d,%d,,,,,,,,,,,,,,", &nStageNum, &stageType);

		for (int nCnt1 = 0; nCnt1 < 9; nCnt1++)
		{
			fscanf(pFile,
				"%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d",
				&nBlock[0], &nBlock[1], &nBlock[2], &nBlock[3],
				&nBlock[4], &nBlock[5], &nBlock[6], &nBlock[7],
				&nBlock[8], &nBlock[9], &nBlock[10], &nBlock[11],
				&nBlock[12], &nBlock[13], &nBlock[14], &nBlock[15]);

			if (nStageNum == g_stageState)
			{
				g_stageType = stageType;

				for (int nCnt2 = 0; nCnt2 < 16; nCnt2++)
				{
					if (nBlock[nCnt2] == 6)
					{
						SetStart(D3DXVECTOR3(80.0f * (float)nCnt2, 80.0f * (float)nCnt1, 0.0f), nBlock[nCnt2] - 1);
					}
					else if (nBlock[nCnt2] == 7)
					{
						SetGoal(D3DXVECTOR3(80.0f * (float)nCnt2, 80.0f * (float)nCnt1, 0.0f), nBlock[nCnt2] - 1);
					}
					else if (nBlock[nCnt2] == 8)
					{
						SetCoin(D3DXVECTOR3(80.0f * (float)nCnt2, 80.0f * (float)nCnt1, 0.0f), nBlock[nCnt2] - 1);
					}
					else if (nBlock[nCnt2] >= 100)
					{
						SetEnemy(D3DXVECTOR3((80.0f * (float)nCnt2) + 40.0f, (80.0f * (float)nCnt1) + 40.0f, 0.0f), nBlock[nCnt2] - 100, 0, 0);
					}
					else if (nBlock[nCnt2] != 0)
					{
						SetBlock(D3DXVECTOR3(80.0f * (float)nCnt2, 80.0f * (float)nCnt1, 0.0f), nBlock[nCnt2] - 1);
					}
				}
			}
		}
	}

	fclose(pFile);
}

//===========================
//ゲーム画面状態取得処理
//===========================
void SetStageState(STAGESTATE state, int nCounter)
{
	g_stageState = state;
}

//===========================
//ゲーム画面状態取得処理
//===========================
int GetStageType()
{
	return g_stageType;
}