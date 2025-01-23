//============================
//
// ��[wall.h]
// Author:YOSHIDA YUUTO
//
//============================

#ifndef _WALL_H_
#define _WALL_H_

//****************************
//�C���N���[�h�t�@�C��
//****************************
#include"main.h"

//****************************
//�}�N����`
//****************************
#define MAX_WALL (8) //�ǂ̍ő吔
#define WALL_WIDTH (100.0f) // ����
#define WALL_HEIGHT (150.0f) // ����

//****************************
//�ǂ̍\����
//****************************
typedef struct
{
	D3DXVECTOR3 pos;
	D3DXVECTOR3 move;
	D3DXVECTOR3 rot;
	D3DXMATRIX mtxWorld;
	float fA;
	D3DXVECTOR3 Size;
	float fWidth;
	float fHeight;
	D3DXVECTOR3 fScal;
	bool bUse;
	D3DXVECTOR3 vecLine;
	D3DXVECTOR3 vecPos;
	D3DXMATRIX rotmtx;
	D3DXMATRIX posmtx;
	D3DXVECTOR3 vtxPos[4];
}WALL;

//****************************
//�v���g�^�C�v�錾
//****************************
void InitWall(void);//�ǂ̏���������
void UninitWall(void);//�ǂ̏I������
void UpdateWall(void);//�ǂ̍X�V����
void DrawWall(void);//�ǂ̕`�揈��
//void SetWall(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 fScal,float fA);//�ǂ̐ݒ菈��
void SetWall(D3DXVECTOR3 pos, D3DXVECTOR3 rot,float fA, D3DXVECTOR3 Size);//�ǂ̐ݒ菈��
void CollisionWall(void);
void DotWall(void);
WALL* GetWall(void);
int NumWall(void);
#endif