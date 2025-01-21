//============================
//
// edit [edit.cpp]
// Author:YOSHIDA YUUTO
//
//============================
 
//***********************
//�C���N���[�h�t�@�C��
//***********************
#include "edit.h"
#include "input.h"
#include "block.h"
#include "meshfield.h"
#include "camera.h"
#include "light.h"
#include <stdbool.h>
#include <stdio.h>
#include "fade.h"
#include "game.h"
#include "mouse.h"

//****************************
//�}�N����`
//****************************
#define OBJ_MOVE (10.0f) // �I�u�W�F�N�g�̈ړ���
#define MAX_WORD (128) // ������

//****************************
//�O���[�o���ϐ�
//****************************
EDIT_INFO g_Edit[MAX_BLOCK];			// �u���b�N�̏��
BLOCKTEX g_BlockTexInfo[BLOCKTYPE_MAX]; // �u���b�N�̃e�N�X�`�����ۑ��p�ϐ�
int g_EditCount,nCntobj;				// �I�u�W�F�N�g�̃J�E���g�ۑ��p�ϐ�
int g_LoadCnt;							// ���[�h����t�@�C����؂�ւ���ϐ�
int g_nNumBlock;						// �ݒu�����I�u�W�F�N�g�̐�

//===========================
//�G�f�B�b�g��ʂ̏���������
//===========================
void InitEdit(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	for (int nCnt = 0; nCnt < MAX_BLOCK; nCnt++)
	{
		g_Edit[nCnt].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_Edit[nCnt].Scal = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
		g_Edit[nCnt].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_Edit[nCnt].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_Edit[nCnt].bUse = false;
		g_Edit[nCnt].ObjCnt = 0;
		g_Edit[nCnt].nEditMode = EDITMODE_BLOCK;
		g_Edit[nCnt].nBlockTypeNumber = 0;
	}

	g_nNumBlock = 0;
	nCntobj = 0;

	for (int nCntNum = 0; nCntNum < BLOCKTYPE_MAX; nCntNum++)
	{
		//X�t�@�C���̓ǂݍ���
		D3DXLoadMeshFromX(MODELTYPE[nCntNum],
			D3DXMESH_SYSTEMMEM,
			pDevice,
			NULL,
			&g_BlockTexInfo[nCntNum].g_pBuffMatBlock,
			NULL,
			&g_BlockTexInfo[nCntNum].g_dwNumMatBlock,
			&g_BlockTexInfo[nCntNum].g_pMeshBlock);

		D3DXMATERIAL* pMat;//�}�e���A���ւ̃|�C���^

		//�}�e���A���̃f�[�^�ւ̃|�C���^���擾
		pMat = (D3DXMATERIAL*)g_BlockTexInfo[nCntNum].g_pBuffMatBlock->GetBufferPointer();

		for (int nCntMat = 0; nCntMat < (int)g_BlockTexInfo[nCntNum].g_dwNumMatBlock; nCntMat++)
		{
			if (pMat[nCntMat].pTextureFilename != NULL)
			{
				//���̃t�@�C�������g�p���ăe�N�X�`����ǂݍ���
					//�e�N�X�`���̓ǂݍ���
				D3DXCreateTextureFromFile(pDevice,
					pMat[nCntMat].pTextureFilename,
					&g_BlockTexInfo[nCntNum].g_apTextureBlock[nCntMat]);
			}
		}
	}

	g_Edit[0].bUse = true;						// 0�Ԗڂ��g�p��Ԃɂ���
	g_Edit[0].ModelTex[0] = g_BlockTexInfo[0];  // ���0�Ԗڂ̃e�N�X�`������
	g_EditCount = 0;							// �I�u�W�F�N�g�̃J�E���g������
	g_LoadCnt = 0;								// ���[�h�̃J�E���g������
	nCntobj = 0;								// �I�u�W�F�N�g�̃J�E���g������
}

