//============================
//
// ���f��[model.h]
// Author:YOSHIDA YUUTO
//
//============================

#ifndef _MODEL_H_
#define _MODEL_H_

//***************************
//�C���N���[�h�t�@�C��
//***************************
#include"main.h"

typedef struct
{
	LPD3DXMESH pMesh;//���b�V��(���_���)�ւ̃|�C���^
	LPD3DXBUFFER pBuffMat;//�}�e���A���ւ̃|�C���^
	DWORD dwNumMat;//�}�e���A���̐�
	int nIdxModelParent;//�e���f���̃C���f�b�N�X
	D3DXVECTOR3 pos;//�ʒu(�I�t�Z�b�g)
	D3DXVECTOR3 move;
	D3DXVECTOR3 rot;//����
	D3DXVECTOR3 offpos;//�ʒu(�I�t�Z�b�g)
	D3DXVECTOR3 offrot;//����
	D3DXVECTOR3 rotDest;//����
	D3DXMATRIX mtxWorld;//���[���h�}�g���b�N�X
	D3DXVECTOR3 vtxMin, vtxMax;
	D3DXVECTOR3 Size;
	D3DXVECTOR3 raidus;
	D3DXVECTOR3 posOld;//�O��̈ʒu
}MODEL;

//***************************
//�v���g�^�C�v�錾
//***************************
void InitModel(void);
void UninitModel(void);
void UpdateModel(void);
void DrawModel(void);
#endif
