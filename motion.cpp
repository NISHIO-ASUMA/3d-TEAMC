//==============================================================================================================
//
// ���[�V���� [motion.cpp]
// Author:YOSHIDA YUUTO
//
//==============================================================================================================

//**************************************************************************************************************
//�C���N���[�h�t�@�C��
//**************************************************************************************************************
#include "motion.h"
#include"meshfield.h"
#include <stdio.h>
#include "HPGauge.h"
#include "player.h"

//**************************************************************************************************************
// �}�N����`
//**************************************************************************************************************
#define MAX_WORD (128) // �ő�̕�����

//**************************************************************************************************************
// �v���g�^�C�v�錾
//**************************************************************************************************************
void UpdateCurrentMotion(MOTION* pMotion,int nCntModel);
void UpdateBlendMotion(MOTION* pMotion,int nCntModel, int nextKey);

//=========================================================================================================
// ���[�V�����̍X�V����
//=========================================================================================================
void UpdateMotion(MOTION *pMotion)
{
	for (int nCntModel = 0; nCntModel < pMotion->nNumModel; nCntModel++)
	{	
		if (pMotion->aMotionInfo[pMotion->motionType].nNumkey != NULL)
		{
			// ���̃L�[
			pMotion->nextKey = (pMotion->nKey + 1) % pMotion->aMotionInfo[pMotion->motionType].nNumkey;
		}
		else if (pMotion->aMotionInfo[pMotion->motiontypeBlend].nNumkey != NULL)
		{
			// �u�����h���[�V�����̎��̃L�[
			pMotion->nNextKeyBlend = (pMotion->nKeyBlend + 1) % pMotion->aMotionInfo[pMotion->motiontypeBlend].nNumkey;
		}
		else
		{
			SetMotion(pMotion, MOTIONTYPE_NEUTRAL, true, 10);
		}

		// ���[�V�����u�����h���I��������
		if (pMotion->bFinishMotion == false)
		{
			// ���݂̃��[�V�����̍X�V����
			UpdateCurrentMotion(pMotion, nCntModel);
		}
		if ((pMotion->bFinishMotion == true || pMotion->bFirstMotion == true) && pMotion->bBlendMotion == true)
		{
			// �u�����h���[�V�����̍X�V����
			UpdateBlendMotion(pMotion, nCntModel, pMotion->nextKey);
		}

		// �I�t�Z�b�g���l�����������ݒ�
		pMotion->aModel[nCntModel].rot += pMotion->aModel[nCntModel].offrot;

		// �I�t�Z�b�g���l�������ʒu�ݒ�
		pMotion->aModel[nCntModel].pos += pMotion->aModel[nCntModel].offpos;
		
	}

	int LastKey = pMotion->aMotionInfo[pMotion->motionType].nNumkey - 1;
	int nBlendFrame = pMotion->aMotionInfo[pMotion->motionType].aKeyInfo[LastKey].nFrame;

	// ���[�V�������I�������
	if (pMotion->aMotionInfo[pMotion->motionType].bLoop == false && pMotion->nKey >= pMotion->aMotionInfo[pMotion->motionType].nNumkey - 1 &&
		pMotion->bBlendMotion == true && pMotion->bFinishMotion == false && pMotion->bFirstMotion == false)
	{
		pMotion->nFrameBlend = nBlendFrame;
		pMotion->nCounterBlend = 0;
		pMotion->nKeyBlend = 0;
		pMotion->bFinishMotion = true;
		pMotion->motiontypeBlend = MOTIONTYPE_NEUTRAL; // ���[�V�����^�C�v���j���[�g�����ɂ���
	}

	// ���[�V�����̏o�����̃u�����h���I������
	if (pMotion->bFirstMotion == true && pMotion->nCounterBlend >= pMotion->nFrameBlend)
	{
		// ���Ƃɖ߂�
		pMotion->bFirstMotion = false;

		// ���[�V�������u�����h�������[�V�����ɂ���
		pMotion->motionType = pMotion->motiontypeBlend;

		// 0�Ԗڂ���n�߂�
		pMotion->nKey = 0;

		// �u�����h�����t���[������J�n
		pMotion->nCountMotion = pMotion->nFrameBlend;

	}

	// ���[�V�������I��邩�L�[���ő傩�u�����h�̃J�E���g���ő�ɂȂ���
	if (pMotion->nCounterBlend >= nBlendFrame && pMotion->bBlendMotion == true && pMotion->bFinishMotion == true && pMotion->bFirstMotion == false)
	{
		pMotion->bFinishMotion = false;           // ���Ƃɖ߂�
		pMotion->bBlendMotion = false;			  // ���Ƃɖ߂�
		pMotion->motionType = MOTIONTYPE_NEUTRAL; // ���[�V�����^�C�v���j���[�g�����ɂ���
	}

	// �u�����h���Ȃ�
	if (!pMotion->bBlendMotion == true && pMotion->bFinishMotion == true)
	{
		pMotion->motionType = MOTIONTYPE_NEUTRAL;
	}

	// ���[�V�����J�E���g�̐ݒ�
	if (pMotion->nCountMotion >= pMotion->aMotionInfo[pMotion->motionType].aKeyInfo[pMotion->nKey].nFrame)
	{
		// ��񂪂Ȃ�������
		if (pMotion->aMotionInfo[pMotion->motionType].nNumkey != NULL)
		{
			//���[�V�����J�E���g���ő�ɂȂ�����0�ɖ߂�
			pMotion->nKey = (pMotion->nKey + 1) % pMotion->aMotionInfo[pMotion->motionType].nNumkey;
		}
		else
		{
			
		}
		pMotion->nCountMotion = 0;
		
	}

	// ���[�V�����J�E���^�[�̍X�V
	if (pMotion->bFirstMotion == false && pMotion->bFinishMotion == false)
	{
		//���[�V�����J�E���g�����Z
		pMotion->nCountMotion++;
	}

	// �u�����h���n�܂�����
	if (pMotion->bFinishMotion == true || pMotion->bFirstMotion == true)
	{
		pMotion->nCounterBlend++; // �u�����h���[�V�����J�E���g���C���N�������g
	}

	// ���[�V���������S��������
	if (pMotion->motionType == MOTIONTYPE_DEATH && pMotion->nKey >= pMotion->aMotionInfo[pMotion->motionType].nNumkey - 1)
	{
		pMotion->nCountMotion--;
		pMotion->nCounterBlend--;
	}
	
}
//================================================================================================================
// ���[�V�����̐ݒ菈��
//================================================================================================================
void SetMotion(MOTION* pMotion, MOTIONTYPE motiontype, bool Blend, int nFrameBlend)
{
	Player* pPlayer = GetPlayer();                       // �v���C���[���擾

	// �ŏ��̃u�����h
	// ���[�V�����u�����h�����邩�ǂ���
	if (Blend == true)
	{
		// �ŏ��̃��[�V�����u�����h���I����Ă���
		if (pMotion->bFirstMotion == false)
		{
			pMotion->bFirstMotion = true;
			pMotion->nKeyBlend = 0;							// 0����n�߂�
			pMotion->nCounterBlend = 0;						// 0����n�߂�
		}
		pMotion->bBlendMotion = Blend;					// �u�����h�����邩�ǂ���
		pMotion->nFrameBlend = 10;						// �u�����h�̃t���[������
		pMotion->motiontypeBlend = motiontype;			// �u�����h���郂�[�V�����̃^�C�v����
		pMotion->bFinishMotion = false;
		
	}
	// ���[�V�����u�����h���Ȃ�
	else
	{
		
		pMotion->bBlendMotion = Blend;					// �u�����h�����邩�ǂ���
		pMotion->motionType = motiontype;			// �u�����h���郂�[�V�����̃^�C�v����
		pMotion->bFinishMotion = false;
	}
}
//================================================================================================================
// �p�x�̐��K��
//================================================================================================================
void NormalizeRotation(D3DXVECTOR3* OutRot, D3DXVECTOR3 Rot)
{
	// �p�x�̐��K��
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
// �A�N�V�������[�V��������Ȃ���
//================================================================================================================
bool CheckActionMotion(MOTION *pMotion)
{
	bool bFrag = false;
	// ���[�V�������U������Ȃ�������
	if (pMotion->motiontypeBlend != MOTIONTYPE_ACTION &&
		pMotion->motiontypeBlend != MOTIONTYPE_ACTION2 &&
		pMotion->motiontypeBlend != MOTIONTYPE_ACTION3 &&
		pMotion->motiontypeBlend != MOTIONTYPE_ACTION4)
	{
		bFrag = true;
	}
	return bFrag;
}
//================================================================================================================
// ���݂̃��[�V�����̍X�V����
//================================================================================================================
void UpdateCurrentMotion(MOTION* pMotion, int nCntModel)
{
	// ���̃L�[
	int nextKey = pMotion->nextKey;

	// ���݂̉�]
	float fCurrentRotX = pMotion->aMotionInfo[pMotion->motionType].aKeyInfo[pMotion->nKey].aKey[nCntModel].fRotX;
	float fCurrentRotY = pMotion->aMotionInfo[pMotion->motionType].aKeyInfo[pMotion->nKey].aKey[nCntModel].fRotY;
	float fCurrentRotZ = pMotion->aMotionInfo[pMotion->motionType].aKeyInfo[pMotion->nKey].aKey[nCntModel].fRotZ;

	// ���̉�]
	float fNextRotX = pMotion->aMotionInfo[pMotion->motionType].aKeyInfo[nextKey].aKey[nCntModel].fRotX;
	float fNextRotY = pMotion->aMotionInfo[pMotion->motionType].aKeyInfo[nextKey].aKey[nCntModel].fRotY;
	float fNextRotZ = pMotion->aMotionInfo[pMotion->motionType].aKeyInfo[nextKey].aKey[nCntModel].fRotZ;

	// ���݂̈ʒu
	float fCurrentPosX = pMotion->aMotionInfo[pMotion->motionType].aKeyInfo[pMotion->nKey].aKey[nCntModel].fPosX;
	float fCurrentPosY = pMotion->aMotionInfo[pMotion->motionType].aKeyInfo[pMotion->nKey].aKey[nCntModel].fPosY;
	float fCurrentPosZ = pMotion->aMotionInfo[pMotion->motionType].aKeyInfo[pMotion->nKey].aKey[nCntModel].fPosZ;

	// ���̈ʒu
	float fNextPosX = pMotion->aMotionInfo[pMotion->motionType].aKeyInfo[nextKey].aKey[nCntModel].fPosX;
	float fNextPosY = pMotion->aMotionInfo[pMotion->motionType].aKeyInfo[nextKey].aKey[nCntModel].fPosY;
	float fNextPosZ = pMotion->aMotionInfo[pMotion->motionType].aKeyInfo[nextKey].aKey[nCntModel].fPosZ;

	// �ʒu�̍����A�����̍���
	float DiffPosX = fNextPosX - fCurrentPosX;
	float DiffPosY = fNextPosY - fCurrentPosY;
	float DiffPosZ = fNextPosZ - fCurrentPosZ;
	float DiffRotX = fNextRotX - fCurrentRotX;
	float DiffRotY = fNextRotY - fCurrentRotY;
	float DiffRotZ = fNextRotZ - fCurrentRotZ;

	// ���K���p�ϐ�
	D3DXVECTOR3 fDiffRot = D3DXVECTOR3(DiffRotX, DiffRotY, DiffRotZ);

	// �����̊p�x�𐳋K��
	NormalizeRotation(&fDiffRot, fDiffRot);

	// �t���[���̑��Βl
	float fRateMotion = (float)pMotion->nCountMotion / (float)pMotion->aMotionInfo[pMotion->motionType].aKeyInfo[pMotion->nKey].nFrame;

	// �ʒu�����߂�
	float fPosX = fCurrentPosX + DiffPosX * fRateMotion;
	float fPosY = fCurrentPosY + DiffPosY * fRateMotion;
	float fPosZ = fCurrentPosZ + DiffPosZ * fRateMotion;

	// ���������߂�
	float fRotX = fCurrentRotX + fDiffRot.x * fRateMotion;
	float fRotY = fCurrentRotY + fDiffRot.y * fRateMotion;
	float fRotZ = fCurrentRotZ + fDiffRot.z * fRateMotion;

	// ���݂̈ʒu��ݒ�
	pMotion->aModel[nCntModel].pos = D3DXVECTOR3(fPosX, fPosY, fPosZ);

	// ���݂̌�����ݒ�
	pMotion->aModel[nCntModel].rot = D3DXVECTOR3(fRotX, fRotY, fRotZ);
}
//================================================================================================================
// �u�����h���[�V�����̍X�V����
//================================================================================================================
void UpdateBlendMotion(MOTION* pMotion, int nCntModel,int nextKey)
{
	float fRateMotion = (float)pMotion->nCountMotion / (float)pMotion->aMotionInfo[pMotion->motionType].aKeyInfo[pMotion->nKey].nFrame; // ���Βl
	float fRateBlend = (float)pMotion->nCounterBlend / (float)pMotion->nFrameBlend;
	
	//���݂̃��[�V�����̉�]X
	float fDiffMotionRX = pMotion->aMotionInfo[pMotion->motionType].aKeyInfo[nextKey].aKey[nCntModel].fRotX -
		pMotion->aMotionInfo[pMotion->motionType].aKeyInfo[pMotion->nKey].aKey[nCntModel].fRotX;

	//���݂̃��[�V�����̉�]Y
	float fDiffMotionRY = pMotion->aMotionInfo[pMotion->motionType].aKeyInfo[nextKey].aKey[nCntModel].fRotY -
		pMotion->aMotionInfo[pMotion->motionType].aKeyInfo[pMotion->nKey].aKey[nCntModel].fRotY;

	//���݂̃��[�V�����̉�]Z
	float fDiffMotionRZ = pMotion->aMotionInfo[pMotion->motionType].aKeyInfo[nextKey].aKey[nCntModel].fRotZ -
		pMotion->aMotionInfo[pMotion->motionType].aKeyInfo[pMotion->nKey].aKey[nCntModel].fRotZ;

	// �p�x�̐��K��
	D3DXVECTOR3 fDiffMotionRot(fDiffMotionRX, fDiffMotionRY, fDiffMotionRZ);
	NormalizeRotation(&fDiffMotionRot, fDiffMotionRot);

	fDiffMotionRX = fDiffMotionRot.x;
	fDiffMotionRY = fDiffMotionRot.y;
	fDiffMotionRZ = fDiffMotionRot.z;

	// ����X
	float fRotXCurrent = pMotion->aMotionInfo[pMotion->motionType].aKeyInfo[pMotion->nKey].aKey[nCntModel].fRotX + (fDiffMotionRX * fRateMotion);

	// ����Y
	float fRotYCurrent = pMotion->aMotionInfo[pMotion->motionType].aKeyInfo[pMotion->nKey].aKey[nCntModel].fRotY + (fDiffMotionRY * fRateMotion);

	// ����Z
	float fRotZCurrent = pMotion->aMotionInfo[pMotion->motionType].aKeyInfo[pMotion->nKey].aKey[nCntModel].fRotZ + (fDiffMotionRZ * fRateMotion);

	//�u�����h���[�V�����̉�]X
	float fDiffMotionBlendRX = pMotion->aMotionInfo[pMotion->motiontypeBlend].aKeyInfo[pMotion->nNextKeyBlend].aKey[nCntModel].fRotX -
		pMotion->aMotionInfo[pMotion->motiontypeBlend].aKeyInfo[pMotion->nKeyBlend].aKey[nCntModel].fRotX;

	//�u�����h���[�V�����̉�]Y
	float fDiffMotionBlendRY = pMotion->aMotionInfo[pMotion->motiontypeBlend].aKeyInfo[pMotion->nNextKeyBlend].aKey[nCntModel].fRotY -
		pMotion->aMotionInfo[pMotion->motiontypeBlend].aKeyInfo[pMotion->nKeyBlend].aKey[nCntModel].fRotY;

	//�u�����h���[�V�����̉�]Z
	float fDiffMotionBlendRZ = pMotion->aMotionInfo[pMotion->motiontypeBlend].aKeyInfo[pMotion->nNextKeyBlend].aKey[nCntModel].fRotZ -
		pMotion->aMotionInfo[pMotion->motiontypeBlend].aKeyInfo[pMotion->nKeyBlend].aKey[nCntModel].fRotZ;

	// �p�x�̐��K��
	D3DXVECTOR3 fDiffBlendRot(fDiffMotionBlendRX, fDiffMotionBlendRY, fDiffMotionBlendRZ);
	NormalizeRotation(&fDiffBlendRot, fDiffBlendRot);

	fDiffMotionBlendRX = fDiffBlendRot.x;
	fDiffMotionBlendRY = fDiffBlendRot.y;
	fDiffMotionBlendRZ = fDiffBlendRot.z;

	// �u�����h���[�V�����̉�]X
	float fRotXBlend = pMotion->aMotionInfo[pMotion->motiontypeBlend].aKeyInfo[pMotion->nKeyBlend].aKey[nCntModel].fRotX + (fDiffMotionBlendRX * fRateBlend);

	// �u�����h���[�V�����̉�]Y
	float fRotYBlend = pMotion->aMotionInfo[pMotion->motiontypeBlend].aKeyInfo[pMotion->nKeyBlend].aKey[nCntModel].fRotY + (fDiffMotionBlendRY * fRateBlend);

	// �u�����h���[�V�����̉�]Z
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

	float fDiffBlendRX = fRotXBlend - fRotXCurrent; // ����X
	float fDiffBlendRY = fRotYBlend - fRotYCurrent; // ����Y
	float fDiffBlendRZ = fRotZBlend - fRotZCurrent; // ����Z

	float fRotX = fRotXCurrent + (fDiffBlendRX * fRateBlend); // ���߂�l
	float fRotY = fRotYCurrent + (fDiffBlendRY * fRateBlend); // ���߂�l
	float fRotZ = fRotZCurrent + (fDiffBlendRZ * fRateBlend); // ���߂�l

	float fDiffBlendPX = fPosXBlend - fPosXCurrent; // ����X
	float fDiffBlendPY = fPosYBlend - fPosYCurrent; // ����Y
	float fDiffBlendPZ = fPosZBlend - fPosZCurrent; // ����Z

	float fPosX = fPosXCurrent + (fDiffBlendPX * fRateBlend); // ���߂�l
	float fPosY = fPosYCurrent + (fDiffBlendPY * fRateBlend); // ���߂�l
	float fPosZ = fPosZCurrent + (fDiffBlendPZ * fRateBlend); // ���߂�l

	// �ʒu��ݒ�
	pMotion->aModel[nCntModel].pos = D3DXVECTOR3(fPosX, fPosY, fPosZ);

	// ������ݒ�
	pMotion->aModel[nCntModel].rot = D3DXVECTOR3(fRotX, fRotY, fRotZ);
}