//===========================
//�G�f�B�b�g��ʂ̏I������
//===========================
void UninitEdit(void)
{
	for (int nCnt = 0; nCnt < MAX_BLOCK; nCnt++)
	{
		for (int nCntNum = 0; nCntNum < BLOCKTYPE_MAX; nCntNum++)
		{
			for (int TexCnt = 0; TexCnt < MAX_TEX; TexCnt++)
			{
				//�e�N�X�`���̔j��
				if (g_Edit[nCnt].ModelTex[nCntNum].g_apTextureBlock[TexCnt] != NULL)
				{
					g_Edit[nCnt].ModelTex[nCntNum].g_apTextureBlock[TexCnt]->Release();
					g_Edit[nCnt].ModelTex[nCntNum].g_apTextureBlock[TexCnt] = NULL;
				}
			}

			//�}�e���A���̔j��
			if (g_Edit[nCnt].ModelTex[nCntNum].g_pBuffMatBlock != NULL)
			{
				g_Edit[nCnt].ModelTex[nCntNum].g_pBuffMatBlock->Release();
				g_Edit[nCnt].ModelTex[nCntNum].g_pBuffMatBlock = NULL;
			}
			//���b�V���̔j��
			if (g_Edit[nCnt].ModelTex[nCntNum].g_pMeshBlock != NULL)
			{
				g_Edit[nCnt].ModelTex[nCntNum].g_pMeshBlock->Release();
				g_Edit[nCnt].ModelTex[nCntNum].g_pMeshBlock = NULL;
			}
		}
	}
}

//===========================
//�G�f�B�b�g��ʂ̍X�V����
//===========================
void UpdateEdit(void)
{
	Camera* pCamera = GetCamera();

	static int nLoad = 0; // �����[�h�񐔂𐧌�����ϐ�

	if (g_Edit[g_EditCount].bUse)
	{
		//�ړ��ʂ̌���
		g_Edit[g_EditCount].move.x += (0.0f - g_Edit[g_EditCount].move.x) * 0.5f;
		g_Edit[g_EditCount].move.z += (0.0f - g_Edit[g_EditCount].move.z) * 0.5f;

		//�ʒu�̍X�V
		g_Edit[g_EditCount].pos += g_Edit[g_EditCount].move;

		//��ړ�
		if (GetKeyboardPress(DIK_W))
		{
			g_Edit[g_EditCount].move.x += sinf(pCamera->rot.y) * OBJ_MOVE;
			g_Edit[g_EditCount].move.z += cosf(pCamera->rot.y) * OBJ_MOVE;
		}
		//���ړ�
		if (GetKeyboardPress(DIK_A))
		{
			g_Edit[g_EditCount].move.x -= cosf(-pCamera->rot.y) * OBJ_MOVE;
			g_Edit[g_EditCount].move.z -= sinf(-pCamera->rot.y) * OBJ_MOVE;
		}
		//���ړ�
		if (GetKeyboardPress(DIK_S))
		{
			g_Edit[g_EditCount].move.x -= sinf(pCamera->rot.y) * OBJ_MOVE;
			g_Edit[g_EditCount].move.z -= cosf(pCamera->rot.y) * OBJ_MOVE;
		}
		//�E�ړ�
		if (GetKeyboardPress(DIK_D))
		{
			g_Edit[g_EditCount].move.x += cosf(-pCamera->rot.y) * OBJ_MOVE;
			g_Edit[g_EditCount].move.z += sinf(-pCamera->rot.y) * OBJ_MOVE;
		}

		//�z�u
		if (KeyboardTrigger(DIK_RETURN))
		{
			g_Edit[g_EditCount + 1].pos = g_Edit[g_EditCount].pos;
			g_Edit[g_EditCount + 1].bUse = true;
			g_Edit[g_EditCount + 1].ModelTex[0] = g_BlockTexInfo[0];
			g_nNumBlock++;
			g_EditCount++;
		}

		//�u���b�N����
		if (KeyboardTrigger(DIK_BACKSPACE)&& g_EditCount > 0)
		{
			g_Edit[g_EditCount].bUse = false;
			g_EditCount--;
			g_nNumBlock--;
		}

		//��ޕύX
		if (KeyboardTrigger(DIK_F) && g_Edit[g_EditCount].nType < BLOCKTYPE_MAX - 1)
		{
			g_Edit[g_EditCount].nType++;
			g_Edit[g_EditCount].ModelTex[g_Edit[g_EditCount].nType] = g_BlockTexInfo[g_Edit[g_EditCount].nType];
		}
		else if (KeyboardTrigger(DIK_G) && g_Edit[g_EditCount].nType > 0)
		{
			g_Edit[g_EditCount].nType--;
			g_Edit[g_EditCount].ModelTex[g_Edit[g_EditCount].nType] = g_BlockTexInfo[g_Edit[g_EditCount].nType];
		}

		if (KeyboardTrigger(DIK_V))
		{
			g_Edit[g_EditCount].Scal *= 1.1f;
		}
		if (KeyboardTrigger(DIK_B))
		{
			g_Edit[g_EditCount].Scal *= 0.9f;
		}
	}

	BLOCK* pBlock = GetBlock();

	if (KeyboardTrigger(DIK_F7))
	{
		SaveEdit();
	}

	else if (KeyboardTrigger(DIK_F8) && nLoad == 0)
	{
		nLoad = 1;
		ReLoadEdit();
	}

	if (KeyboardTrigger(DIK_F2) && nLoad == 1)
	{
		nLoad = 0;
	}
}

