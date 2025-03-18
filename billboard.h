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
#include "boss.h"

//***************************
// �}�N����`
//***************************
#define MAX_BILLBOARD (712)    // �ő吔

//*****************************
// �r���{�[�h�̎�ނ̗񋓌^�錾
//*****************************
typedef enum
{
	BILLBOARDSTATE_NOSET = 0,
	BILLBOARDSTATE_SET,
	BILLBOARDSTATE_MAX
}BILLBOARDSTATE;

//*****************************
// �r���{�[�h�̎�ނ̗񋓌^�錾
//*****************************
typedef enum
{
	BILLBOARDTYPE_FIRST = 0,
	BILLBOARDTYPE_SECOND,
	BILLBOARDTYPE_TARGET,
	BILLBOARDTYPE_MAX
}BILLBOARDTYPE;

//************************************
// �r���{�[�h�̃e�N�X�`���̗񋓌^�錾
//************************************
static const char* BILLBOARD_TEXTURE[BILLBOARDTYPE_MAX] = 
{
	"data\\TEXTURE\\billboard_wepon.png",
	"data\\TEXTURE\\billboard_Key_get.png",
	"data\\TEXTURE\\target.png",
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
	int state;           // �r���{�[�h�̏��
	int nIdx;            // �C���f�b�N�X�ԍ�
}Billboard;

//**********************
// �v���g�^�C�v�錾
//**********************
void InitBillboard(void);		// �r���{�[�h����������
void UninitBillboard(void);		// �r���{�[�h�I������
void UpdateBillboard(void);		// �r���{�[�h�X�V����
void DrawBillboard(void);		// �r���{�[�h�`�揈��
int SetBillboard(D3DXVECTOR3 pos,int nType, float fWidth, float fHeight, int state); // �r���{�[�h�̐ݒ菈��
void DeletIdxBillboard(int nIdx);
Billboard* GetBillBoard(); // �r���{�[�h�̏��擾
void SetPositiontarget(int nIdx,D3DXVECTOR3 pos); // �^�[�Q�b�g�̈ʒu�ݒ菈��
#endif 

