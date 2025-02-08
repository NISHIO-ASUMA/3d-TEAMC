//==============================================================================================================
//
// ���[�V���� [motion.cpp]
// Author:YOSHIDA YUUTO
//
//==============================================================================================================

//****************************
//�C���N���[�h�t�@�C��
//****************************
#include "motion.h"
#include"meshfield.h"
#include <stdio.h>
#include "HPGauge.h"
#include "player.h"

//****************************
//�}�N����`
//****************************
#define MAX_WORD (128) // �ő�̕�����

//=========================================================================================================
// ���[�V�����̍X�V����
//=========================================================================================================
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

		pMotion->nNextKeyBlend = (pMotion->nKeyBlend + 1) % pMotion->aMotionInfo[pMotion->motiontypeBlend].nNumkey; // �u�����h���[�V�����̎��̃L�[

		float fRateMotion = (float)pMotion->nCountMotion / (float)pMotion->aMotionInfo[pMotion->motionType].aKeyInfo[pMotion->nKey].nFrame; // ���Βl
		float fRateBlend = (float)pMotion->nCounterBlend / (float)pMotion->nFrameBlend;

		if (pMotion->bFinishMotion && pMotion->bBlendMotion)
		{
			//���݂̃��[�V�����̉�]X
			float fDiffMotionRX = pMotion->aMotionInfo[pMotion->motionType].aKeyInfo[nextKey].aKey[nCntModel].fRotX -
				pMotion->aMotionInfo[pMotion->motionType].aKeyInfo[pMotion->nKey].aKey[nCntModel].fRotX;

			float fRotXCurrent = pMotion->aMotionInfo[pMotion->motionType].aKeyInfo[pMotion->nKey].aKey[nCntModel].fRotX + (fDiffMotionRX * fRateMotion);

			//���݂̃��[�V�����̉�]Y
			float fDiffMotionRY = pMotion->aMotionInfo[pMotion->motionType].aKeyInfo[nextKey].aKey[nCntModel].fRotY -
				pMotion->aMotionInfo[pMotion->motionType].aKeyInfo[pMotion->nKey].aKey[nCntModel].fRotY;

			float fRotYCurrent = pMotion->aMotionInfo[pMotion->motionType].aKeyInfo[pMotion->nKey].aKey[nCntModel].fRotY + (fDiffMotionRY * fRateMotion);

			//���݂̃��[�V�����̉�]Z
			float fDiffMotionRZ = pMotion->aMotionInfo[pMotion->motionType].aKeyInfo[nextKey].aKey[nCntModel].fRotZ -
				pMotion->aMotionInfo[pMotion->motionType].aKeyInfo[pMotion->nKey].aKey[nCntModel].fRotZ;

			float fRotZCurrent = pMotion->aMotionInfo[pMotion->motionType].aKeyInfo[pMotion->nKey].aKey[nCntModel].fRotZ + (fDiffMotionRZ * fRateMotion);


			//�u�����h���[�V�����̉�]X
			float fDiffMotionBlendRX = pMotion->aMotionInfo[pMotion->motiontypeBlend].aKeyInfo[pMotion->nNextKeyBlend].aKey[nCntModel].fRotX -
				pMotion->aMotionInfo[pMotion->motiontypeBlend].aKeyInfo[pMotion->nKeyBlend].aKey[nCntModel].fRotX;

			float fRotXBlend = pMotion->aMotionInfo[pMotion->motiontypeBlend].aKeyInfo[pMotion->nKeyBlend].aKey[nCntModel].fRotX + (fDiffMotionBlendRX * fRateBlend);

			//�u�����h���[�V�����̉�]Y
			float fDiffMotionBlendRY = pMotion->aMotionInfo[pMotion->motiontypeBlend].aKeyInfo[pMotion->nNextKeyBlend].aKey[nCntModel].fRotY -
				pMotion->aMotionInfo[pMotion->motiontypeBlend].aKeyInfo[pMotion->nKeyBlend].aKey[nCntModel].fRotY;

			float fRotYBlend = pMotion->aMotionInfo[pMotion->motiontypeBlend].aKeyInfo[pMotion->nKeyBlend].aKey[nCntModel].fRotY + (fDiffMotionBlendRY * fRateBlend);

			//�u�����h���[�V�����̉�]Z
			float fDiffMotionBlendRZ = pMotion->aMotionInfo[pMotion->motiontypeBlend].aKeyInfo[pMotion->nNextKeyBlend].aKey[nCntModel].fRotZ -
				pMotion->aMotionInfo[pMotion->motiontypeBlend].aKeyInfo[pMotion->nKeyBlend].aKey[nCntModel].fRotZ;

			float fRotZBlend = pMotion->aMotionInfo[pMotion->motiontypeBlend].aKeyInfo[pMotion->nKeyBlend].aKey[nCntModel].fRotZ + (fDiffMotionBlendRZ * fRateBlend);


			//���݂̃��[�V�����̈ʒuX
			float fDiffMotionPX = pMotion->aMotionInfo[pMotion->motionType].aKeyInfo[nextKey].aKey[nCntModel].fPosX -
				pMotion->aMotionInfo[pMotion->motionType].aKeyInfo[pMotion->nKey].aKey[nCntModel].fPosX;

			float fPosXCurrent = pMotion->aMotionInfo[pMotion->motionType].aKeyInfo[pMotion->nKey].aKey[nCntModel].fPosX + (fDiffMotionPX * fRateMotion);

			//���݂̃��[�V�����̈ʒuY
			float fDiffMotionPY = pMotion->aMotionInfo[pMotion->motionType].aKeyInfo[nextKey].aKey[nCntModel].fPosY -
				pMotion->aMotionInfo[pMotion->motionType].aKeyInfo[pMotion->nKey].aKey[nCntModel].fPosY;

			float fPosYCurrent = pMotion->aMotionInfo[pMotion->motionType].aKeyInfo[pMotion->nKey].aKey[nCntModel].fPosY + (fDiffMotionPY * fRateMotion);

			//���݂̃��[�V�����̈ʒuZ
			float fDiffMotionPZ = pMotion->aMotionInfo[pMotion->motionType].aKeyInfo[nextKey].aKey[nCntModel].fPosZ -
				pMotion->aMotionInfo[pMotion->motionType].aKeyInfo[pMotion->nKey].aKey[nCntModel].fPosZ;

			float fPosZCurrent = pMotion->aMotionInfo[pMotion->motionType].aKeyInfo[pMotion->nKey].aKey[nCntModel].fPosZ + (fDiffMotionPZ * fRateMotion);


			//�u�����h���[�V�����̈ʒuX
			float fDiffMotionBlendPX = pMotion->aMotionInfo[pMotion->motiontypeBlend].aKeyInfo[pMotion->nNextKeyBlend].aKey[nCntModel].fPosX -
				pMotion->aMotionInfo[pMotion->motiontypeBlend].aKeyInfo[pMotion->nKeyBlend].aKey[nCntModel].fPosX;

			float fPosXBlend = pMotion->aMotionInfo[pMotion->motiontypeBlend].aKeyInfo[pMotion->nKeyBlend].aKey[nCntModel].fPosX + (fDiffMotionBlendPX * fRateBlend);

			//�u�����h���[�V�����̈ʒuY
			float fDiffMotionBlendPY = pMotion->aMotionInfo[pMotion->motiontypeBlend].aKeyInfo[pMotion->nNextKeyBlend].aKey[nCntModel].fPosY -
				pMotion->aMotionInfo[pMotion->motiontypeBlend].aKeyInfo[pMotion->nKeyBlend].aKey[nCntModel].fPosY;

			float fPosYBlend = pMotion->aMotionInfo[pMotion->motiontypeBlend].aKeyInfo[pMotion->nKeyBlend].aKey[nCntModel].fPosY + (fDiffMotionBlendPY * fRateBlend);

			//�u�����h���[�V�����̈ʒuZ
			float fDiffMotionBlendPZ = pMotion->aMotionInfo[pMotion->motiontypeBlend].aKeyInfo[pMotion->nNextKeyBlend].aKey[nCntModel].fPosZ -
				pMotion->aMotionInfo[pMotion->motiontypeBlend].aKeyInfo[pMotion->nKeyBlend].aKey[nCntModel].fPosZ;

			float fPosZBlend = pMotion->aMotionInfo[pMotion->motiontypeBlend].aKeyInfo[pMotion->nKeyBlend].aKey[nCntModel].fPosZ + (fDiffMotionBlendPZ * fRateBlend);

			float fDiffBlendRX = fRotXBlend - fRotXCurrent; // ����
			float fDiffBlendRY = fRotYBlend - fRotYCurrent; // ����
			float fDiffBlendRZ = fRotZBlend - fRotZCurrent; // ����

			float fRotX = fRotXCurrent + (fDiffBlendRX * fRateBlend); // ���߂�l
			float fRotY = fRotYCurrent + (fDiffBlendRY * fRateBlend); // ���߂�l
			float fRotZ = fRotZCurrent + (fDiffBlendRZ * fRateBlend); // ���߂�l

			float fDiffBlendPX = fPosXBlend - fPosXCurrent; // ����
			float fDiffBlendPY = fPosYBlend - fPosYCurrent; // ����
			float fDiffBlendPZ = fPosZBlend - fPosZCurrent; // ����

			float fPosX = fPosXCurrent + (fDiffBlendPX * fRateBlend); // ���߂�l
			float fPosY = fPosYCurrent + (fDiffBlendPY * fRateBlend); // ���߂�l
			float fPosZ = fPosZCurrent + (fDiffBlendPZ * fRateBlend); // ���߂�l

			// �ʒu��ݒ�
			pMotion->aModel[nCntModel].pos = D3DXVECTOR3(fPosX, fPosY, fPosZ);

			// ������ݒ�
			pMotion->aModel[nCntModel].rot = D3DXVECTOR3(fRotX, fRotY, fRotZ);
		}

		// �I�t�Z�b�g���l�����������ݒ�
		pMotion->aModel[nCntModel].rot += pMotion->aModel[nCntModel].offrot;

		// �I�t�Z�b�g���l�������ʒu�ݒ�
		pMotion->aModel[nCntModel].pos += pMotion->aModel[nCntModel].offpos;
	}

	if (!pMotion->aMotionInfo[pMotion->motionType].bLoop &&
		pMotion->nKey >= pMotion->aMotionInfo[pMotion->motionType].nNumkey - 1)
	{
		pMotion->bFinishMotion = true;
		//pMotion->motionType = MOTIONTYPE_NEUTRAL; // ���[�V�����^�C�v���j���[�g�����ɂ���
	}

	// ���[�V�������I��邩�L�[���ő傩�u�����h�̃J�E���g���ő�ɂȂ���
	if (pMotion->bFinishMotion &&
		pMotion->nKey >= pMotion->aMotionInfo[pMotion->motionType].nNumkey - 1 &&
		pMotion->nCounterBlend >= pMotion->nFrameBlend &&
		pMotion->bBlendMotion)
	{
		pMotion->bFinishMotion = false;           // ���Ƃɖ߂�
		pMotion->bBlendMotion = false;			  // ���Ƃɖ߂�
		pMotion->motionType = MOTIONTYPE_NEUTRAL; // ���[�V�����^�C�v���j���[�g�����ɂ���
	}

	if (!pMotion->bBlendMotion && pMotion->bFinishMotion)
	{
		pMotion->motionType = MOTIONTYPE_NEUTRAL;
	}

	if (pMotion->nCountMotion >= pMotion->aMotionInfo[pMotion->motionType].aKeyInfo[pMotion->nKey].nFrame)
	{
		//���[�V�����J�E���g���ő�ɂȂ�����0�ɖ߂�
		pMotion->nKey = (pMotion->nKey + 1) % pMotion->aMotionInfo[pMotion->motionType].nNumkey;
		pMotion->nCountMotion = 0;
	}


	//���[�V�����J�E���g�����Z
	pMotion->nCountMotion++;
	
	if (pMotion->bFinishMotion)
	{
		pMotion->nCounterBlend++; // �u�����h���[�V�����J�E���g���C���N�������g
	}

}
//================================================================================================================
// ���[�V�����̐ݒ菈��
//================================================================================================================
void SetMotion(MOTION *pMotion,MOTIONTYPE motiontype, MOTIONTYPE motiontypeBlend, bool Blend, int nFrameBlend)
{
	Player* pPlayer = GetPlayer();                       // �v���C���[���擾

	if (Blend == true)
	{
		pMotion->nKey = 0;							 // 0����n�߂�
		pMotion->nKeyBlend = 0;						 // 0����n�߂�
		pMotion->nCountMotion = 0;					 // 0����n�߂�
		pMotion->nCounterBlend = 0;					 // 0����n�߂�
		pMotion->bBlendMotion = Blend;				 // �u�����h�����邩�ǂ���
		pMotion->nFrameBlend = nFrameBlend;			 // �u�����h�̃t���[������
		pMotion->motiontypeBlend = motiontypeBlend;  // �u�����h���郂�[�V�����̃^�C�v����
	}
	pMotion->motionType = motiontype;				 // ���[�V�����̃^�C�v����
	pMotion->bFinishMotion = false;				     // ���Ƃɖ߂�
}

