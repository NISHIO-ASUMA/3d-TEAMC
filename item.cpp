//============================
//
// �A�C�e��[item.cpp]
// Author:YOSHIDA YUUTO
//
//============================

//****************************
//�C���N���[�h�t�@�C��
//****************************
#include "input.h"
#include "mouse.h"
#include "camera.h"
#include "item.h"
#include "block.h"
#include"player.h"

//****************************
//�}�N����`
//****************************
#define MOUSE_SIZE (50.0f)
#define MAX_WORD (256)

//****************************
//�v���g�^�C�v�錾
//****************************
void SetMtxItem(int nCnt);
void LoadItemModel(void); // �A�C�e���̃��[�h����

//****************************
//�O���[�o���ϐ��錾
//****************************
Item g_Item[MAX_ITEM];
int g_ItemTypeMax;
Item g_TexItem[ITEMTYPE_MAX];

//=============================
//�u���b�N�̏���������
//=============================
void InitItem(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();//�f�o�C�X�̃|�C���^

	for (int nCntItem = 0; nCntItem < MAX_ITEM; nCntItem++)
	{
		g_Item[nCntItem].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f); //���W
		g_Item[nCntItem].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//�ړ���
		g_Item[nCntItem].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f); //�p�x
		g_Item[nCntItem].Scal = D3DXVECTOR3(1.0f, 1.0f, 1.0f);//�g�嗦
		g_Item[nCntItem].nType = ITEMTYPE_BAT;				  //���
		g_Item[nCntItem].bUse = false;						  //���g�p����
		g_Item[nCntItem].nLife = 20;						  //�̗�
		g_Item[nCntItem].state = ITEMSTATE_NORMAL;			  //���
		g_Item[nCntItem].fRadius = 100.0f;					  //���a
		g_Item[nCntItem].nLife = 180;						  //�̗�
	}

	LoadItemModel(); // �A�C�e���̃��[�h����

	for (int nCntNum = 0; nCntNum < g_ItemTypeMax; nCntNum++)
	{
		//g_TexItem[nCntNum].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		//g_TexItem[nCntNum].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		//g_TexItem[nCntNum].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_TexItem[nCntNum].Scal = D3DXVECTOR3(1.0f, 1.0f, 1.0f);//�g�嗦
		g_TexItem[nCntNum].nLife = 120;							//�̗�

		D3DXMATERIAL* pMat;//�}�e���A���ւ̃|�C���^

		//�}�e���A���̃f�[�^�ւ̃|�C���^���擾
		pMat = (D3DXMATERIAL*)g_TexItem[nCntNum].ItemTex[nCntNum].g_pBuffMatItem->GetBufferPointer();

		for (int nCntMat = 0; nCntMat < (int)g_TexItem[nCntNum].ItemTex[nCntNum].g_dwNumMatItem; nCntMat++)
		{
			if (pMat[nCntMat].pTextureFilename != NULL)
			{
				//���̃t�@�C�������g�p���ăe�N�X�`����ǂݍ���
					//�e�N�X�`���̓ǂݍ���
				D3DXCreateTextureFromFile(pDevice,
					pMat[nCntMat].pTextureFilename,
					&g_TexItem[nCntNum].ItemTex[nCntNum].g_apTextureItem[nCntMat]);
			}
		}
	}

	//���_���W�̎擾
	int nNumVtx;//���_��
	DWORD sizeFVF;//���_�t�H�[�}�b�g�̃T�C�Y
	BYTE* pVtxBuff;//���_�o�b�t�@�ւ̃|�C���^

	for (int nCntNum = 0; nCntNum < g_ItemTypeMax; nCntNum++)
	{
		//���_���̎擾
		nNumVtx = g_TexItem[nCntNum].ItemTex[nCntNum].g_pMeshItem->GetNumVertices();

		//���_�t�H�[�}�b�g�̃T�C�Y�擾
		sizeFVF = D3DXGetFVFVertexSize(g_TexItem[nCntNum].ItemTex[nCntNum].g_pMeshItem->GetFVF());

		//���_�o�b�t�@�̃��b�N
		g_TexItem[nCntNum].ItemTex[nCntNum].g_pMeshItem->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

		for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
		{
			//���_���W�̑��
			D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVtxBuff;

			//���_���W���r���ău���b�N�̍ŏ��l,�ő�l���擾
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

			//���_�t�H�[�}�b�g�̃T�C�Y���|�C���^��i�߂�
			pVtxBuff += sizeFVF;

			//�T�C�Y����
			g_TexItem[nCntNum].Size.x = g_TexItem[nCntNum].ItemTex[nCntNum].vtxMax.x - g_TexItem[nCntNum].ItemTex[nCntNum].vtxMin.x;
			g_TexItem[nCntNum].Size.y = g_TexItem[nCntNum].ItemTex[nCntNum].vtxMax.y - g_TexItem[nCntNum].ItemTex[nCntNum].vtxMin.y;
			g_TexItem[nCntNum].Size.z = g_TexItem[nCntNum].ItemTex[nCntNum].vtxMax.z - g_TexItem[nCntNum].ItemTex[nCntNum].vtxMin.z;
		}


		//���_�o�b�t�@�̃A�����b�N
		g_TexItem[nCntNum].ItemTex[nCntNum].g_pMeshItem->UnlockVertexBuffer();
	}
}
//=============================
//�u���b�N�̏I������
//=============================
void UninitItem(void)
{
	for (int nCntNum = 0; nCntNum < g_ItemTypeMax; nCntNum++)
	{
		//�e�N�X�`���̔j��
		for (int nCntTex = 0; nCntTex < MAX_TEX; nCntTex++)
		{
			if (g_TexItem[nCntNum].ItemTex[nCntNum].g_apTextureItem[nCntTex] != NULL)
			{
				g_TexItem[nCntNum].ItemTex[nCntNum].g_apTextureItem[nCntTex]->Release();
				g_TexItem[nCntNum].ItemTex[nCntNum].g_apTextureItem[nCntTex] = NULL;
			}
		}

		//���b�V���̔j��
		if (g_TexItem[nCntNum].ItemTex[nCntNum].g_pMeshItem != NULL)
		{
			g_TexItem[nCntNum].ItemTex[nCntNum].g_pMeshItem->Release();
			g_TexItem[nCntNum].ItemTex[nCntNum].g_pMeshItem = NULL;
		}

		//�}�e���A��
		if (g_TexItem[nCntNum].ItemTex[nCntNum].g_pBuffMatItem != NULL)
		{
			g_TexItem[nCntNum].ItemTex[nCntNum].g_pBuffMatItem->Release();
			g_TexItem[nCntNum].ItemTex[nCntNum].g_pBuffMatItem = NULL;
		}
	}
}
//=============================
//�u���b�N�̍X�V����
//=============================
void UpdateItem(void)
{
	Player* pPlayer = GetPlayer();//�v���C���[�擾

	for (int nCntItem = 0; nCntItem < MAX_ITEM; nCntItem++)
	{
		if (!g_Item[nCntItem].bUse)
		{
			//�g�p������Ȃ�������
			continue;
		}

		if (g_Item[nCntItem].state == ITEMSTATE_HOLD && KeyboardTrigger(DIK_RETURN))
		{
			//�v���C���[�̌����Ă�������ɔ�΂�
			g_Item[nCntItem].move.x = sinf(pPlayer->rot.y + D3DX_PI) * 10.0f;
			g_Item[nCntItem].move.y = 0.0f;
			g_Item[nCntItem].move.z = cosf(pPlayer->rot.y + D3DX_PI) * 10.0f;

			//���˒n�_��e�̈ʒu�ɂ���
			g_Item[nCntItem].pos.x = pPlayer->Motion.aModel[PARENT].mtxWorld._41;
			g_Item[nCntItem].pos.y = pPlayer->Motion.aModel[PARENT].mtxWorld._42;
			g_Item[nCntItem].pos.z = pPlayer->Motion.aModel[PARENT].mtxWorld._43;

			//�u���b�N�̓������Ă����Ԃɂ���
			g_Item[nCntItem].state = ITEMSTATE_THROW;

			//�v���C���[�����������Ă��Ȃ���Ԃɂ���
			pPlayer->HandState = PLAYERHOLD_NO;
		}
		if (g_Item[nCntItem].state == ITEMSTATE_HOLD && KeyboardTrigger(DIK_O))
		{
			g_Item[nCntItem].state = ITEMSTATE_NORMAL;
			pPlayer->HandState = PLAYERHOLD_NO;

			//���˒n�_��e�̈ʒu�ɂ���
			g_Item[nCntItem].pos.x = pPlayer->pos.x;
			g_Item[nCntItem].pos.y = 0.0f;
			g_Item[nCntItem].pos.z = pPlayer->pos.z;

		}

		if (g_Item[nCntItem].state == ITEMSTATE_THROW)
		{
			//�̗͂����炷
			g_Item[nCntItem].nLife--;

			if (g_Item[nCntItem].nLife <= 0)
			{//�̗͂�0�ȉ��Ȃ�
				//���g�p����
				g_Item[nCntItem].bUse = false;
			}
		}

		//�ʒu�̍X�V
		g_Item[nCntItem].pos += g_Item[nCntItem].move;

		CollisionItem(nCntItem,// �A�C�e���̃C���f�b�N�X��n��
			20.0f, // �A�C�e���̔��a
			20.0f); // �v���C���[�̔��a
	}

}
//=============================
//�u���b�N�̕`�揈��
//=============================
void DrawItem(void)
{
	MODE mode = GetMode();//���݂̃��[�h���擾

	Player* pPlayer = GetPlayer();//�v���C���[�̎擾

	LPDIRECT3DDEVICE9 pDevice = GetDevice();//�f�o�C�X�̃|�C���^

	//�v�Z�p�̃}�g���b�N�X
	D3DXMATRIX mtxRot, mtxTrans, mtxScal, mtxParent;

	D3DMATERIAL9 matDef;//���݂̃}�e���A���ۑ��p

	D3DXMATERIAL* pMat;//�}�e���A���f�[�^�ւ̃|�C���^

	for (int nCntNum = 0; nCntNum < g_ItemTypeMax; nCntNum++)
	{
		for (int nCntItem = 0; nCntItem < MAX_ITEM; nCntItem++)
		{
			if (!g_Item[nCntItem].bUse)
			{//���g�p��ԂȂ�
				//���̏�����ʂ����J�E���g��i�߂�
				continue;
			}

			if (g_Item[nCntItem].state == ITEMSTATE_HOLD)
			{
				//�擾����
				SetMtxItem(nCntItem);
			}
			else
			{
				//���[���h�}�g���b�N�X�̏�����
				D3DXMatrixIdentity(&g_Item[nCntItem].mtxWorldItem);

				//�傫���𔽉f
				D3DXMatrixScaling(&mtxScal, g_Item[nCntItem].Scal.y, g_Item[nCntItem].Scal.x, g_Item[nCntItem].Scal.z);
				D3DXMatrixMultiply(&g_Item[nCntItem].mtxWorldItem, &g_Item[nCntItem].mtxWorldItem, &mtxScal);

				//�����𔽉f
				D3DXMatrixRotationYawPitchRoll(&mtxRot, g_Item[nCntItem].rot.y, g_Item[nCntItem].rot.x, g_Item[nCntItem].rot.z);
				D3DXMatrixMultiply(&g_Item[nCntItem].mtxWorldItem, &g_Item[nCntItem].mtxWorldItem, &mtxRot);

				//�ʒu�𔽉f
				D3DXMatrixTranslation(&mtxTrans, g_Item[nCntItem].pos.x, g_Item[nCntItem].pos.y, g_Item[nCntItem].pos.z);
				D3DXMatrixMultiply(&g_Item[nCntItem].mtxWorldItem, &g_Item[nCntItem].mtxWorldItem, &mtxTrans);

				//���[���h�}�g���b�N�X�̐ݒ�
				pDevice->SetTransform(D3DTS_WORLD, &g_Item[nCntItem].mtxWorldItem);
			}

			//���݂̃}�e���A�����擾
			pDevice->GetMaterial(&matDef);

			for (int nCntMat = 0; nCntMat < (int)g_Item[nCntItem].ItemTex[nCntNum].g_dwNumMatItem; nCntMat++)
			{
				//�}�e���A���̃f�[�^�ւ̃|�C���^���擾
				pMat = (D3DXMATERIAL*)g_Item[nCntItem].ItemTex[nCntNum].g_pBuffMatItem->GetBufferPointer();

				//�}�e���A���̐ݒ�
				pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

				//�e�N�X�`���̐ݒ�
				pDevice->SetTexture(0, g_Item[nCntItem].ItemTex[nCntNum].g_apTextureItem[nCntMat]);

				//�u���b�N(�p�[�c)�̕`��
				g_Item[nCntItem].ItemTex[nCntNum].g_pMeshItem->DrawSubset(nCntMat);
			}
		}
	}
}
//=======================
//�u���b�N�̐ݒ菈��
//=======================
void SetItem(D3DXVECTOR3 pos, int nType,D3DXVECTOR3 Scal)
{
	for (int nCntItem = 0; nCntItem < MAX_ITEM; nCntItem++)
	{
		if (!g_Item[nCntItem].bUse)
		{//���g�p��ԂȂ�
			g_Item[nCntItem] = g_TexItem[nType]; // �K�v�ȏ�����

			g_Item[nCntItem].pos = pos;	   //���W
			g_Item[nCntItem].nType = nType;//���
			g_Item[nCntItem].Scal = Scal;  //�g�嗦
			g_Item[nCntItem].bUse = true;  //�g�p����

			break;
		}
	}
}
//=======================
//�u���b�N�̎擾����
//=======================
void Itemchange(int nType)
{
	Player* pPlayer = GetPlayer();

	pPlayer->Motion.aModel[15].dwNumMat = g_TexItem[nType].ItemTex[nType].g_dwNumMatItem;
	pPlayer->Motion.aModel[15].pBuffMat = g_TexItem[nType].ItemTex[nType].g_pBuffMatItem;
	pPlayer->Motion.aModel[15].pMesh = g_TexItem[nType].ItemTex[nType].g_pMeshItem;
}
//=======================
//�A�C�e���̎擾
//=======================
Item* GetItem(void)
{
	return &g_Item[0];
}
////=======================
////�u���b�N�̎擾����
////=======================
//ITEM* GetItem()
//{
//	return &g_Item[0];
//}
//int NumItem(void)
//{
//	return g_NumItem;
//}
//=======================
//�u���b�N�̐e����
//=======================
void SetMtxItem(int nCnt)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();//�f�o�C�X�̃|�C���^

	//�v�Z�p�̃}�g���b�N�X
	D3DXMATRIX mtxRot, mtxTrans, mtxScal, mtxParent;

	Player* pPlayer = GetPlayer();//�v���C���[�̎擾

	//���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&g_Item[nCnt].mtxWorldItem);

	//�傫���𔽉f
	D3DXMatrixScaling(&mtxScal, g_Item[nCnt].Scal.y, g_Item[nCnt].Scal.x, g_Item[nCnt].Scal.z);
	D3DXMatrixMultiply(&g_Item[nCnt].mtxWorldItem, &g_Item[nCnt].mtxWorldItem, &mtxScal);

	//�����𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, g_Item[nCnt].rot.y, g_Item[nCnt].rot.x, g_Item[nCnt].rot.z);
	D3DXMatrixMultiply(&g_Item[nCnt].mtxWorldItem, &g_Item[nCnt].mtxWorldItem, &mtxRot);

	//�ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, g_Item[nCnt].pos.x, g_Item[nCnt].pos.y, g_Item[nCnt].pos.z);
	D3DXMatrixMultiply(&g_Item[nCnt].mtxWorldItem, &g_Item[nCnt].mtxWorldItem, &mtxTrans);

	g_Item[nCnt].mtxParent = pPlayer->Motion.aModel[PARENT].mtxWorld;

	//�Z�o����[�p�[�c�̃��[���h�}�g���b�N�X]��[�e�̃}�g���b�N�X]���������킹��
	D3DXMatrixMultiply(&g_Item[nCnt].mtxWorldItem,
		&g_Item[nCnt].mtxWorldItem,
		&g_Item[nCnt].mtxParent);//���������e

	//���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &g_Item[nCnt].mtxWorldItem);
}
//============================
// �A�C�e���̃��[�h����
//============================
void LoadItemModel(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();//�f�o�C�X�̃|�C���^

	FILE* pFile; // �t�@�C���̃|�C���^

	char skip[5];
	int nType = 0;

	pFile = fopen("data\\MODEL_TXT\\ITEM.txt", "r");

	if (pFile != NULL)
	{
		char aString[MAX_WORD];

		while (1)
		{
			fscanf(pFile, "%s", &aString[0]);

			if (strcmp(aString, "MAX_TYPE") == 0)
			{
				fscanf(pFile, "%s", &skip[0]);
				fscanf(pFile, "%d", &g_ItemTypeMax);
			}
			else if (strcmp(aString, "MODEL_FILENAME") == 0)
			{
				fscanf(pFile, "%s", &skip[0]);

				fscanf(pFile, "%s", &aString[0]);

				const char* MODEL_FILENAME = {};

				MODEL_FILENAME = aString;

				//X�t�@�C���̓ǂݍ���
				D3DXLoadMeshFromX(MODEL_FILENAME,
					D3DXMESH_SYSTEMMEM,
					pDevice,
					NULL,
					&g_TexItem[nType].ItemTex[nType].g_pBuffMatItem,
					NULL,
					&g_TexItem[nType].ItemTex[nType].g_dwNumMatItem,
					&g_TexItem[nType].ItemTex[nType].g_pMeshItem);

				nType++;
			}
			else if (strcmp(aString, "END_SCRIPT") == 0)
			{
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
	fclose(pFile);
}
