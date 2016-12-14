

#include "math.h"

const float SAMPINGRATE = 200000;
const float SAMPINGTIME = 1/SAMPINGRATE;
const float NOTZERO = 0.00001f;
const int CHANNEL = 8;
const int NOISECHECKTIME =(int)(5 * SAMPINGRATE);
const int TABLELEN = 2048;
const long long int INF = 10000000000000;
const float PI2 = 3.1415926f * 2;
const float PI = 3.1415926f;
const int SAMPLETIMES = 100;

typedef struct SetPara
{
	int   iTestMode;             //����ģʽ
	int   iRunMode;              //����ģʽ
	int   iTestSweeps;           //����ɨƵ����
	float fTestTime;             //������ʱ��
	int   iTestCycles;           //����������
	float fMaxFreq;              //Ƶ������
	float fMinFreq;              //Ƶ������
	float fSweepRate;            //ɨ����
	float fSweepTime;            //����ɨƵʱ��
	float fDwellFreq;            //פ��Ƶ��
	int   iFDDMode;              //Ƶ�򱣴�
	int   iFDDTimes;             //Ƶ�򱣴�Ƶ��
	int   iTPDMode;              //ʱ�򱣴�
	int   iTPDTimes;             //Ƶ�򱣴�Ƶ��

	float fInitialLevel;         //�����ʼ����
	int   iStartDirect;          //��ʼ����
	int   iWeightMode;           //���Ƽ�Ȩ����
	int   iDriveFix;             //��������
	float fTheoryWeight;         //����ϵ��

	int   iCompressionMode;      //ѹ������
	float fLowRadioFreq;         //ѹ������_Ƶ�ʼ��1
	float fHighRadioFreq;        //ѹ������_Ƶ�ʼ��2
	float fLowRadioTime;         //ѹ������_��Ƶ�θ���ʱ��
	float fHighRadioTime;        //ѹ������_��Ƶ�θ�������
	int   fMidRadioCycles;       //ѹ������_��Ƶ�θ���ʱ��

	float fppScheTable[101][9];      //�ƻ���
	int   iScheTableLen;             //�ƻ�����

	float fppTableFreq[8][101];       //8���׵�Ƶ���۵�
	float fppTableAcc[8][101];        //8���׵ļ��ٶ��۵�
	float fppTableVel[8][101];        //8���׵��ٶ��۵�
	float fppTableDesp[8][101];       //8���׵�λ���۵�
	float fppTableLeftSlp[8][101];    //8���׵���б���۵�
	float fppTableRightSlp[8][101];   //8���׵���б���۵�
	float fppTableLowAlm[8][101];     //8���׵ı��������۵�
	float fppTableHighAlm[8][101];    //8���׵ı��������۵�
	float fppTableLowAbt[8][101];     //8���׵���ֹ�����۵�
	float fppTableHighAbt[8][101];    //8���׵���ֹ�����۵�
	int   ipProfileType[8];           //8���׵����ͣ�����orEu��
	int   iTableDisPlay;              //����ʾ�����ٶ���/�ٶ���/λ���ף�

	int ipChannelType[CHANNEL];       //ͨ������
	int ipChannelCheck[CHANNEL];      //�Լ�
	int ipRecogMode[CHANNEL];         //ʶ��ģʽ
	float fpWeighting[CHANNEL];       //Ȩ��
	int ipSenserType[CHANNEL];        //����������
	int ipCouplingType[CHANNEL];      //��Ϸ�ʽ
	float fpSensitivity[CHANNEL];     //������
	float fpSenserUnit[CHANNEL];      //��λ
	float fpSenserRange[CHANNEL];     //����
	int ipChannelAbort[CHANNEL];      //��ֵ�ж�
	float fpChannellimit[CHANNEL];    //ͨ����ֵ
	int ipReferType[CHANNEL];         //������

	float fMaxShakerInput;      //�����������
	float fMaxShakerD;          //��̨���λ��
	float fMaxShakerV;          //��̨����ٶ�
	float fMaxShakerSineA;      //��̨������Ҽ��ٶ�
	float fMaxShakerRandA;      //��̨���������ٶ�
	float fMaxShakerShockA;     //��̨��������ٶ�
	float fMaxShakerSineF;      //��̨�����������
	float fMaxShakerRandF;      //��̨����������
	float fMaxShakerShockF;     //��̨���������

	float fLevelupRate;          //����̧������
	float fLeveldownRate;        //�����½�����

	float fNoiseThreshold;      //���������ֵ
	float fLoopCheckFreq;       //�ջ����Ƶ��
	float fLoopCheckLevel;      //�ջ�ͨ������
	float fLoopCheckLimit;      //�ջ��������

	float fCSLLevel;            //�����źŶ�ʧ
	float fMaxDrive;            //���������ѹ
	int   iAbortLoops;          //�ж�����
}SetPara;

