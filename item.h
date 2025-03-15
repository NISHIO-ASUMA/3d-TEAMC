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
#define MAX_ITEM (456)
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
}Item;

//***************************
//�v���g�^�C�v�錾
//***************************
void InitItem(void);
void UninitItem(void);
void UpdateItem(void);
void DrawItem(void);
void SetItem(D3DXVECTOR3 pos, int nType);
bool HitThrowItem(D3DXVECTOR3* pPos, float ItemRadius, float EnemyRadius);
void Itemchange(int nIdx,int nType);
Item* GetItem(void);
MODEL_INFO* GetItemOrigin(void);
ITEM_INFO* GetItemInfo(void);
void EnableCraftIcon(int Item1, int Item2);
void CraftMixItem(int HoldIdx, int StockIdx);
#endif
