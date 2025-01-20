//============================
//
// モーション [motion.cpp]
// Author:YOSHIDA YUUTO
//
//============================

//****************************
//インクルードファイル
//****************************
#include "motion.h"

//=======================
// モーションの更新処理
//=======================
void UpdateMotion(MOTION *Motion)
{
	int nextKey = 0;

	for (int nCntModel = 0; nCntModel < Motion->nNumModel; nCntModel++)
	{
		float DiffPosX, DiffPosY, DiffPosZ, DiffRotX, DiffRotY, DiffRotZ;

		DiffPosX = DiffPosY = DiffPosZ = DiffRotX = DiffRotY = DiffRotZ = 0.0f;

		nextKey = (Motion->nKey + 1) % Motion->aMotionInfo[Motion->motionType].nNumkey;//次のキー

		DiffPosX = Motion->aMotionInfo[Motion->motionType].aKeyInfo[nextKey].aKey[nCntModel].fPosX - Motion->aMotionInfo[Motion->motionType].aKeyInfo[Motion->nKey].aKey[nCntModel].fPosX;
		DiffPosY = Motion->aMotionInfo[Motion->motionType].aKeyInfo[nextKey].aKey[nCntModel].fPosY - Motion->aMotionInfo[Motion->motionType].aKeyInfo[Motion->nKey].aKey[nCntModel].fPosY;
		DiffPosZ = Motion->aMotionInfo[Motion->motionType].aKeyInfo[nextKey].aKey[nCntModel].fPosZ - Motion->aMotionInfo[Motion->motionType].aKeyInfo[Motion->nKey].aKey[nCntModel].fPosZ;
		DiffRotX = Motion->aMotionInfo[Motion->motionType].aKeyInfo[nextKey].aKey[nCntModel].fRotX - Motion->aMotionInfo[Motion->motionType].aKeyInfo[Motion->nKey].aKey[nCntModel].fRotX;
		DiffRotY = Motion->aMotionInfo[Motion->motionType].aKeyInfo[nextKey].aKey[nCntModel].fRotY - Motion->aMotionInfo[Motion->motionType].aKeyInfo[Motion->nKey].aKey[nCntModel].fRotY;
		DiffRotZ = Motion->aMotionInfo[Motion->motionType].aKeyInfo[nextKey].aKey[nCntModel].fRotZ - Motion->aMotionInfo[Motion->motionType].aKeyInfo[Motion->nKey].aKey[nCntModel].fRotZ;

//**********************************
// パーツの位置を設定
//**********************************

		Motion->aModel[nCntModel].pos.x =
			Motion->aMotionInfo[Motion->motionType].aKeyInfo[Motion->nKey].aKey[nCntModel].fPosX +
			DiffPosX *
			((float)Motion->nCountMotion /
				(float)Motion->aMotionInfo[Motion->motionType].aKeyInfo[Motion->nKey].nFrame);

		Motion->aModel[nCntModel].pos.y =
			Motion->aMotionInfo[Motion->motionType].aKeyInfo[Motion->nKey].aKey[nCntModel].fPosY +
			DiffPosY *
			((float)Motion->nCountMotion /
				(float)Motion->aMotionInfo[Motion->motionType].aKeyInfo[Motion->nKey].nFrame);

		Motion->aModel[nCntModel].pos.z =
			Motion->aMotionInfo[Motion->motionType].aKeyInfo[Motion->nKey].aKey[nCntModel].fPosZ +
			DiffPosZ *
			((float)Motion->nCountMotion /
				(float)Motion->aMotionInfo[Motion->motionType].aKeyInfo[Motion->nKey].nFrame);

//**********************************
// パーツの向きを設定
//**********************************
		Motion->aModel[nCntModel].rot.x =
			Motion->aMotionInfo[Motion->motionType].aKeyInfo[Motion->nKey].aKey[nCntModel].fRotX +
			DiffRotX *
			((float)Motion->nCountMotion /
				(float)Motion->aMotionInfo[Motion->motionType].aKeyInfo[Motion->nKey].nFrame);

		Motion->aModel[nCntModel].rot.y =
			Motion->aMotionInfo[Motion->motionType].aKeyInfo[Motion->nKey].aKey[nCntModel].fRotY +
			DiffRotY *
			((float)Motion->nCountMotion /
				(float)Motion->aMotionInfo[Motion->motionType].aKeyInfo[Motion->nKey].nFrame);

		Motion->aModel[nCntModel].rot.z =
			Motion->aMotionInfo[Motion->motionType].aKeyInfo[Motion->nKey].aKey[nCntModel].fRotZ +
			DiffRotZ *
			((float)Motion->nCountMotion /
				(float)Motion->aMotionInfo[Motion->motionType].aKeyInfo[Motion->nKey].nFrame);

//**********************************
//オフセット
//**********************************
		
		//g_player.Motion.aModel[nCntModel].rot += g_player.Motion.aModel[nCntModel].offrot;
		Motion->aModel[nCntModel].pos += Motion->aModel[nCntModel].offpos;
	}

	if (Motion->nCountMotion >= Motion->aMotionInfo[Motion->motionType].aKeyInfo[Motion->nKey].nFrame)
	{
		//モーションカウントが最大になったら0に戻す
		Motion->nKey = (Motion->nKey + 1) % Motion->aMotionInfo[Motion->motionType].nNumkey;
		Motion->nCountMotion = 0;
	}

	if (Motion->nKey > Motion->aMotionInfo[Motion->motionType].nNumkey)
	{
		if (Motion->aMotionInfo[Motion->motionType].bLoop == false && Motion->nKey >= Motion->aMotionInfo[Motion->motionType].nNumkey - 1)
		{
			Motion->nKey = 0;
		}
		//モーションカウントが最大になったら0に戻す
		Motion->nKey = 0;
	}

	if (Motion->bLoopMotion == true)
	{
		//モーションカウントを加算
		Motion->nCountMotion++;
	}
}
