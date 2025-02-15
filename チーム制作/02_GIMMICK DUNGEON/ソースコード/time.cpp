//==================================================================================================
//
//Time.cpp
//Author:ûüRç
//
//==================================================================================================

#include "main.h"
#include "time.h"
#include "game.h"
#include "input.h"

//}Nè`
#define NUM_PLACE				(4)											//XRAÌ
#define NUM_TEX					(3)											//eNX`Ì

#define TIME_WIDTH				(50)										//¡
#define TIME_HEIGHT				(50)										//³

#define BACK_WIDTH				(300)										//
#define BACK_HEIGHT				(100)										//³

#define CLOCK_WIDTH				(5)											//
#define CLOCK_HEIGHT			(70)										//³

#define INITIAL_MINITE			(3)											//úÌÔiªj
#define INITIAL_SECOND			(0)											//úÌÔibj

#define TIME_TEX_FILE			"data\\TEXTURE\\number001.png"			//eNX`¼
#define BACK_TEX_FILE			"data\\TEXTURE\\clock000.png"			//eNX`¼
#define CLOCK_TEX_FILE			"data\\TEXTURE\\clockhands000.png"		//eNX`¼

//vÌ\¢Ì
typedef struct
{
	D3DXVECTOR3 pos;						//Êu
	D3DXVECTOR3 rot;						//ü«
	float fAngle;							//ÎpüÌpx
	float fLength;							//ÎpüÌ·³
}Clock;

//O[oÏé¾
LPDIRECT3DTEXTURE9 g_apTextureTime[NUM_TEX] = {};		//eNX`ÖÌ|C^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffTime = NULL;			//¸_obt@ÖÌ|C^
D3DXVECTOR3 g_posTime;									//XRAÌÊu
Clock g_clock;											//vÌ\¢Ìîñ
int g_nTime;											//XRAÌl
int g_nLastTime;										//OñÌXRAÌl
int g_nCntSecond;										//êbðÍ©éJE^
int g_nMinute;											//ªÌ

//vg^Cvé¾
void ControlDebug(void);

