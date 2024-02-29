//==================================================================================================
//
//îƒópìñÇΩÇËîªíËèàóù
//Author:íÜë∫ó§
//
//==================================================================================================
#include "main.h"
#include "collision.h"
#include "calculation.h"
#include "effect.h"

//==================================================================================================
//â~ÇÃìñÇΩÇËîªíË(x.zïΩñ )
//==================================================================================================
bool CollisionCircle(D3DXVECTOR3 posA, D3DXVECTOR3 posB, float length)
{
	//ïœêîêÈåæ
	bool bHit = false;

	if (hypotf(posA.x - posB.x, posA.z - posB.z) < fabsf(length))
	{//â~ÇÃê⁄êGîªíË
		bHit = true;
	}

	return bHit;
}

//==================================================================================================
//â~ÇÃìñÇΩÇËîªíË(2D)
//==================================================================================================
bool CollisionCircle2D(D3DXVECTOR3 posA, D3DXVECTOR3 posB, float length)
{
	//ïœêîêÈåæ
	bool bHit = false;

	if (hypotf(posA.x - posB.x, posA.y - posB.y) < fabsf(length))
	{//â~ÇÃê⁄êGîªíË
		bHit = true;
	}

	return bHit;
}

//==================================================================================================
//ãÖÇÃìñÇΩÇËîªíË
//==================================================================================================
bool CollisionSphere(D3DXVECTOR3 posA, D3DXVECTOR3 posB, float length)
{
	//ïœêîêÈåæ
	bool bHit = false;
	D3DXVECTOR3 posDiff;

	//à íuÇÃç∑ï™åvéZ
	posDiff = posA - posB;

	if (fabsf(D3DXVec3Length(&posDiff)) < fabsf(length))
	{//ãÖÇÃê⁄êGîªíË
		bHit = true;
	}

	return bHit;
}

//==================================================================================================
//ë»â~ÇÃìñÇΩÇËîªíË(x.zïΩñ )
//==================================================================================================
bool CollisionEllipse(D3DXVECTOR3 posA, D3DXVECTOR3 posB, D3DXVECTOR3 rot, float axisL, float axisS)
{
	//ïœêîêÈåæ
	bool bHit = false;
	D3DXVECTOR3 posFocusA, posFocusB;
	float fAxisTemp;

	if (axisL < axisS)
	{
		fAxisTemp = axisL;
		axisL = axisS;
		axisS = fAxisTemp;
	}

	//è≈ì_ÇÃåvéZ
	posFocusA.x = posA.x + (cosf(rot.y) * sqrtf((float)pow(axisL, 2) - (float)pow(axisS, 2)));
	posFocusA.z = posA.x + (sinf(rot.y) * sqrtf((float)pow(axisL, 2) - (float)pow(axisS, 2)));

	posFocusB.x = posA.x + (cosf(rot.y + D3DX_PI) * sqrtf((float)pow(axisL, 2) - (float)pow(axisS, 2)));
	posFocusB.z = posA.x + (sinf(rot.y + D3DX_PI) * sqrtf((float)pow(axisL, 2) - (float)pow(axisS, 2)));

	if (axisL * 2 > hypotf(posFocusA.x - posB.x, posFocusA.z - posB.z) + hypotf(posFocusB.x - posB.x, posFocusB.z - posB.z))
	{//ë»â~ÇÃê⁄êGîªíË
		bHit = true;
	}

	return bHit;
}

//==================================================================================================
//ë»â~ÇÃìñÇΩÇËîªíË(2D)
//==================================================================================================
bool CollisionEllipse2D(D3DXVECTOR3 posA, D3DXVECTOR3 posB, D3DXVECTOR3 rot, float axisL, float axisS)
{
	//ïœêîêÈåæ
	bool bHit = false;
	D3DXVECTOR3 posFocusA, posFocusB;
	float fAxisTemp;

	if (axisL < axisS)
	{
		fAxisTemp = axisL;
		axisL = axisS;
		axisS = fAxisTemp;
	}

	//è≈ì_ÇÃåvéZ
	posFocusA.x = posA.x + (cosf(rot.z) * sqrtf((float)pow(axisL, 2) - (float)pow(axisS, 2)));
	posFocusA.y = posA.y + (sinf(rot.z) * sqrtf((float)pow(axisL, 2) - (float)pow(axisS, 2)));

	posFocusB.x = posA.x + (cosf(rot.z + D3DX_PI) * sqrtf((float)pow(axisL, 2) - (float)pow(axisS, 2)));
	posFocusB.y = posA.y + (sinf(rot.z + D3DX_PI) * sqrtf((float)pow(axisL, 2) - (float)pow(axisS, 2)));
	
	//ìÒÇ¬ÇÃè≈ì_Ç∆ÇÃãóó£ÇÃòaÇ™àÍíËÇÃílà»â∫ÇÇ∆ÇÈÇ∆Ç´îªíËÇ∑ÇÈ
	if (axisL * 2 > hypotf(posFocusA.x - posB.x, posFocusA.y - posB.y) + hypotf(posFocusB.x - posB.x, posFocusB.y - posB.y))
	{//ë»â~ÇÃê⁄êGîªíË
		bHit = true;
	}

	return bHit;
}