SetPara SSetPara=
{
	0,//int   iTestMode;             //����ģʽ
	2,//int   iRunMode;              //����ģʽ
	3,//int   iTestSweeps;           //����ɨƵ����
	300,//float fTestTime;             //������ʱ��
	300,//int   iTestCycles;           //����������
	2000,//float fMaxFreq;              //Ƶ������
	5,//float fMinFreq;              //Ƶ������
	4,//float fSweepRate;            //ɨ����
	100,//float fSweepTime;            //����ɨƵʱ��
	200,//float fDwellFreq;            //פ��Ƶ��
	0,//int   iFDDMode;              //Ƶ�򱣴�
	1,//int   iFDDTimes;             //Ƶ�򱣴�Ƶ��
	0,//int   iTPDMode;              //ʱ�򱣴�
	1,//int   iTPDTimes;             //ʱ�򱣴�Ƶ��

	-20,//float fInitialLevel;         //�����ʼ����
	0,//int   iStartDirect;          //��ʼ����
	0,//int   iWeightMode;           //���Ƽ�Ȩ����
	0,//int   iDriveFix;             //��������
	0,//float fTheoryWeight;         //����ϵ��

	3,//int   iCompressionMode;      //ѹ������
	50,//float fLowRadioFreq;         //ѹ������_Ƶ�ʼ��1
	250,//float fHighRadioFreq;        //ѹ������_Ƶ�ʼ��2
	0.16,//float fLowRadioTime;         //ѹ������_��Ƶ�θ���ʱ��
	0.032,//float fHighRadioTime;        //ѹ������_��Ƶ�θ�������
	8,//int   fMidRadioCycles;       //ѹ������_��Ƶ�θ���ʱ��

	{
		{ 0, 5, 2000, 4, 0, 2, INF, 2, 2 },
		{ 1, 5, 5, 3000, -3 },
		{ 2, 5 },
		{ 0, 5, 2000, 4, 0, 2, INF, 2, 2 }, 
		{ 1, 5, 5, 3000, -3 }, 
		{ 3 }
	},//float fppScheTable[101][9];      //�ƻ���
	5, //int   iScheTableLen;             //�ƻ�����

	{ { 5, 200, 2000 } },//float fppTableFreq[8][101];       //8���׵�Ƶ���۵�
	{ { 1, 2, 2 } },//float fppTableAcc[8][101];        //8���׵ļ��ٶ��۵�
	{ { 5, 200, 2000 } },//float fppTableVel[8][101];        //8���׵��ٶ��۵�
	{ { 5, 200, 2000 } },//float fppTableDesp[8][101];       //8���׵�λ���۵�
	{ { 5, 200, 2000 } },//float fppTableLeftSlp[8][101];    //8���׵���б���۵�
	{ { 5, 200, 2000 } },//float fppTableRightSlp[8][101];   //8���׵���б���۵�
	{ { -3, -3, -3 } },//float fppTableLowAlm[8][101];     //8���׵ı��������۵�
	{ { 3, 3, 3 } },//float fppTableHighAlm[8][101];    //8���׵ı��������۵�
	{ { -6, -6, -6 } },//float fppTableLowAbt[8][101];     //8���׵���ֹ�����۵�
	{ { 6, 6, 6 } },//float fppTableHighAbt[8][101];    //8���׵���ֹ�����۵�
	{ 0, 0, 0, 0, 0, 0, 0, 0 },//int   ipProfileType[8];           //8���׵����ͣ�����orEu��
	0,//int   iTableDisPlay;              //����ʾ�����ٶ���/�ٶ���/λ���ף�

	{ 2, 2, 2, 2, 2, 2, 2, 2 }, //int ipChannelType[CHANNEL];       //ͨ������
	{ 1, 1, 1, 1, 1, 1, 1, 1 }, //int ipChannelCheck[CHANNEL];      //�Լ�
	{ 0, 0, 0, 0, 0, 0, 0, 0 },//int ipRecogMode[CHANNEL];         //ʶ��ģʽ
	{ 1, 1, 1, 1, 1, 1, 1, 1 },//float fpWeighting[CHANNEL];       //Ȩ��
	{ 1, 1, 1, 1, 1, 1, 1, 1 },//int ipSenserType[CHANNEL];        //����������
	{ 1, 1, 1, 1, 1, 1, 1, 1 },//int ipCouplingType[CHANNEL];      //��Ϸ�ʽ
	{ 100, 100, 100, 100, 100, 100, 100, 100 },//float fpSensitivity[CHANNEL];     //������
	{ 1, 1, 1, 1, 1, 1, 1, 1 },//float fpSenserUnit[CHANNEL];      //��λ
	{ 1, 1, 1, 1, 1, 1, 1, 1 },//float fpSenserRange[CHANNEL];     //����
	{ 0, 0, 0, 0, 0, 0, 0, 0 },//int ipChannelAbort[CHANNEL];      //��ֵ�ж�
	{ 0, 0, 0, 0, 0, 0, 0, 0 },//float fpChannellimit[CHANNEL];    //ͨ����ֵ
	{ 0, 0, 0, 0, 0, 0, 0, 0 },//int ipReferType[CHANNEL];         //������

	10,//float fMaxShakerInput;      //�����������
	50,//float fMaxShakerD;          //��̨���λ��
	50,//float fMaxShakerV;          //��̨����ٶ�
	50,//float fMaxShakerSineA;      //��̨������Ҽ��ٶ�
	50,//float fMaxShakerRandA;      //��̨���������ٶ�
	50,//float fMaxShakerShockA;     //��̨��������ٶ�
	50,//float fMaxShakerSineF;      //��̨�����������
	50,//float fMaxShakerRandF;      //��̨����������
	50,//float fMaxShakerShockF;     //��̨���������

	20,//float fLevelupRate;          //����̧������
	-20,//float fLeveldownRate;        //�����½�����

	50,//float fNoiseThreshold;      //���������ֵ
	200,//float fLoopCheckFreq;       //�ջ����Ƶ��
	6,//float fLoopCheckLevel;      //�ջ�ͨ������
	300,//float fLoopCheckLimit;      //�ջ��������

	-12,//float fCSLLevel;            //�����źŶ�ʧ
	10,//float fMaxDrive;            //���������ѹ
	1,//int   iAbortLoops;          //�ж�����
};