//==================================================================================================
//ú»
//==================================================================================================
void InitTime(void)
{
	//foCX|C^Ýè
	LPDIRECT3DDEVICE9 pDevice;

	//foCXÌæ¾
	pDevice = GetDevice();

	//eNX`ÇÝÝ
	D3DXCreateTextureFromFile(pDevice,
		TIME_TEX_FILE,
		&g_apTextureTime[0]);

	//eNX`ÇÝÝ
	D3DXCreateTextureFromFile(pDevice,
		BACK_TEX_FILE,
		&g_apTextureTime[1]);

	//eNX`ÇÝÝ
	D3DXCreateTextureFromFile(pDevice,
		CLOCK_TEX_FILE,
		&g_apTextureTime[2]);

	//JE^Ìú»
	g_nCntSecond = 0;
	g_nMinute = INITIAL_MINITE;

	//XRAlÌú»
	g_nTime = 0;
	g_nLastTime = INITIAL_SECOND;

	//XRAÊuÌú»
	g_posTime = D3DXVECTOR3(SCREEN_WIDTH * 0.5f,50.0f,0.0f);
	D3DXVECTOR3 posBack = { SCREEN_WIDTH * 0.5f,50.0f,0.0f };

	//vÌú»
	g_clock.pos = D3DXVECTOR3(posBack.x - 89.0f, posBack.y - 3.0f, 0.0f);
	g_clock.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_clock.fAngle = atan2f(CLOCK_WIDTH,CLOCK_HEIGHT);
	g_clock.fLength = sqrtf(CLOCK_WIDTH * CLOCK_WIDTH + CLOCK_HEIGHT * CLOCK_HEIGHT) * 0.5f;

	//¸_obt@Ì¶¬
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * (NUM_PLACE + NUM_TEX - 1),
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffTime,
		NULL);

	//¸_îñÌ|C^
	VERTEX_2D *pVtx;

	//¸_obt@ðbNµA¸_îñÖÌ|C^ðæ¾
	g_pVtxBuffTime->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntTime = 0; nCntTime < NUM_PLACE; nCntTime++)
	{//SÄÌú»

		if (nCntTime >= NUM_PLACE / 2)
		{
			g_posTime.x += TIME_WIDTH / 2;
		}

		//¸_ÀWÌÝè
		pVtx[0].pos = D3DXVECTOR3(g_posTime.x - TIME_WIDTH * 0.5f + (nCntTime * TIME_WIDTH),
								  g_posTime.y - TIME_HEIGHT * 0.5f, 0.0f);

		pVtx[1].pos = D3DXVECTOR3(g_posTime.x + TIME_WIDTH * 0.5f + (nCntTime * TIME_WIDTH),
								  g_posTime.y - TIME_HEIGHT * 0.5f, 0.0f);

		pVtx[2].pos = D3DXVECTOR3(g_posTime.x - TIME_WIDTH * 0.5f + (nCntTime * TIME_WIDTH),
								  g_posTime.y + TIME_HEIGHT * 0.5f, 0.0f);

		pVtx[3].pos = D3DXVECTOR3(g_posTime.x + TIME_WIDTH * 0.5f + (nCntTime * TIME_WIDTH),
								  g_posTime.y + TIME_HEIGHT * 0.5f, 0.0f);

		if (nCntTime >= NUM_PLACE / 2)
		{
			g_posTime.x -= TIME_WIDTH / 2;
		}

		//rhwÌÝè
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		//¸_J[ÌÝè
		pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 125);
		pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 125);
		pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 125);
		pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 125);

		//eNX`ÀW
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(0.1f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.1f, 1.0f);

		//¸_f[^Ì|C^ð4Âªißé
		pVtx += 4;
	}

	//UIwiÝè========================================================================
	//¸_ÀWÌÝè
	pVtx[0].pos = D3DXVECTOR3
	(
		posBack.x - BACK_WIDTH * 0.5f,
		posBack.y - BACK_HEIGHT * 0.5f,
		0.0f
	);
	pVtx[1].pos = D3DXVECTOR3
	(
		posBack.x + BACK_WIDTH * 0.5f,
		posBack.y - BACK_HEIGHT * 0.5f,
		0.0f
	);
	pVtx[2].pos = D3DXVECTOR3
	(
		posBack.x - BACK_WIDTH * 0.5f,
		posBack.y + BACK_HEIGHT * 0.5f,
		0.0f
	);
	pVtx[3].pos = D3DXVECTOR3
	(
		posBack.x + BACK_WIDTH * 0.5f,
		posBack.y + BACK_HEIGHT * 0.5f,
		0.0f
	);

	//rhwÌÝè
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//¸_J[ÌÝè
	pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

	//eNX`Ýè
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	pVtx += 4;

	//jÝè========================================================================
	//¸_ÀWÌÝè
	pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	//rhwÌÝè
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//¸_J[ÌÝè
	pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

	//eNX`Ýè
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//¸_obt@ðAbN
	g_pVtxBuffTime->Unlock();
}

//==================================================================================================
//I¹
//==================================================================================================
void UninitTime(void)
{
	for (int nCnt = 0; nCnt < NUM_TEX; nCnt++)
	{
		if (g_apTextureTime[nCnt] != NULL)
		{//eNX`ÖÌ|C^jü
			g_apTextureTime[nCnt]->Release();
			g_apTextureTime[nCnt] = NULL;
		}
	}

	if (g_pVtxBuffTime != NULL)
	{//¸_obt@ÖÌ|C^jü
		g_pVtxBuffTime->Release();
		g_pVtxBuffTime = NULL;
	}
}

