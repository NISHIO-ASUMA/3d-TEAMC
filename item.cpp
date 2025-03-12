//==============================================================================================================
//
// �A�C�e�� [item.cpp]
// Author:YOSHIDA YUUTO
//
//==============================================================================================================

//**************************************************************************************************************
//�C���N���[�h�t�@�C��
//**************************************************************************************************************
#include "input.h"
#include "mouse.h"
#include "camera.h"
#include "item.h"
#include "block.h"
#include "player.h"
#include "explosion.h"
#include "HPGauge.h"
#include "Shadow.h"
#include "Particle.h"
#include "icon.h"
#include "craftui.h"
#include "sound.h"
#include "Effect.h"
#include "billboard.h"
#include "math.h"
#include "easing.h"
#include "meshimpact.h"

//**************************************************************************************************************
//�}�N����`
//**************************************************************************************************************
#define MAX_WORD (256)
#define MAX_DURABILITY (100) // �ϋv��

//**************************************************************************************************************
//�v���g�^�C�v�錾
//**************************************************************************************************************
void LoadItemModel(void); // �A�C�e���̃��[�h����
void CraftItem(void);
void CraftMixItem(int nCntItem,int MixItem,int motionchange);
void EnableCraftIcon(int nCntItem, int Item1, int Item2, int MixItem);
void LoadItemInfo(void); // �A�C�e���̏��̃��[�h����
void PickUpItemAnimation(int nCntItem); // �A�C�e�����E���鎞�̉��o
bool CheckMixItemMat(int nCntItem,int HoldItem,int StockItem,int HoldIdx);			// �A�C�e�����N���t�g�ł��邩�ǂ���
void UpdateCraftItemParam(int nCnt);                                                // �N���t�g�A�C�e���̃p�����[�^�ݒ�

//**************************************************************************************************************
//�O���[�o���ϐ��錾
//**************************************************************************************************************
Item g_Item[MAX_ITEM];					// �\���̕ϐ�
int g_ItemTypeMax;						// ��ސ�
MODEL_INFO g_TexItem[ITEMTYPE_MAX];		// �e�N�X�`���֌W
ITEM_INFO g_aItemInfo[ITEMTYPE_MAX];	// �A�C�e���̏��
bool bFIrstCraftItem = false;