//===========================
//�G�f�B�b�g��ʂ̕`�揈��
//===========================
void DrawEdit(void)
{
	MODE mode = GetMode();
	EDIT_INFO* pEdit = GetEdit();

	LPDIRECT3DDEVICE9 pDevice = GetDevice();;

	//�v�Z�p�̃}�g���b�N�X
	D3DXMATRIX mtxRot, mtxTrans, mtxScal;

	D3DMATERIAL9 matDef;//���݂̃}�e���A���ۑ��p

	D3DXMATERIAL* pMat;//�}�e���A���f�[�^�ւ̃|�C���^

	for (int nCntBlock = 0; nCntBlock < MAX_BLOCK; nCntBlock++)
	{
		for (int nCntNum = 0; nCntNum < BLOCKTYPE_MAX; nCntNum++)
		{
			if (!g_Edit[nCntBlock].bUse)
			{
				continue;
			}

			int nType = g_Edit[nCntBlock].nType;

			//���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&g_Edit[nCntBlock].mtxWorld);

			//�����𔽉f
			D3DXMatrixScaling(&mtxScal, g_Edit[nCntBlock].Scal.y, g_Edit[nCntBlock].Scal.x, g_Edit[nCntBlock].Scal.z);
			D3DXMatrixMultiply(&g_Edit[nCntBlock].mtxWorld, &g_Edit[nCntBlock].mtxWorld, &mtxScal);

			//�����𔽉f
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_Edit[nCntBlock].rot.y, g_Edit[nCntBlock].rot.x, g_Edit[nCntBlock].rot.z);
			D3DXMatrixMultiply(&g_Edit[nCntBlock].mtxWorld, &g_Edit[nCntBlock].mtxWorld, &mtxRot);

			//�ʒu�𔽉f
			D3DXMatrixTranslation(&mtxTrans, g_Edit[nCntBlock].pos.x, g_Edit[nCntBlock].pos.y, g_Edit[nCntBlock].pos.z);
			D3DXMatrixMultiply(&g_Edit[nCntBlock].mtxWorld, &g_Edit[nCntBlock].mtxWorld, &mtxTrans);

			//���[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &g_Edit[nCntBlock].mtxWorld);

			//���݂̃}�e���A�����擾
			pDevice->GetMaterial(&matDef);

			for (int nCntMat = 0; nCntMat < (int)g_Edit[nCntBlock].ModelTex[nType].g_dwNumMatBlock; nCntMat++)
			{
				D3DXMATERIAL color;

				//�}�e���A���̃f�[�^�ւ̃|�C���^���擾
				pMat = (D3DXMATERIAL*)g_Edit[nCntBlock].ModelTex[nType].g_pBuffMatBlock->GetBufferPointer();

				//�I�𒆂̃u���b�N��������
				if (nCntBlock == g_EditCount)
				{
					color = pMat[nCntMat];

					color.MatD3D.Diffuse.r = 1.0f;
					color.MatD3D.Diffuse.g = 0.0f;
					color.MatD3D.Diffuse.b = 0.0f;
					color.MatD3D.Diffuse.a = 1.0f;

					//�}�e���A���̐ݒ�
					pDevice->SetMaterial(&color.MatD3D);
				}
				else
				{
					//�}�e���A���̐ݒ�
					pDevice->SetMaterial(&pMat[nCntMat].MatD3D);
				}

				//�e�N�X�`���̐ݒ�
				pDevice->SetTexture(0, g_Edit[nCntBlock].ModelTex[nType].g_apTextureBlock[nCntMat]);

				//�u���b�N(�p�[�c)�̕`��
				g_Edit[nCntBlock].ModelTex[nType].g_pMeshBlock->DrawSubset(nCntMat);
			}
			
		}
	}
	pDevice->SetMaterial(&matDef);
}

