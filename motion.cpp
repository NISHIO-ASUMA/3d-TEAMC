//============================
//
// ���[�V���� [motion.cpp]
// Author:YOSHIDA YUUTO
//
//============================

//****************************
//�C���N���[�h�t�@�C��
//****************************
#include "motion.h"
#include"meshfield.h"
#include <stdio.h>

//****************************
//�}�N����`
//****************************
#define MAX_WORD (128) // �ő�̕�����

//=======================
// ���[�V�����̍X�V����
//=======================
void UpdateMotion(MOTION *pMotion)
{
	int nextKey = 0;

	for (int nCntModel = 0; nCntModel < pMotion->nNumModel; nCntModel++)
	{
		float DiffPosX, DiffPosY, DiffPosZ, DiffRotX, DiffRotY, DiffRotZ;

		DiffPosX = DiffPosY = DiffPosZ = DiffRotX = DiffRotY = DiffRotZ = 0.0f;

		nextKey = (pMotion->nKey + 1) % pMotion->aMotionInfo[pMotion->motionType].nNumkey;//���̃L�[

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
// �p�[�c�̈ʒu��ݒ�
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
// �p�[�c�̌�����ݒ�
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

//**********************************
//�I�t�Z�b�g
//**********************************
		
		pMotion->aModel[nCntModel].pos += pMotion->aModel[nCntModel].offpos;
		pMotion->aModel[nCntModel].rot += pMotion->aModel[nCntModel].offrot;
	}

	if (pMotion->nCountMotion >= pMotion->aMotionInfo[pMotion->motionType].aKeyInfo[pMotion->nKey].nFrame)
	{
		//���[�V�����J�E���g���ő�ɂȂ�����0�ɖ߂�
		pMotion->nKey = (pMotion->nKey + 1) % pMotion->aMotionInfo[pMotion->motionType].nNumkey;
		pMotion->nCountMotion = 0;
	}

	if (pMotion->nKey > pMotion->aMotionInfo[pMotion->motionType].nNumkey)
	{	//���[�V�����̃L�[��0�ɖ߂�
		pMotion->nKey = 0;
	}

	if (!pMotion->aMotionInfo[pMotion->motionType].bLoop && pMotion->nKey >= pMotion->aMotionInfo[pMotion->motionType].nNumkey - 1)
	{
		pMotion->motionType = MOTIONTYPE_NEUTRAL;
	}

	if (pMotion->bLoopMotion)
	{
		//���[�V�����J�E���g�����Z
		pMotion->nCountMotion++;
	}
}

