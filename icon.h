//=================================================================================================================
//
// ����̃A�C�R������ [icon.h]
// Author: Asuma Nishio
//
//=================================================================================================================
#ifndef _ICON_H_
#define _ICON_H_

//****************************
// �C���N���[�h�t�@�C��
//****************************
#include "main.h"

//****************************
// �A�C�R���̎�ނ̗񋓌^�錾
//****************************
typedef enum
{
	WEPONTYPE_BAT = 0, // �o�b�g
	WEPONTYPE_GOLF,	   // �S���t
	WEPONTYPE_HUNMER,  // �n���}�[
	WEPONTYPE_STONE,   // ��
	WEPONTYPE_WOOD,	   // ��
	WEPONTYPE_STONEBAT,// �΃o�b�g
	WEPONTYPE_LIGHT,   // �u����
	WEPONTYPE_LIGHTWOOD,// �u�����\�[�h
	WEPONTYPE_HARISEN,	// �n���Z��
	WEPONTYPE_ICEBLOCK,	// �X

	WEPONTYPE_ICEBLOCKSOWRD, // �X�̌�
	WEPONTYPE_IRON,			 // �S�p�C�v
	WEPONTYPE_IRONBAT,		 // �����o�b�g
	WEPONTYPE_SURFBOARD,	 // �T�[�t�{�[�h
	WEPONTYPE_TORCH,		 // �g�[�`
	WEPONTYPE_TORCHSWORD,	 // �g�[�`�\�[�h
	WEPONTYPE_BAR,			 // �o�[��
	WEPONTYPE_HEADSTATUE,	 // ����
	WEPONTYPE_HEADSTATUTORSO,// ���ƃg���\�[�̍���
	WEPONTYPE_MEGAPHONE,	 // ���K�z��
	WEPONTYPE_RUBBERCUP,	 // �����ۂ�(���o�[�J�b�v)

	WEPONTYPE_TELEPHONEPOLE, // �d��
	WEPONTYPE_TORSO,		 // �g���\�[
	WEPONTYPE_FLUORESCENTLIGHTMEGAPHONE, // ���K�z���̍�������
	WEPONTYPE_BONESPEAR,	 // ���̑�
	WEPONTYPE_FISH,			 // ��
	WEPONTYPE_HEX,			 // ����
	WEPONTYPE_HEXMANDOLIN,	 // �n�[�v (�����̍�������)
	WEPONTYPE_SURFBOARDFISH, // ���ƃT�[�t�{�[�h�̍���
	WEPONTYPE_TUTORIAL,		 // �`���[�g���A���Ŕ�
	WEPONTYPE_MAX
}WEPON_TYPE;

//****************************
// �A�C�R���̃e�N�X�`���錾
//****************************
static const char* ICON_TEXTURE[WEPONTYPE_MAX] =
{
	"data\\TEXTURE\\bat.png", // �o�b�g
	"data\\TEXTURE\\golf.png",// �S���t
	"data\\TEXTURE\\hunmer.png",// �n���}�[
	"data\\TEXTURE\\stone.png", // ��
	"data\\TEXTURE\\wood.png",	// ��
	"data\\TEXTURE\\stonebat.png", // �΃o�b�g
	"data\\TEXTURE\\fluorescentlight.png", // �u����
	"data\\TEXTURE\\fluorescentlightsword.png", // �u�����\�[�h
	"data\\TEXTURE\\harisen.png", // �n���Z��
	"data\\TEXTURE\\iceblock.png", // �X

	"data\\TEXTURE\\iceblocksword.png", // �X�̌�
	"data\\TEXTURE\\iron.png",			// �S�p�C�v
	"data\\TEXTURE\\ironbat.png",		// �����o�b�g
	"data\\TEXTURE\\surfboard.png",		// �T�[�t�{�[�h
	"data\\TEXTURE\\torch.png",			// �g�[�`
	"data\\TEXTURE\\torchsword.png",	// �g�[�`�\�[�h
	"data\\TEXTURE\\bar.png",			// �o�[��
	"data\\TEXTURE\\headstatue.png",	// ����
	"data\\TEXTURE\\headstatutorso.png",// �g���\�[�Ɠ��̍���
	"data\\TEXTURE\\megaphone.png",		// ���K�z��
	"data\\TEXTURE\\rubbercup.png",		// ���o�[�J�b�v

	"data\\TEXTURE\\telephonepole.png", // �d��
	"data\\TEXTURE\\torso.png",			// �g���\�[
	"data\\TEXTURE\\fluorescentlightmegaphone.png", // ���K�z���̍�������
	"data\\TEXTURE\\bonespear.png",		// ���̑�
	"data\\TEXTURE\\fish.png",			// ��
	"data\\TEXTURE\\hex.png",			// ����
	"data\\TEXTURE\\hexmandolin.png",	// �n�[�v
	"data\\TEXTURE\\surfboardfish.png", // �T�[�t�{�[�h�̍�������
	"data\\TEXTURE\\tutorial.png",		// �`���[�g���A���̊Ŕ�
};

//****************************
// �A�C�R���̍\���̐錾
//****************************
typedef struct
{
	D3DXVECTOR3 pos; // ���W
	D3DXVECTOR3 move;// �ړ���
	int nType;		 // ���
	float fWidth;	 // ����
	float fHeight;	 // ����
	bool bUse;		 // �g�p���Ă��邩
}ICON;

//****************************
// �v���g�^�C�v�錾
//****************************
void InitIcon();	// ����������
void UninitIcon();	// �I������
void UpdateIcon();	// �X�V����
void DrawIcon();	// �`�揈��
void SetIcon(D3DXVECTOR3 pos, float fWidth,float fHeight,int nType); // �ݒ菈��

#endif