//==================================================================================================
//XV
//==================================================================================================
void UpdateTime(void)
{
#ifdef  _DEBUG
	//fobOì
	ControlDebug();
#endif //  _DEBUG

	//Ïé¾
	int aTexU[NUM_PLACE], nCount;
	int aData1, aData2;
	D3DXCOLOR UICol = { 1.0f,1.0f,1.0f,1.0f };

	//|C^é¾
	VERTEX_2D *pVtx;

	//¸_obt@ðbNµA¸_îñÖÌ|C^ðæ¾
	g_pVtxBuffTime->Lock(0, 0, (void**)&pVtx, 0);
	
	if (g_nMinute <= 0 && g_nLastTime <= 0)
	{//Ô´ßÌâ³
		g_nMinute = 0;
		g_nLastTime = 0;
	}

	if ((g_nMinute != 0 || g_nLastTime != 0) && GetGameState() == GAMESTATE_NORMAL)
	{//ÔØêÅÈ¢êÌÝJEg_E
		g_nCntSecond++;

		if (g_nCntSecond >= 60)
		{//PbÌvª
			g_nLastTime--;

			g_nCntSecond = 0;
		}

		if (g_nLastTime < 0)
		{//êªÌvª
			g_nMinute--;

			g_nLastTime = 59;
		}

		//jðñ·
		g_clock.rot.z -= (D3DX_PI * 2) / ((INITIAL_MINITE * 60 + INITIAL_SECOND) * 60);
	}

	if (g_nMinute == 0 && g_nLastTime < 30)
	{//Ôx
		UICol = {1.0f,0.0f,0.0f,1.0f};
	}
	else if (g_nMinute == 0)
	{//©Fx
		UICol = { 1.0f,0.86f,0.6f,1.0f };
	}

	for (nCount = 0; nCount < NUM_PLACE / 2; nCount++)
	{//XRA\¦
	 //eèoµ
		aData1 = pow(10.0, NUM_PLACE / 2 - nCount);
		aData2 = pow(10.0, NUM_PLACE / 2 - nCount - 1);

		aTexU[nCount] = g_nMinute % aData1 / aData2;

		//eNX`ÀW
		pVtx[0].tex = D3DXVECTOR2(0.0f + aTexU[nCount] * 0.1f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(0.1f + aTexU[nCount] * 0.1f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f + aTexU[nCount] * 0.1f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.1f + aTexU[nCount] * 0.1f, 1.0f);
		
		//¸_J[ÌÝè
		pVtx[0].col = UICol;
		pVtx[1].col = UICol;
		pVtx[2].col = UICol;
		pVtx[3].col = UICol;

		//¸_îñÌ|C^ðSÂªißé
		pVtx += 4;
	}

	for (; nCount < NUM_PLACE; nCount++)
	{//XRA\¦
	 //eèoµ
		aData1 = pow(10.0, NUM_PLACE - nCount);
		aData2 = pow(10.0, NUM_PLACE - nCount - 1);

		aTexU[nCount] = g_nLastTime % aData1 / aData2;

		//eNX`ÀW
		pVtx[0].tex = D3DXVECTOR2(0.0f + aTexU[nCount] * 0.1f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(0.1f + aTexU[nCount] * 0.1f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f + aTexU[nCount] * 0.1f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.1f + aTexU[nCount] * 0.1f, 1.0f);

		//¸_J[ÌÝè
		pVtx[0].col = UICol;
		pVtx[1].col = UICol;
		pVtx[2].col = UICol;
		pVtx[3].col = UICol;

		//¸_îñÌ|C^ðSÂªißé
		pVtx += 4;
	}

	//¸_J[ÌÝè
	pVtx[0].col = UICol;
	pVtx[1].col = UICol;
	pVtx[2].col = UICol;
	pVtx[3].col = UICol;

	//jÜÅ|C^ðißé
	pVtx += 4;

	//¸_ÀWÌÝè
	pVtx[0].pos = D3DXVECTOR3
	(
		g_clock.pos.x + sinf(g_clock.rot.z + g_clock.fAngle) * g_clock.fLength,
		g_clock.pos.y + cosf(g_clock.rot.z + g_clock.fAngle) * g_clock.fLength,
		0.0f
	);
	pVtx[1].pos = D3DXVECTOR3
	(
		g_clock.pos.x + sinf(g_clock.rot.z - g_clock.fAngle) * g_clock.fLength,
		g_clock.pos.y + cosf(g_clock.rot.z - g_clock.fAngle) * g_clock.fLength,
		0.0f
	);
	pVtx[2].pos = D3DXVECTOR3
	(
		g_clock.pos.x + sinf(g_clock.rot.z - g_clock.fAngle + D3DX_PI) * g_clock.fLength,
		g_clock.pos.y + cosf(g_clock.rot.z - g_clock.fAngle + D3DX_PI) * g_clock.fLength,
		0.0f
	);
	pVtx[3].pos = D3DXVECTOR3
	(
		g_clock.pos.x + sinf(g_clock.rot.z + g_clock.fAngle - D3DX_PI) * g_clock.fLength,
		g_clock.pos.y + cosf(g_clock.rot.z + g_clock.fAngle - D3DX_PI) * g_clock.fLength,
		0.0f
	);

	//¸_J[ÌÝè
	/*pVtx[0].col = UICol;
	pVtx[1].col = UICol;
	pVtx[2].col = UICol;
	pVtx[3].col = UICol;*/

	//¸_obt@ðAbN
	g_pVtxBuffTime->Unlock();
}

//==================================================================================================
//fobOì
//==================================================================================================
void ControlDebug(void)
{
	if (GetKeyboardTrigger(DIK_1))
	{
		g_nLastTime -= 10;
	}
	else if (GetKeyboardTrigger(DIK_2))
	{
		g_nLastTime += 10;
	}
}

//==================================================================================================
//`æ
//==================================================================================================
void DrawTime(void)
{
	//foCX|C^Ýè
	LPDIRECT3DDEVICE9 pDevice;

	//Ïé¾
	int nCntTime;

	//foCXÌæ¾
	pDevice = GetDevice();

	//¸_obt@ðf[^Xg[ÉÝè
	pDevice->SetStreamSource(0, g_pVtxBuffTime, 0, sizeof(VERTEX_2D));

	//¸_tH[}bgÌÝè
	pDevice->SetFVF(FVF_VERTEX_2D);

	//eNX`Ýè
	pDevice->SetTexture(0, g_apTextureTime[0]);
	
	for (nCntTime = 0; nCntTime < NUM_PLACE; nCntTime++)
	{//SÄÌeð`FbN·é
		//|SÌ`æ
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntTime * 4, 2);
	}

	//eNX`Ýè
	pDevice->SetTexture(0, g_apTextureTime[1]);

	//|SÌ`æ
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntTime * 4, 2);

	//eNX`Ýè
	pDevice->SetTexture(0, g_apTextureTime[2]);

	//|SÌ`æ
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, (nCntTime + 1) * 4, 2);
}

