//===================================================================================================================
//
// ���b�V���V�����_�[[meshcylinder.h]
// Author: YOSHIDA YUTO
//
//===================================================================================================================

#ifndef _MESHCYLINDER_H_
#define _MESHCYLINDER_H_

//**************************************************************************************************************
//�C���N���[�h�t�@�C��
//**************************************************************************************************************
#include"main.h"

//**************************************************************************************************************
//�}�N����`
//**************************************************************************************************************
#define CYLINDER_WIDTH (100.0f) //����
#define CYLINDER_HEIGHT (100.0f) //����

#define CYLINDER_X (8) //X�̒l
#define CYLINDER_Z (1) //y�̒l

#define CYLINDER_VERTEX ((CYLINDER_X + 1) * (CYLINDER_Z + 1)) //���_��
#define CYLINDER_POLYGON (((CYLINDER_X * CYLINDER_Z) * 2)) + (4 * (CYLINDER_Z - 1)) //�|���S����
#define CYLINDER_INDEX (CYLINDER_POLYGON + 2)  //�C���f�b�N�X

//**************************************************************************************************************
// ���b�V���V�����_�[�̎��
//**************************************************************************************************************
typedef enum
{
	CYLINDERTYPE_NORMAL = 0,
	CYLINDERTYPE_HEAL,
	CYLINDERTYPE_EVENT,
	CYLINDERTYPE_TERRITORY,
	CYLINDERTYPE_ITEM,
	CYLINDERTYPE_MAX
}CYLINDERTYPE;

//**************************************************************************************************************
// ���b�V���V�����_�[�̍\����
//**************************************************************************************************************
typedef struct
{
	LPDIRECT3DTEXTURE9 g_pTextureMeshCylinder;	    // �e�N�X�`���ւ̃|�C���^
	LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffMeshCylinder; // ���_�o�b�t�@�ւ̃|�C���^
	LPDIRECT3DINDEXBUFFER9 g_pIdxBuffMeshCylinder;  // �C���f�b�N�X�o�b�t�@�ւ̃|�C���^
	D3DXVECTOR3 pos;								// ���b�V���V�����_�[�̈ʒu
	D3DXVECTOR3 rot;								// ���b�V���V�����_�[�̌���
	D3DXMATRIX mtxWorld;							// ���b�V���V�����_�[�̃��[���h�}�g���b�N�X
	int nType;										// ���b�V���V�����_�[�̎��
	int nLife;										// ����
	float fRadius;									// ���a
	bool bUse;										// �g�p���
	D3DXCOLOR col;									// �F
	float Speed;									// ����
	int Vertex, polygon, Index;						// ���_���A�|���S���A�C���f�b�N�X
	int nNumPosX, nNumPosZ;							// ���_�̕�����
	float decAlv;                                   // ���l�̌����l
	float fWidth, fHeight;                          // �����@����
}MeshCylinder;

//**************************************************************************************************************
//�v���g�^�C�v�錾
//**************************************************************************************************************
void InitMeshCylinder(void);//���b�V���V�����_�[�̏���������
void UninitMeshCylinder(void);//���b�V���V�����_�[�̏I������
void UpdateMeshCylinder(void);//���b�V���V�����_�[�̍X�V����
void DrawMeshCylinder(void);//���b�V���V�����_�[�̕`�揈��
int SetMeshCylinder(D3DXVECTOR3 pos, int nType, int nLife, float fRadius, D3DXCOLOR col, int nNumPosX, int nNumPosZ, float speed, float fHeight); // ���b�V���V�����_�[�̐ݒ�
void CreateMeshCylinder(int nCntCylinder, int Vertex, int Index, int nNumPosX, int nNumPosZ, float fRadius, float fHeight);// ���b�V���V�����_�[�̍쐬����
void SetPotisionCylinder(int nIdx,D3DXVECTOR3 pos); // �V�����_�[�̈ʒu�ݒ菈��
void DeleteCylinder(int nIdx);
bool KeepInCylinder(D3DXVECTOR3 *pPos);
bool CollisionCylinder(int nIdx, D3DXVECTOR3* pPos);
#endif
