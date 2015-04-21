#include "para.h"
#include "spi_flash.h"


int AD654F = 0 ;

int NewSecurity = 0;
int NewSecurityAgain = 0;
//��Ҫһ���ṹ�壬�����Ĳ��ֱ��޸���

//��Ҫһ����������������趨ֵ����ÿһ���趨ֵ�����߼��ϵļ��

int ParaArray[70]={0};
int ParaArray_copy[70]={0};//������FLASH��ȡ����֮���������һ����������������Ա��������飬ȷ���Ƿ��иı������ѯ��ʱ�򱣴��޸�


//���Ĳ���
//������Ʋ���
int	ValvePosValue = 0;		//��λ�������һλ��С��λ 
//ǰһ�εı�����ֵ
int	ValvePosValueLast = 0;		//��λ�������һλ��С��λ

int ValveSetExecute_ValveRec = 0 ; // ��λ������¼��ʼִ��Զ������ĳ�ʼ��λ

extern int tempmax , tempmin ;
extern float weight ;
extern int ValveOpenError,ValveCloseError;
extern float set_k;	

int ssr0_750msflag = 0;
int ssr1_750msflag = 0;

int ValveOpenLedFlag = 0;
int ValveCloseLedFlag = 0;

int PCMode = 0;		
int PCHoldOpenSig = 0;	//ά��ʽ�����ź�
int PCHoldCloseSig = 0;	//ά��ʽ�����ź�
int PCPluseOpenSig = 0;	//����ʽ�����ź�
int PCPluseCloseSig = 0;//����ʽ�����ź�
int PCPluseStopSig = 0 ;//����ʽ��ͣ�ź�
int PCAllOpenSig = 0;	//ȫ���ź�
int PCAllCLoseSig = 0;	//ȫ���ź�
int PCSetSig = 0;		//�趨 ģʽ
int PCSetValve = 0; 	//�趨ֵ

//������ʼ����Ӧ����ȥflash��ȡ���в���	  
void ParaInit(void)
{
	int i=0;
	SPI_FLASH_BufferRead( (u8*)ParaArray , FLASH_ReadAddress, ParaArrayLength*4 );
	//��ParaArray_copy[]����һ�����ݣ������Ա�
	for( i=0;i<ParaArrayLength ; i++ ){
		*(ParaArray_copy+i) = *(ParaArray+i);
	}
	if( ParaArray[SelfLearnRec] == NoHavedSelfLearn )
	{
		tempmax = ParaArray[SetValveMax];
		tempmin = ParaArray[SetValveMin];
		weight = 10000.0/((float)tempmax - (float)tempmin);
		//����Ҫ��ֻ��һ������ֵ
		if( weight < 0 )
			weight = 0-weight;
		//error1 = (int)(17.0*weight);
		//error2 = (int)(18.0*weight);
		ValveOpenError = (int)(19.0*weight);
		ValveCloseError = 0-(int)(19.0*weight);
	}else if(ParaArray[SelfLearnRec] == HavedSelfLearn ){
		ValveOpenError = ParaArray[OpenError];
		ValveCloseError = ParaArray[CloseError];	//����һ��λ��		�Ǹ�����
	}
	
	set_k = 1000.0/((float)ParaArray[Set20ma] - (float)ParaArray[Set4ma]);		
}

//����int�������Ƚϣ��������ͬ�򷵻�1�������ͬ�򷵻�0
int BufferCmp(int* pBuffer1, int* pBuffer2, int BufferLength)
{
	while(BufferLength--)
	{
		if(*pBuffer1 != *pBuffer2)
		{
		  return 1;
		}
		
		pBuffer1++;
		pBuffer2++;
	}
	return 0;
}

//-------------------------------------------------------------------------------------------------------
//WZ 2015-3-19 �����޸��˲������Լ������洢flash�ķ�ʽ
//һ��Ϊԭ���Ĳ���������ʽ�������޸�Ϊ��ϣ��ʽ
