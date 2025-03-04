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
void LoadDurability(void); // �A�C�e���̑ϋv�͂̃��[�h����

//**************************************************************************************************************
//�O���[�o���ϐ��錾
//**************************************************************************************************************
Item g_Item[MAX_ITEM];			// �\���̕ϐ�
int g_ItemTypeMax;				// ��ސ�
Item g_TexItem[ITEMTYPE_MAX];	// �e�N�X�`���֌W
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
		g_Item[nCntItem].nLife = 20;						   // �̗�
		g_Item[nCntItem].state = ITEMSTATE_NORMAL;			   // ���
		g_Item[nCntItem].fRadius = 100.0f;					   // ���a
		g_Item[nCntItem].nLife = 180;						   // �̗�
		g_Item[nCntItem].durability = 0;		   // �ϋv��
		g_Item[nCntItem].EnableCraft = false;				   // �N���t�g�ł��邩�ۂ�
		g_Item[nCntItem].grabity = 0.0f;				   // �N���t�g�ł��邩�ۂ�

		for (int nCntNum = 0; nCntNum < ITEMTYPE_MAX; nCntNum++)
		{
			g_Item[nCntItem].bMixItem[nCntNum] = false;				   // �N���t�g��̃A�C�e���\���p�t���O
		}
	}

	LoadDurability();
	LoadItemModel(); // �A�C�e���̃��[�h����

	bFIrstCraftItem = false;

	for (int nCntNum = 0; nCntNum < g_ItemTypeMax; nCntNum++)
	{
		//g_TexItem[nCntNum].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		//g_TexItem[nCntNum].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		//g_TexItem[nCntNum].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_TexItem[nCntNum].Scal = D3DXVECTOR3(1.0f, 1.0f, 1.0f);// �g�嗦
		g_TexItem[nCntNum].nLife = 120;							// �̗�
		g_TexItem[nCntNum].state = ITEMSTATE_NORMAL;			// ���
		g_TexItem[nCntNum].nType = nCntNum;			            // �ԍ�
		g_TexItem[nCntNum].nElement = ITEMELEMENT_STANDARD;     // ������
		ElementChange(nCntNum);
		g_TexItem[nCntNum].bMixItem[nCntNum] = false;           // �N���t�g��̃A�C�e���\���p�t���O
		g_TexItem[nCntNum].grabity = 0.0f;           // �N���t�g��̃A�C�e���\���p�t���O
		g_TexItem[nCntNum].Maxdurability = g_TexItem[nCntNum].durability;           // �A�C�e���̑ϋv��

		D3DXMATERIAL* pMat; // �}�e���A���ւ̃|�C���^

		// �}�e���A���̃f�[�^�ւ̃|�C���^���擾
		pMat = (D3DXMATERIAL*)g_TexItem[nCntNum].ItemTex[nCntNum].g_pBuffMatModel->GetBufferPointer();

		for (int nCntMat = 0; nCntMat < (int)g_TexItem[nCntNum].ItemTex[nCntNum].g_dwNumMatModel; nCntMat++)
		{
			if (pMat[nCntMat].pTextureFilename != NULL)
			{
				//���̃t�@�C�������g�p���ăe�N�X�`����ǂݍ���
				//�e�N�X�`���̓ǂݍ���
				D3DXCreateTextureFromFile(pDevice,
					pMat[nCntMat].pTextureFilename,
					&g_TexItem[nCntNum].ItemTex[nCntNum].g_apTextureModel[nCntMat]);
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
		nNumVtx = g_TexItem[nCntNum].ItemTex[nCntNum].g_pMeshModel->GetNumVertices();

		// ���_�t�H�[�}�b�g�̃T�C�Y�擾
		sizeFVF = D3DXGetFVFVertexSize(g_TexItem[nCntNum].ItemTex[nCntNum].g_pMeshModel->GetFVF());

		// ���_�o�b�t�@�̃��b�N
		g_TexItem[nCntNum].ItemTex[nCntNum].g_pMeshModel->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

		for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
		{
			// ���_���W�̑��
			D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVtxBuff;

			// ���_���W���r���ău���b�N�̍ŏ��l,�ő�l���擾
			if (vtx.x < g_TexItem[nCntNum].ItemTex[nCntNum].vtxMin.x)
			{
				g_TexItem[nCntNum].ItemTex[nCntNum].vtxMin.x = vtx.x;
			}
			else if (vtx.y < g_TexItem[nCntNum].ItemTex[nCntNum].vtxMin.y)
			{
				g_TexItem[nCntNum].ItemTex[nCntNum].vtxMin.y = vtx.y;
			}
			else if (vtx.z < g_TexItem[nCntNum].ItemTex[nCntNum].vtxMin.z)
			{
				g_TexItem[nCntNum].ItemTex[nCntNum].vtxMin.z = vtx.z;
			}
			else if (vtx.x > g_TexItem[nCntNum].ItemTex[nCntNum].vtxMax.x)
			{
				g_TexItem[nCntNum].ItemTex[nCntNum].vtxMax.x = vtx.x;
			}
			else if (vtx.y > g_TexItem[nCntNum].ItemTex[nCntNum].vtxMax.y)
			{
				g_TexItem[nCntNum].ItemTex[nCntNum].vtxMax.y = vtx.y;
			}
			else if (vtx.z > g_TexItem[nCntNum].ItemTex[nCntNum].vtxMax.z)
			{
				g_TexItem[nCntNum].ItemTex[nCntNum].vtxMax.z = vtx.z;
			}

			// ���_�t�H�[�}�b�g�̃T�C�Y���|�C���^��i�߂�
			pVtxBuff += sizeFVF;

			// �T�C�Y����
			g_TexItem[nCntNum].Size.x = g_TexItem[nCntNum].ItemTex[nCntNum].vtxMax.x - g_TexItem[nCntNum].ItemTex[nCntNum].vtxMin.x;
			g_TexItem[nCntNum].Size.y = g_TexItem[nCntNum].ItemTex[nCntNum].vtxMax.y - g_TexItem[nCntNum].ItemTex[nCntNum].vtxMin.y;
			g_TexItem[nCntNum].Size.z = g_TexItem[nCntNum].ItemTex[nCntNum].vtxMax.z - g_TexItem[nCntNum].ItemTex[nCntNum].vtxMin.z;
		}

		// ���_�o�b�t�@�̃A�����b�N
		g_TexItem[nCntNum].ItemTex[nCntNum].g_pMeshModel->UnlockVertexBuffer();
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

	for (int nCntNum = 0; nCntNum < g_ItemTypeMax; nCntNum++)
	{
		// �e�N�X�`���̔j��
		for (int nCntTex = 0; nCntTex < MAX_TEX; nCntTex++)
		{
			if (g_TexItem[nCntNum].ItemTex[nCntNum].g_apTextureModel[nCntTex] != NULL)
			{
				g_TexItem[nCntNum].ItemTex[nCntNum].g_apTextureModel[nCntTex]->Release();
				g_TexItem[nCntNum].ItemTex[nCntNum].g_apTextureModel[nCntTex] = NULL;
			}
		}

		// ���b�V���̔j��
		if (g_TexItem[nCntNum].ItemTex[nCntNum].g_pMeshModel != NULL)
		{
			g_TexItem[nCntNum].ItemTex[nCntNum].g_pMeshModel->Release();
			g_TexItem[nCntNum].ItemTex[nCntNum].g_pMeshModel = NULL;
		}

		// �}�e���A���̔j��
		if (g_TexItem[nCntNum].ItemTex[nCntNum].g_pBuffMatModel != NULL)
		{
			g_TexItem[nCntNum].ItemTex[nCntNum].g_pBuffMatModel->Release();
			g_TexItem[nCntNum].ItemTex[nCntNum].g_pBuffMatModel = NULL;
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

	for (int nCntItem = 0; nCntItem < MAX_ITEM; nCntItem++)
	{
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
			Itemchange(g_Item[nCntItem].nType);

			// ���[�V�����̕ύX
			MotionChange(MOTION_KATANA, 0);

			// �A�C�e���̃C���f�b�N�X��ۑ�
			pPlayer->ItemIdx = nCntItem;
			g_Item[nCntItem].state = ITEMSTATE_HOLD;

			// �X�e�[�^�X�ύX
			StatusChange(3.1f, D3DXVECTOR3(0.0f, 75.0f, 0.0f), 50);

			// �ŏ��ɐ؂�ւ���
			bFIrstCraftItem = true;
		}

		// ������ꂽ�A�C�e���ɃG�t�F�N�g
		if (g_Item[nCntItem].state == ITEMSTATE_THROW)
		{
			// �A�C�e���̑����𔽉f��
			int nType = g_Item[nCntItem].nType;
			g_Item[nCntItem].nElement = g_TexItem[nType].nElement;
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

		CollisionItem(nCntItem,// �A�C�e���̃C���f�b�N�X��n��
			20.0f,  // �A�C�e���̔��a
			20.0f); // �v���C���[�̔��a

		// �v���C���[���N���t�g��Ԃ�������
		if (pPlayer->bCraft)
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

	for (int nCntNum = 0; nCntNum < g_ItemTypeMax; nCntNum++)
	{
		for (int nCntItem = 0; nCntItem < MAX_ITEM; nCntItem++)
		{
			if (!g_Item[nCntItem].bUse)
			{//���g�p��ԂȂ�
				//���̏�����ʂ����J�E���g��i�߂�
				continue;
			}

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

			for (int nCntMat = 0; nCntMat < (int)g_Item[nCntItem].ItemTex[nCntNum].g_dwNumMatModel; nCntMat++)
			{
				// �}�e���A���̃f�[�^�ւ̃|�C���^���擾
				pMat = (D3DXMATERIAL*)g_Item[nCntItem].ItemTex[nCntNum].g_pBuffMatModel->GetBufferPointer();

				// �}�e���A���̐ݒ�
				pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

				// �e�N�X�`���̐ݒ�
				pDevice->SetTexture(0, g_Item[nCntItem].ItemTex[nCntNum].g_apTextureModel[nCntMat]);

				// �u���b�N(�p�[�c)�̕`��
				g_Item[nCntItem].ItemTex[nCntNum].g_pMeshModel->DrawSubset(nCntMat);
			}
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

			g_Item[nCntItem] = g_TexItem[nType]; // �K�v�ȏ�����

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
void Itemchange(int nType)
{
	// �v���C���[�̎擾
	Player* pPlayer = GetPlayer();

	pPlayer->Motion.aModel[15].dwNumMat = g_TexItem[nType].ItemTex[nType].g_dwNumMatModel; // �A�C�e���̃}�e���A���̏�����
	pPlayer->Motion.aModel[15].pBuffMat = g_TexItem[nType].ItemTex[nType].g_pBuffMatModel; // �A�C�e���̃o�b�t�@�̏�����
	pPlayer->Motion.aModel[15].pMesh = g_TexItem[nType].ItemTex[nType].g_pMeshModel;       // �A�C�e���̃��b�V���̏�����
	pPlayer->nElement = g_TexItem[nType].nElement; // �A�C�e���̑���������
	
	//if (g_Item[pPlayer->ItemIdx].state == ITEMSTATE_HOLD)
	//{
	//	g_Item[pPlayer->ItemIdx].nType = nType;
	//}
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
					&g_TexItem[nType].ItemTex[nType].g_pBuffMatModel,
					NULL,
					&g_TexItem[nType].ItemTex[nType].g_dwNumMatModel,
					&g_TexItem[nType].ItemTex[nType].g_pMeshModel);

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
// �A�C�e���̑����ύX����
//==============================================================================================================
void ElementChange(int nTypeItem)
{
	// �����𕐊킲�Ƃɔ��ʂ���
	if (g_TexItem[nTypeItem].nType == 0) // �o�b�g�̑���
	{
		g_TexItem[nTypeItem].nElement = ITEMELEMENT_STANDARD;
	}
	else if (g_TexItem[nTypeItem].nType == 1) // �S���t�N���u�̑���
	{
		g_TexItem[nTypeItem].nElement = ITEMELEMENT_STANDARD;
	}
	else if (g_TexItem[nTypeItem].nType == 2) // �n���}�[�̑���
	{
		g_TexItem[nTypeItem].nElement = ITEMELEMENT_STANDARD;
	}
	else if(g_TexItem[nTypeItem].nType == 3) // �΂̑���
	{
		g_TexItem[nTypeItem].nElement = ITEMELEMENT_STANDARD;
	}
	else if (g_TexItem[nTypeItem].nType == 4) // �p�ނ̑���
	{
		g_TexItem[nTypeItem].nElement = ITEMELEMENT_STANDARD;
	}
	else if (g_TexItem[nTypeItem].nType == 5) // �Εt���o�b�g�̑���
	{
		g_TexItem[nTypeItem].nElement = ITEMELEMENT_BLOOD;
	}
	else if (g_TexItem[nTypeItem].nType == 6) // �u�����̑���
	{
		g_TexItem[nTypeItem].nElement = ITEMELEMENT_SPARK;
	}
	else if (g_TexItem[nTypeItem].nType == 7) // �d�����̑���
	{
		g_TexItem[nTypeItem].nElement = ITEMELEMENT_SPARK;
	}
	else if (g_TexItem[nTypeItem].nType == 8) // �n���Z���̑���
	{
		g_TexItem[nTypeItem].nElement = ITEMELEMENT_STANDARD;
	}
	else if (g_TexItem[nTypeItem].nType == 9) // �X��̑���
	{
		g_TexItem[nTypeItem].nElement = ITEMELEMENT_FREEZE;
	}
	else if (g_TexItem[nTypeItem].nType == 10) // �������̑���
	{
		g_TexItem[nTypeItem].nElement = ITEMELEMENT_FREEZE;
	}
	else if (g_TexItem[nTypeItem].nType == 11) // �S�p�C�v�̑���
	{
		g_TexItem[nTypeItem].nElement = ITEMELEMENT_BLOOD;
	}
	else if (g_TexItem[nTypeItem].nType == 12) // �����o�b�g�̑���
	{
		g_TexItem[nTypeItem].nElement = ITEMELEMENT_BLOOD;
	}
	else if (g_TexItem[nTypeItem].nType == 13) // �T�[�t�{�[�h�̑���
	{
		g_TexItem[nTypeItem].nElement = ITEMELEMENT_AQUA;
	}
	else if (g_TexItem[nTypeItem].nType == 14) // �����̑���
	{
		g_TexItem[nTypeItem].nElement = ITEMELEMENT_FIRE;
	}
	else if (g_TexItem[nTypeItem].nType == 15) // �҉Ό��̑���
	{
		g_TexItem[nTypeItem].nElement = ITEMELEMENT_FIRE;
	}
	else if (g_TexItem[nTypeItem].nType == 16) // �o�[���̑���
	{
		g_TexItem[nTypeItem].nElement = ITEMELEMENT_BLOOD;
	}
	else if (g_TexItem[nTypeItem].nType == 17) // �����̑���
	{
		g_TexItem[nTypeItem].nElement = ITEMELEMENT_STANDARD;
	}
	else if (g_TexItem[nTypeItem].nType == 18) // �}�l�L���̑���
	{
		g_TexItem[nTypeItem].nElement = ITEMELEMENT_STANDARD;
	}
	else if (g_TexItem[nTypeItem].nType == 19) // ���K�z���̑���
	{
		g_TexItem[nTypeItem].nElement = ITEMELEMENT_STANDARD;
	}
	else if (g_TexItem[nTypeItem].nType == 20) // �����ۂ�̑���(��)
	{
		g_TexItem[nTypeItem].nElement = ITEMELEMENT_STANDARD;
	}
	else if (g_TexItem[nTypeItem].nType == 21) // �d���̑���(��)
	{
		g_TexItem[nTypeItem].nElement = ITEMELEMENT_BLOOD;
	}
	else if (g_TexItem[nTypeItem].nType == 22) // �g���\�[�̑���
	{
		g_TexItem[nTypeItem].nElement = ITEMELEMENT_STANDARD;
	}
	else if (g_TexItem[nTypeItem].nType == 23) // �g����̑���
	{
		g_TexItem[nTypeItem].nElement = ITEMELEMENT_SPARK;
	}
	else if (g_TexItem[nTypeItem].nType == 24) // �����̑���
	{
		g_TexItem[nTypeItem].nElement = ITEMELEMENT_DARK;
	}
	else if (g_TexItem[nTypeItem].nType == 25) // ���̑���
	{
		g_TexItem[nTypeItem].nElement = ITEMELEMENT_AQUA;
	}
	else if (g_TexItem[nTypeItem].nType == 26) // �����̑���
	{
		g_TexItem[nTypeItem].nElement = ITEMELEMENT_DARK;
	}
	else if (g_TexItem[nTypeItem].nType == 27) // �n�[�v�̑���
	{
		g_TexItem[nTypeItem].nElement = ITEMELEMENT_DARK;
	}
	else if (g_TexItem[nTypeItem].nType == 28) // �L�����ւ̑���
	{
		g_TexItem[nTypeItem].nElement = ITEMELEMENT_AQUA;
	}
	else if (g_TexItem[nTypeItem].nType == 29) // �Ŕ̑���
	{
		g_TexItem[nTypeItem].nElement = ITEMELEMENT_BLOOD;
	}
	else if (g_TexItem[nTypeItem].nType == 30) // �������̑���
	{
		g_TexItem[nTypeItem].nElement = ITEMELEMENT_STANDARD;
	}
}
//==============================================================================================================
// �A�C�e���̎擾
//==============================================================================================================
Item* GetItemOrigin(void)
{
	return &g_TexItem[0];
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
		if (OnMouseTriggerDown(LEFT_MOUSE) || JoypadTrigger(JOYKEY_A))
		{
			// �X�g�[���o�b�g�̍ޗ����������
			if (((g_Item[nCnt].nType == ITEMTYPE_STONE && g_Item[pPlayer->ItemIdx].nType == ITEMTYPE_BAT || (g_Item[pPlayer->ItemIdx].nType == ITEMTYPE_STONE && g_Item[nCnt].nType == ITEMTYPE_BAT))
				&& g_Item[nCnt].state == ITEMSTATE_STOCK && g_Item[pPlayer->ItemIdx].state == ITEMSTATE_HOLD))
			{
				// �N���t�g��̃A�C�e���̏���
				CraftMixItem(nCnt, ITEMTYPE_STONEBAT, MOTION_DBHAND);

				// �X�e�[�^�X�̕ύX
				StatusChange(3.1f, D3DXVECTOR3(0.0f, 75.0f, 0.0f), 150);

				g_Item[pPlayer->StockItemIdx].state = ITEMSTATE_NORMAL;
			}
			// �X�̌��̍ޗ����������
			if (((g_Item[nCnt].nType == ITEMTYPE_ICEBLOCK && g_Item[pPlayer->ItemIdx].nType == ITEMTYPE_KATANA || (g_Item[pPlayer->ItemIdx].nType == ITEMTYPE_ICEBLOCK && g_Item[nCnt].nType == ITEMTYPE_KATANA))
				&& g_Item[nCnt].state == ITEMSTATE_STOCK && g_Item[pPlayer->ItemIdx].state == ITEMSTATE_HOLD))
			{
				CraftMixItem(nCnt, ITEMTYPE_ICEBLOCKSOWRD, MOTION_KATANA);

				// �X�e�[�^�X�̕ύX
				StatusChange(3.1f, D3DXVECTOR3(0.0f, 75.0f, 0.0f), 150);

				g_Item[pPlayer->StockItemIdx].state = ITEMSTATE_NORMAL;
			}
			// �X�̌��̍ޗ����������
			if (((g_Item[nCnt].nType == ITEMTYPE_TORCH && g_Item[pPlayer->ItemIdx].nType == ITEMTYPE_KATANA || (g_Item[pPlayer->ItemIdx].nType == ITEMTYPE_TORCH && g_Item[nCnt].nType == ITEMTYPE_KATANA))
				&& g_Item[nCnt].state == ITEMSTATE_STOCK && g_Item[pPlayer->ItemIdx].state == ITEMSTATE_HOLD))
			{
				CraftMixItem(nCnt, ITEMTYPE_TORCHSWORD, MOTION_KATANA);

				// �X�e�[�^�X�̕ύX
				StatusChange(3.1f, D3DXVECTOR3(0.0f, 75.0f, 0.0f), 150);

				g_Item[pPlayer->StockItemIdx].state = ITEMSTATE_NORMAL;
			}
			if (((g_Item[nCnt].nType == ITEMTYPE_LIGHT && g_Item[pPlayer->ItemIdx].nType == ITEMTYPE_WOOD || (g_Item[pPlayer->ItemIdx].nType == ITEMTYPE_LIGHT && g_Item[nCnt].nType == ITEMTYPE_WOOD))
				&& g_Item[nCnt].state == ITEMSTATE_STOCK && g_Item[pPlayer->ItemIdx].state == ITEMSTATE_HOLD))
			{
				CraftMixItem(nCnt, ITEMTYPE_LIGHTWOOD, MOTION_DBHAND);

				// �X�e�[�^�X�̕ύX
				StatusChange(3.1f, D3DXVECTOR3(0.0f, 75.0f, 0.0f), 150);

				g_Item[pPlayer->StockItemIdx].state = ITEMSTATE_NORMAL;
			}
			if (((g_Item[nCnt].nType == ITEMTYPE_IRON && g_Item[pPlayer->ItemIdx].nType == ITEMTYPE_BAT || (g_Item[pPlayer->ItemIdx].nType == ITEMTYPE_IRON && g_Item[nCnt].nType == ITEMTYPE_BAT))
				&& g_Item[nCnt].state == ITEMSTATE_STOCK && g_Item[pPlayer->ItemIdx].state == ITEMSTATE_HOLD))
			{
				CraftMixItem(nCnt, ITEMTYPE_IRONBAT, MOTION_DBHAND);

				// �X�e�[�^�X�̕ύX
				StatusChange(3.1f, D3DXVECTOR3(0.0f, 75.0f, 0.0f), 150);

				g_Item[pPlayer->StockItemIdx].state = ITEMSTATE_NORMAL;
			}
			if (((g_Item[nCnt].nType == ITEMTYPE_HEADSTATUE && g_Item[pPlayer->ItemIdx].nType == ITEMTYPE_TORSO || (g_Item[pPlayer->ItemIdx].nType == ITEMTYPE_HEADSTATUE && g_Item[nCnt].nType == ITEMTYPE_TORSO))
				&& g_Item[nCnt].state == ITEMSTATE_STOCK && g_Item[pPlayer->ItemIdx].state == ITEMSTATE_HOLD))
			{
				CraftMixItem(nCnt, ITEMTYPE_HEADSTATUTORSO, MOTION_BIGWEPON);

				// �X�e�[�^�X�̕ύX
				StatusChange(2.9f, D3DXVECTOR3(0.0f, 75.0f, 0.0f), 150);

				g_Item[pPlayer->StockItemIdx].state = ITEMSTATE_NORMAL;
			}
			if (((g_Item[nCnt].nType == ITEMTYPE_FISH && g_Item[pPlayer->ItemIdx].nType == ITEMTYPE_SURFBOARD || (g_Item[pPlayer->ItemIdx].nType == ITEMTYPE_FISH && g_Item[nCnt].nType == ITEMTYPE_SURFBOARD))
				&& g_Item[nCnt].state == ITEMSTATE_STOCK && g_Item[pPlayer->ItemIdx].state == ITEMSTATE_HOLD))
			{
				CraftMixItem(nCnt, ITEMTYPE_SURFBOARDFISH, MOTION_BIGWEPON);

				// �X�e�[�^�X�̕ύX
				StatusChange(2.9f, D3DXVECTOR3(0.0f, 75.0f, 0.0f), 150);

				g_Item[pPlayer->StockItemIdx].state = ITEMSTATE_NORMAL;
			}
		}

		// �N���t�g�A�C�R����\�����邩���Ȃ���
		EnableCraftIcon(nCnt, ITEMTYPE_STONE, ITEMTYPE_BAT, ITEMTYPE_STONEBAT);
		EnableCraftIcon(nCnt, ITEMTYPE_ICEBLOCK, ITEMTYPE_KATANA, ITEMTYPE_ICEBLOCKSOWRD);
		EnableCraftIcon(nCnt, ITEMTYPE_TORCH, ITEMTYPE_KATANA, ITEMTYPE_TORCHSWORD);
		EnableCraftIcon(nCnt, ITEMTYPE_WOOD, ITEMTYPE_LIGHT, ITEMTYPE_LIGHTWOOD);
		EnableCraftIcon(nCnt, ITEMTYPE_BAT, ITEMTYPE_IRON, ITEMTYPE_IRONBAT);
		EnableCraftIcon(nCnt, ITEMTYPE_HEADSTATUE, ITEMTYPE_TORSO, ITEMTYPE_HEADSTATUTORSO);
		EnableCraftIcon(nCnt, ITEMTYPE_FISH, ITEMTYPE_SURFBOARD, ITEMTYPE_SURFBOARDFISH);
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
	Itemchange(MixItem);

	// �N���t�g�Ɏg�����A�C�e��������
	g_Item[nCntItem].bUse = false;

	// ���[�V�����̕ύX
	MotionChange(motionchange, 0);

	// ��ނ���
	int nType = g_Item[pPlayer->ItemIdx].nType;

	// �A�C�e���̌����ڂ�ς���
	g_Item[pPlayer->ItemIdx].ItemTex[nType] = g_TexItem[MixItem].ItemTex[MixItem];
	g_Item[pPlayer->ItemIdx].durability = g_TexItem[MixItem].durability;
	g_Item[pPlayer->ItemIdx].Maxdurability = g_TexItem[MixItem].durability;

	// ��Ɏ����Ă�A�C�e���̎�ނ�΃o�b�g�ɂ���
	g_Item[pPlayer->ItemIdx].nType = MixItem;
}
//==============================================================================================================
// �N���t�g��̃A�C�e���̃A�C�R���̕\��
//==============================================================================================================
void EnableCraftIcon(int nCntItem, int Item1, int Item2, int MixItem)
{
	Player* pPlayer = GetPlayer();

	// �΃o�b�g�̑f�ނ��͈͓��ɂ��鎞
	if (((g_Item[pPlayer->StockItemIdx].nType == Item1 && g_Item[pPlayer->ItemIdx].nType == Item2) || (g_Item[pPlayer->ItemIdx].nType == Item1 && g_Item[pPlayer->StockItemIdx].nType == Item2)) &&
		g_Item[pPlayer->StockItemIdx].state == ITEMSTATE_STOCK && g_Item[pPlayer->ItemIdx].state == ITEMSTATE_HOLD)
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
void LoadDurability(void)
{
	FILE* pFile; // �t�@�C���̃|�C���^

	// �t�@�C�����J��
	pFile = fopen("data\\item_Info.txt", "r");

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
			int nData = fscanf(pFile, "%s", &aString[0]);

			if (strcmp(&aString[0], "ITEMTYPE") == 0)
			{
				fscanf(pFile, "%s", &skip[0]);
				fscanf(pFile, "%d", &nType);
			}

			if (strcmp(&aString[0], "DURABILITY") == 0)
			{
				fscanf(pFile, "%s", &skip[0]);
				fscanf(pFile, "%d", &g_TexItem[nType].durability);			
			}

			if (strcmp(&aString[0], "END_ITEMSET") == 0)
			{
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