typedef struct InputPara
{
	float fSamplingRate;          //������
	float fSamplingTime;         //�������

	int   iTestMode;             //����ģʽ
	int   iRunMode;              //����ģʽ
	int   iTestSweeps;           //����ɨƵ����
	float fTestTime;             //������ʱ��
	int   iTestCycles;           //����������
	float fMaxFreq;              //Ƶ������
	float fMinFreq;              //Ƶ������
	float fSweepRate;            //ɨ����
	float fSweepTime;            //����ɨƵʱ��
	float fDwellFreq;            //פ��Ƶ��
	int   iFDDMode;              //Ƶ�򱣴�
	int   iFDDTimes;             //Ƶ�򱣴�Ƶ��
	int   iTPDMode;              //ʱ�򱣴�
	int   iTPDTimes;             //Ƶ�򱣴�Ƶ��

	float fInitialLevel;         //�����ʼ����
	int   iStartDirect;          //��ʼ����
	int   iWeightMode;           //���Ƽ�Ȩ����
	int   iDriveFix;             //��������
	float fTheoryWeight;         //����ϵ��

	int   iCompressionMode;      //ѹ������
	float fLowRadioFreq;         //ѹ������_Ƶ�ʼ��1
	float fHighRadioFreq;        //ѹ������_Ƶ�ʼ��2
	float fLowRadioTime;         //ѹ������_��Ƶ�θ���ʱ��
	float fHighRadioTime;        //ѹ������_��Ƶ�θ�������
	int   fMidRadioCycles;       //ѹ������_��Ƶ�θ���ʱ��

	float fppScheTable[101][8];      //�ƻ���
	int   iScheTableLen;             //�ƻ�����

	float fppTableFreq[8][101];       //8���׵�Ƶ���۵�
	float fppTableAcc[8][101];        //8���׵ļ��ٶ��۵�
	float fppTableVel[8][101];        //8���׵��ٶ��۵�
	float fppTableDesp[8][101];       //8���׵�λ���۵�
	float fppTableLeftSlp[8][101];    //8���׵���б���۵�
	float fppTableRightSlp[8][101];   //8���׵���б���۵�
	float fppTableLowAlm[8][101];     //8���׵ı��������۵�
	float fppTableHighAlm[8][101];    //8���׵ı��������۵�
	float fppTableLowAbt[8][101];     //8���׵���ֹ�����۵�
	float fppTableHighAbt[8][101];    //8���׵���ֹ�����۵�
	int   ipProfileType[8];           //8���׵����ͣ�����orEu��
	int   iTableDisPlay;              //����ʾ�����ٶ���/�ٶ���/λ���ף�

	int ipChannelType[CHANNEL];       //ͨ������
	int ipChannelCheck[CHANNEL];      //�Լ�
	int ipRecogMode[CHANNEL];         //ʶ��ģʽ
	float fpWeighting[CHANNEL];       //Ȩ��
	int ipSenserType[CHANNEL];        //����������
	int ipCouplingType[CHANNEL];      //��Ϸ�ʽ
	float fpSensitivity[CHANNEL];     //������
	float fpSenserUnit[CHANNEL];      //��λ
	float fpSenserRange[CHANNEL];     //����
	int ipChannelAbort[CHANNEL];      //��ֵ�ж�
	float fpChannellimit[CHANNEL];    //ͨ����ֵ
	int ipReferType[CHANNEL];         //������

	float fMaxShakerInput;      //�����������
	float fMaxShakerD;          //��̨���λ��
	float fMaxShakerV;          //��̨����ٶ�
	float fMaxShakerSineA;      //��̨������Ҽ��ٶ�
	float fMaxShakerRandA;      //��̨���������ٶ�
	float fMaxShakerShockA;     //��̨��������ٶ�
	float fMaxShakerSineF;      //��̨�����������
	float fMaxShakerRandF;      //��̨����������
	float fMaxShakerShockF;     //��̨���������

	float fLevelupRate;          //����̧������
	float fLeveldownRate;        //�����½�����
	float fLevelupOffset;        //����̧��ƫ��
	float fLeveldownOffset;      //�����½�ƫ��

	float fNoiseThreshold;      //���������ֵ
	float fLoopCheckFreq;       //�ջ����Ƶ��
	float fLoopCheckLevel;      //�ջ�ͨ������
	float fLoopCheckLimit;      //�ջ��������

	float fCSLLevel;            //�����źŶ�ʧ
	float fMaxDrive;            //���������ѹ
	int   iAbortLoops;          //�ж�����

}InputPara;

