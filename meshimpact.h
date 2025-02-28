//===================================================================================================================
//
// ���b�V���C���p�N�g[meshimpact.h]
// Author: YOSHIDA YUTO
//
//===================================================================================================================

#ifndef _MESHIMPACT_H_
#define _MESHIMPACT_H_

//**************************************************************************************************************
// �C���N���[�h�t�@�C��
//**************************************************************************************************************
#include"main.h"

//**************************************************************************************************************
// �}�N����`
//**************************************************************************************************************
#define IMPACT_Z (1)

//**************************************************************************************************************
// ���b�V���C���p�N�g�̎��
//**************************************************************************************************************
typedef enum
{
	IMPACTTYPE_NORMAL = 0,
	IMPACTTYPE_PLAYER,
	IMPACTTYPE_ENEMY,
	IMPACTTYPE_SPKATANA,
	IMPACTTYPE_MAX
}IMPACTTYPE;

//**************************************************************************************************************
//���b�V���C���p�N�g�̍\����
//**************************************************************************************************************
typedef struct
{
	LPDIRECT3DTEXTURE9 g_pTextureMeshImpact;	    // �e�N�X�`���ւ̃|�C���^
	LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffMeshImpact; // ���_�o�b�t�@�ւ̃|�C���^
	LPDIRECT3DINDEXBUFFER9 g_pIdxBuffMeshImpact;  // �C���f�b�N�X�o�b�t�@�ւ̃|�C���^
	D3DXVECTOR3 pos;			//���b�V���C���p�N�g�̈ʒu
	D3DXVECTOR3 rot;			//���b�V���C���p�N�g�̌���
	D3DXMATRIX mtxWorld;		//���b�V���C���p�N�g�̃��[���h�}�g���b�N�X
	D3DXVECTOR3 move;			// �ړ���
	D3DXCOLOR col;              // �F
	int Vertex,Polygon,Index;	// ���_���A�|���S�����A�C���f�b�N�X��
	int nImpactPosX;            // �C���p�N�g��X�̕�����
	float OutRadius,InRadius;	// �O���̔��a�A�����̔��a
	int nType;					// �C���p�N�g�̎��
	float speed;				// �C���p�N�g�̑��x
	float decAlv;				// �C���p�N�g�̓������̑��x
	int nLife;                  // ����
	bool bUse;                  // �g�p���
	int nDamege;                // �U����
}MeshImpact;

//**************************************************************************************************************
//�v���g�^�C�v�錾
//**************************************************************************************************************
void InitMeshImpact(void);//���b�V���C���p�N�g�̏���������
void UninitMeshImpact(void);//���b�V���C���p�N�g�̏I������
void UpdateMeshImpact(void);//���b�V���C���p�N�g�̍X�V����
void DrawMeshImpact(void);//���b�V���C���p�N�g�̕`�揈��
void SetImpact(D3DXVECTOR3 pos, D3DXCOLOR col,int nImpactPosX,float OutRadius,float InRadius,float Speed,int nLife,int nType,int nDamege); // �C���p�N�g�̐ݒ菈��(Out > In)
void CreateImpact(int nCntImpact, int nImpactPosX, int Vertex, int Index);                                                     // �C���p�N�g�̍쐬����
bool CollisionImpact(D3DXVECTOR3 *pPos);
int ImpactDamege(int nID);
#endif

