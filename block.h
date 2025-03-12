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

//***************************
// �}�N����`
//***************************
#define MAX_TEX (32)
#define MAX_BLOCK (450)
#define INDXPARENT (5)
#define TAGWORD (64)

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
	BLOCKTYPE_WOOD,
	BLOCKTYPE_ENEMYSPAWN000,
	BLOCKTYPE_ENEMYSPAWN001,
	BLOCKTYPE_ENEMYSPAWN002,
	BLOCKTYPE_GUARDRAIL,
	BLOCKTYPE_TRASHBOX,
	BLOCKTYPE_SHCOOL,

	BLOCKTYPE_GAMECENTER,
	BLOCKTYPE_SEESAW,
	BLOCKTYPE_SLIDE,
	BLOCKTYPE_SWING,
	BLOCKTYPE_SWING1,
	BLOCKTYPE_BURGERSHOP,
	BLOCKTYPE_BILLDINGSTRUCTURE,
	BLOCKTYPE_TUTORIAL300,
	BLOCKTYPE_WOOD001,
	BLOCKTYPE_WOOD002,
	BLOCKTYPE_MOUNT000,
	BLOCKTYPE_CURYY,
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
	LPD3DXMESH g_pMeshModel;		// ���b�V��(���_���W)�ւ̃|�C���^
	LPD3DXBUFFER g_pBuffMatModel;   // �}�e���A���ւ̃|�C���^
	DWORD g_dwNumMatModel;          // �}�e���A���̐�
	LPDIRECT3DTEXTURE9 g_apTextureModel[MAX_TEX]; // �e�N�X�`��
	D3DXVECTOR3 vtxMin, vtxMax;		// �ŏ��l,�ő�l
	D3DXVECTOR3 Size;
}MODEL_INFO;


//***************************
//�u���b�N�̍\����
//***************************
typedef struct
{
	D3DXVECTOR3 CenterPos; // ���S�_�̍��W
	D3DXVECTOR3 VecRot[3];   // ��]
	float Length[3];         // XYZ�̒���
	D3DXMATRIX ObbMtx;       // OBB�̃��[���h�}�g���b�N�X
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
	MODEL_INFO BlockTex[BLOCKTYPE_MAX];
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
void SetBlock(D3DXVECTOR3 pos, D3DXVECTOR3 rot,int nType);	// �u���b�N�̐ݒ菈��
bool CollisionBlock(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld,D3DXVECTOR3 *pMove,D3DXVECTOR3 *pSize);	// �����蔻��
bool CollisionBlockItem(D3DXVECTOR3* pPos, D3DXVECTOR3* pPosOld, D3DXVECTOR3* pMove, D3DXVECTOR3* pSize);// �A�C�e���Ƃ̓����蔻��
int NumBlock(void);	// �u���b�N���̎擾
void LoadTitleState(void);	// �^�C�g���̃u���b�N�ǂݍ��ݏ���
BLOCK* GetBlock();	// �u���b�N���̎擾
void tutoload(void);// �`���[�g���A���ł̓ǂݍ��ݏ���
void CreateObb(int nCnt);
bool collisionObb(int nCnt);
bool collisionObbEnemy(int nCntBlock);
bool collisionObbBoss(int nCntBlock);
float LenSegOnSeparateAxis(D3DXVECTOR3* Sep, D3DXVECTOR3* e1, D3DXVECTOR3* e2, D3DXVECTOR3* e3);
void CraftRange(BLOCK* pBlock);
BLOCK* GetBlockOrigin(void);
#endif
