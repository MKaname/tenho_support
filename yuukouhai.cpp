
#include "yuukouhai.h"


//��v�Z�b�g
void Yuukouhai::set_tehai(int t[])
{
	for(int i=0;i<38;i++){
		tehai[i]=t[i];
	}
	fuurosuu=0;
}

//�G���[�`�F�b�N
bool Yuukouhai::error_check()
{
	int tmp=0;
	for(int i=0;i<38;i++){
		tmp+=tehai[i];
	}
	if(tmp%3!=1||tmp>=14||tmp<1) return true;
	return false;
}


void Yuukouhai::NotKoritu ()
{
	int work[38];//��Ɨp
	int i=0,p=0;
	//�z�񏉊���
	memset(work,0,sizeof(work));
	memset(not_koritu,0,sizeof(not_koritu));
	//�v�Z
	for(;i<=37;i++){ 
		for(;!tehai[i];i++);if(i>=38) continue;
		//���v
		if(i<30){
			if(i%10==1) work[i]=work[i+1]=work[i+2]=1;
			else if(i%10==2) work[i]=work[i+1]=work[i+2]=work[i-1]=1;
			else if(i%10>=3 &&i%10<=7) work[i]=work[i-1]=work[i+1]=work[i-2]=work[i+2] =1;
			else if(i%10==8) work[i]=work[i+1]=work[i-2]=work[i-1]=1;
			else if(i%10==9) work[i]=work[i-1]=work[i-2]=1;             
		}
		//���v
		else if(i>30) work[i]=1;
	}
	//���ʂ��i�[
	for(i=0;i<=37;i++){ 
		for(;!work[i];i++);if(i>=38) continue;
				not_koritu[p++]=i;
	}
}

/*
* �ʏ��̗L���v
*/ 
bool Yuukouhai::NormalYuukou (vector<int>  NormalYuukou[])
{
	if(error_check()) return false;

	int p_koritu=0;		//�z��̓Y�����|�C���^�i�Ǘ��v�j
	int p_yuukou=0;		//�z��̓Y�����|�C���^�i�L���v�j
	int syanten=NormalSyanten();   	//���݂̔v�̃V�����e���� 
	NormalYuukou->clear();//������ 
	NotKoritu ();
	while(not_koritu[p_koritu] != 0){
		//�یǗ��v��ǉ�
		tehai[not_koritu[p_koritu]]++;
		//���ݒl�Ɣ�r
		if(NormalSyanten() <syanten)
			NormalYuukou->push_back(not_koritu[p_koritu]);
		//�ǉ������یǗ��v���}�C�i�X
		tehai[not_koritu[p_koritu]]--;
		p_koritu++;
	}
	return true;
}

/*
* �`�[�g�C�c�̗L���v
*/
bool Yuukouhai::TiitoiYuukou (vector<int>  TiitoiYuukou [])
{
	if(error_check()) return false;

	int i=0;//�z��̓Y�����|�C���^�i�Ǘ��v�j
	int syanten=TiitoituSyanten();  //���݂̔v�̃V�����e���� 
	TiitoiYuukou->clear();//������ 
	while(i<=37){
		//�یǗ��v��ǉ�
		for(;!tehai[i];i++);if(i>=38) continue;
		tehai[i]++;
		//���ݒl�Ɣ�r
		if(TiitoituSyanten() <syanten)
		TiitoiYuukou->push_back(i);
		//�ǉ������یǗ��v���}�C�i�X
		tehai[i]--;
		i++;
	}	
	return true;
}

/*
* ���m���o�̗L���v
*/ 
bool Yuukouhai::KokusiYuukou (vector<int>  KokusiYuukou [])
{
	if(error_check()) return false;

	int i=0;
	int syanten=KokusiSyanten();//���݂̔v�̃V�����e����
	KokusiYuukou->clear();//������ 
	int yaotyuu[13]={1,9,11,19,21,29,31,32,33,34,35,36,37};
	for(;i<13;i++){
		//�یǗ��v��ǉ�
		tehai[yaotyuu[i]]++;
		//���ݒl�Ɣ�r
		if(KokusiSyanten() <syanten)
			KokusiYuukou->push_back(yaotyuu[i]);
		//�ǉ������یǗ��v���}�C�i�X
		tehai[yaotyuu[i]]--;
	}
	return true;
}