InputPara SInputPara;

typedef struct InputCmd
{
	int iExit;
	int iRunMode;
	int iCmd;
	int iTestCase;
	int iScheduleAdd;
	int iSweepDirect;
	float fTestlevel;
	float fSweepRate;
	float fCtrRadio;
	int   iCmdCount;
}InputCmd;

InputCmd SBufCmd = {
	0,//int iExit;
	0,//int iRunMode;
	0,//int iCmd;
	1,//int iTestCase;
	0,//int iScheduleAdd;
	1,//int iSweepDirect;
	0.03f,//float fTestlevel;
	0.0f,//float fSweepRate;
	1.0f,//float fCtrRadio;
	0//int   iCmdCount;
};
InputCmd SInputCmd = {
	0,//int iExit;
	0,//int iCmd;
	0,//int iTestCase;
	0,//int iScheduleAdd;
	1,//int iSweepDirect;
	0.03f,//float fTestlevel;
	0.0f,//float fSweepRate;
	1.0f,//float fCtrRadio;
	0//int   iCmdCount;
};
typedef struct UpdataPara
{
	int iCmd;
	int iRunMode;
	int iSweepDirect;
	float fTestlevel;
	float fSweepRate;
	float fCtrRadio;
	int iTestCase;
	int iStopReason;
	int iScheduleAdd;
	float fChannelNoise;
	int iNoiseResult;
	int iLoopResult;
	int iAlarmorNot;
	int iAbortorNot; 
	int iCSLorNot;
	int iCSLResult;
	int iChannelAbort;
	int iChannelAbortResult;
	int iEffectCh;
	int iLimitChannelEnable;
	int iDriveCliped;
	int iDataCliped;
}UpdataPara;

typedef struct UpdataData
{
	float fFreq;
	float fCtrRefer;
	float fCtrResp;
	float fCtrDrive;
	float fpFourPhase[8];
	float fpFourResp[8];
	float fpRMSResp[8];
	float fpPeakResp[8];

}UpdataData;


float dBtoLevel(float dB);  //dBתʵ��ֵ

float LevetodB(float Level); //ʵ��ֵתdB

void LevelRate(float *fRate, float *fOffset, float RealRate, float LowT, float HighT);  //��Ч�����ٶȼ���

void InterpPoints(float *fpX, float *fpY, float *fpx, float *fpy, int iXLenth, int ixLenth); //����ֵ����

float OcttoRate(float Oct, float fSamplingtime);//ɨ����תƵ��ʵ���ٶ�

float RatetoOct(float Rate, float fSamplingRate);//Ƶ��ʵ���ٶ�תɨ����

float Interp(float x1, float y1, float x2, float y2, float x);//�����ֵ����

void SettoInput(InputPara *SInputPara, SetPara *SSetPara);//���ò������·�����ת������

UpdataPara SUpdataPara;
UpdataData SUpdataData;
double x;