//==================================================================================================
//ë»â~ãÖÇÃìñÇΩÇËîªíË(ñ¢äÆê¨)
//==================================================================================================
bool CollisionOval(D3DXVECTOR3 posA, D3DXVECTOR3 posB, D3DXVECTOR3 rot, float axisL, float axisS)
{
	//ïœêîêÈåæ
	bool bHit = false;
	D3DXVECTOR3 posFocusA, posFocusB;

	//è≈ì_ÇÃåvéZ
	posFocusA.x = posA.x + (cosf(rot.y) * sqrtf((float)pow(axisL, 2) - (float)pow(axisS, 2)));
	posFocusA.z = posA.x + (sinf(rot.y) * sqrtf((float)pow(axisL, 2) - (float)pow(axisS, 2)));

	posFocusB.x = posA.x - (cosf(rot.y) * sqrtf((float)pow(axisL, 2) - (float)pow(axisS, 2)));
	posFocusB.z = posA.x - (sinf(rot.y) * sqrtf((float)pow(axisL, 2) - (float)pow(axisS, 2)));

	if (hypotf(posA.x - posB.x, posA.z - posB.z) < hypotf(posFocusA.x - posB.x, posFocusA.z - posB.z) + hypotf(posFocusB.x - posB.x, posFocusB.z - posB.z))
	{//ãÖÇÃê⁄êGîªíË
		bHit = true;
	}

	return bHit;
}

//==================================================================================================
//ãÈå`ÇÃìñÇΩÇËîªíË
//==================================================================================================
bool CollisionRectangle(D3DXVECTOR3 posA, D3DXVECTOR3 posB, D3DXVECTOR3 rot, float height, float width)
{
	bool bRec;
	bRec = false;

	return bRec;
}

//==================================================================================================
//ãÈå`ÇÃìñÇΩÇËîªíË(2D)
//==================================================================================================
bool CollisionRectangle2D(D3DXVECTOR3 posA, D3DXVECTOR3 posB, D3DXVECTOR3 rot, float height, float width)
{
	bool bRec;
	bRec = false;

	D3DXVECTOR3 point[4];
	float fAngle, fLength;
	int nRec = 0;

	//ëŒäpê¸ÇÃí∑Ç≥ÇéZèoÇ∑ÇÈ
	fLength = hypotf(width, height) * 0.5f;

	//ëŒäpê¸ÇÃäpìxÇéZèoÇ∑ÇÈ
	fAngle = atan2f(width, height);

	//í∏ì_ç¿ïWÇÃê›íË
	point[0].x = posA.x + (sinf(rot.z - D3DX_PI + fAngle) * fLength);
	point[0].y = posA.y + (cosf(rot.z - D3DX_PI + fAngle) * fLength);
	point[0].z = 0.0f;

	point[1].x = posA.x + (sinf(rot.z - fAngle) * fLength);
	point[1].y = posA.y + (cosf(rot.z - fAngle) * fLength);
	point[1].z = 0.0f;

	point[2].x = posA.x + (sinf(rot.z + fAngle) * fLength);
	point[2].y = posA.y + (cosf(rot.z + fAngle) * fLength);
	point[2].z = 0.0f;

	point[3].x = posA.x + (sinf(rot.z + D3DX_PI - fAngle) * fLength);
	point[3].y = posA.y + (cosf(rot.z + D3DX_PI - fAngle) * fLength);
	point[3].z = 0.0f;

	for (int nCntRectangle = 0; nCntRectangle < 4; nCntRectangle++)
	{
		if (nCntRectangle == 3)
		{
			if (OuterProduct2D(point[nCntRectangle], posB, point[0]) > 0.0f)
			{
				nRec++;
			}
		}
		else
		{
			if (OuterProduct2D(point[nCntRectangle], posB, point[nCntRectangle + 1]) > 0.0f)
			{
				nRec++;
			}
		}
	}

	if (nRec == 2)
	{
		bRec = true;
	}

	return bRec;
}

//==================================================================================================
//î†ÇÃìñÇΩÇËîªíË(ñ¢äÆê¨)
//==================================================================================================
bool CollisionSquare(D3DXVECTOR3 posA, D3DXVECTOR3 posB, D3DXVECTOR3 rot, float axisL, float axisS)
{
	bool bSquare;
	bSquare = false;

	return bSquare;
}