//===========================
//�G�f�B�b�g��ʂ̃Z�[�u����
//===========================
void SaveEdit(void)
{
	FILE* pFile;

	pFile = fopen("data\\saveEdit.txt", "w");

	if (pFile != NULL)
	{
		fwrite(&g_EditCount, sizeof(int), 0, pFile);

		fprintf(pFile,

			"+==========================================+\n"
			"+			   �}�b�v�G�f�B�^�[             +\n"
			"+==========================================+\n\n");

		for (int nCnt = 0; nCnt < g_EditCount; nCnt++)
		{
			if (g_Edit[nCnt].bUse)
			{
				fprintf(pFile, "BLOCKSET\n");

				fprintf(pFile, "   BLOCKTYPE = %d\n", g_Edit[nCnt].nType);

				fprintf(pFile, "   POS = %.1f %.1f %.1f\n",g_Edit[nCnt].pos.x,g_Edit[nCnt].pos.y, g_Edit[nCnt].pos.z);

				fprintf(pFile, "   SIZE = %.1f %.1f %.1f\n", g_Edit[nCnt].Scal.x, g_Edit[nCnt].Scal.y, g_Edit[nCnt].Scal.z);

				fprintf(pFile, "END_BLOCKSET\n\n");
			}
		}

		fprintf(pFile, "BLOCK_COUNT = %d \n", g_EditCount);
		fprintf(pFile, "END_SCRIPT");
	}
	else
	{
		return;
	}
	fclose(pFile);
}