//===============================================================================================================
//�u���b�N�̏���������
//===============================================================================================================
void InitItem(void)
{
	// �f�o�C�X�̃|�C���^
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	for (int nCntItem = 0; nCntItem < MAX_ITEM; nCntItem++)
	{
		g_Item[nCntItem].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);  // ���W
		g_Item[nCntItem].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f); // �ړ���
		g_Item[nCntItem].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);  // �p�x
		g_Item[nCntItem].Scal = D3DXVECTOR3(1.0f, 1.0f, 1.0f); // �g�嗦
		g_Item[nCntItem].nType = ITEMTYPE_BAT;				   // ���
		g_Item[nCntItem].nElement = ITEMELEMENT_STANDARD;	   // ���
		g_Item[nCntItem].bUse = false;						   // ���g�p����
		g_Item[nCntItem].nLife = 120;						   // �̗�
		g_Item[nCntItem].state = ITEMSTATE_NORMAL;			   // ���
		g_Item[nCntItem].fRadius = 100.0f;					   // ���a
		g_Item[nCntItem].durability = 0;					   // �ϋv��
		g_Item[nCntItem].EnableCraft = false;				   // �N���t�g�ł��邩�ۂ�
		g_Item[nCntItem].grabity = 0.0f;					   // �N���t�g�ł��邩�ۂ�
		g_Item[nCntItem].nEasingCnt = 0;					   // �C�[�W���O�̃J�E���g
		g_Item[nCntItem].nImpactCount = 110;				   // �Ռ��g�̃J�E���g
		g_Item[nCntItem].Maxdurability = 0;					   // �ő�̑ϋv��
		g_Item[nCntItem].Itemtag[0] = {};					   // �^�O
		g_Item[nCntItem].Power = 0;							   // �U����

		// �A�C�e���̎�ޕ�
		for (int nCntNum = 0; nCntNum < ITEMTYPE_MAX; nCntNum++)
		{
			g_Item[nCntItem].bMixItem[nCntNum] = false;				   // �N���t�g��̃A�C�e���\���p�t���O
		}
	}

	LoadItemInfo();  // �A�C�e���̏��
	LoadItemModel(); // �A�C�e���̃��[�h����

	bFIrstCraftItem = false;

	for (int nCntNum = 0; nCntNum < g_ItemTypeMax; nCntNum++)
	{
		g_aItemInfo[nCntNum].nType = nCntNum;			            // �ԍ�
		g_aItemInfo[nCntNum].Maxdurability = 0;     // ������
		g_aItemInfo[nCntNum].Maxdurability = g_aItemInfo[nCntNum].durability;

		D3DXMATERIAL* pMat; // �}�e���A���ւ̃|�C���^

		if (g_TexItem[nCntNum].g_pBuffMatModel == NULL)
		{
			continue;
		}

		// �}�e���A���̃f�[�^�ւ̃|�C���^���擾
		pMat = (D3DXMATERIAL*)g_TexItem[nCntNum].g_pBuffMatModel->GetBufferPointer();

		for (int nCntMat = 0; nCntMat < (int)g_TexItem[nCntNum].g_dwNumMatModel; nCntMat++)
		{
			if (pMat[nCntMat].pTextureFilename != NULL)
			{
				//���̃t�@�C�������g�p���ăe�N�X�`����ǂݍ���
				//�e�N�X�`���̓ǂݍ���
				D3DXCreateTextureFromFile(pDevice,
					pMat[nCntMat].pTextureFilename,
					&g_TexItem[nCntNum].g_apTextureModel[nCntMat]);
			}
		}
	}

	// ���_���W�̎擾
	int nNumVtx;	// ���_��
	DWORD sizeFVF;  // ���_�t�H�[�}�b�g�̃T�C�Y
	BYTE* pVtxBuff; // ���_�o�b�t�@�ւ̃|�C���^

	for (int nCntNum = 0; nCntNum < g_ItemTypeMax; nCntNum++)
	{
		// ���_���̎擾
		nNumVtx = g_TexItem[nCntNum].g_pMeshModel->GetNumVertices();

		// ���_�t�H�[�}�b�g�̃T�C�Y�擾
		sizeFVF = D3DXGetFVFVertexSize(g_TexItem[nCntNum].g_pMeshModel->GetFVF());

		// ���_�o�b�t�@�̃��b�N
		g_TexItem[nCntNum].g_pMeshModel->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

		for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
		{
			// ���_���W�̑��
			D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVtxBuff;

			// ���_���W���r���ău���b�N�̍ŏ��l,�ő�l���擾
			if (vtx.x < g_TexItem[nCntNum].vtxMin.x)
			{
				g_TexItem[nCntNum].vtxMin.x = vtx.x;
			}
			else if (vtx.y < g_TexItem[nCntNum].vtxMin.y)
			{
				g_TexItem[nCntNum].vtxMin.y = vtx.y;
			}
			else if (vtx.z < g_TexItem[nCntNum].vtxMin.z)
			{
				g_TexItem[nCntNum].vtxMin.z = vtx.z;
			}
			else if (vtx.x > g_TexItem[nCntNum].vtxMax.x)
			{
				g_TexItem[nCntNum].vtxMax.x = vtx.x;
			}
			else if (vtx.y > g_TexItem[nCntNum].vtxMax.y)
			{
				g_TexItem[nCntNum].vtxMax.y = vtx.y;
			}
			else if (vtx.z > g_TexItem[nCntNum].vtxMax.z)
			{
				g_TexItem[nCntNum].vtxMax.z = vtx.z;
			}

			// ���_�t�H�[�}�b�g�̃T�C�Y���|�C���^��i�߂�
			pVtxBuff += sizeFVF;

			// �T�C�Y����
			g_TexItem[nCntNum].Size.x = g_TexItem[nCntNum].vtxMax.x - g_TexItem[nCntNum].vtxMin.x;
			g_TexItem[nCntNum].Size.y = g_TexItem[nCntNum].vtxMax.y - g_TexItem[nCntNum].vtxMin.y;
			g_TexItem[nCntNum].Size.z = g_TexItem[nCntNum].vtxMax.z - g_TexItem[nCntNum].vtxMin.z;
		}

		// ���_�o�b�t�@�̃A�����b�N
		g_TexItem[nCntNum].g_pMeshModel->UnlockVertexBuffer();
	}

	Player* pPlayer = GetPlayer();
}
//===============================================================================================================
//�u���b�N�̏I������
//===============================================================================================================
void UninitItem(void)
{
	// ���y���~
	StopSound();

	// ��ޕ���
	for (int nCntNum = 0; nCntNum < g_ItemTypeMax; nCntNum++)
	{
		// �e�N�X�`���̔j��
		for (int nCntTex = 0; nCntTex < MAX_TEX; nCntTex++)
		{
			if (g_TexItem[nCntNum].g_apTextureModel[nCntTex] != NULL)
			{
				g_TexItem[nCntNum].g_apTextureModel[nCntTex]->Release();
				g_TexItem[nCntNum].g_apTextureModel[nCntTex] = NULL;
			}
		}

		// ���b�V���̔j��
		if (g_TexItem[nCntNum].g_pMeshModel != NULL)
		{
			g_TexItem[nCntNum].g_pMeshModel->Release();
			g_TexItem[nCntNum].g_pMeshModel = NULL;
		}

		// �}�e���A���̔j��
		if (g_TexItem[nCntNum].g_pBuffMatModel != NULL)
		{
			g_TexItem[nCntNum].g_pBuffMatModel->Release();
			g_TexItem[nCntNum].g_pBuffMatModel = NULL;
		}
	}

	for (int nCntItem = 0; nCntItem < MAX_ITEM; nCntItem++)
	{
		int nType = g_Item[nCntItem].nType;

		// �e�N�X�`���̔j��
		for (int nCntTex = 0; nCntTex < MAX_TEX; nCntTex++)
		{
			if (g_Item[nCntItem].ItemTex[nType].g_apTextureModel[nCntTex] != NULL)
			{
				g_Item[nCntItem].ItemTex[nType].g_apTextureModel[nCntTex] = NULL;
			}
		}

		// ���b�V���̔j��
		if (g_Item[nCntItem].ItemTex[nType].g_pMeshModel != NULL)
		{
			g_Item[nCntItem].ItemTex[nType].g_pMeshModel = NULL;
		}

		// �}�e���A���̔j��
		if (g_Item[nCntItem].ItemTex[nType].g_pBuffMatModel != NULL)
		{
			g_Item[nCntItem].ItemTex[nType].g_pBuffMatModel = NULL;
		}
	}
}
//===============================================================================================================
//�u���b�N�̍X�V����
//===============================================================================================================
void UpdateItem(void)
{
	// �v���C���[�̎擾
	Player* pPlayer = GetPlayer();

	// �����Ă���A�C�e���̏������������邩����
	bool bTypeChange = false;

	for (int nCntItem = 0; nCntItem < MAX_ITEM; nCntItem++)
	{
		// �z�[���h�̃A�C�e������������
		if (g_Item[nCntItem].state == ITEMSTATE_HOLD)
		{
			// �����Ă���A�C�e���̎�ނ���
			pPlayer->HoldItemType = g_Item[nCntItem].nType;

			// �ύX����
			bTypeChange = true;
		}

		// �X�g�b�N�̃A�C�e������������
		if (g_Item[nCntItem].state == ITEMSTATE_STOCK)
		{
			// �X�g�b�N���Ă���A�C�e���̎�ނ���
			pPlayer->StockItemType = g_Item[nCntItem].nType;

			// �ύX����
			bTypeChange = true;
		}

		// �ύX�Ȃ��Ȃ�
		if (bTypeChange == false)
		{
			// ���݂��Ȃ���ނ���
			pPlayer->StockItemType = ITEMTYPE_NONEXISTENT;

			// ���݂��Ȃ���ނ���
			pPlayer->HoldItemType = ITEMTYPE_NONEXISTENT;
		}

		// �ϋv�͂�0�ɂȂ�����
		if (g_Item[nCntItem].durability <= 0 && g_Item[nCntItem].state == ITEMSTATE_HOLD)
		{
			int nType = g_Item[nCntItem].nType;

			// ���y�Đ�
			PlaySound(SPUND_LABEL_WEPONBREAK);

			// �A�C�e������
			pPlayer->Itembreak[nCntItem] = true;
			g_Item[nCntItem].bUse = false; // ����
		}

		// �v���C���[�����̂������Ă��邩�U�����[�V�����̃L�[��3�ɂȂ�����
		if (!pPlayer->AttackSp&&pPlayer->HandState == PLAYERHOLD_HOLD && pPlayer->Motion.nKey == 3 && pPlayer->Motion.motionType == MOTIONTYPE_ACTION)
		{
			ThrowItem();
		}
		if (!g_Item[nCntItem].bUse)
		{//�g�p������Ȃ�������
			// ������ǂݔ�΂�
			continue;
		}

		// ��Ԃ̑J��
		switch (g_Item[nCntItem].state)
		{
		case ITEMSTATE_NORMAL:
			break;
		case ITEMSTATE_HOLD:
			break;
		case ITEMSTATE_THROW:
			break;
		case ITEMSTATE_STOCK:
			break;
		case ITEMSTATE_RELEASE:
			g_Item[nCntItem].nCounterState--;
			if (g_Item[nCntItem].nCounterState <= 0)
			{
				g_Item[nCntItem].state = ITEMSTATE_NORMAL;
			}
			break;
		default:
			break;
		}
		// �v���C���[�̃A�C�e���𓁂ɂ���
		if (bFIrstCraftItem == false && g_Item[nCntItem].nType == ITEMTYPE_KATANA)
		{
			// �A�C�e���ύX
			Itemchange(nCntItem,g_Item[nCntItem].nType);

			// ���[�V�����̕ύX
			MotionChange(MOTION_KATANA, 0);

			// �A�C�e���̃C���f�b�N�X��ۑ�
			pPlayer->ItemIdx = nCntItem;
			pPlayer->StockItemIdx = nCntItem;

			g_Item[nCntItem].state = ITEMSTATE_HOLD;

			// �X�e�[�^�X�ύX
			StatusChange(3.1f, D3DXVECTOR3(0.0f, g_Item[nCntItem].Size.y, 0.0f), 100);

			// �ŏ��ɐ؂�ւ���
			bFIrstCraftItem = true;
		}

		// ������ꂽ�A�C�e���ɃG�t�F�N�g
		if (g_Item[nCntItem].state == ITEMSTATE_THROW)
		{
			// �A�C�e���̑����𔽉f��
			int nType = g_Item[nCntItem].nType;
			g_Item[nCntItem].nElement = g_aItemInfo[nType].nElement;
			if (g_Item[nCntItem].nElement == ITEMELEMENT_STANDARD)// �������Ȃ�
			{
				// �G�t�F�N�g���o��
				SetEffect(g_Item[nCntItem].pos, g_Item[nCntItem].rot, 20, D3DXCOLOR(0.0f, 1.0f, 1.0f, 1.0f), 0.0f, g_Item[nCntItem].Size.y);
				// �p�[�e�B�N�����o���A�ȉ��S�ē���
				SetParticle(g_Item[nCntItem].pos,
					g_Item[nCntItem].rot,
					D3DXVECTOR3(1.0f, 1.0f, 1.0f),
					D3DXVECTOR3(0.0f, 0.0f, 0.0f),
					D3DXCOLOR(0.0f, 1.0f, 1.0f, 1.0f),
					2.0f, 2, 20, 7, 3.0f, 10.0f,
					false, D3DXVECTOR3(0.0f, 0.0f, 0.0f));
			}
			else if(g_Item[nCntItem].nElement == ITEMELEMENT_BLOOD)// �o������
			{
				// �G�t�F�N�g���o��
				SetEffect(g_Item[nCntItem].pos, g_Item[nCntItem].rot, 20, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), 0.0f, g_Item[nCntItem].Size.y);
				SetParticle(g_Item[nCntItem].pos,
					g_Item[nCntItem].rot,
					D3DXVECTOR3(1.0f, 1.0f, 1.0f),
					D3DXVECTOR3(0.0f, 0.0f, 0.0f),
					D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f),
					3.0f, 2, 20, 7, 3.0f, 10.0f,
					false, D3DXVECTOR3(0.0f, 0.0f, 0.0f));
			}
			else if (g_Item[nCntItem].nElement == ITEMELEMENT_FIRE)// ������
			{
				// �G�t�F�N�g���o��
				SetEffect(g_Item[nCntItem].pos, g_Item[nCntItem].rot, 20, D3DXCOLOR(1.0f, 0.5f, 0.0f, 1.0f), 0.0f, g_Item[nCntItem].Size.y);

				SetParticle(g_Item[nCntItem].pos,
					g_Item[nCntItem].rot,
					D3DXVECTOR3(1.0f, 1.0f, 1.0f),
					D3DXVECTOR3(0.0f, 0.0f, 0.0f),
					D3DXCOLOR(1.0f, 0.5f, 0.0f, 1.0f),
					4.0f, 2, 20, 7, 3.0f, 20.0f,
					true, D3DXVECTOR3(0.0f, 4.0f, 0.0f));

				SetParticle(g_Item[nCntItem].pos,
					g_Item[nCntItem].rot,
					D3DXVECTOR3(1.0f, 1.0f, 1.0f),
					D3DXVECTOR3(0.0f, 0.0f, 0.0f),
					D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f),
					4.0f, 2, 20, 7, 3.0f, 20.0f,
					true, D3DXVECTOR3(0.0f, 4.0f, 0.0f));
			}
			else if (g_Item[nCntItem].nElement == ITEMELEMENT_FREEZE)// �X����
			{
				// �G�t�F�N�g���o��
				SetEffect(g_Item[nCntItem].pos, g_Item[nCntItem].rot, 20, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 0.0f, g_Item[nCntItem].Size.y);
				SetParticle(g_Item[nCntItem].pos,
					g_Item[nCntItem].rot,
					D3DXVECTOR3(1.0f, 1.0f, 1.0f),
					D3DXVECTOR3(0.0f, 0.0f, 0.0f),
					D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f),
					2.0f, 2, 50, 7, 0.5f, 10.0f,
					false, D3DXVECTOR3(0.0f, 0.0f, 0.0f));
			}
			else if (g_Item[nCntItem].nElement == ITEMELEMENT_SPARK)// ��������
			{
				// �G�t�F�N�g���o��
				SetEffect(g_Item[nCntItem].pos, g_Item[nCntItem].rot, 20, D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f), 0.0f, g_Item[nCntItem].Size.y);
				SetParticle(g_Item[nCntItem].pos,
					g_Item[nCntItem].rot,
					D3DXVECTOR3(2.0f, 2.0f, 2.0f),
					D3DXVECTOR3(0.0f, 0.0f, 0.0f),
					D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f),
					1.0f, 2, 10, 20, 9.0f, 10.0f,
					false, D3DXVECTOR3(0.0f, 0.0f, 0.0f));
			}
			else if (g_Item[nCntItem].nElement == ITEMELEMENT_AQUA)// ������
			{
				// �G�t�F�N�g���o��
				SetEffect(g_Item[nCntItem].pos, g_Item[nCntItem].rot, 20, D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f), 0.0f, g_Item[nCntItem].Size.y);
				SetParticle(g_Item[nCntItem].pos,
					g_Item[nCntItem].rot,
					D3DXVECTOR3(1.0f, 1.0f, 1.0f),
					D3DXVECTOR3(0.0f, 0.0f, 0.0f),
					D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f),
					5.0f, 2, 20, 7, 3.0f, 20.0f,
					true, D3DXVECTOR3(0.0f, -4.0f, 0.0f));
			}
			else if (g_Item[nCntItem].nElement == ITEMELEMENT_DARK)// �ő���
			{
				// �G�t�F�N�g���o��
				SetEffect(g_Item[nCntItem].pos, g_Item[nCntItem].rot, 20, D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f), 0.0f, g_Item[nCntItem].Size.y);
				SetParticle(g_Item[nCntItem].pos,
					g_Item[nCntItem].rot,
					D3DXVECTOR3(1.0f, 1.0f, 1.0f),
					D3DXVECTOR3(0.0f, 0.0f, 0.0f),
					D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f),
					5.0f, 2, 35, 7, 3.0f, 7.0f,
					false, D3DXVECTOR3(0.0f, 0.0f, 0.0f));
			}
		}

		// �O��̈ʒu����
		g_Item[nCntItem].posOld = g_Item[nCntItem].pos;

		// �ʒu�̍X�V
		g_Item[nCntItem].pos += g_Item[nCntItem].move;

		if (CollisionBlockItem(&g_Item[nCntItem].pos, &g_Item[nCntItem].posOld, &g_Item[nCntItem].move, &g_Item[nCntItem].Size))
		{
			g_Item[nCntItem].bUse = false;
			SetExplosion(g_Item[nCntItem].pos, D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f), 60, 30.0f, 30.0f, EXPLOSION_HIT);
		}

		if (g_Item[nCntItem].state == ITEMSTATE_NORMAL || g_Item[nCntItem].state == ITEMSTATE_RELEASE)
		{
			// �A�C�e�����E����͈͂ɓ�����
			if (CollisionItem(nCntItem, 30.0f, 30.0f) == true)
			{
				// �E����A�C�e���̍X�V
				PickUpItemAnimation(nCntItem);
			}
			else
			{
				g_Item[nCntItem].nEasingCnt = 0;
				g_Item[nCntItem].pos.y += SetSmoothAprroach(0.0f, g_Item[nCntItem].pos.y, 0.1f);
				g_Item[nCntItem].rot.x += SetSmoothAprroach(0.0f, g_Item[nCntItem].rot.x, 0.1f);;
				g_Item[nCntItem].rot.y += SetSmoothAprroach(0.0f, g_Item[nCntItem].rot.y, 0.1f);;
			}
		}

		//// �p�x�̐��K��X
		//if (g_Item[nCntItem].rot.x > D3DX_PI)
		//{
		//	g_Item[nCntItem].rot.x += -D3DX_PI * 2.0f;
		//}
		//else if (g_Item[nCntItem].rot.x < D3DX_PI)
		//{
		//	g_Item[nCntItem].rot.x += D3DX_PI * 2.0f;
		//}

		//// �p�x�̐��K��Y
		//if (g_Item[nCntItem].rot.y > D3DX_PI)
		//{
		//	g_Item[nCntItem].rot.y += -D3DX_PI * 2.0f;
		//}
		//else if (g_Item[nCntItem].rot.y < D3DX_PI)
		//{
		//	g_Item[nCntItem].rot.y += D3DX_PI * 2.0f;
		//}

		// �v���C���[���N���t�g��Ԃ�������
		if (pPlayer->bCraft == true)
		{
			CraftItem();
		}

		// ��Ԃ𓊂���ɂ���
		if (g_Item[nCntItem].state == ITEMSTATE_THROW)
		{
			g_Item[nCntItem].nLife--; // �f�N�������g

			if (g_Item[nCntItem].nLife <= 0)
			{
				g_Item[nCntItem].bUse = false; // ����
			}
		}
	}

}
//===============================================================================================================
//�u���b�N�̕`�揈��
//===============================================================================================================
void DrawItem(void)
{
	// �f�o�C�X�̃|�C���^
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// ���݂̃��[�h���擾
	MODE mode = GetMode();

	// �v���C���[�̎擾
	Player* pPlayer = GetPlayer();

	// �v�Z�p�̃}�g���b�N�X
	D3DXMATRIX mtxRot, mtxTrans, mtxScal, mtxParent;

	D3DMATERIAL9 matDef; // ���݂̃}�e���A���ۑ��p

	D3DXMATERIAL* pMat; // �}�e���A���f�[�^�ւ̃|�C���^

	for (int nCntItem = 0; nCntItem < MAX_ITEM; nCntItem++)
	{
		if (!g_Item[nCntItem].bUse)
		{//���g�p��ԂȂ�
			//���̏�����ʂ����J�E���g��i�߂�
			continue;
		}

		int nType = g_Item[nCntItem].nType;

		// ���[���h�}�g���b�N�X�̏�����
		D3DXMatrixIdentity(&g_Item[nCntItem].mtxWorldItem);

		// �����𔽉f
		D3DXMatrixRotationYawPitchRoll(&mtxRot, g_Item[nCntItem].rot.y, g_Item[nCntItem].rot.x, g_Item[nCntItem].rot.z);
		D3DXMatrixMultiply(&g_Item[nCntItem].mtxWorldItem, &g_Item[nCntItem].mtxWorldItem, &mtxRot);

		// �ʒu�𔽉f
		D3DXMatrixTranslation(&mtxTrans, g_Item[nCntItem].pos.x, g_Item[nCntItem].pos.y, g_Item[nCntItem].pos.z);
		D3DXMatrixMultiply(&g_Item[nCntItem].mtxWorldItem, &g_Item[nCntItem].mtxWorldItem, &mtxTrans);

		// ���[���h�}�g���b�N�X�̐ݒ�
		pDevice->SetTransform(D3DTS_WORLD, &g_Item[nCntItem].mtxWorldItem);
			
		// ���݂̃}�e���A�����擾
		pDevice->GetMaterial(&matDef);

		for (int nCntMat = 0; nCntMat < (int)g_Item[nCntItem].ItemTex[nType].g_dwNumMatModel; nCntMat++)
		{
			// �}�e���A���̃f�[�^�ւ̃|�C���^���擾
			pMat = (D3DXMATERIAL*)g_Item[nCntItem].ItemTex[nType].g_pBuffMatModel->GetBufferPointer();

			// �}�e���A���̐ݒ�
			pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

			// �e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_Item[nCntItem].ItemTex[nType].g_apTextureModel[nCntMat]);

			// �u���b�N(�p�[�c)�̕`��
			g_Item[nCntItem].ItemTex[nType].g_pMeshModel->DrawSubset(nCntMat);
		}
	}
	
}
//=========================================================================================================
//�u���b�N�̐ݒ菈��
//=========================================================================================================
void SetItem(D3DXVECTOR3 pos, int nType)
{
	for (int nCntItem = 0; nCntItem < MAX_ITEM; nCntItem++)
	{
		if (!g_Item[nCntItem].bUse)
		{// ���g�p��ԂȂ�

			g_Item[nCntItem].ItemTex[nType] = g_TexItem[nType]; // �K�v�ȏ�����
			g_Item[nCntItem].pos = pos;			 // ���W
			g_Item[nCntItem].nType = nType;		 // ���
			g_Item[nCntItem].bUse = true;		 // �g�p����

			g_Item[nCntItem].nIdxBillboardCount = SetBillboard(D3DXVECTOR3(pos.x,pos.y + 100.0f,pos.z), BILLBOARDTYPE_SECOND, 40.0f, 20.0f, BILLBOARDSTATE_NOSET);
			break;
		}
	}
}
//=========================================================================================================
//�A�C�e���̓����蔻��
//=========================================================================================================
bool HitThrowItem(D3DXVECTOR3* pPos, float ItemRadius, float EnemyRadius)
{
	// �v���C���[�̎擾
	Player* pPlayer = GetPlayer();

	bool bHit = false;  // �����������ǂ���

	D3DXVECTOR3 DisPos; // �����v�Z�p�ϐ�

	for (int nCnt = 0; nCnt < MAX_ITEM; nCnt++)
	{
		if (g_Item[nCnt].state == ITEMSTATE_THROW && g_Item[nCnt].bUse)
		{// ITEMSTATE_THROW�@���� bUse��true

			// ���W��ݒ�
			DisPos.x = pPos->x - g_Item[nCnt].pos.x;
			DisPos.y = pPos->y - g_Item[nCnt].pos.y;
			DisPos.z = pPos->z - g_Item[nCnt].pos.z;

			// �������v�Z
			float fDistance = (DisPos.x * DisPos.x) + (DisPos.y * DisPos.y) + (DisPos.z * DisPos.z);

			// ���a���v�Z
			float fRadius = ItemRadius + EnemyRadius;

			fRadius = fRadius * fRadius;

			if (fDistance <= fRadius)
			{// fDistance��fRadius��菬����

				bHit = true;	// �����L����

				if (!pPlayer->FeverMode)
				{
					g_Item[nCnt].durability--; // �ϋv�͂��ւ炷
				}
				break;
			}
		}
	}
	// �����Ԃ�
	return bHit;
}
//===========================================================================================================
//�A�C�e���̃e�N�X�`���擾
//===========================================================================================================
void Itemchange(int nIdx, int nType)
{
	// �v���C���[�̎擾
	Player* pPlayer = GetPlayer();

	pPlayer->Motion.aModel[15].dwNumMat = g_TexItem[nType].g_dwNumMatModel; // �A�C�e���̃}�e���A���̏�����
	pPlayer->Motion.aModel[15].pBuffMat = g_TexItem[nType].g_pBuffMatModel; // �A�C�e���̃o�b�t�@�̏�����
	pPlayer->Motion.aModel[15].pMesh = g_TexItem[nType].g_pMeshModel;       // �A�C�e���̃��b�V���̏�����
	pPlayer->nElement = g_aItemInfo[nType].nElement; // �A�C�e���̑���������

	// �����C���f�b�N�X����Ȃ����ϋv�l�������Ă��Ȃ��Ȃ�
	if (nIdx != pPlayer->ItemIdx && g_Item[nIdx].durability == g_Item[nIdx].Maxdurability)
	{
		// �ϋv�͂���
		g_Item[nIdx].durability = g_aItemInfo[nType].durability;
		g_Item[nIdx].Maxdurability = g_aItemInfo[nType].Maxdurability;
	}
	// �傫������
	g_Item[nIdx].Size = g_TexItem[nType].Size;
}
//=========================================================================================================
//�A�C�e���̎擾
//=========================================================================================================
Item* GetItem(void)
{
	return &g_Item[0];
}
//==============================================================================================================
// �A�C�e���̃��[�h����
//==============================================================================================================
void LoadItemModel(void)
{
	// �f�o�C�X�̃|�C���^
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// �t�@�C���|�C���^��錾
	FILE* pFile;

	// ���[�J���ϐ��錾-----------
	char skip[5];
	int nType = 0;
	//----------------------------

	// �t�@�C�����J��
	pFile = fopen("data\\MODEL_TXT\\ITEM.txt", "r");

	if (pFile != NULL)
	{
		char aString[MAX_WORD];

		while (1)
		{
			fscanf(pFile, "%s", &aString[0]);

			if (strcmp(aString, "MAX_TYPE") == 0)
			{// MAX_TYPE��ǂݎ����
				fscanf(pFile, "%s", &skip[0]);
				fscanf(pFile, "%d", &g_ItemTypeMax);
			}
			else if (strcmp(aString, "MODEL_FILENAME") == 0)
			{// MODEL_FILENAME��ǂݎ����
				fscanf(pFile, "%s", &skip[0]);

				fscanf(pFile, "%s", &aString[0]);

				const char* MODEL_FILENAME = {};

				MODEL_FILENAME = aString;

				//X�t�@�C���̓ǂݍ���
				D3DXLoadMeshFromX(MODEL_FILENAME,
					D3DXMESH_SYSTEMMEM,
					pDevice,
					NULL,
					&g_TexItem[nType].g_pBuffMatModel,
					NULL,
					&g_TexItem[nType].g_dwNumMatModel,
					&g_TexItem[nType].g_pMeshModel);

				nType++;
			}
			else if (strcmp(aString, "END_SCRIPT") == 0)
			{// END_SCRIPT��ǂݎ����
				break;
			}
		}
	}
	else
	{
		//���b�Z�[�W�{�b�N�X
		MessageBox(NULL, "�t�@�C�����J���܂���B", "�G���[(item.cpp)", MB_OK);
		return;
	}
	// �t�@�C�������
	fclose(pFile);
}
//==============================================================================================================
// �A�C�e���̎擾
//==============================================================================================================
MODEL_INFO* GetItemOrigin(void)
{
	return &g_TexItem[0];
}
//==============================================================================================================
// �A�C�e���̏��̎擾
//==============================================================================================================
ITEM_INFO* GetItemInfo(void)
{
	return &g_aItemInfo[0];
}
//==============================================================================================================
// �A�C�e���̃N���t�g
//==============================================================================================================
void CraftItem(void)
{
	Player* pPlayer = GetPlayer();
	Craftui* pMix = GetMixUI();

	// �L�[����������
	for (int nCnt = 0; nCnt < MAX_ITEM; nCnt++)
	{
		// �N���t�g�̃p�����[�^�[�̐ݒ菈��
		UpdateCraftItemParam(nCnt);

		// �N���t�g�A�C�R����\�����邩���Ȃ���
		EnableCraftIcon(nCnt, ITEMTYPE_STONE, ITEMTYPE_BAT, ITEMTYPE_STONEBAT);
		EnableCraftIcon(nCnt, ITEMTYPE_ICEBLOCK, ITEMTYPE_KATANA, ITEMTYPE_ICEBLOCKSOWRD);
		EnableCraftIcon(nCnt, ITEMTYPE_TORCH, ITEMTYPE_KATANA, ITEMTYPE_TORCHSWORD);
		EnableCraftIcon(nCnt, ITEMTYPE_KATANA, ITEMTYPE_LIGHT, ITEMTYPE_LIGHTWOOD);
		EnableCraftIcon(nCnt, ITEMTYPE_BAT, ITEMTYPE_IRON, ITEMTYPE_IRONBAT);
		EnableCraftIcon(nCnt, ITEMTYPE_HEADSTATUE, ITEMTYPE_TORSO, ITEMTYPE_HEADSTATUTORSO);
		EnableCraftIcon(nCnt, ITEMTYPE_FISH, ITEMTYPE_SURFBOARD, ITEMTYPE_SURFBOARDFISH);
		EnableCraftIcon(nCnt, ITEMTYPE_HEX, ITEMTYPE_MANDORIN, ITEMTYPE_HEXMANDOLIN);
		EnableCraftIcon(nCnt, ITEMTYPE_BONE, ITEMTYPE_SPEAR, ITEMTYPE_BONESPEAR);
		EnableCraftIcon(nCnt, ITEMTYPE_GOLF, ITEMTYPE_HUNMER, ITEMTYPE_GOLFHUNMER);

	}
}
//==============================================================================================================
// �N���t�g��̃A�C�e��
//==============================================================================================================
void CraftMixItem(int nCntItem, int MixItem, int motionchange)
{
	Player* pPlayer = GetPlayer();

	// �ΉԂ��ۂ��G�t�F�N�g
	SetParticle(D3DXVECTOR3(g_Item[nCntItem].pos.x, g_Item[nCntItem].pos.y + 30.0f, g_Item[nCntItem].pos.z),
		D3DXVECTOR3(0.0f, 0.0f, 0.0f),
		D3DXVECTOR3(3.14f, 3.14f, 3.14f),
		D3DXVECTOR3(0.0f, 0.0f, 0.0f),
		D3DXCOLOR(1.0f, 0.5f, 0.0f, 1.0f),
		2.0f, 2, 40, 20, 5.0f, 2.0f, false,
		D3DXVECTOR3(0.0f, 0.0f, 0.0f));

	SetParticle(D3DXVECTOR3(g_Item[nCntItem].pos.x, g_Item[nCntItem].pos.y + 30.0f, g_Item[nCntItem].pos.z),
		D3DXVECTOR3(0.0f, 0.0f, 0.0f),
		D3DXVECTOR3(3.14f, 3.14f, 3.14f),
		D3DXVECTOR3(0.0f, 0.0f, 0.0f),
		D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f),
		3.0f, 2, 40, 20, 3.0f, 2.0f, false,
		D3DXVECTOR3(0.0f, 0.0f, 0.0f));

	SetParticle(D3DXVECTOR3(g_Item[nCntItem].pos.x, g_Item[nCntItem].pos.y + 30.0f, g_Item[nCntItem].pos.z), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(1.0f, 1.0f, 1.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 0.5f, 0.0f, 1.0f), 3.0f, 2, 30, 10, 10.0f, 40.0f, true, D3DXVECTOR3(0.0f, 4.0f, 0.0f));
	SetParticle(D3DXVECTOR3(g_Item[nCntItem].pos.x, g_Item[nCntItem].pos.y + 30.0f, g_Item[nCntItem].pos.z), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(1.0f, 1.0f, 1.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f), 3.0f, 2, 30, 10, 10.0f, 40.0f, true, D3DXVECTOR3(0.0f, 4.0f, 0.0f));
	
	// SE���o��
	PlaySound(SOUND_LABEL_CRAFT);

	// �����Ă���A�C�e����ύX
	Itemchange(nCntItem,MixItem);

	// �N���t�g�Ɏg�����A�C�e��������
	g_Item[nCntItem].bUse = false;

	// ���[�V�����̕ύX
	MotionChange(motionchange, 0);

	// ��ނ���
	int nType = g_Item[pPlayer->ItemIdx].nType;

	// �A�C�e���̌����ڂ�ς���
	g_Item[pPlayer->ItemIdx].ItemTex[nType] = g_TexItem[MixItem];

	g_Item[pPlayer->ItemIdx].durability = g_aItemInfo[MixItem].durability;
	g_Item[pPlayer->ItemIdx].Maxdurability = g_aItemInfo[MixItem].Maxdurability;

	// ��Ɏ����Ă�A�C�e���̎�ނ�΃o�b�g�ɂ���
	g_Item[pPlayer->ItemIdx].nType = MixItem;

	g_Item[pPlayer->ItemIdx].state = ITEMSTATE_HOLD;

	g_Item[pPlayer->StockItemIdx].state = ITEMSTATE_NORMAL;

	pPlayer->StockItemIdx = pPlayer->ItemIdx;
}
//==============================================================================================================
// �N���t�g��̃A�C�e���̃A�C�R���̕\��
//==============================================================================================================
void EnableCraftIcon(int nCntItem, int Item1, int Item2, int MixItem)
{
	Player* pPlayer = GetPlayer();

	// �N���t�g�̑g�ݍ��킹1�������Ă��邩�𔻒�
	const bool HaveCraftItemSet0 = g_Item[pPlayer->StockItemIdx].nType == Item1 && g_Item[pPlayer->ItemIdx].nType == Item2;

	// �N���t�g�̑g�ݍ��킹2�������Ă��邩�𔻒�
	const bool HaveCraftItemSet1 = g_Item[pPlayer->ItemIdx].nType == Item1 && g_Item[pPlayer->StockItemIdx].nType == Item2;

	// �A�C�e�����X�g�b�N����Ă���A�����Ă��邩����
	const bool is_CanCraftState = g_Item[pPlayer->StockItemIdx].state == ITEMSTATE_STOCK && g_Item[pPlayer->ItemIdx].state == ITEMSTATE_HOLD;

	// �N���t�g�̃A�C�R����\���ł��邩�𔻒�
	const bool Cancraft = (HaveCraftItemSet0 || HaveCraftItemSet1) && is_CanCraftState;

	// �N���t�g�̃A�C�R����\���ł���
	if (Cancraft == true)
	{
		// �A�C�R����\������
		g_Item[nCntItem].bMixItem[MixItem] = true;
	}
	else
	{
		// �A�C�R��������
		g_Item[nCntItem].bMixItem[MixItem] = false;
	}
}
//==============================================================================================================
// �A�C�e���̑ϋv�͂̃��[�h����
//==============================================================================================================
void LoadItemInfo(void)
{
	FILE* pFile; // �t�@�C���̃|�C���^

	// �t�@�C�����J��
	pFile = fopen("data\\ITEM\\item_Info.txt", "r");

	// ������i�[�ϐ�
	char aString[MAX_WORD] = {};

	// [=]�ǂݔ�΂��悤�ϐ�
	char skip[3] = {};

	// ���
	int nType = 0;

	// �t�@�C�����J������
	if (pFile != NULL)
	{
		while (1)
		{
			// ������ǂݎ��
			int nData = fscanf(pFile, "%s", &aString[0]);

			// ITEMTYPE��ǂݎ������
			if (strcmp(&aString[0], "ITEMTYPE") == 0)
			{
				// [=]���΂�
				nData = fscanf(pFile, "%s", &skip[0]);

				// ��ނ���
				nData = fscanf(pFile, "%d", &nType);
			}

			// DURABILITY��ǂݎ������
			if (strcmp(&aString[0], "DURABILITY") == 0)
			{
				// [=]���΂�
				nData = fscanf(pFile, "%s", &skip[0]);

				// �ϋv�͂���
				nData = fscanf(pFile, "%d", &g_aItemInfo[nType].durability);
			}

			// TAG��ǂݎ������
			if (strcmp(&aString[0], "TAG") == 0)
			{
				// [=]���΂�
				nData = fscanf(pFile, "%s", &skip[0]);

				// �A�C�e���̃^�O����
				nData = fscanf(pFile, "%s", &g_aItemInfo[nType].Itemtag[0]);
			}

			// ELEMENT��ǂݎ������
			if (strcmp(&aString[0], "ELEMENT") == 0)
			{
				// [=]���΂�
				nData = fscanf(pFile, "%s", &skip[0]);

				// ��������
				nData = fscanf(pFile, "%d", &g_aItemInfo[nType].nElement);
			}

			// POWER��ǂݎ������
			if (strcmp(&aString[0], "POWER") == 0)
			{
				// [=]���΂�
				nData = fscanf(pFile, "%s", &skip[0]);

				// �U���͂���
				nData = fscanf(pFile, "%d", &g_aItemInfo[nType].Power);
			}

			// END_ITEMSET��ǂݎ������
			if (strcmp(&aString[0], "END_ITEMSET") == 0)
			{
				// ���̎�ނ̏���
				nType++;
			}

			// �t�@�C���̍Ō�܂ōs������
			if (nData == EOF)
			{
				// While���𔲂���
				break;
			}
		}
	}
	else
	{// �J���Ȃ������Ƃ�
		// ���b�Z�[�W�{�b�N�X��\��
		MessageBox(NULL, "�t�@�C�����J���܂���", "LoadDuability", MB_OK);
		return;
	}

	// �t�@�C�������
	fclose(pFile);
}
//==============================================================================================================
// �A�C�e�����E���鎞�̉��o
//==============================================================================================================
void PickUpItemAnimation(int nCntItem)
{
	// �C���N�������g
	g_Item[nCntItem].nEasingCnt++;
	
	float time = SetEase(g_Item[nCntItem].nEasingCnt,120);

	// �ړI�̒l�ɋ߂Â���
	g_Item[nCntItem].pos.y += SetSmoothAprroach(20.0f, g_Item[nCntItem].pos.y, EaseInOutCubic(time));

	// �ړI�̒l�ɋ߂Â���
	g_Item[nCntItem].rot.x += SetSmoothAprroach(D3DX_PI * 0.15f, g_Item[nCntItem].rot.x, EaseInOutCubic(time));

	// �ړI�̒l�ɋ߂Â���
	g_Item[nCntItem].rot.y += 0.01f;

	// �A�C�e���̎���Ƀp�[�e�B�N���o��
	if (g_Item[nCntItem].nType == ITEMTYPE_ONIGIRI)
	{
		SetParticle(D3DXVECTOR3(g_Item[nCntItem].pos.x, g_Item[nCntItem].pos.y + g_Item[nCntItem].Size.y / 1.5f, g_Item[nCntItem].pos.z),
			D3DXVECTOR3(1.57f, g_Item[nCntItem].rot.y, 1.57f),
			D3DXVECTOR3(0.2f, 3.14f, 0.2f),
			D3DXVECTOR3(0.0f, 0.0f, 0.0f),
			D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f),
			2.0f, 2, 20, 4, 4.0f, 40.0f,
			false, D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	}
	else
	{
		SetParticle(D3DXVECTOR3(g_Item[nCntItem].pos.x, g_Item[nCntItem].pos.y + g_Item[nCntItem].Size.y / 1.5f, g_Item[nCntItem].pos.z),
			D3DXVECTOR3(1.57f, g_Item[nCntItem].rot.y, 1.57f),
			D3DXVECTOR3(0.2f, 3.14f, 0.2f),
			D3DXVECTOR3(0.0f, 0.0f, 0.0f),
			D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f),
			2.0f, 2, 20, 4, 4.0f, 40.0f,
			false, D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	}

	if (g_Item[nCntItem].nImpactCount >= 120)
	{
		SetImpact(D3DXVECTOR3(g_Item[nCntItem].pos.x,0.0f, g_Item[nCntItem].pos.z),COLOR_GOLD,20,10.0f,7.0f,1.0f,60,IMPACTTYPE_NORMAL,0);
		SetImpact(D3DXVECTOR3(g_Item[nCntItem].pos.x, 0.0f, g_Item[nCntItem].pos.z), COLOR_GOLD, 20, 1.0f, 0.0f, 1.0f, 60, IMPACTTYPE_NORMAL, 0);
		g_Item[nCntItem].nImpactCount = 0;
	}
	else
	{
		// 120�ɂȂ�܂ŃC���N�������g
		g_Item[nCntItem].nImpactCount++;
	}
}
//==============================================================================================================
// �A�C�e�����N���t�g�ł��邩�ǂ���
//==============================================================================================================
bool CheckMixItemMat(int nCntItem, int HoldItem, int StockItem, int HoldIdx) 
{
	// �����̃e���v���[�g�������Ă��邩����
	const bool SetMixTemplate1 = g_Item[nCntItem].nType == StockItem && g_Item[HoldIdx].nType == HoldItem;

	// �����̃e���v���[�g2�������Ă��邩����
	const bool SetMixTemplate2 = g_Item[nCntItem].nType == HoldItem && g_Item[HoldIdx].nType == StockItem;

	// �A�C�e�����X�g�b�N���Ă��邩����
	const bool ItemStock = g_Item[nCntItem].state == ITEMSTATE_STOCK;

	// �A�C�e���������Ă��邩����
	const bool ItemHold = g_Item[HoldIdx].state == ITEMSTATE_HOLD;

	// �N���t�g�ł��邩����
	const bool CanCraft = (SetMixTemplate1 || SetMixTemplate2) && ItemStock && ItemHold;

	// �ޗ����������
	if (CanCraft == true)
	{
		return true;
	}

	return false;
}
//==============================================================================================================
// �N���t�g�A�C�e���̃p�����[�^�ݒ�
//==============================================================================================================
void UpdateCraftItemParam(int nCnt)
{
	Player* pPlayer = GetPlayer();

	if (OnMouseTriggerDown(LEFT_MOUSE) || JoypadTrigger(JOYKEY_A))
	{
		// �΃o�b�g�̑f�ނ��������
		if (CheckMixItemMat(nCnt, ITEMTYPE_BAT, ITEMTYPE_STONE, pPlayer->ItemIdx) == true)
		{
			// �N���t�g��̃A�C�e���̏���
			CraftMixItem(nCnt, ITEMTYPE_STONEBAT, MOTION_DBHAND);

			// �X�e�[�^�X�̕ύX
			StatusChange(3.5f, D3DXVECTOR3(0.0f, g_Item[nCnt].Size.y, 0.0f), 150);
		}

		// �X�̌��̍ޗ����������
		if (CheckMixItemMat(nCnt, ITEMTYPE_KATANA, ITEMTYPE_ICEBLOCK, pPlayer->ItemIdx) == true)
		{
			// �N���t�g��̃A�C�e���̏���
			CraftMixItem(nCnt, ITEMTYPE_ICEBLOCKSOWRD, MOTION_KATANA);

			// �X�e�[�^�X�̕ύX
			StatusChange(3.7f, D3DXVECTOR3(0.0f, g_Item[nCnt].Size.y, 0.0f), 150);
		}

		// ���̌��̍ޗ����������
		if (CheckMixItemMat(nCnt, ITEMTYPE_KATANA, ITEMTYPE_TORCH, pPlayer->ItemIdx) == true)
		{
			// �N���t�g��̃A�C�e���̏���
			CraftMixItem(nCnt, ITEMTYPE_TORCHSWORD, MOTION_KATANA);

			// �X�e�[�^�X�̕ύX
			StatusChange(3.7f, D3DXVECTOR3(0.0f, g_Item[nCnt].Size.y, 0.0f), 150);
		}

		// ���̌��̍ޗ����������
		if (CheckMixItemMat(nCnt, ITEMTYPE_LIGHT, ITEMTYPE_KATANA, pPlayer->ItemIdx) == true)
		{
			// �N���t�g��̃A�C�e���̏���
			CraftMixItem(nCnt, ITEMTYPE_LIGHTWOOD, MOTION_DBHAND);

			// �X�e�[�^�X�̕ύX
			StatusChange(3.7f, D3DXVECTOR3(0.0f, g_Item[nCnt].Size.y, 0.0f), 150);
		}

		// �����o�b�g�̍ޗ����������
		if (CheckMixItemMat(nCnt, ITEMTYPE_IRON, ITEMTYPE_BAT, pPlayer->ItemIdx) == true)
		{
			// �N���t�g��̃A�C�e���̏���
			CraftMixItem(nCnt, ITEMTYPE_IRONBAT, MOTION_DBHAND);

			// �X�e�[�^�X�̕ύX
			StatusChange(3.5f, D3DXVECTOR3(0.0f, g_Item[nCnt].Size.y, 0.0f), 150);
		}

		// �S�g�}�l�L���̍ޗ����������
		if (CheckMixItemMat(nCnt, ITEMTYPE_HEADSTATUE, ITEMTYPE_TORSO, pPlayer->ItemIdx) == true)
		{
			// �N���t�g��̃A�C�e���̏���
			CraftMixItem(nCnt, ITEMTYPE_HEADSTATUTORSO, MOTION_BIGWEPON);

			// �X�e�[�^�X�̕ύX
			StatusChange(3.1f, D3DXVECTOR3(0.0f, g_Item[nCnt].Size.y, 0.0f), 150);
		}

		// �L�����ւ̍ޗ����������
		if (CheckMixItemMat(nCnt, ITEMTYPE_FISH, ITEMTYPE_SURFBOARD, pPlayer->ItemIdx) == true)
		{
			// �N���t�g��̃A�C�e���̏���
			CraftMixItem(nCnt, ITEMTYPE_SURFBOARDFISH, MOTION_BIGWEPON);

			// �X�e�[�^�X�̕ύX
			StatusChange(3.1f, D3DXVECTOR3(0.0f, g_Item[nCnt].Size.y, 0.0f), 150);
		}

		// �􂢂̊y��̍ޗ����������
		if (CheckMixItemMat(nCnt, ITEMTYPE_HEX, ITEMTYPE_MANDORIN, pPlayer->ItemIdx) == true)
		{
			// �N���t�g��̃A�C�e���̏���
			CraftMixItem(nCnt, ITEMTYPE_HEXMANDOLIN, MOTION_ONE_HAND);

			// �X�e�[�^�X�̕ύX
			StatusChange(3.5f, D3DXVECTOR3(0.0f, g_Item[nCnt].Size.y, 0.0f), 150);
		}

		// ���̑��̍ޗ����������
		if (CheckMixItemMat(nCnt, ITEMTYPE_BONE, ITEMTYPE_SPEAR, pPlayer->ItemIdx) == true)
		{
			// �N���t�g��̃A�C�e���̏���
			CraftMixItem(nCnt, ITEMTYPE_BONESPEAR, MOTION_PIERCING);

			// �X�e�[�^�X�̕ύX
			StatusChange(3.5f, D3DXVECTOR3(0.0f, g_Item[nCnt].Size.y, 0.0f), 150);
		}

		// �S���t�n���}�[�̍ޗ����������
		if (CheckMixItemMat(nCnt, ITEMTYPE_GOLF, ITEMTYPE_HUNMER, pPlayer->ItemIdx) == true)
		{
			// �N���t�g��̃A�C�e���̏���
			CraftMixItem(nCnt, ITEMTYPE_GOLFHUNMER, MOTION_BIGWEPON);

			// �X�e�[�^�X�̕ύX
			StatusChange(2.5f, D3DXVECTOR3(0.0f, g_Item[nCnt].Size.y, 0.0f), 180);
		}

	}
}
