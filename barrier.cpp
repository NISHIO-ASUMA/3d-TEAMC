//==============================================================================================================
//
// �o���A [barrier.h]
// Author:YOSHIDA YUUTO
//
//==============================================================================================================

//**************************************************************************************************************
// �C���N���[�h�t�@�C��
//**************************************************************************************************************
#include "barrier.h"
#include "mark.h"
#include "effect2.h"

//**************************************************************************************************************
// �}�N����`
//**************************************************************************************************************
#define HALF_VALUE (0.5f) // �����ɂ���

//**************************************************************************************************************
// �v���g�^�C�v�錾
//**************************************************************************************************************
bool isCollisionZ(D3DXVECTOR3* pPos, D3DXVECTOR3* pPosOld, D3DXVECTOR3* pMove, D3DXVECTOR3* pSize, int nCntBarrier);		// Z�͈̔�
bool isCollisionRightX(D3DXVECTOR3* pPos, D3DXVECTOR3* pPosOld, D3DXVECTOR3* pMove, D3DXVECTOR3* pSize, int nCntBarrier);	// X�̖ʂɍ�����E�ɓ�������
bool isCollisionLeftX(D3DXVECTOR3* pPos, D3DXVECTOR3* pPosOld, D3DXVECTOR3* pMove, D3DXVECTOR3* pSize, int nCntBarrier);	// X�̖ʂɉE���獶�ɓ�������
bool isCollisionX(D3DXVECTOR3* pPos, D3DXVECTOR3* pPosOld, D3DXVECTOR3* pMove, D3DXVECTOR3* pSize, int nCntBarrier);		// X�͈̔�
bool isCollisionFrontZ(D3DXVECTOR3* pPos, D3DXVECTOR3* pPosOld, D3DXVECTOR3* pMove, D3DXVECTOR3* pSize, int nCntBarrier);	// �O�ʂɓ�������
bool isCollisionBehindZ(D3DXVECTOR3* pPos, D3DXVECTOR3* pPosOld, D3DXVECTOR3* pMove, D3DXVECTOR3* pSize, int nCntBarrier);  // �w�ʂɓ�������

//**************************************************************************************************************
// �O���[�o���ϐ�
//**************************************************************************************************************
Barrier g_Barrier[MAX_BARRIER]; // �o���A�̏��

