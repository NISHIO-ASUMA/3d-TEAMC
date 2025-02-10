//==============================================================================================================
//
// �e[shadow.cpp]
// Author:YOSHIDA YUUTO
//
//==============================================================================================================

//**************************************************************************************************************
//�C���N���[�h�t�@�C��
//**************************************************************************************************************
#include"edit2d.h"
#include "player.h"
#include "camera.h"
#include "input.h"

//**************************************************************************************************************
//�}�N����`
//**************************************************************************************************************
#define MAX_EDIT (256)

//**************************************************************************************************************
//�O���[�o���ϐ��錾
//**************************************************************************************************************
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffEdit2d = NULL; //���_�o�b�t�@�ւ̃|�C���^
LPDIRECT3DTEXTURE9 g_pTextureEdit2d[POLYGON_TYPE_MAX] = {};//�e�N�X�`���ւ̃|�C���^�ۑ��p
EDIT_INFO_2D g_Edit2d[MAX_EDIT];//�e�̍\����
int g_EditCnt;
int g_ObjCount;

//===============================================================================================================
//�e�̏���������
//===============================================================================================================
void InitEdit2d(void)
{
	LPDIRECT3DDEVICE9 pDevice;

	pDevice = GetDevice();

	for (int nCntEdit2d = 0; nCntEdit2d < POLYGON_TYPE_MAX; nCntEdit2d++)
	{
		//�e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(pDevice,
			POLYGON_TYPE[nCntEdit2d],
			&g_pTextureEdit2d[nCntEdit2d]);
	}

	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4 * MAX_EDIT,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffEdit2d,
		NULL);

	VERTEX_3D* pVtx = NULL;

	for (int nCntEdit2d = 0; nCntEdit2d < MAX_EDIT; nCntEdit2d++)
	{
		g_Edit2d[nCntEdit2d].pos = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		g_Edit2d[nCntEdit2d].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_Edit2d[nCntEdit2d].fWidth = 100.0f;
		g_Edit2d[nCntEdit2d].fHeight = 100.0f;
		g_Edit2d[nCntEdit2d].fLength = 0.0f;
		g_Edit2d[nCntEdit2d].bUse = false;
		g_Edit2d[nCntEdit2d].fMove = 10.0f;
	}

	g_Edit2d[0].bUse = true;
	g_EditCnt = 0;
	g_ObjCount = 0;

	//���_�o�b�t�@�����b�N
	g_pVtxBuffEdit2d->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntEdit2d = 0; nCntEdit2d < MAX_EDIT; nCntEdit2d++)
	{
		//���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 50.0f);
		pVtx[1].pos = D3DXVECTOR3(50.0f, 0.0f, 50.0f);
		pVtx[2].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(50.0f, 0.0f, 0.0f);

		//�e���_�̖@���̐ݒ�(�x�N�g���̑傫���͂P�ɂ���K�v������)
		pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

		//���_�J���[
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		//�e�N�X�`�����W�̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4;
	}

	g_pVtxBuffEdit2d->Unlock();
}
//===============================================================================================================
//�e�̏I������
//===============================================================================================================
void UninitEdit2d(void)
{
	for (int nCntEdit2d = 0; nCntEdit2d < POLYGON_TYPE_MAX; nCntEdit2d++)
	{
		if (g_pTextureEdit2d[nCntEdit2d] != NULL)
		{
			g_pTextureEdit2d[nCntEdit2d]->Release();
			g_pTextureEdit2d[nCntEdit2d] = NULL;
		}
	}

	//���_�o�b�t�@�̉��
	if (g_pVtxBuffEdit2d != NULL)
	{
		g_pVtxBuffEdit2d->Release();
		g_pVtxBuffEdit2d = NULL;
	}
}
//===============================================================================================================
//�e�̍X�V����
//===============================================================================================================
void UpdateEdit2d(void)
{
	Camera* pCamera = GetCamera();

	VERTEX_3D* pVtx;

	//���_�o�b�t�@�����b�N
	g_pVtxBuffEdit2d->Lock(0, 0, (void**)&pVtx, 0);

	if (g_Edit2d[g_EditCnt].bUse)
	{
		//�ړ��ʂ̌���
		g_Edit2d[g_EditCnt].move.x += (0.0f - g_Edit2d[g_EditCnt].move.x) * 0.5f;
		g_Edit2d[g_EditCnt].move.y += (0.0f - g_Edit2d[g_EditCnt].move.y) * 0.5f;
		g_Edit2d[g_EditCnt].move.z += (0.0f - g_Edit2d[g_EditCnt].move.z) * 0.5f;

		//�ʒu�̍X�V
		g_Edit2d[g_EditCnt].pos += g_Edit2d[g_EditCnt].move;

		//��ړ�
		if (GetKeyboardPress(DIK_W))
		{
			g_Edit2d[g_EditCnt].move.x += sinf(pCamera->rot.y) * g_Edit2d[g_EditCnt].fMove;
			g_Edit2d[g_EditCnt].move.z += cosf(pCamera->rot.y) * g_Edit2d[g_EditCnt].fMove;
		}
		//���ړ�
		if (GetKeyboardPress(DIK_A))
		{
			g_Edit2d[g_EditCnt].move.x -= cosf(-pCamera->rot.y) * g_Edit2d[g_EditCnt].fMove;
			g_Edit2d[g_EditCnt].move.z -= sinf(-pCamera->rot.y) * g_Edit2d[g_EditCnt].fMove;
		}
		//���ړ�
		if (GetKeyboardPress(DIK_S))
		{
			g_Edit2d[g_EditCnt].move.x -= sinf(pCamera->rot.y) * g_Edit2d[g_EditCnt].fMove;
			g_Edit2d[g_EditCnt].move.z -= cosf(pCamera->rot.y) * g_Edit2d[g_EditCnt].fMove;
		}
		//�E�ړ�
		if (GetKeyboardPress(DIK_D))
		{
			g_Edit2d[g_EditCnt].move.x += cosf(-pCamera->rot.y) * g_Edit2d[g_EditCnt].fMove;
			g_Edit2d[g_EditCnt].move.z += sinf(-pCamera->rot.y) * g_Edit2d[g_EditCnt].fMove;
		}

		//���ړ�
		if (GetKeyboardPress(DIK_Q))
		{
			g_Edit2d[g_EditCnt].move.y -= g_Edit2d[g_EditCnt].fMove;
		}
		//�E�ړ�
		if (GetKeyboardPress(DIK_E))
		{
			g_Edit2d[g_EditCnt].move.y += g_Edit2d[g_EditCnt].fMove;
		}

		if (KeyboardTrigger(DIK_RETURN))
		{
			g_Edit2d[g_EditCnt + 1] = g_Edit2d[g_EditCnt];
			g_Edit2d[g_EditCnt + 1].bUse = true;

			g_EditCnt++;
		}

		// �傫���ύX(�g��)
		if (GetKeyboardPress(DIK_RIGHT))
		{
			g_Edit2d[g_EditCnt].fWidth += 1.0f;
		}
		else if (GetKeyboardPress(DIK_UP))
		{
			g_Edit2d[g_EditCnt].fHeight += 1.0f;
		}

		// �傫���ύX(�k��)
		if (GetKeyboardPress(DIK_LEFT) && g_Edit2d[g_EditCnt].fWidth > 1.0f)
		{
			g_Edit2d[g_EditCnt].fWidth -= 1.0f;
		}
		else if (GetKeyboardPress(DIK_DOWN) && g_Edit2d[g_EditCnt].fHeight > 1.0f)
		{
			g_Edit2d[g_EditCnt].fHeight -= 1.0f;
		}

		// ��]Y
		if (GetKeyboardPress(DIK_Z))
		{
			g_Edit2d[g_EditCnt].rot.y -= 0.001f;
		}
		if (GetKeyboardPress(DIK_C))
		{
			g_Edit2d[g_EditCnt].rot.y += 0.001f;
		}

		// ��]X
		if (GetKeyboardPress(DIK_V))
		{
			g_Edit2d[g_EditCnt].rot.x -= 0.001f;
		}
		if (GetKeyboardPress(DIK_B))
		{
			g_Edit2d[g_EditCnt].rot.x += 0.001f;
		}

		// ��]Z
		if (GetKeyboardPress(DIK_F))
		{
			g_Edit2d[g_EditCnt].rot.z -= 0.001f;
		}
		if (GetKeyboardPress(DIK_G))
		{
			g_Edit2d[g_EditCnt].rot.z += 0.001f;
		}

		// �J������I�𒆂̃I�u�W�F�N�g�̏ꏊ�ֈړ�
		if (KeyboardTrigger(DIK_F6))
		{
			pCamera->posV.x = g_Edit2d[g_EditCnt].pos.x;
			pCamera->posV.z = g_Edit2d[g_EditCnt].pos.z;
		}

		//�u���b�N����
		if (KeyboardTrigger(DIK_BACKSPACE) && g_EditCnt > 0)
		{
			g_Edit2d[g_EditCnt].bUse = false; // �I�𒆂̃I�u�W�F�N�g������
			g_EditCnt--;                    // �I�u�W�F�N�g�̃J�E���g�����炷
		}

		if (GetKeyboardPress(DIK_O))
		{
			g_Edit2d[g_EditCnt].fMove += 0.1f; // �ړ��ʕύX
		}
		else if (GetKeyboardPress(DIK_I) && g_Edit2d[g_EditCnt].fMove > 0.0f)
		{
			g_Edit2d[g_EditCnt].fMove -= 0.1f; // �ړ��ʕύX
		}

		if (KeyboardTrigger(DIK_1)&& g_Edit2d[g_EditCnt].nType < POLYGON_TYPE_MAX - 1)
		{
			g_Edit2d[g_EditCnt].nType++;
		}
		else if (KeyboardTrigger(DIK_2) && g_Edit2d[g_EditCnt].nType > 0)
		{
			g_Edit2d[g_EditCnt].nType--;
		}

		pVtx += 4 * g_EditCnt;

		//���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(g_Edit2d[g_EditCnt].pos.x - g_Edit2d[g_EditCnt].fWidth,0.0f, g_Edit2d[g_EditCnt].pos.z + g_Edit2d[g_EditCnt].fHeight);
		pVtx[1].pos = D3DXVECTOR3(g_Edit2d[g_EditCnt].pos.x + g_Edit2d[g_EditCnt].fWidth, 0.0f, g_Edit2d[g_EditCnt].pos.z + g_Edit2d[g_EditCnt].fHeight);
		pVtx[2].pos = D3DXVECTOR3(g_Edit2d[g_EditCnt].pos.x - g_Edit2d[g_EditCnt].fWidth, 0.0f, g_Edit2d[g_EditCnt].pos.z - g_Edit2d[g_EditCnt].fHeight);
		pVtx[3].pos = D3DXVECTOR3(g_Edit2d[g_EditCnt].pos.x + g_Edit2d[g_EditCnt].fWidth, 0.0f, g_Edit2d[g_EditCnt].pos.z - g_Edit2d[g_EditCnt].fHeight);

		//�e�N�X�`�����W�̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
	}

	g_pVtxBuffEdit2d->Unlock();
}
//===============================================================================================================
//�e�̕`�揈��
//===============================================================================================================
void DrawEdit2d(void)
{
	LPDIRECT3DDEVICE9 pDevice;

	pDevice = GetDevice();

	//�v�Z�p�̃}�g���b�N�X
	D3DXMATRIX mtxRot, mtxTrans;

	for (int nCntEdit2d = 0; nCntEdit2d < MAX_EDIT; nCntEdit2d++)
	{
		if (!g_Edit2d[nCntEdit2d].bUse)
		{
			continue;
		}

		// ��ނ���
		int nType = g_Edit2d[nCntEdit2d].nType;

		//���[���h�}�g���b�N�X�̏�����
		D3DXMatrixIdentity(&g_Edit2d[nCntEdit2d].mtxWorld);

		//�����𔽉f
		D3DXMatrixRotationYawPitchRoll(&mtxRot, g_Edit2d[nCntEdit2d].rot.y, g_Edit2d[nCntEdit2d].rot.x, g_Edit2d[nCntEdit2d].rot.z);
		D3DXMatrixMultiply(&g_Edit2d[nCntEdit2d].mtxWorld, &g_Edit2d[nCntEdit2d].mtxWorld, &mtxRot);

		//�ʒu�𔽉f
		D3DXMatrixTranslation(&mtxTrans, g_Edit2d[nCntEdit2d].pos.x, g_Edit2d[nCntEdit2d].pos.y, g_Edit2d[nCntEdit2d].pos.z);
		D3DXMatrixMultiply(&g_Edit2d[nCntEdit2d].mtxWorld, &g_Edit2d[nCntEdit2d].mtxWorld, &mtxTrans);

		//���[���h�}�g���b�N�X�̐ݒ�
		pDevice->SetTransform(D3DTS_WORLD, &g_Edit2d[nCntEdit2d].mtxWorld);

		//���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���ɐݒ�
		pDevice->SetStreamSource(0, g_pVtxBuffEdit2d, 0, sizeof(VERTEX_3D));

		//�e�N�X�`���t�H�[�}�b�g�̐ݒ�
		pDevice->SetFVF(FVF_VERTEX_3D);

		//�e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_pTextureEdit2d[nType]);
		
		//�|���S���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntEdit2d * 4, 2);
	}
}
//========================================================================================================
// �����o������
//========================================================================================================
void SaveEdit2d(void)
{
	FILE* pFile;

	pFile = fopen("data\\save2d.txt", "w");

	if (pFile != NULL)
	{
		fwrite(&g_EditCnt, sizeof(int), 0, pFile);

		fprintf(pFile,

			"+==========================================+\n"
			"+	  �}�b�v�G�f�B�^�[ �쐬�� �g�c�S�l      +\n"
			"+==========================================+\n\n");

		for (int nCnt = 0; nCnt < g_EditCnt; nCnt++)
		{
			if (g_Edit2d[nCnt].bUse)
			{
				fprintf(pFile, "POLYGONSET\n");

				fprintf(pFile, "   TEX_TYPE = %d					# [ 2d�I�u�W�F�N�g�̎�� ]\n", g_Edit2d[nCnt].nType);

				fprintf(pFile, "   POS = %.1f %.1f %.1f				# [ �ʒu ]\n", g_Edit2d[nCnt].pos.x, g_Edit2d[nCnt].pos.y, g_Edit2d[nCnt].pos.z);

				fprintf(pFile, "   ROT = %.1f %.1f %.1f				# [ ���� ]\n", g_Edit2d[nCnt].rot.x, g_Edit2d[nCnt].rot.y, g_Edit2d[nCnt].rot.z);

				fprintf(pFile, "   WIDTH = %.1f						# [ ���� ]\n", g_Edit2d[nCnt].fWidth);

				fprintf(pFile, "   HEIGHT = %.1f					# [ ���� ]\n", g_Edit2d[nCnt].fHeight);

				fprintf(pFile, "END_POLYGONSET\n\n");
			}
		}

		fprintf(pFile, "OBJ_COUNT = %d    # [ 2d�I�u�W�F�N�g�̔z�u�� ]\n", g_EditCnt);
		fprintf(pFile, "END_SCRIPT          # ���̍s�͏����Ȃ��ł�������");
	}
	else
	{
		//���b�Z�[�W�{�b�N�X
		MessageBox(NULL, "�t�@�C�����J���܂���B", "�G���[(SaveEdit2d)", MB_OK);
		return;
	}
	fclose(pFile);

}
//========================================================================================================
// �����o������
//========================================================================================================
void LoadEdit2d(void)
{
	FILE* pFile;

	D3DXVECTOR3 pos, rot;
	int nType = 0;
	D3DXVECTOR2 fLength;
	char skip[3];

	pFile = fopen("data\\save2d.txt", "r");

	if (pFile != NULL)
	{
		char aString[256];

		while (1)
		{
			fscanf(pFile, "%s", &aString[0]);

			if (strcmp(aString, "POLYGONSET") == 0)
			{
				while (1)
				{
					fscanf(pFile, "%s", &aString[0]);

					if (strcmp(aString, "TEX_TYPE") == 0)
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
					else if (strcmp(aString, "ROT") == 0)
					{
						fscanf(pFile, "%s", &skip[0]);
						fscanf(pFile, "%f", &rot.x);
						fscanf(pFile, "%f", &rot.y);
						fscanf(pFile, "%f", &rot.z);
					}

					else if (strcmp(aString, "WIDTH") == 0)
					{
						fscanf(pFile, "%s", &skip[0]);
						fscanf(pFile, "%f", &fLength.x);
					}
					else if (strcmp(aString, "HEIGHT") == 0)
					{
						fscanf(pFile, "%s", &skip[0]);
						fscanf(pFile, "%f", &fLength.y);
					}
					else if (strcmp(aString, "END_POLYGONSET") == 0)
					{
						SetPolygon(pos, rot, fLength.x, fLength.y, nType);
						break;
					}
				}
			}
			if (strcmp(aString, "OBJ_COUNT") == 0)
			{
				fscanf(pFile, "%s", &skip[0]);
				fscanf(pFile, "%d", &g_ObjCount);
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
		MessageBox(NULL, "�t�@�C�����J���܂���B", "�G���[(LoadEdit2d)", MB_OK);
		return;
	}
	fclose(pFile);
}
//========================================================================================================
// �ēǂݍ��ݏ���
//========================================================================================================
void ReLoadEdit2d(void)
{
	FILE* pFile;

	D3DXVECTOR3 pos, rot;
	int nType = 0;
	D3DXVECTOR2 fLength;
	char skip[3];
	int nIdx = 0;

	pFile = fopen("data\\save2d.txt", "r");

	if (pFile != NULL)
	{
		char aString[256];

		while (1)
		{
			fscanf(pFile, "%s", &aString[0]);

			if (strcmp(aString, "POLYGONSET") == 0)
			{
				while (1)
				{
					fscanf(pFile, "%s", &aString[0]);

					if (strcmp(aString, "TEX_TYPE") == 0)
					{
						fscanf(pFile, "%s", &skip[0]);
						fscanf(pFile, "%d", &g_Edit2d[nIdx].nType);
					}
					else if (strcmp(aString, "POS") == 0)
					{
						fscanf(pFile, "%s", &skip[0]);
						fscanf(pFile, "%f", &g_Edit2d[nIdx].pos.x);
						fscanf(pFile, "%f", &g_Edit2d[nIdx].pos.y);
						fscanf(pFile, "%f", &g_Edit2d[nIdx].pos.z);
					}
					else if (strcmp(aString, "ROT") == 0)
					{
						fscanf(pFile, "%s", &skip[0]);
						fscanf(pFile, "%f", &g_Edit2d[nIdx].rot.x);
						fscanf(pFile, "%f", &g_Edit2d[nIdx].rot.y);
						fscanf(pFile, "%f", &g_Edit2d[nIdx].rot.z);
					}

					else if (strcmp(aString, "WIDTH") == 0)
					{
						fscanf(pFile, "%s", &skip[0]);
						fscanf(pFile, "%f", &g_Edit2d[nIdx].fWidth);
					}
					else if (strcmp(aString, "HEIGHT") == 0)
					{
						fscanf(pFile, "%s", &skip[0]);
						fscanf(pFile, "%f", &g_Edit2d[nIdx].fHeight);
					}
					else if (strcmp(aString, "END_POLYGONSET") == 0)
					{
						nIdx++;
						break;
					}
				}
			}
			if (strcmp(aString, "OBJ_COUNT") == 0)
			{
				fscanf(pFile, "%s", &skip[0]);
				fscanf(pFile, "%d", &g_ObjCount);
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
		MessageBox(NULL, "�t�@�C�����J���܂���B", "�G���[(ReLoadEdit2d)", MB_OK);
		return;
	}
	fclose(pFile);

	for (int nCnt = 0; nCnt < g_ObjCount; nCnt++)
	{
		g_Edit2d[g_EditCnt].bUse = true;

		if (nCnt != g_ObjCount - 1)
		{
			g_EditCnt++;
		}
	}
	g_Edit2d[g_EditCnt].bUse = true;
}
//======================================================================================================================
// �G�f�B�b�g2d�̎擾����
//======================================================================================================================
EDIT_INFO_2D* GetEditInfo2D(void)
{
	return &g_Edit2d[0];
}
//======================================================================================================================
// �z�u���̎擾
//======================================================================================================================
int GetNum2d(void)
{
	return g_EditCnt;
}