//===========================
//�G�f�B�b�g��ʂ̃��[�h����
//===========================
void LoadEdit(void)
{
	FILE* pFile;
	int nType;
	D3DXVECTOR3 pos;
	D3DXVECTOR3 move;

	float fWidth;
	float fHeight;
	char skip[5];
	fWidth = 0.0f;
	fHeight = 0.0f;

	nType = 0;

	move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 Scal(0.0f, 0.0f, 0.0f);

	//switch (g_LoadCnt)
	//{
	//case 0:
	//	//�t�@�C�����J��
	//	pFile = fopen("data\\save.txt,","r");
	//	break;
	//default:
	//	pFile = NULL;
	//	break;
	//}

	pFile = fopen("data\\saveEdit.txt", "r");

	if (pFile != NULL)
	{
		while (1)
		{
			char aString[MAX_WORD];

			fscanf(pFile, "%s", &aString[0]);

			if (strcmp(aString, "BLOCKSET") == 0)
			{
				while (1)
				{
					fscanf(pFile, "%s", &aString[0]);

					if (strcmp(aString, "BLOCKTYPE") == 0)
					{
						fscanf(pFile, "%s", &skip[0]);
						fscanf(pFile, "%d", &nType);
					}
					else if (strcmp(aString, "POS") == 0)
					{
						fscanf(pFile, "%s", &skip[0]);
						fscanf(pFile, "%f", &pos.x);
						fscanf(pFile, "%f", &pos.y);
						fscanf(pFile, "%f", &pos.z);
					}
					else if (strcmp(aString, "SIZE") == 0)
					{
						fscanf(pFile, "%s", &skip[0]);
						fscanf(pFile, "%f", &Scal.x);
						fscanf(pFile, "%f", &Scal.y);
						fscanf(pFile, "%f", &Scal.z);
					}
					else if (strcmp(aString, "END_BLOCKSET") == 0)
					{
						SetBlock(pos,nType, Scal);
						break;
					}
				}
			}

			if (strcmp(aString, "BLOCK_COUNT") == 0)
			{
				fscanf(pFile, "%s", &skip[0]);
				fscanf(pFile, "%d", &nCntobj);
			}
			else if (strcmp(aString, "END_SCRIPT") == 0)
			{
				break;
			}
		}
		/*g_Edit++;*/

	}
	else
	{//�J���Ȃ������Ƃ�	
		return;
	}
	fclose(pFile);
}
//===========================
//�G�f�B�b�g�̃����[�h
//===========================
void ReLoadEdit(void)
{
	FILE* pFile;
	int nType;
	D3DXVECTOR3 pos;
	D3DXVECTOR3 move;

	int nIdx = 0;
	float fWidth;
	float fHeight;
	char skip[5];
	fWidth = 0.0f;
	fHeight = 0.0f;

	nType = 0;

	move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 Scal(0.0f, 0.0f, 0.0f);

	//switch (g_LoadCnt)
	//{
	//case 0:
	//	//�t�@�C�����J��
	//	pFile = fopen("data\\save.txt,","r");
	//	break;
	//default:
	//	pFile = NULL;
	//	break;
	//}

	pFile = fopen("data\\saveEdit.txt", "r");

	if (pFile != NULL)
	{
		while (1)
		{
			char aString[MAX_WORD];

			fscanf(pFile, "%s", &aString[0]);

			if (strcmp(aString, "BLOCKSET") == 0)
			{
				while (1)
				{
					fscanf(pFile, "%s", &aString[0]);

					if (strcmp(aString, "BLOCKTYPE") == 0)
					{
						fscanf(pFile, "%s", &skip[0]);
						fscanf(pFile, "%d", &g_Edit[nIdx].nType);
					}
					else if (strcmp(aString, "POS") == 0)
					{
						fscanf(pFile, "%s", &skip[0]);
						fscanf(pFile, "%f", &g_Edit[nIdx].pos.x);
						fscanf(pFile, "%f", &g_Edit[nIdx].pos.y);
						fscanf(pFile, "%f", &g_Edit[nIdx].pos.z);
					}
					else if (strcmp(aString, "SIZE") == 0)
					{
						fscanf(pFile, "%s", &skip[0]);
						fscanf(pFile, "%f", &g_Edit[nIdx].Scal.x);
						fscanf(pFile, "%f", &g_Edit[nIdx].Scal.y);
						fscanf(pFile, "%f", &g_Edit[nIdx].Scal.z);
					}
					else if (strcmp(aString, "END_BLOCKSET") == 0)
					{
						nIdx++;
						break;
					}
				}
			}

			if (strcmp(aString, "BLOCK_COUNT") == 0)
			{
				fscanf(pFile, "%s", &skip[0]);
				fscanf(pFile, "%d", &nCntobj);
			}
			else if (strcmp(aString, "END_SCRIPT") == 0)
			{
				break;
			}
		}
		/*g_Edit++;*/

	}
	else
	{//�J���Ȃ������Ƃ�	
		return;
	}
	fclose(pFile);

	for (int nCnt = 0; nCnt < nCntobj; nCnt++)
	{
		g_Edit[g_EditCount + 1].bUse = true;
		g_Edit[g_EditCount + 1].ModelTex[0] = g_BlockTexInfo[0];
		g_EditCount++;
		g_nNumBlock++;
	}

}

//===========================
//�G�f�B�b�g�K��
//===========================
EDIT_INFO* GetEdit(void)
{
	return &g_Edit[0];
}
//===========================
//�I�u�W�F�N�g�̎擾
//===========================
int GetNumobj(void)
{
	return g_nNumBlock;
}
