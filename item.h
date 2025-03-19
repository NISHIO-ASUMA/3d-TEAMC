//==============================================================================================================
//
// �A�C�e�� [item.h]
// Author:YOSHIDA YUUTO
//
//==============================================================================================================

#ifndef _ITEM_H_
#define _ITEM_H_

//***************************
//�C���N���[�h�t�@�C��
//***************************
#include"main.h"
#include"player.h"
#include"block.h"

//***************************
//�}�N����`
//***************************
//#define MAX_TEX (32)
#define MAX_ITEM (512)
#define PARENT (5)

//***************************
//�u���b�N�̎��
//***************************
typedef enum
{
	ITEMTYPE_BAT = 0,
	ITEMTYPE_GOLF,
	ITEMTYPE_HUNMER,
	ITEMTYPE_STONE,
	ITEMTYPE_WOOD,
	ITEMTYPE_STONEBAT,
	ITEMTYPE_LIGHT,
	ITEMTYPE_LIGHTWOOD,
	ITEMTYPE_HARISEN,
	ITEMTYPE_ICEBLOCK,
	ITEMTYPE_ICEBLOCKSOWRD,
	ITEMTYPE_IRON,
	ITEMTYPE_IRONBAT,
	ITEMTYPE_SURFBOARD,
	ITEMTYPE_TORCH,
	ITEMTYPE_TORCHSWORD,
	ITEMTYPE_BAR,
	ITEMTYPE_HEADSTATUE,
	ITEMTYPE_HEADSTATUTORSO,
	ITEMTYPE_MEGAPHONE,
	ITEMTYPE_RUBBERCUP,
	ITEMTYPE_TELEPHONEPOLE,
	ITEMTYPE_TORSO,
	ITEMTYPE_FLUORESCENTLIGHTMEGAPHONE,
	ITEMTYPE_BONESPEAR,
	ITEMTYPE_FISH,
	ITEMTYPE_HEX,
	ITEMTYPE_HEXMANDOLIN,
	ITEMTYPE_SURFBOARDFISH,
	ITEMTYPE_TUTORIAL,
	ITEMTYPE_KATANA,
	ITEMTYPE_ONIGIRI,
	ITEMTYPE_SPEAR, // ���P��
	ITEMTYPE_MANDORIN, // �}���h����
	ITEMTYPE_BONE, // ���P��
	ITEMTYPE_GOLFHUNMER,
	ITEMTYPE_MAX
}ITEMTYPE;

//***************************
//�u���b�N�̏�Ԃ̎��
//***************************
typedef enum
{
	ITEMSTATE_NORMAL = 0,
	ITEMSTATE_HOLD,
	ITEMSTATE_THROW,
	ITEMSTATE_STOCK,
	ITEMSTATE_RELEASE,
	ITEMSTATE_MAX
}ItemSTATE;

//***************************
//�u���b�N�̑����̎��(���o��)
//***************************
typedef enum
{
	ITEMELEMENT_STANDARD = 0,
	ITEMELEMENT_BLOOD,
	ITEMELEMENT_FIRE,
	ITEMELEMENT_FREEZE,
	ITEMELEMENT_SPARK,
	ITEMELEMENT_AQUA,
	ITEMELEMENT_DARK,
	ITEMELEMENT_MAX
}ITEMELEMENT;

//***************************
// �A�C�e���̎�ނ̏��
//***************************
typedef struct
{
	int nElement;			// ����
	int nType;				// ���
	int durability;			// �ϋv��
	int Maxdurability;		// �ő�̑ϋv��
	char Itemtag[TAGWORD];  // �A�C�e���̃^�O
	int Power;              // �U����
}ITEM_INFO;

//***************************
//�u���b�N�̍\����
//***************************
typedef struct
{
	D3DXVECTOR3 pos,posOld;				//�ʒu
	D3DXVECTOR3 move;					//�ړ�
	D3DXVECTOR3 rot;					//����
	D3DXVECTOR3 Scal;					//�傫��
	D3DXMATRIX mtxWorldItem;			//���[���h�}�g���b�N�X
	D3DXVECTOR3 Size;
	bool bUse;
	int nType;
	int nLife;
	ItemSTATE state;					//�u���b�N�̏��
	int nElement;						//�A�C�e���̑���
	float fRadius;
	MODEL_INFO ItemTex[ITEMTYPE_MAX];
	D3DXMATRIX mtxParent;
	int durability;						// �ϋv��
	int nIdxShadow;						// �e�̃C���f�b�N�X
	bool EnableCraft;					// �N���t�g���L���ȃA�C�e�����ۂ�
	//bool bMixItem[ITEMTYPE_MAX];		// �����i�\���p�t���O
	float grabity;
	int nCounterState;					// ��ԃJ�E���^�[
	int Maxdurability;					// �ő�̑ϋv��
	int nIdxBillboardCount;
	int nEasingCnt;						// �C�[�W���O�̃J�E���g
	int nImpactCount;					// �C���p�N�g�̃J�E���g
	char Itemtag[TAGWORD];
	ITEM_INFO aItemInfo[ITEMTYPE_MAX];
	int Power;							// �U����
	int nMinimapIdx;					// �~�j�}�b�v�̃C���f�b�N�X
	bool bTracking;						// �v���C���[�ɂ��Ă�����
}Item;

//***************************
//�v���g�^�C�v�錾
//***************************
void InitItem(void);							// �A�C�e���̏���������
void UninitItem(void);							// �A�C�e���̏I������
void UpdateItem(void);							// �A�C�e���̍X�V����
void DrawItem(void);							// �A�C�e���̕`�揈��
void SetItem(D3DXVECTOR3 pos, int nType);		// �A�C�e���̐ݒ菈��
bool HitThrowItem(D3DXVECTOR3* pPos, float ItemRadius, float EnemyRadius); // �������A�C�e���Ƃ̓����蔻��
void Itemchange(int nIdx,int nType);			// �A�C�e���̕ύX����
Item* GetItem(void);							// �A�C�e���̎擾����
MODEL_INFO* GetItemOrigin(void);				// �A�C�e���̃��f���̎擾����
ITEM_INFO* GetItemInfo(void);					// �A�C�e���̏��擾����
void EnableCraftIcon(int Item1, int Item2);		// �A�C�e���̃A�C�R���\������
void CraftMixItem(int HoldIdx, int StockIdx);   // �A�C�e���̃N���t�g����
void SpawonItem(D3DXVECTOR3 pos, int nType);    // �A�C�e���̃X�|�[������
void SetFirstCraftItem(D3DXVECTOR3 pos, int nType);  // �ŏ��ɃN���t�g������A�C�e���̐ݒ菈��
bool CheckMixItemMat(int pCraftMat, int pStockMat, int HoldIdx, int StockIdx);		// �A�C�e�����N���t�g�ł��邩�ǂ���
#endif
