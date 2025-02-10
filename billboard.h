//================================
//
// �r���{�[�h���� [billboard.h]
// Author: Asuma Nishio
//
//================================
#ifndef _BILLBOARD_H_
#define _BILLBOARD_H_

//***************************
// �C���N���[�h�t�@�C���錾
//***************************
#include "main.h"

//***************************
// �}�N����`
//***************************
#define MAX_BILLBOARD (50)    // �ő吔

//*****************************
// �r���{�[�h�̎�ނ̗񋓌^�錾
//*****************************
typedef enum
{
	BILLBOARDTYPE_FIRST = 0,
	BILLBOARDTYPE_MAX
}BILLBOARDTYPE;

//************************************
// �r���{�[�h�̃e�N�X�`���̗񋓌^�錾
//************************************
static const char* BILLBOARD_TEXTURE[BILLBOARDTYPE_MAX] = 
{
	"data\\TEXTURE\\billboard_wepon.png",
};

//***************************
// �r���{�[�h�\���̐錾
//***************************
typedef struct
{
	D3DXVECTOR3 pos;	 // �ʒu
	D3DXVECTOR3 rot;	 // ����
	D3DXVECTOR3 move;	 // �ړ���
	D3DXMATRIX  mtxWorld;// ���[���h�}�g���b�N�X
	bool bUse;			 // �g�p����
	int nType;		     // ���
	float fWidth;		 // ����
	float fHeight;		 // ����
}Billboard;

//**********************
// �v���g�^�C�v�錾
//**********************
void InitBillboard(void);		// �r���{�[�h����������
void UninitBillboard(void);		// �r���{�[�h�I������
void UpdateBillboard(void);		// �r���{�[�h�X�V����
void DrawBillboard(void);		// �r���{�[�h�`�揈��
void SetBillboard(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType, float fWidth, float fHeight); // �r���{�[�h�ݒ菈��
#endif 

