//==============================================================================================================
//
// math [math.cpp]
// Author: YOSHIDA YUTO
//
//==============================================================================================================

//*******************************************************************************************************************
// �C���N���[�h�t�@�C���錾
//*******************************************************************************************************************
#include "math.h"

//==============================================================================================================
// ���񂾂�ڕW�̒l�ɋ߂Â���֐�
//==============================================================================================================
float SetSmoothAprroach(float fDestValue, float Value, float coefficient)
{
	// ���񂾂�ڕW�̒l�ɋ߂Â���
	float OutValue = (fDestValue - Value) * coefficient;
	return OutValue;
}
//===============================================================================================================
// �~�̔���
//===============================================================================================================
bool sphererange(D3DXVECTOR3* pPos1, D3DXVECTOR3* pPos2, float radius1, float radius2) // �~�̓����蔻��
{
	bool bRange = false;

	D3DXVECTOR3 DiffPos; // �v�Z�p

	// ����XYZ�����߂�
	DiffPos.x = pPos1->x - pPos2->x;
	DiffPos.y = pPos1->y - pPos2->y;
	DiffPos.z = pPos1->z - pPos2->z;

	// ���������߂�
	float fDistance = (DiffPos.x * DiffPos.x) + (DiffPos.y * DiffPos.y) + (DiffPos.z * DiffPos.z);

	// ���a�����߂�
	float fRadius = radius1 + radius2;

	// ���a�����߂�
	fRadius = fRadius * fRadius;

	if (fDistance <= fRadius)
	{
		bRange = true;
	}
	return bRange;
}
//=====================================================================================================
// ���[���h�}�g���b�N�X��D3DXVECTOR3�ɕϊ�����
//=====================================================================================================
D3DXVECTOR3 SetMtxConversion(D3DXMATRIX mtxworld)
{
	// XYZ�ɕϊ�
	D3DXVECTOR3 OutPut(mtxworld._41, mtxworld._42, mtxworld._43);

	// �ϊ������l��Ԃ�
	return OutPut;
}
////=====================================================================================================
//// �G�t�F�N�g�̃e�N�X�`���A�j���[�V����
////=====================================================================================================
//void SetTextureAnimation(int *pPosX,int *pPosY,int *pAnimSpeed,int *pCounterAnim,int *pPatternAnim, VERTEX_3D *pVtx, LPDIRECT3DVERTEXBUFFER9 *pVtxBuff)
//{
//	VERTEX_3D* pVtx;
//
//	//���_�o�b�t�@�����b�N
//	g_pVtxBuffEffectX->Lock(0, 0, (void**)&pVtx, 0);
//
//	// �A�j���[�V�����J�E���^�[�����Z
//	g_EffectX[nCnt].g_nCounterAnim++;
//
//	// �e�N�X�`�����W���v�Z
//	float Uv = 1.0f / XtexPos;
//	float Hv = 1.0f / YtexPos;
//
//	// �A�j���[�V�����̃X�s�[�h
//	if (g_EffectX[nCnt].g_nCounterAnim >= Animspeed)
//	{
//		g_EffectX[nCnt].g_nCounterAnim = NULL;//�J�E���^�[�������l�ɖ߂�
//
//		g_EffectX[nCnt].g_nPatternAnim++;//�p�^�[���i���o�[���X�V
//
//		pVtx += 4 * nCnt;
//
//		// �e�N�X�`�����W�̍X�V
//		pVtx[0].tex = D3DXVECTOR2(g_EffectX[nCnt].g_nPatternAnim * Uv, (g_EffectX[nCnt].g_nPatternAnim / XtexPos) * Hv);
//		pVtx[1].tex = D3DXVECTOR2(Uv + g_EffectX[nCnt].g_nPatternAnim * Uv, (g_EffectX[nCnt].g_nPatternAnim / XtexPos) * Hv);
//		pVtx[2].tex = D3DXVECTOR2(g_EffectX[nCnt].g_nPatternAnim * Uv, (g_EffectX[nCnt].g_nPatternAnim / XtexPos) * Hv + Hv);
//		pVtx[3].tex = D3DXVECTOR2(Uv + g_EffectX[nCnt].g_nPatternAnim * Uv, (g_EffectX[nCnt].g_nPatternAnim / XtexPos) * Hv + Hv);
//	}
//
//	// �p�^�[�����ő�ɂȂ�����
//	if (g_EffectX[nCnt].g_nPatternAnim > XtexPos)
//	{
//		g_EffectX[nCnt].g_nPatternAnim = 0;//�p�^�[���i���o�[�������l�ɖ߂�
//	}
//
//	// ���_�o�b�t�@�̃A�����b�N
//	g_pVtxBuffEffectX->Unlock();
//}
