//=====================================================================================================================
//
//�e�\���֘A�̃w�b�_�[
//Author;�����I��
//
//=====================================================================================================================

// �d���m�F
#ifndef _SHADOW_H_
#define _SHADOW_H_

// �C���N���[�h�t�@�C��
#include "main.h"

// �\���̐錾
typedef struct
{
	D3DXVECTOR3 pos;
	D3DXVECTOR3 rot;
	D3DXCOLOR col;
	D3DXMATRIX mtxWorld;
	float fALv;
	float fSize;
	bool bUse;
	bool bHide;
}Shadow;

// �v���g�^�C�v�錾
void InitShadow(void);
void UninitShadow(void);
void UpdateShadow(void);
void DrawShadow(void);

int SetShadow(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float fSize,float fAlv);
void SetPositionShadow(int nIdxShadow, D3DXVECTOR3 pos, float fSize, float fAlv);
void KillShadow(int nIdxShadow);
void HideShadow(int nIdxShadow);
void NoHideShadow(int nIdxShadow);
#endif