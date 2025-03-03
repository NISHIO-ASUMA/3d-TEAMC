//==============================================================================================================
//
// モーション [motion.cpp]
// Author:YOSHIDA YUUTO
//
//==============================================================================================================

//**************************************************************************************************************
//インクルードファイル
//**************************************************************************************************************
#include "motion.h"
#include"meshfield.h"
#include <stdio.h>
#include "HPGauge.h"
#include "player.h"

//**************************************************************************************************************
// マクロ定義
//**************************************************************************************************************
#define MAX_WORD (128) // 最大の文字数

//**************************************************************************************************************
// プロトタイプ宣言
//**************************************************************************************************************
void UpdateCurrentMotion(MOTION* pMotion,int nCntModel);
void UpdateBlendMotion(MOTION* pMotion,int nCntModel, int nextKey);

//=========================================================================================================
// モーションの更新処理
//=========================================================================================================
void UpdateMotion(MOTION *pMotion)
{
	if (pMotion->aMotionInfo[pMotion->motionType].nNumkey == NULL)
	{
		return;
	}

	for (int nCntModel = 0; nCntModel < pMotion->nNumModel; nCntModel++)
	{
		// 次のキー
		pMotion->nextKey = (pMotion->nKey + 1) % pMotion->aMotionInfo[pMotion->motionType].nNumkey;

		if (pMotion->bFinishMotion == false && pMotion->bFirstMotion == false)
		{
			// 現在のモーションの更新処理
			UpdateCurrentMotion(pMotion,nCntModel);
		}
		if ((pMotion->bFinishMotion == true || pMotion->bFirstMotion == true) && pMotion->bBlendMotion == true)
		{
			// ブレンドモーションの更新処理
			UpdateBlendMotion(pMotion, nCntModel, pMotion->nextKey);
		}

		// オフセットを考慮した向き設定
		pMotion->aModel[nCntModel].rot += pMotion->aModel[nCntModel].offrot;

		// オフセットを考慮した位置設定
		pMotion->aModel[nCntModel].pos += pMotion->aModel[nCntModel].offpos;
	}

	// モーションが終わったら
	if (pMotion->aMotionInfo[pMotion->motionType].bLoop == false &&
		pMotion->nKey >= pMotion->aMotionInfo[pMotion->motionType].nNumkey - 1 &&
		pMotion->bBlendMotion == true)
	{
		pMotion->bFinishMotion = true;
		//pMotion->motionType = MOTIONTYPE_NEUTRAL; // モーションタイプをニュートラルにする
	}

	// 
	if (pMotion->bFirstMotion == true && pMotion->nKeyBlend <= 0 && pMotion->nCounterBlend >= 10)
	{
		pMotion->bFirstMotion = false;
		pMotion->motionType = pMotion->motiontypeBlend;
	}

	// モーションが終わるかつキーが最大かつブレンドのカウントが最大になった
	if (pMotion->bFinishMotion == true &&
		pMotion->nKey >= pMotion->aMotionInfo[pMotion->motionType].nNumkey - 1 &&
		pMotion->nCounterBlend >= pMotion->nFrameBlend &&
		pMotion->bBlendMotion == true && pMotion->bFirstMotion == false)
	{
		pMotion->bFinishMotion = false;           // もとに戻す
		pMotion->bBlendMotion = false;			  // もとに戻す
		pMotion->motionType = MOTIONTYPE_NEUTRAL; // モーションタイプをニュートラルにする
	}

	// モーションブレンドのキー
	if (pMotion->nCounterBlend >= pMotion->nFrameBlend && pMotion->bBlendMotion == true && (pMotion->bFinishMotion == true || pMotion->bFirstMotion == true))
	{
		pMotion->nCounterBlend = 0;
		pMotion->nKeyBlend = (pMotion->nKeyBlend + 1) % pMotion->aMotionInfo[pMotion->motiontypeBlend].nNumkey;
	}

	if (!pMotion->bBlendMotion == true && pMotion->bFinishMotion == true)
	{
		pMotion->motionType = MOTIONTYPE_NEUTRAL;
	}

	if (pMotion->nCountMotion >= pMotion->aMotionInfo[pMotion->motionType].aKeyInfo[pMotion->nKey].nFrame)
	{
		//モーションカウントが最大になったら0に戻す
		pMotion->nKey = (pMotion->nKey + 1) % pMotion->aMotionInfo[pMotion->motionType].nNumkey;
		pMotion->nCountMotion = 0;
	}

	// モーションが死亡だったら
	if (pMotion->motionType == MOTIONTYPE_DEATH && pMotion->nKey >= pMotion->aMotionInfo[pMotion->motionType].nNumkey - 1)
	{
		pMotion->nCountMotion--;
		pMotion->nCounterBlend--;
	}

	//モーションカウントを加算
	pMotion->nCountMotion++;

	// ブレンドが始まったら
	if (pMotion->bFinishMotion == true || pMotion->bFirstMotion == true)
	{
		pMotion->nCounterBlend++; // ブレンドモーションカウントをインクリメント
	}
	//else
	//{
	//	//モーションがなかったらニュートラルに戻す
	//	SetMotion(pMotion, MOTIONTYPE_NEUTRAL, MOTIONTYPE_NEUTRAL, false, 10);
	//}


}
//================================================================================================================
// モーションの設定処理
//================================================================================================================
void SetMotion(MOTION *pMotion,MOTIONTYPE motiontype, MOTIONTYPE motiontypeBlend, bool Blend, int nFrameBlend)
{
	Player* pPlayer = GetPlayer();                       // プレイヤーを取得

	if (pMotion->bFirstMotion == false)
	{
		// モーションブレンドがあるかどうか
		if (Blend == true)
		{
			pMotion->nKey = 0;								// 0から始める
			pMotion->nKeyBlend = 0;							// 0から始める
			pMotion->nCountMotion = 0;						// 0から始める
			pMotion->nCounterBlend = 0;						// 0から始める
			pMotion->bBlendMotion = Blend;					// ブレンドがあるかどうか
			pMotion->nFrameBlend = nFrameBlend;				// ブレンドのフレームを代入
			pMotion->motiontypeBlend = motiontypeBlend;		// ブレンドするモーションのタイプを代入
			//pMotion->motionType = motiontype;				// モーションのタイプを代入
		}
		// モーションブレンドがない
		else
		{
			pMotion->nKey = 0;								// 0から始める
			pMotion->nKeyBlend = 0;							// 0から始める
			pMotion->nCountMotion = 0;						// 0から始める
			pMotion->nCounterBlend = 0;						// 0から
			//pMotion->motionType = motiontype;				// モーションのタイプを代入
			pMotion->bFinishMotion = false;					// もとに戻す
		}
		pMotion->bFirstMotion = true;
	}
}
//================================================================================================================
// 角度の正規化
//================================================================================================================
void NormalizeRotation(D3DXVECTOR3* OutRot, D3DXVECTOR3 Rot)
{
	// 角度の正規化
	if (Rot.x > D3DX_PI)
	{
		Rot.x += -D3DX_PI * 2.0f;
		OutRot->x = Rot.x;
	}
	else if (Rot.x < -D3DX_PI)
	{
		Rot.x += D3DX_PI * 2.0f;
		OutRot->x = Rot.x;
	}

	if (Rot.y > D3DX_PI)
	{
		Rot.y += -D3DX_PI * 2.0f;
		OutRot->y = Rot.y;
	}
	else if (Rot.y < -D3DX_PI)
	{
		Rot.y += D3DX_PI * 2.0f;
		OutRot->y = Rot.y;
	}

	if (Rot.z > D3DX_PI)
	{
		Rot.z += -D3DX_PI * 2.0f;
		OutRot->z = Rot.z;
	}
	else if (Rot.z < -D3DX_PI)
	{
		Rot.z += D3DX_PI * 2.0f;
		OutRot->z = Rot.z;
	}
}
//================================================================================================================
// 現在のモーションの更新処理
//================================================================================================================
void UpdateCurrentMotion(MOTION* pMotion, int nCntModel)
{
	// 次のキー
	int nextKey = pMotion->nextKey;

	// 現在の回転
	float fCurrentRotX = pMotion->aMotionInfo[pMotion->motionType].aKeyInfo[pMotion->nKey].aKey[nCntModel].fRotX;
	float fCurrentRotY = pMotion->aMotionInfo[pMotion->motionType].aKeyInfo[pMotion->nKey].aKey[nCntModel].fRotY;
	float fCurrentRotZ = pMotion->aMotionInfo[pMotion->motionType].aKeyInfo[pMotion->nKey].aKey[nCntModel].fRotZ;

	// 次の回転
	float fNextRotX = pMotion->aMotionInfo[pMotion->motionType].aKeyInfo[nextKey].aKey[nCntModel].fRotX;
	float fNextRotY = pMotion->aMotionInfo[pMotion->motionType].aKeyInfo[nextKey].aKey[nCntModel].fRotY;
	float fNextRotZ = pMotion->aMotionInfo[pMotion->motionType].aKeyInfo[nextKey].aKey[nCntModel].fRotZ;

	// 現在の位置
	float fCurrentPosX = pMotion->aMotionInfo[pMotion->motionType].aKeyInfo[pMotion->nKey].aKey[nCntModel].fPosX;
	float fCurrentPosY = pMotion->aMotionInfo[pMotion->motionType].aKeyInfo[pMotion->nKey].aKey[nCntModel].fPosY;
	float fCurrentPosZ = pMotion->aMotionInfo[pMotion->motionType].aKeyInfo[pMotion->nKey].aKey[nCntModel].fPosZ;

	// 次の位置
	float fNextPosX = pMotion->aMotionInfo[pMotion->motionType].aKeyInfo[nextKey].aKey[nCntModel].fPosX;
	float fNextPosY = pMotion->aMotionInfo[pMotion->motionType].aKeyInfo[nextKey].aKey[nCntModel].fPosY;
	float fNextPosZ = pMotion->aMotionInfo[pMotion->motionType].aKeyInfo[nextKey].aKey[nCntModel].fPosZ;

	// 位置の差分、向きの差分
	float DiffPosX = fNextPosX - fCurrentPosX;
	float DiffPosY = fNextPosY - fCurrentPosY;
	float DiffPosZ = fNextPosZ - fCurrentPosZ;
	float DiffRotX = fNextRotX - fCurrentRotX;
	float DiffRotY = fNextRotY - fCurrentRotY;
	float DiffRotZ = fNextRotZ - fCurrentRotZ;

	// 正規化用変数
	D3DXVECTOR3 fDiffRot = D3DXVECTOR3(DiffRotX, DiffRotY, DiffRotZ);

	// 差分の角度を正規化
	NormalizeRotation(&fDiffRot, fDiffRot);

	// フレームの相対値
	float fRateMotion = (float)pMotion->nCountMotion / (float)pMotion->aMotionInfo[pMotion->motionType].aKeyInfo[pMotion->nKey].nFrame;

	// 位置を求める
	float fPosX = fCurrentPosX + DiffPosX * fRateMotion;
	float fPosY = fCurrentPosY + DiffPosY * fRateMotion;
	float fPosZ = fCurrentPosZ + DiffPosZ * fRateMotion;

	// 向きを求める
	float fRotX = fCurrentRotX + fDiffRot.x * fRateMotion;
	float fRotY = fCurrentRotY + fDiffRot.y * fRateMotion;
	float fRotZ = fCurrentRotZ + fDiffRot.z * fRateMotion;

	// 現在の位置を設定
	pMotion->aModel[nCntModel].pos = D3DXVECTOR3(fPosX, fPosY, fPosZ);

	// 現在の向きを設定
	pMotion->aModel[nCntModel].rot = D3DXVECTOR3(fRotX, fRotY, fRotZ);
}
//================================================================================================================
// ブレンドモーションの更新処理
//================================================================================================================
void UpdateBlendMotion(MOTION* pMotion, int nCntModel,int nextKey)
{
	// ブレンドモーションの次のキー
	pMotion->nNextKeyBlend = (pMotion->nKeyBlend + 1) % pMotion->aMotionInfo[pMotion->motiontypeBlend].nNumkey;

	float fRateMotion = (float)pMotion->nCountMotion / (float)pMotion->aMotionInfo[pMotion->motionType].aKeyInfo[pMotion->nKey].nFrame; // 相対値
	float fRateBlend = (float)pMotion->nCounterBlend / (float)pMotion->nFrameBlend;

	
	//現在のモーションの回転X
	float fDiffMotionRX = pMotion->aMotionInfo[pMotion->motionType].aKeyInfo[nextKey].aKey[nCntModel].fRotX -
		pMotion->aMotionInfo[pMotion->motionType].aKeyInfo[pMotion->nKey].aKey[nCntModel].fRotX;

	float fRotXCurrent = pMotion->aMotionInfo[pMotion->motionType].aKeyInfo[pMotion->nKey].aKey[nCntModel].fRotX + (fDiffMotionRX * fRateMotion);

	//現在のモーションの回転Y
	float fDiffMotionRY = pMotion->aMotionInfo[pMotion->motionType].aKeyInfo[nextKey].aKey[nCntModel].fRotY -
		pMotion->aMotionInfo[pMotion->motionType].aKeyInfo[pMotion->nKey].aKey[nCntModel].fRotY;

	float fRotYCurrent = pMotion->aMotionInfo[pMotion->motionType].aKeyInfo[pMotion->nKey].aKey[nCntModel].fRotY + (fDiffMotionRY * fRateMotion);

	//現在のモーションの回転Z
	float fDiffMotionRZ = pMotion->aMotionInfo[pMotion->motionType].aKeyInfo[nextKey].aKey[nCntModel].fRotZ -
		pMotion->aMotionInfo[pMotion->motionType].aKeyInfo[pMotion->nKey].aKey[nCntModel].fRotZ;

	float fRotZCurrent = pMotion->aMotionInfo[pMotion->motionType].aKeyInfo[pMotion->nKey].aKey[nCntModel].fRotZ + (fDiffMotionRZ * fRateMotion);

	//ブレンドモーションの回転X
	float fDiffMotionBlendRX = pMotion->aMotionInfo[pMotion->motiontypeBlend].aKeyInfo[pMotion->nNextKeyBlend].aKey[nCntModel].fRotX -
		pMotion->aMotionInfo[pMotion->motiontypeBlend].aKeyInfo[pMotion->nKeyBlend].aKey[nCntModel].fRotX;

	float fRotXBlend = pMotion->aMotionInfo[pMotion->motiontypeBlend].aKeyInfo[pMotion->nKeyBlend].aKey[nCntModel].fRotX + (fDiffMotionBlendRX * fRateBlend);

	//ブレンドモーションの回転Y
	float fDiffMotionBlendRY = pMotion->aMotionInfo[pMotion->motiontypeBlend].aKeyInfo[pMotion->nNextKeyBlend].aKey[nCntModel].fRotY -
		pMotion->aMotionInfo[pMotion->motiontypeBlend].aKeyInfo[pMotion->nKeyBlend].aKey[nCntModel].fRotY;

	float fRotYBlend = pMotion->aMotionInfo[pMotion->motiontypeBlend].aKeyInfo[pMotion->nKeyBlend].aKey[nCntModel].fRotY + (fDiffMotionBlendRY * fRateBlend);

	//ブレンドモーションの回転Z
	float fDiffMotionBlendRZ = pMotion->aMotionInfo[pMotion->motiontypeBlend].aKeyInfo[pMotion->nNextKeyBlend].aKey[nCntModel].fRotZ -
		pMotion->aMotionInfo[pMotion->motiontypeBlend].aKeyInfo[pMotion->nKeyBlend].aKey[nCntModel].fRotZ;

	float fRotZBlend = pMotion->aMotionInfo[pMotion->motiontypeBlend].aKeyInfo[pMotion->nKeyBlend].aKey[nCntModel].fRotZ + (fDiffMotionBlendRZ * fRateBlend);

	//現在のモーションの位置X
	float fDiffMotionPX = pMotion->aMotionInfo[pMotion->motionType].aKeyInfo[nextKey].aKey[nCntModel].fPosX -
		pMotion->aMotionInfo[pMotion->motionType].aKeyInfo[pMotion->nKey].aKey[nCntModel].fPosX;

	float fPosXCurrent = pMotion->aMotionInfo[pMotion->motionType].aKeyInfo[pMotion->nKey].aKey[nCntModel].fPosX + (fDiffMotionPX * fRateMotion);

	//現在のモーションの位置Y
	float fDiffMotionPY = pMotion->aMotionInfo[pMotion->motionType].aKeyInfo[nextKey].aKey[nCntModel].fPosY -
		pMotion->aMotionInfo[pMotion->motionType].aKeyInfo[pMotion->nKey].aKey[nCntModel].fPosY;

	float fPosYCurrent = pMotion->aMotionInfo[pMotion->motionType].aKeyInfo[pMotion->nKey].aKey[nCntModel].fPosY + (fDiffMotionPY * fRateMotion);

	//現在のモーションの位置Z
	float fDiffMotionPZ = pMotion->aMotionInfo[pMotion->motionType].aKeyInfo[nextKey].aKey[nCntModel].fPosZ -
		pMotion->aMotionInfo[pMotion->motionType].aKeyInfo[pMotion->nKey].aKey[nCntModel].fPosZ;

	float fPosZCurrent = pMotion->aMotionInfo[pMotion->motionType].aKeyInfo[pMotion->nKey].aKey[nCntModel].fPosZ + (fDiffMotionPZ * fRateMotion);

	//ブレンドモーションの位置X
	float fDiffMotionBlendPX = pMotion->aMotionInfo[pMotion->motiontypeBlend].aKeyInfo[pMotion->nNextKeyBlend].aKey[nCntModel].fPosX -
		pMotion->aMotionInfo[pMotion->motiontypeBlend].aKeyInfo[pMotion->nKeyBlend].aKey[nCntModel].fPosX;

	float fPosXBlend = pMotion->aMotionInfo[pMotion->motiontypeBlend].aKeyInfo[pMotion->nKeyBlend].aKey[nCntModel].fPosX + (fDiffMotionBlendPX * fRateBlend);

	//ブレンドモーションの位置Y
	float fDiffMotionBlendPY = pMotion->aMotionInfo[pMotion->motiontypeBlend].aKeyInfo[pMotion->nNextKeyBlend].aKey[nCntModel].fPosY -
		pMotion->aMotionInfo[pMotion->motiontypeBlend].aKeyInfo[pMotion->nKeyBlend].aKey[nCntModel].fPosY;

	float fPosYBlend = pMotion->aMotionInfo[pMotion->motiontypeBlend].aKeyInfo[pMotion->nKeyBlend].aKey[nCntModel].fPosY + (fDiffMotionBlendPY * fRateBlend);

	//ブレンドモーションの位置Z
	float fDiffMotionBlendPZ = pMotion->aMotionInfo[pMotion->motiontypeBlend].aKeyInfo[pMotion->nNextKeyBlend].aKey[nCntModel].fPosZ -
		pMotion->aMotionInfo[pMotion->motiontypeBlend].aKeyInfo[pMotion->nKeyBlend].aKey[nCntModel].fPosZ;

	float fPosZBlend = pMotion->aMotionInfo[pMotion->motiontypeBlend].aKeyInfo[pMotion->nKeyBlend].aKey[nCntModel].fPosZ + (fDiffMotionBlendPZ * fRateBlend);

	float fDiffBlendRX = fRotXBlend - fRotXCurrent; // 差分X
	float fDiffBlendRY = fRotYBlend - fRotYCurrent; // 差分Y
	float fDiffBlendRZ = fRotZBlend - fRotZCurrent; // 差分Z

	float fRotX = fRotXCurrent + (fDiffBlendRX * fRateBlend); // 求める値
	float fRotY = fRotYCurrent + (fDiffBlendRY * fRateBlend); // 求める値
	float fRotZ = fRotZCurrent + (fDiffBlendRZ * fRateBlend); // 求める値

	float fDiffBlendPX = fPosXBlend - fPosXCurrent; // 差分X
	float fDiffBlendPY = fPosYBlend - fPosYCurrent; // 差分Y
	float fDiffBlendPZ = fPosZBlend - fPosZCurrent; // 差分Z

	float fPosX = fPosXCurrent + (fDiffBlendPX * fRateBlend); // 求める値
	float fPosY = fPosYCurrent + (fDiffBlendPY * fRateBlend); // 求める値
	float fPosZ = fPosZCurrent + (fDiffBlendPZ * fRateBlend); // 求める値

	// 位置を設定
	pMotion->aModel[nCntModel].pos = D3DXVECTOR3(fPosX, fPosY, fPosZ);

	// 向きを設定
	pMotion->aModel[nCntModel].rot = D3DXVECTOR3(fRotX, fRotY, fRotZ);
}
