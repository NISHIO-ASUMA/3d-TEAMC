//==============================================================================================================
//
// モーション [motion.cpp]
// Author:YOSHIDA YUUTO
//
//==============================================================================================================

//****************************
//インクルードファイル
//****************************
#include "motion.h"
#include"meshfield.h"
#include <stdio.h>
#include "HPGauge.h"
#include "player.h"

//****************************
//マクロ定義
//****************************
#define MAX_WORD (128) // 最大の文字数

//=========================================================================================================
// モーションの更新処理
//=========================================================================================================
void UpdateMotion(MOTION *pMotion)
{
	int nextKey = 0;

	for (int nCntModel = 0; nCntModel < pMotion->nNumModel; nCntModel++)
	{
		float DiffPosX, DiffPosY, DiffPosZ, DiffRotX, DiffRotY, DiffRotZ;

		DiffPosX = DiffPosY = DiffPosZ = DiffRotX = DiffRotY = DiffRotZ = 0.0f;

		nextKey = (pMotion->nKey + 1) % pMotion->aMotionInfo[pMotion->motionType].nNumkey;//次のキー

		DiffPosX = pMotion->aMotionInfo[pMotion->motionType].aKeyInfo[nextKey].aKey[nCntModel].fPosX - pMotion->aMotionInfo[pMotion->motionType].aKeyInfo[pMotion->nKey].aKey[nCntModel].fPosX;
		DiffPosY = pMotion->aMotionInfo[pMotion->motionType].aKeyInfo[nextKey].aKey[nCntModel].fPosY - pMotion->aMotionInfo[pMotion->motionType].aKeyInfo[pMotion->nKey].aKey[nCntModel].fPosY;
		DiffPosZ = pMotion->aMotionInfo[pMotion->motionType].aKeyInfo[nextKey].aKey[nCntModel].fPosZ - pMotion->aMotionInfo[pMotion->motionType].aKeyInfo[pMotion->nKey].aKey[nCntModel].fPosZ;
		DiffRotX = pMotion->aMotionInfo[pMotion->motionType].aKeyInfo[nextKey].aKey[nCntModel].fRotX - pMotion->aMotionInfo[pMotion->motionType].aKeyInfo[pMotion->nKey].aKey[nCntModel].fRotX;
		DiffRotY = pMotion->aMotionInfo[pMotion->motionType].aKeyInfo[nextKey].aKey[nCntModel].fRotY - pMotion->aMotionInfo[pMotion->motionType].aKeyInfo[pMotion->nKey].aKey[nCntModel].fRotY;
		DiffRotZ = pMotion->aMotionInfo[pMotion->motionType].aKeyInfo[nextKey].aKey[nCntModel].fRotZ - pMotion->aMotionInfo[pMotion->motionType].aKeyInfo[pMotion->nKey].aKey[nCntModel].fRotZ;

		if (DiffRotX > D3DX_PI)
		{
			DiffRotX += -D3DX_PI * 2.0f;
		}
		else if (DiffRotX < -D3DX_PI)
		{
			DiffRotX += D3DX_PI * 2.0f;
		}

		if (DiffRotY > D3DX_PI)
		{
			DiffRotY += -D3DX_PI * 2.0f;
		}
		else if (DiffRotY < -D3DX_PI)
		{
			DiffRotY += D3DX_PI * 2.0f;
		}

		if (DiffRotZ > D3DX_PI)
		{
			DiffRotZ += -D3DX_PI * 2.0f;
		}
		else if (DiffRotZ < -D3DX_PI)
		{
			DiffRotZ += D3DX_PI * 2.0f;
		}

//**********************************
// パーツの位置を設定
//**********************************

		pMotion->aModel[nCntModel].pos.x =
			pMotion->aMotionInfo[pMotion->motionType].aKeyInfo[pMotion->nKey].aKey[nCntModel].fPosX +
			DiffPosX *
			((float)pMotion->nCountMotion /
				(float)pMotion->aMotionInfo[pMotion->motionType].aKeyInfo[pMotion->nKey].nFrame);

		pMotion->aModel[nCntModel].pos.y =
			pMotion->aMotionInfo[pMotion->motionType].aKeyInfo[pMotion->nKey].aKey[nCntModel].fPosY +
			DiffPosY *
			((float)pMotion->nCountMotion /
				(float)pMotion->aMotionInfo[pMotion->motionType].aKeyInfo[pMotion->nKey].nFrame);

		pMotion->aModel[nCntModel].pos.z =
			pMotion->aMotionInfo[pMotion->motionType].aKeyInfo[pMotion->nKey].aKey[nCntModel].fPosZ +
			DiffPosZ *
			((float)pMotion->nCountMotion /
				(float)pMotion->aMotionInfo[pMotion->motionType].aKeyInfo[pMotion->nKey].nFrame);

//**********************************
// パーツの向きを設定
//**********************************
		pMotion->aModel[nCntModel].rot.x =
			pMotion->aMotionInfo[pMotion->motionType].aKeyInfo[pMotion->nKey].aKey[nCntModel].fRotX +
			DiffRotX *
			((float)pMotion->nCountMotion /
				(float)pMotion->aMotionInfo[pMotion->motionType].aKeyInfo[pMotion->nKey].nFrame);

		pMotion->aModel[nCntModel].rot.y =
			pMotion->aMotionInfo[pMotion->motionType].aKeyInfo[pMotion->nKey].aKey[nCntModel].fRotY +
			DiffRotY *
			((float)pMotion->nCountMotion /
				(float)pMotion->aMotionInfo[pMotion->motionType].aKeyInfo[pMotion->nKey].nFrame);

		pMotion->aModel[nCntModel].rot.z =
			pMotion->aMotionInfo[pMotion->motionType].aKeyInfo[pMotion->nKey].aKey[nCntModel].fRotZ +
			DiffRotZ *
			((float)pMotion->nCountMotion /
				(float)pMotion->aMotionInfo[pMotion->motionType].aKeyInfo[pMotion->nKey].nFrame);

		pMotion->nNextKeyBlend = (pMotion->nKeyBlend + 1) % pMotion->aMotionInfo[pMotion->motiontypeBlend].nNumkey; // ブレンドモーションの次のキー

		float fRateMotion = (float)pMotion->nCountMotion / (float)pMotion->aMotionInfo[pMotion->motionType].aKeyInfo[pMotion->nKey].nFrame; // 相対値
		float fRateBlend = (float)pMotion->nCounterBlend / (float)pMotion->nFrameBlend;

		if (pMotion->bFinishMotion && pMotion->bBlendMotion)
		{
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

			float fDiffBlendRX = fRotXBlend - fRotXCurrent; // 差分
			float fDiffBlendRY = fRotYBlend - fRotYCurrent; // 差分
			float fDiffBlendRZ = fRotZBlend - fRotZCurrent; // 差分

			float fRotX = fRotXCurrent + (fDiffBlendRX * fRateBlend); // 求める値
			float fRotY = fRotYCurrent + (fDiffBlendRY * fRateBlend); // 求める値
			float fRotZ = fRotZCurrent + (fDiffBlendRZ * fRateBlend); // 求める値

			float fDiffBlendPX = fPosXBlend - fPosXCurrent; // 差分
			float fDiffBlendPY = fPosYBlend - fPosYCurrent; // 差分
			float fDiffBlendPZ = fPosZBlend - fPosZCurrent; // 差分

			float fPosX = fPosXCurrent + (fDiffBlendPX * fRateBlend); // 求める値
			float fPosY = fPosYCurrent + (fDiffBlendPY * fRateBlend); // 求める値
			float fPosZ = fPosZCurrent + (fDiffBlendPZ * fRateBlend); // 求める値

			// 位置を設定
			pMotion->aModel[nCntModel].pos = D3DXVECTOR3(fPosX, fPosY, fPosZ);

			// 向きを設定
			pMotion->aModel[nCntModel].rot = D3DXVECTOR3(fRotX, fRotY, fRotZ);
		}

		// オフセットを考慮した向き設定
		pMotion->aModel[nCntModel].rot += pMotion->aModel[nCntModel].offrot;

		// オフセットを考慮した位置設定
		pMotion->aModel[nCntModel].pos += pMotion->aModel[nCntModel].offpos;
	}

	if (!pMotion->aMotionInfo[pMotion->motionType].bLoop &&
		pMotion->nKey >= pMotion->aMotionInfo[pMotion->motionType].nNumkey - 1)
	{
		pMotion->bFinishMotion = true;
		//pMotion->motionType = MOTIONTYPE_NEUTRAL; // モーションタイプをニュートラルにする
	}

	// モーションが終わるかつキーが最大かつブレンドのカウントが最大になった
	if (pMotion->bFinishMotion &&
		pMotion->nKey >= pMotion->aMotionInfo[pMotion->motionType].nNumkey - 1 &&
		pMotion->nCounterBlend >= pMotion->nFrameBlend &&
		pMotion->bBlendMotion)
	{
		pMotion->bFinishMotion = false;           // もとに戻す
		pMotion->bBlendMotion = false;			  // もとに戻す
		pMotion->motionType = MOTIONTYPE_NEUTRAL; // モーションタイプをニュートラルにする
	}

	if (!pMotion->bBlendMotion && pMotion->bFinishMotion)
	{
		pMotion->motionType = MOTIONTYPE_NEUTRAL;
	}

	if (pMotion->nCountMotion >= pMotion->aMotionInfo[pMotion->motionType].aKeyInfo[pMotion->nKey].nFrame)
	{
		//モーションカウントが最大になったら0に戻す
		pMotion->nKey = (pMotion->nKey + 1) % pMotion->aMotionInfo[pMotion->motionType].nNumkey;
		pMotion->nCountMotion = 0;
	}


	//モーションカウントを加算
	pMotion->nCountMotion++;
	
	if (pMotion->bFinishMotion)
	{
		pMotion->nCounterBlend++; // ブレンドモーションカウントをインクリメント
	}

}
//================================================================================================================
// モーションの設定処理
//================================================================================================================
void SetMotion(MOTION *pMotion,MOTIONTYPE motiontype, MOTIONTYPE motiontypeBlend, bool Blend, int nFrameBlend)
{
	Player* pPlayer = GetPlayer();                       // プレイヤーを取得

	if (Blend == true)
	{
		pMotion->nKey = 0;							 // 0から始める
		pMotion->nKeyBlend = 0;						 // 0から始める
		pMotion->nCountMotion = 0;					 // 0から始める
		pMotion->nCounterBlend = 0;					 // 0から始める
		pMotion->bBlendMotion = Blend;				 // ブレンドがあるかどうか
		pMotion->nFrameBlend = nFrameBlend;			 // ブレンドのフレームを代入
		pMotion->motiontypeBlend = motiontypeBlend;  // ブレンドするモーションのタイプを代入
	}
	pMotion->motionType = motiontype;				 // モーションのタイプを代入
	pMotion->bFinishMotion = false;				     // もとに戻す
}

