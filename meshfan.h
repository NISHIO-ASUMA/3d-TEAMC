//======================================
//
// ���b�V���̋��^�̏��� [mashfan.h]
// Author: Asuma Nishio
//
//======================================
#ifndef _MESHFAN_H_
#define _MESHFAN_H_

//***************************
// �C���N���[�h�t�@�C���錾
//***************************
#include "main.h"

//***************************
// �}�N����`
//***************************
#define meshwall_X_BLOCK 10 // X�����̃u���b�N��
#define meshwall_Z_BLOCK 10 // Z�����̃u���b�N��

#define meshwall_VERTEX_NUM ((meshwall_X_BLOCK + 1) * (meshwall_Z_BLOCK + 1)) // ���b�V���t�B�[���h�̒��_��

#define meshwall_INDEX_NUM (meshwall_X_BLOCK * meshwall_Z_BLOCK * 6) // ���b�V���t�B�[���h�̃C���f�b�N�X��

#define meshwall_PRIMITIVE_NUM ((meshwall_X_BLOCK * meshwall_Z_BLOCK * 2) + ((meshwall_Z_BLOCK - 1) * 2)) // ���b�V���t�B�[���h�̃v���~�e�B�u��

//***************************
// �v���g�^�C�v�錾
//***************************
void InitmeshFan(void);   // ����������
void UninitmeshFan(void); // �I������
void UpdatemeshFan(void); // �X�V����
void DrawmeshFan(void);   // �`�揈��

#endif // !_MESHFAN_H_