//==================================================================================================
//Ýè
//==================================================================================================
void SetTime(int nTime)
{
	//Ïé¾
	int aTexU[NUM_PLACE],nCount;
	int aData1, aData2;

	//úXRAðüè
	g_nTime = nTime;

	//¸_îñÌ|C^
	VERTEX_2D *pVtx;

	//¸_obt@ðbNµA¸_îñÖÌ|C^ðæ¾
	g_pVtxBuffTime->Lock(0, 0, (void**)&pVtx, 0);

	for (nCount = 0; nCount < NUM_PLACE / 2; nCount++)
	{//XRA\¦
	 //eèoµ
		aData1 = pow(10.0, NUM_PLACE / 2 - nCount);
		aData2 = pow(10.0, NUM_PLACE / 2 - nCount - 1);

		aTexU[nCount] = g_nMinute % aData1 / aData2;

		//eNX`ÀW
		pVtx[0].tex = D3DXVECTOR2(0.0f + aTexU[nCount] * 0.1f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(0.1f + aTexU[nCount] * 0.1f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f + aTexU[nCount] * 0.1f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.1f + aTexU[nCount] * 0.1f, 1.0f);

		//¸_îñÌ|C^ðSÂªißé
		pVtx += 4;
	}

	for (; nCount < NUM_PLACE; nCount++)
	{//XRA\¦
	 //eèoµ
		aData1 = pow(10.0, NUM_PLACE - nCount);
		aData2 = pow(10.0, NUM_PLACE - nCount - 1);

		aTexU[nCount] = g_nLastTime % aData1 / aData2;

		//eNX`ÀW
		pVtx[0].tex = D3DXVECTOR2(0.0f + aTexU[nCount] * 0.1f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(0.1f + aTexU[nCount] * 0.1f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f + aTexU[nCount] * 0.1f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.1f + aTexU[nCount] * 0.1f, 1.0f);

		//¸_îñÌ|C^ðSÂªißé
		pVtx += 4;
	}

	//¸_obt@ðAbN
	g_pVtxBuffTime->Unlock();
}

//==================================================================================================
//XRAÁZ
//==================================================================================================
void AddTime(int nValue)
{
	//ÁZXRAðüè
	g_nTime += nValue;

	if (g_nTime >= 100000000)
	{//ãÀÉBµ½ç
		g_nTime = 99999999;
	}
}

//==================================================================================================
//^Cæ¾
//==================================================================================================
int GetTime(void)
{
	//Ïé¾
	int nTime = g_nMinute * 60 + g_nLastTime;

	return nTime;
}

//==================================================================================================
//^CAbvæ¾
//==================================================================================================
bool GetTimeUp(void)
{
	if (g_nMinute == 0 && g_nLastTime == 0)
	{//ÔØê
		return true;
	}
	else
	{
		return false;
	}
}

//==================================================================================================
//UgæÊÅÌXV
//==================================================================================================
void UpdateTimeResult(void)
{
	//Ïé¾
	int aTexU[NUM_PLACE], nCount;
	int aData1, aData2;
	D3DXCOLOR UICol = { 1.0f,1.0f,1.0f,1.0f };

	//|C^é¾
	VERTEX_2D *pVtx;

	//¸_obt@ðbNµA¸_îñÖÌ|C^ðæ¾
	g_pVtxBuffTime->Lock(0, 0, (void**)&pVtx, 0);

	if (g_nMinute == 0 && g_nLastTime < 30)
	{//Ôx
		UICol = { 1.0f,0.0f,0.0f,1.0f };
	}
	else if (g_nMinute == 0)
	{//©Fx
		UICol = { 1.0f,0.86f,0.6f,1.0f };
	}

	for (nCount = 0; nCount < NUM_PLACE / 2; nCount++)
	{//XRA\¦
	 //eèoµ
		aData1 = pow(10.0f, NUM_PLACE / 2 - nCount);
		aData2 = pow(10.0f, NUM_PLACE / 2 - nCount - 1);

		aTexU[nCount] = g_nMinute % aData1 / aData2;

		//eNX`ÀW
		pVtx[0].tex = D3DXVECTOR2(0.0f + aTexU[nCount] * 0.1f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(0.1f + aTexU[nCount] * 0.1f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f + aTexU[nCount] * 0.1f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.1f + aTexU[nCount] * 0.1f, 1.0f);

		//¸_J[ÌÝè
		pVtx[0].col = UICol;
		pVtx[1].col = UICol;
		pVtx[2].col = UICol;
		pVtx[3].col = UICol;

		//¸_îñÌ|C^ðSÂªißé
		pVtx += 4;
	}

	for (; nCount < NUM_PLACE; nCount++)
	{//XRA\¦
	 //eèoµ
		aData1 = pow(10.0, NUM_PLACE - nCount);
		aData2 = pow(10.0, NUM_PLACE - nCount - 1);

		aTexU[nCount] = g_nLastTime % aData1 / aData2;

		//eNX`ÀW
		pVtx[0].tex = D3DXVECTOR2(0.0f + aTexU[nCount] * 0.1f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(0.1f + aTexU[nCount] * 0.1f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f + aTexU[nCount] * 0.1f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.1f + aTexU[nCount] * 0.1f, 1.0f);

		//¸_J[ÌÝè
		pVtx[0].col = UICol;
		pVtx[1].col = UICol;
		pVtx[2].col = UICol;
		pVtx[3].col = UICol;

		//¸_îñÌ|C^ðSÂªißé
		pVtx += 4;
	}

	//¸_J[ÌÝè
	pVtx[0].col = UICol;
	pVtx[1].col = UICol;
	pVtx[2].col = UICol;
	pVtx[3].col = UICol;

	//jÜÅ|C^ðißé
	pVtx += 4;

	//¸_ÀWÌÝè
	pVtx[0].pos = D3DXVECTOR3
	(
		g_clock.pos.x + sinf(g_clock.rot.z + g_clock.fAngle) * g_clock.fLength,
		g_clock.pos.y + cosf(g_clock.rot.z + g_clock.fAngle) * g_clock.fLength,
		0.0f
	);
	pVtx[1].pos = D3DXVECTOR3
	(
		g_clock.pos.x + sinf(g_clock.rot.z - g_clock.fAngle) * g_clock.fLength,
		g_clock.pos.y + cosf(g_clock.rot.z - g_clock.fAngle) * g_clock.fLength,
		0.0f
	);
	pVtx[2].pos = D3DXVECTOR3
	(
		g_clock.pos.x + sinf(g_clock.rot.z - g_clock.fAngle + D3DX_PI) * g_clock.fLength,
		g_clock.pos.y + cosf(g_clock.rot.z - g_clock.fAngle + D3DX_PI) * g_clock.fLength,
		0.0f
	);
	pVtx[3].pos = D3DXVECTOR3
	(
		g_clock.pos.x + sinf(g_clock.rot.z + g_clock.fAngle - D3DX_PI) * g_clock.fLength,
		g_clock.pos.y + cosf(g_clock.rot.z + g_clock.fAngle - D3DX_PI) * g_clock.fLength,
		0.0f
	);

	//¸_J[ÌÝè
	/*pVtx[0].col = UICol;
	pVtx[1].col = UICol;
	pVtx[2].col = UICol;
	pVtx[3].col = UICol;*/

	//¸_obt@ðAbN
	g_pVtxBuffTime->Unlock();
}