#ifndef ATOM_INCLUDE
#define ATOM_INCLUDE

float AD654ToCurrent(void);

void AD654Read(void);

void ValveSetShow(void);

//���Ͻǵ�ʵʱ��ʾ��λ����
void ValvePosShow(void);

void ValveErrorShow(void);

//��ʾ����  �ڵ�����
void ParaShow(int x);

//ҳ��3area0 ������������  ������λ���г�
void page3area0atom0op0(void);
//ҳ��3area0 ������������  ������λ��ת��
void page3area0atom0op1(void);

//ҳ��3area1 ������������  ������λ���г�
void page3area0atom1op0(void);

//ҳ��3area1 ������������  ������λ��ת��
void page3area0atom1op1(void);
//ҳ��3area2 ������������   �м�λ�ã�������û��ѡ��ģ�Ӧ����һ���������뺯��
void page3area0atom2op0(void);
void page3area0atom2inop(void);

//ҳ��3area3 ������������   ��������ת�أ�������û��ѡ��ģ�Ӧ����һ���������뺯��
void page3area0atom3inop(void);

//ҳ��3area4 ������������   ��������ת�أ�������û��ѡ��ģ�Ӧ����һ���������뺯��
void page3area0atom4inop(void);

//ҳ��3area5 ������������   �������Ʒ�ʽ����λ
void page3area0atom5op0(void);
//ҳ��3area5 ������������   �������Ʒ�ʽ��ת��λ��
void page3area0atom5op1(void);


//ҳ��3area1 ���Ʋ������� ���͵�  ������Ӧ��Ҫ����subarea
void page3area1atom0op0(void);
//�͵ؿ��Ʋ������ã�ѡΪ����ʽ
void page3area1atom0sub0op0(void);
//�͵ؿ��Ʋ������ã�ѡΪά��ʽ
void page3area1atom0sub0op1(void);


//ҳ��3area1 ���Ʋ������� ��Զ��
void page3area1atom0op1(void);

//Զ�̿��Ʋ������ã�ѡΪ����ʽ
void page3area1atom0sub1op0(void);
//Զ�̿��Ʋ���ѡ��ѡΪά��ʽ
void page3area1atom0sub1op1(void);
//Զ�̿��Ʋ���ѡ��ѡΪ˫λʽ
void page3area1atom0sub1op2(void);
//Զ�̿��Ʋ���ѡ��ѡΪģ����ʽ
void page3area1atom0sub1op3(void);


//ҳ��3area2 I/O��������    �����ź�1������
void page3area2atom0op0(void);
//ҳ��3area2 I/O��������    �����ź�1������
void page3area2atom0op1(void);

//ҳ��3area2 I/O��������    �����ź�1����ʾ  12��������Ϣ
void page3area2atom1op(void);
void page3area2atom2op(void);
void page3area2atom3op(void);
void page3area2atom4op(void);
void page3area2atom5op(void);
void page3area2atom6op(void);
void page3area2atom7op(void);
void page3area2atom8op(void);
void page3area2atom9op(void);
void page3area2atom10op(void);
void page3area2atom11op(void);
void page3area2atom12op(void);

//�ն�
void page3area3atom0op(void);
void page3area3atom1op(void);


void page3area4atom0op(void);
void page3area4atom1op(void);
//ҳ��3area3 �궨����       �궨���� �趨ֵ8ma 
void page3area4atom2op(void);
//ҳ��3area3 �궨����       �궨���� �趨ֵ16ma    
void page3area4atom3op(void);

//ҳ��3area3atom4op �궨����  �趨��ǰλ��Ϊ��λ0%
void page3area3atom4op(void);


//ҳ��3area3op5 �궨����  �趨��ǰλ��Ϊ��λ100%  
void page3area3atom5op(void); 

//ҳ��3area4 ��������       �ָ��������ã����ָ�
void page3area4atom0op0(void);
//ҳ��3area4 ��������       �ָ��������ã��ָ� 
void page3area4atom0op1(void);
//ҳ��3area4 ��������       ��������������
void page3area4atom1op0(void);
//ҳ��3area4 ��������       ������������ 
void page3area4atom1op1(void);

//ҳ��3area5 ��������       �޸����룺���޸� 
void page3area5atom2op0(void);
//ҳ��3area5 ��������       �޸����룺�޸� 
void page3area5atom2op1(void);

//������������Ľ�����ѧϰģʽ
void page3area5atom3op(void);
void page3area5atom0op0(void); //�������˳�

#endif
