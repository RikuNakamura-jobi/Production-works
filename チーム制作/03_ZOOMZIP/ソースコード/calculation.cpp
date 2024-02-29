//==================================================================================================
//
//個人用計算処理
//Author:中村陸
//
//==================================================================================================
#include "main.h"
#include "calculation.h"

//========================================
//外積左右判定用関数(点Aから伸びる2つのベクトル(ABとAC)の外積)
//点Bが直線ACの左にいるときはプラス、点Bが直線ACの右にいるときはマイナスの値を返し直線上にいる場合0を返す
//========================================
float OuterProduct(D3DXVECTOR3 posA, D3DXVECTOR3 posB, D3DXVECTOR3 posC)
{
	D3DXVECTOR3 vecLine1, vecLine2;
	float vecAnswer;

	//点ABのベクトル
	vecLine1.x = posB.x - posA.x;
	vecLine1.z = posB.z - posA.z;

	//点ACのベクトル
	vecLine2.x = posC.x - posA.x;
	vecLine2.z = posC.z - posA.z;

	//計算したベクトル
	vecAnswer = (vecLine1.z * vecLine2.x) - (vecLine1.x * vecLine2.z);

	return vecAnswer;
}

//========================================
//外積左右判定用関数(点Aから伸びる2つのベクトル(ABとAC)の外積)
//点Bが直線ACの左にいるときはプラス、点Bが直線ACの右にいるときはマイナスの値を返し直線上にいる場合0を返す
//========================================
float OuterProduct2D(D3DXVECTOR3 posA, D3DXVECTOR3 posB, D3DXVECTOR3 posC)
{
	D3DXVECTOR3 vecLine1, vecLine2;
	float vecAnswer;

	//点ABのベクトル
	vecLine1.x = posB.x - posA.x;
	vecLine1.y = posB.y - posA.y;

	//点ACのベクトル
	vecLine2.x = posC.x - posA.x;
	vecLine2.y = posC.y - posA.y;

	//計算したベクトル
	vecAnswer = (vecLine1.y * vecLine2.x) - (vecLine1.x * vecLine2.y);

	return vecAnswer;
}

//========================================
//3次元空間での行列による回転座標変換関数
//(任意の点からのオフセット位置を角度と距離で変換)
//========================================
D3DXVECTOR3 PosRelative(D3DXVECTOR3 posO, D3DXVECTOR3 rot, float length)
{
	D3DXVECTOR3 posAnswer;

	posAnswer.x = posO.x + (cosf(rot.y) * cosf(rot.z) * length);
	posAnswer.y = posO.y + (-sinf(rot.x) * sinf(rot.z) * length);
	posAnswer.z = posO.z + (-sinf(rot.y) * cosf(rot.x) * length);

	return posAnswer;
}

//==================================================
//点と直線の距離を求める関数
//==================================================
float LengthPToL(D3DXVECTOR3 point, D3DXVECTOR3 linePoint1, D3DXVECTOR3 linePoint2)
{
	D3DXVECTOR3 lineCoe;		//直線の係数
	float den, num;				//分母と分子
	float answer;				//答え

	lineCoe.x = (linePoint2.z - linePoint1.z) / (linePoint2.x - linePoint1.x);
	lineCoe.y = -1.0f;
	lineCoe.z = (lineCoe.x * linePoint1.x) - linePoint1.z;

	den = hypotf(lineCoe.x, lineCoe.y);
	num = fabsf((lineCoe.x * point.x) + (lineCoe.y * point.z) + lineCoe.z);

	answer = num / den;

	return answer;
}

//==================================================
//3次元空間上の2点をつなぐ直線のrotを求める関数(予定地)
//AからBとBからAで結果が反転します
//==================================================