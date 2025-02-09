//============================
//
// �u���b�N[block.h]
// Author:YOSHIDA YUUTO
//
//============================
#ifndef _BLOCK_H_
#define _BLOCK_H_

//***************************
// �C���N���[�h�t�@�C��
//***************************
#include"main.h"
#include"player.h"

//***************************
// �}�N����`
//***************************
#define MAX_TEX (32)
#define MAX_BLOCK (64)
#define INDXPARENT (5)

//***************************
// �u���b�N�̎��
//***************************
typedef enum
{
	BLOCKTYPE_APARTMENT = 0,
	BLOCKTYPE_BILL000,
	BLOCKTYPE_BILL001,
	BLOCKTYPE_BILL002,

	BLOCKTYPE_BANNER,
	BLOCKTYPE_BUS_STOP,
	BLOCKTYPE_DUMPSTER,
	BLOCKTYPE_HYDRANT,
	BLOCKTYPE_ORVIS,
	BLOCKTYPE_PHONEBOOTH,
	BLOCKTYPE_TOILE,
	BLOCKTYPE_PYLON000,
	BLOCKTYPE_PYLON001,
	BLOCKTYPE_PYLONBAR000,
	BLOCKTYPE_PYLONBAR001,
	BLOCKTYPE_VENDING_MACINE000,
	BLOCKTYPE_VENDING_MACINE001,
	BLOCKTYPE_VENDING_MACINE002,
	BLOCKTYPE_BILL003,

	BLOCKTYPE_TUTORIAL000,
	BLOCKTYPE_TUTORIAL100,
	BLOCKTYPE_TUTORIAL200,

	BLOCKTYPE_CAR000,
	BLOCKTYPE_CAR001,
	BLOCKTYPE_CAR002,
	BLOCKTYPE_CAR003,
	BLOCKTYPE_CAR004,

	BLOCKTYPE_VACANT_LAND,
	BLOCKTYPE_PUBLIC_TOILET2,
	BLOCKTYPE_HYDRAULIC_EXECAVATOR,
	BLOCKTYPE_WORKBENCH,
	BLOCKTYPE_MANHOLE,
	BLOCKTYPE_CONTAINER,

	BLOCKTYPE_MAX
}BLOCKTYPE;

//***************************
// �u���b�N�̏�Ԃ̎��
//***************************
typedef enum
{
	BLOCKSTATE_NORMAL=0,
	BLOCKSTATE_HOLD,
	BLOCKSTATE_THROW,
	BLOCKSTATE_MAX
}BLOCKSTATE;

//***************************
// �u���b�N�̃e�N�X�`���\����
//***************************
typedef struct
{
	LPD3DXMESH g_pMeshBlock;		// ���b�V��(���_���W)�ւ̃|�C���^
	LPD3DXBUFFER g_pBuffMatBlock;   // �}�e���A���ւ̃|�C���^
	DWORD g_dwNumMatBlock;          // �}�e���A���̐�
	LPDIRECT3DTEXTURE9 g_apTextureBlock[MAX_TEX]; // �e�N�X�`��
	D3DXVECTOR3 vtxMin, vtxMax;		// �ŏ��l,�ő�l
}BLOCKTEX;

//***************************
//�u���b�N�̍\����
//***************************
typedef struct
{
	D3DXVECTOR3 CenterPos; // ���S�_�̍��W
	D3DXVECTOR3 VecRot[3];   // ��]
	float Length[3];         // XYZ�̒���
	D3DXMATRIX ObbMtx;
}OBB;

//***************************
// �u���b�N�̍\����
//***************************
typedef struct
{
	D3DXVECTOR3 pos; // �ʒu
	D3DXVECTOR3 move;// �ړ�
	D3DXVECTOR3 rot; // ����
	D3DXVECTOR3 Scal;// �傫��
	D3DXVECTOR3 Size;// �T�C�Y
	D3DXVECTOR3 nor; // �@���x�N�g��

	D3DXMATRIX mtxWorldBlock;// ���[���h�}�g���b�N�X
	bool bUse;
	int nType;
	int nLife;
	BLOCKSTATE state; // �u���b�N�̏��
	float fRadius;
	BLOCKTEX BlockTex[BLOCKTYPE_MAX];
	D3DXMATRIX mtxParent;
	OBB Obb;
	int nIdxShadow;   // �e�̃C���f�b�N�X
}BLOCK;

//***************************
//�v���g�^�C�v�錾
//***************************
void InitBlock(void);			// �u���b�N�̏���������
void UninitBlock(void);			// �u���b�N�̏I������
void UpdateBlock(void);			// �u���b�N�̍X�V����
void DrawBlock(void);			// �u���b�N�̕`�揈��
void SetBlock(D3DXVECTOR3 pos, D3DXVECTOR3 rot,int nType, D3DXVECTOR3 Scal);	// �u���b�N�̐ݒ菈��
bool CollisionBlock(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld,D3DXVECTOR3 *pMove,D3DXVECTOR3 *pSize);	// �����蔻��
bool CollisionBlockItem(D3DXVECTOR3* pPos, D3DXVECTOR3* pPosOld, D3DXVECTOR3* pMove, D3DXVECTOR3* pSize);// �A�C�e���Ƃ̓����蔻��
int NumBlock(void);	// �u���b�N���̎擾
void LoadTitleState(void);	// �^�C�g���̃u���b�N�ǂݍ��ݏ���
BLOCK* GetBlock();	// �u���b�N���̎擾
void tutoload(void);// �`���[�g���A���ł̓ǂݍ��ݏ���
void CreateObb(int nCnt);
bool collisionObb(int nCnt);
bool collisionObbEne(int nCntBlock);

float LenSegOnSeparateAxis(D3DXVECTOR3* Sep, D3DXVECTOR3* e1, D3DXVECTOR3* e2, D3DXVECTOR3* e3);

#endif