//���m�V�����e��
int Yuukouhai::KokusiSyanten()
{
	int kokusi_toitu=0,syanten_kokusi=13,i;             
	//�V���v
	for(i=1;i<30;i++){        
		if(i%10==1||i%10==9||i%20==1||i%20==9){	   
			if(tehai[i])
				syanten_kokusi--;
			if(tehai[i] >=2 && kokusi_toitu==0)
				kokusi_toitu=1;	
		}
	}             
	//���v
	for(i=31;i<38;i++){      
		if(tehai[i]){	        
			syanten_kokusi--;
			if(tehai[i] >=2 && kokusi_toitu==0)
				kokusi_toitu=1;			            
		}
	}             
	//��
	syanten_kokusi-= kokusi_toitu;             
	return syanten_kokusi;
}

//�`�[�g�C�V�����e��
int Yuukouhai::TiitoituSyanten()
{
	int i=1,toitu=0,syurui=0,syanten_tiitoi;
	//�g�C�c���𐔂���
	for(;i<=37;i++){ 
		for(;!tehai[i];i++);
		if(i>=38) continue;
		syurui++;
		if(tehai[i] >=2)
		toitu++;
	}
	syanten_tiitoi=6-toitu;
	//�S���������l��
	if(syurui<7)
		syanten_tiitoi+= 7-syurui;
	return syanten_tiitoi;
}


//�ʏ��V�����e��
int Yuukouhai::NormalSyanten()
{
	//������
	mentu=0;
	toitu=0;
	kouho=0;
	temp=0;
	syanten_normal=8;
	for(int i=1;i<38;i++)
	{
		//�������o��
		if(2 <= tehai[i])
		{            
			toitu++;
			tehai[i] -= 2;
			mentu_cut(1);
			tehai[i] += 2;
			toitu--;
		}
	}
	mentu_cut(1);   //�������Ɖ��肵�Čv�Z

	return syanten_normal-fuurosuu*2;	//�ŏI�I�Ȍ���
}

//�����c�����o��
void Yuukouhai::mentu_cut(int i)
{
	for(;!tehai[i];i++);
	if(i>=38){taatu_cut(1);return;}//�����c�𔲂��I������̂Ń^�[�c������
	//�R�[�c
	if(tehai[i]>=3)
	{
		mentu++;
		tehai[i]-=3;
		mentu_cut(i);
		tehai[i]+=3;
		mentu--;
	}
	//�V�����c
	if(tehai[i+1]&&tehai[i+2]&&i<30)
	{
		mentu++;
		tehai[i]--,tehai[i+1]--,tehai[i+2]--;
		mentu_cut(i);
		tehai[i]++,tehai[i+1]++,tehai[i+2]++;
		mentu--;
	}
	//�����c�����Ɖ���
	mentu_cut(i+1);
}
//�^�[�c�����o��
void Yuukouhai::taatu_cut(int i)
{													   
	for(;!tehai[i];i++);
	if(i>=38) //�����o���I��
	{
		temp=8-mentu*2-kouho-toitu;
		if(temp<syanten_normal) { syanten_normal=temp; }
		return;
	}
	if(mentu+kouho+fuurosuu<4)
	{            
		//�g�C�c
		if(tehai[i]==2)
		{
			kouho++;
			tehai[i]-=2;
			taatu_cut(i);
			tehai[i]+=2;
			kouho--;
		}
	             
		//�y���`����or����������
		if(tehai[i+1]&&i<30)
		{
			kouho++;
			tehai[i]--,tehai[i+1]--;
			taatu_cut(i);
			tehai[i]++,tehai[i+1]++;
			kouho--;
		}
	             
		//�J���`����
		if(tehai[i+2]&&i<30&&i%10<=8)
		{
			kouho++;
			tehai[i]--,tehai[i+2]--;
			taatu_cut(i);
			tehai[i]++,tehai[i+2]++;
			kouho--;
		}
	}
	taatu_cut(i+1);
}