//==============================================================================================================
// �o���A�̏���������
//==============================================================================================================
void InitBarrier(void)
{
	// �o���A�̍ő吔����
	for (int nCnt = 0; nCnt < MAX_BARRIER; nCnt++)
	{
		g_Barrier[nCnt].pos = NULLVECTOR3;		// �ʒu
		g_Barrier[nCnt].vtxMax = NULLVECTOR3;	// �ő�̒��_
		g_Barrier[nCnt].vtxMin = NULLVECTOR3;	// �ŏ��̒��_
		g_Barrier[nCnt].Size = NULLVECTOR3;		// �傫��
		g_Barrier[nCnt].bUse = false;			// �g�p���
	}
}
//==============================================================================================================
// �o���A�̐ݒ菈��
//==============================================================================================================
void SetBarrier(D3DXVECTOR3 pos, D3DXVECTOR3 vtxMax, D3DXVECTOR3 vtxMin)
{
	// �o���A�̍ő吔����
	for (int nCnt = 0; nCnt < MAX_BARRIER; nCnt++)
	{
		// ���g�p��������
		if (g_Barrier[nCnt].bUse == false)
		{
			g_Barrier[nCnt].pos = pos;			// �ʒu
			g_Barrier[nCnt].vtxMax = vtxMax;	// �ő�̒��_
			g_Barrier[nCnt].vtxMin = vtxMin;	// �ŏ��̒��_

			g_Barrier[nCnt].Size = vtxMax - vtxMin; // �傫�������߂�
			g_Barrier[nCnt].bUse = true;			// �g�p��Ԃɂ���
			break;
		}
	}
}
//==============================================================================================================
// �o���A�̔��菈��
//==============================================================================================================
void CollisionBarrier(D3DXVECTOR3* pPos, D3DXVECTOR3* pPosOld, D3DXVECTOR3* pMove, D3DXVECTOR3* pSize)
{
	// �o���A�̍ő吔����
	for (int nCnt = 0; nCnt < MAX_BARRIER; nCnt++)
	{
		// ���g�p��������
		if (g_Barrier[nCnt].bUse == false) continue;

		// Z�͈̔͂ɓ����Ă���
		if (isCollisionZ(pPos, pPosOld, pMove, pSize, nCnt) == true)
		{
			// ������E�̖ʂɓ�������
			isCollisionRightX(pPos, pPosOld, pMove, pSize, nCnt);

			// �E���獶�̖ʂɓ�������
			isCollisionLeftX(pPos, pPosOld, pMove, pSize, nCnt);
		}

		// X�͈̔͂ɓ�����
		if (isCollisionX(pPos, pPosOld, pMove, pSize, nCnt) == true)
		{
			// �O����߂荞��
			isCollisionFrontZ(pPos, pPosOld, pMove, pSize, nCnt);

			// ��납��߂荞��
			isCollisionBehindZ(pPos, pPosOld, pMove, pSize, nCnt); 
		}

		float MaxX = g_Barrier[nCnt].pos.x + g_Barrier[nCnt].Size.x * 0.5f;
		float MaxY = g_Barrier[nCnt].pos.y + g_Barrier[nCnt].Size.y * 0.5f;
		float MaxZ = g_Barrier[nCnt].pos.z + g_Barrier[nCnt].Size.z * 0.5f;

		float MinX = g_Barrier[nCnt].pos.x - g_Barrier[nCnt].Size.x * 0.5f;
		float MinY = g_Barrier[nCnt].pos.y - g_Barrier[nCnt].Size.y * 0.5f;
		float MinZ = g_Barrier[nCnt].pos.z - g_Barrier[nCnt].Size.z * 0.5f;

		SetEffectX(D3DXVECTOR3(MaxX, MaxY, MaxZ),D3DXVECTOR3(0.0f,0.0f,0.0f),COLOR_RED,10,100.0f,0.0f,0);
		SetEffectX(D3DXVECTOR3(MinX, MinY, MinZ), D3DXVECTOR3(0.0f, 0.0f, 0.0f), COLOR_RED, 10, 100.0f, 0.0f, 0);
		//SetEffectX(D3DXVECTOR3(0.0f, 0.0f, MaxZ), D3DXVECTOR3(0.0f, 0.0f, 0.0f), COLOR_RED, 10, 10.0f, 0.0f, 0);
		//SetEffectX(D3DXVECTOR3(MinX, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), COLOR_RED, 10, 10.0f, 0.0f, 0);
		//SetEffectX(D3DXVECTOR3(0.0f, MinY, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), COLOR_RED, 10, 10.0f, 0.0f, 0);
		//SetEffectX(D3DXVECTOR3(0.0f, 0.0f, MinZ), D3DXVECTOR3(0.0f, 0.0f, 0.0f), COLOR_RED, 10, 10.0f, 0.0f, 0);
	}
}
//==============================================================================================================
// Z�͈̔�
//==============================================================================================================
bool isCollisionZ(D3DXVECTOR3* pPos, D3DXVECTOR3* pPosOld, D3DXVECTOR3* pMove, D3DXVECTOR3* pSize,int nCntBarrier)
{
	// �o���A�̒��S����ŏ��܂ł͈̔�
	const float BarrierZMin = g_Barrier[nCntBarrier].pos.z - g_Barrier[nCntBarrier].Size.z * HALF_VALUE;

	// �o���A�̒��S����ő�܂ł͈̔�
	const float BarrierZMax = g_Barrier[nCntBarrier].pos.z + g_Barrier[nCntBarrier].Size.z * HALF_VALUE;

	// �u���b�N�ɓ�����Ώۂ̈ʒu����ŏ��܂�
	const float pPosZMin = pPos->z - pSize->z * HALF_VALUE;

	// �u���b�N�ɓ�����Ώۂ̈ʒu����ő�܂�
	const float pPosZMax = pPos->z + pSize->z * HALF_VALUE;
	
	// Z�͈̔͂ɓ����Ă���
	const bool InRangeZ = (pPosZMin < BarrierZMax) && (pPosZMax > BarrierZMin);

	// Z�͈̔͂ɓ����Ă���
	if (InRangeZ == true) return true;
		
	// �������Ă��Ȃ�
	return false;
}
//==============================================================================================================
// X�̖ʂɍ�����E�ɓ�������
//==============================================================================================================
bool isCollisionRightX(D3DXVECTOR3* pPos, D3DXVECTOR3* pPosOld, D3DXVECTOR3* pMove, D3DXVECTOR3* pSize, int nCntBarrier)
{
	// ������Ώۂ̑O�̃t���[���̈ʒu
	const float pPosOldMaxX = pPosOld->x + pSize->x * HALF_VALUE;

	// ������Ώۂ̈ʒu
	const float pPosMaxX = pPos->x + pSize->x * HALF_VALUE;

	// �o���A�̍ŏ��͈̔�
	const float BarrierMinX = g_Barrier[nCntBarrier].pos.x - g_Barrier[nCntBarrier].Size.x * HALF_VALUE;

	// �������Ă���Ȃ�
	const bool isCollision = (pPosOldMaxX < BarrierMinX) && (pPosMaxX > BarrierMinX);

	// x��������E�ɂ߂荞��	
	if (isCollision == true)
	{
		// �����߂�
		pPos->x = g_Barrier[nCntBarrier].pos.x - g_Barrier[nCntBarrier].Size.x * HALF_VALUE - pSize->x * HALF_VALUE - 0.1f;
		return true;
	}
	return false;
}
//==============================================================================================================
// X�̖ʂɉE���獶�ɓ�������
//==============================================================================================================
bool isCollisionLeftX(D3DXVECTOR3* pPos, D3DXVECTOR3* pPosOld, D3DXVECTOR3* pMove, D3DXVECTOR3* pSize, int nCntBarrier)
{
	// ������Ώۂ̑O�̃t���[���̈ʒu
	const float pPosOldMinX = pPosOld->x - pSize->x * HALF_VALUE;

	// ������Ώۂ̈ʒu
	const float pPosMinX = pPos->x - pSize->x * HALF_VALUE;

	// �o���A�̍ŏ��͈̔�
	const float BarrierMaxX = g_Barrier[nCntBarrier].pos.x + g_Barrier[nCntBarrier].Size.x * HALF_VALUE;

	// �������Ă���Ȃ�
	const bool isCollision = (pPosOldMinX > BarrierMaxX) && (pPosMinX < BarrierMaxX);

	// x���E���獶�ɂ߂荞��	
	if (isCollision == true)
	{
		// �����߂�
		pPos->x = g_Barrier[nCntBarrier].pos.x + g_Barrier[nCntBarrier].Size.x * HALF_VALUE + pSize->x * HALF_VALUE + 0.1f;
		return true;
	}
	return false;
}
//==============================================================================================================
// X�͈̔�
//==============================================================================================================
bool isCollisionX(D3DXVECTOR3* pPos, D3DXVECTOR3* pPosOld, D3DXVECTOR3* pMove, D3DXVECTOR3* pSize, int nCntBarrier)
{
	// �o���A�̒��S����ŏ��܂ł͈̔�
	const float BarrierXMin = g_Barrier[nCntBarrier].pos.x - g_Barrier[nCntBarrier].Size.x * HALF_VALUE;

	// �o���A�̒��S����ő�܂ł͈̔�
	const float BarrierXMax = g_Barrier[nCntBarrier].pos.x + g_Barrier[nCntBarrier].Size.x * HALF_VALUE;

	// �u���b�N�ɓ�����Ώۂ̈ʒu����ŏ��܂�
	const float pPosXMin = pPos->x - pSize->x * HALF_VALUE;

	// �u���b�N�ɓ�����Ώۂ̈ʒu����ő�܂�
	const float pPosXMax = pPos->x + pSize->x * HALF_VALUE;

	// Z�͈̔͂ɓ����Ă���
	const bool InRangeX = (pPosXMin < BarrierXMax) && (pPosXMax > BarrierXMin);

	// X�͈̔͂ɓ�����
	if (InRangeX == true)
	{
		// �͈͂ɓ����Ă���
		return true;
	}

	// �͈͊O
	return false;
}
//==============================================================================================================
// �O�ʂɓ�������
//==============================================================================================================
bool isCollisionFrontZ(D3DXVECTOR3* pPos, D3DXVECTOR3* pPosOld, D3DXVECTOR3* pMove, D3DXVECTOR3* pSize, int nCntBarrier)
{
	// ������Ώۂ̑O�̃t���[���̈ʒu
	const float pPosOldMaxZ = pPosOld->z + pSize->z * HALF_VALUE;

	// ������Ώۂ̈ʒu
	const float pPosMaxZ = pPos->z + pSize->z * HALF_VALUE;

	// �o���A�̍ŏ��͈̔�
	const float BarrierMinZ = g_Barrier[nCntBarrier].pos.z - g_Barrier[nCntBarrier].Size.z * HALF_VALUE;

	// �������Ă���Ȃ�
	const bool isCollisionZ = (pPosOldMaxZ < BarrierMinZ) && (pPosMaxZ > BarrierMinZ);

	// �������Ă���
	if(isCollisionZ == true)
	{
		// �����߂�
		pPos->z = g_Barrier[nCntBarrier].pos.z - g_Barrier[nCntBarrier].Size.z * HALF_VALUE - pSize->z * HALF_VALUE - 0.1f;
		return true;
	}
	return false;
}
//==============================================================================================================
// �w�ʂɓ�������
//==============================================================================================================
bool isCollisionBehindZ(D3DXVECTOR3* pPos, D3DXVECTOR3* pPosOld, D3DXVECTOR3* pMove, D3DXVECTOR3* pSize, int nCntBarrier)
{
	// ������Ώۂ̑O�̃t���[���̈ʒu
	const float pPosOldMinZ = pPosOld->z - pSize->z * HALF_VALUE;

	// ������Ώۂ̈ʒu
	const float pPosMinZ = pPos->z - pSize->z * HALF_VALUE;

	// �o���A�̍ŏ��͈̔�
	const float BarrierMaxZ = g_Barrier[nCntBarrier].pos.z + g_Barrier[nCntBarrier].Size.z * HALF_VALUE;

	// �������Ă���Ȃ�
	const bool isCollisionZ = (pPosOldMinZ > BarrierMaxZ) && (pPosMinZ < BarrierMaxZ);

	// �������Ă���
	if (isCollisionZ == true)
	{
		// �����߂�
		pPos->z = g_Barrier[nCntBarrier].pos.z + g_Barrier[nCntBarrier].Size.z * HALF_VALUE + pSize->z * HALF_VALUE + HALF_VALUE;
		return true;
	}
	return false;
}
