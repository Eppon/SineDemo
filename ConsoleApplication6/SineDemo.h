

#include "math.h"

const float SAMPINGRATE = 200000;
const float SAMPINGTIME = 1/SAMPINGRATE;
const float NOTZERO = 0.00001f;
const int CHANNEL = 8;
const int NOISECHECKTIME =(int)(5 * SAMPINGRATE);
const int TABLELEN = 2048;
const long long int INF = 10000000000000;
const float PI2 = 3.1415926f*2;
const int SAMPLETIMES = 100;

typedef struct ControlPara
{
	int iTestMode;
	int iTestSweeps;
	float fTestTime;
	int iTestCycles;
	float fSweepRate;
	float fSweepTime;
	float fMaxFreq;
	float fMinFreq;
	float fInitialLevel;
	int iRunMode;
	int iStartDirect;
	int iDriveFix;
	float fpHisUpFRF[2048];
	float fpHisDownFRF[2048];
	float fTheoryWeight;
	int iWeightMode;
	float fLowRadioFreq;
	float fHighRadioFreq;
	float fLowRadioTime;
	float fHighRadioTime;
	int fMidRadioCycles;
}ControlPara;
ControlPara SControlPara;

typedef struct PretestPara
{
	int iPretestType;
	int iSignalType;
	float fpPretestTableAmp[100];
	float fpPretestTableFreq[100];
	float fNoiseThreshold;
	float fLoopCheckFreq;
	float fLoopCheckLevel;
	float fLoopCheckLimit;
	float fPretestInitLevel;
	int iFreqLine;
	int iAvgTimes;
	int iStopButtonCheck;
	int iGainCheck;
	float fLinerity;
	int CohCheck;
	float MinCohLevel;
}PretestPara;
PretestPara SPretestPara;

typedef struct ProfilePara
{
	float fppTableFreq[8][101];
	float fppTableAcc[8][101];
	float fppTableVel[8][101];
	float fppTableDesp[8][101];
	float fppTableLeftSlp[8][101];
	float fppTableRightSlp[8][101];
	float fppTableLowAlm[8][101];
	float fppTableHighAlm[8][101];
	float fppTableLowAbt[8][101];
	float fppTableHighAbt[8][101];
	int iProfileType[8];
	float fpUnitType[8][3];
}ProfilePara;
ProfilePara SProfilePara;

typedef struct ChannelPara
{
	int ipChannelType[CHANNEL];
	int ipChannelCheck[CHANNEL];
	int ipRecogMode[CHANNEL];
	float fpWeighting[CHANNEL];
	int ipSenserType[CHANNEL];
	int ipCouplingType[CHANNEL];
	float fpSensitivity[CHANNEL];
	float fpSenserUnit[CHANNEL];
	float fpSenserRange[CHANNEL];
	int ipChannelAbort[CHANNEL];
	float fpChannellimit[CHANNEL];
	int ipProfileType[CHANNEL];
}ChannelPara;
ChannelPara SChannelPara;

typedef struct SaftyPara
{
	float fEqualRate;
	float fStopRate;
	float fLevelupRate;
	float fLeveldownRate;
	float fMaxDrive;
	float fSystemGain;
	int   iAbortLoops;
	float fCSLLevel;
	float fMaxShakerInput;
	float fMaxShakerD;
	float fMaxShakerV;
	float fMaxShakerSineA;
	float fMaxShakerRandA;
	float fMaxShakerShockA;
	float fMaxShakerSineF;
	float fMaxShakerRandF;
	float fMaxShakerShockF;
	float fDUT;
	float fFixture;
	float fArmature;
	float fExpander;
	float fMisc;
}SaftyPara;
SaftyPara SSaftyPara;

typedef struct SchedulePara
{
	float fppScheTable[101][6];
	float fScheTableLen;
}SchedulePara;
SchedulePara SSchedulePara;

typedef struct InputPara
{
	int iTestMode;
	int iTestSweeps;
	float fTestTime;
	int iTestCycles;
	float fSweepRate;
	float fSweepTime;
	float fMaxFreq;
	float fMinFreq;
	float fInitialLevel;
	int iRunMode;
	int iStartDirect;
	int iDriveFix;
	float fpHisUpFRF[2048];
	float fpHisDownFRF[2048];
	float fTheoryWeight;
	int iWeightMode;
	float fLowRadioFreq;
	float fHighRadioFreq;
	float fLowRadioTime;
	float fHighRadioTime;
	int fMidRadioCycles;
	int iPretestType;
	int iSignalType;
	float fpPretestTableAmp[100];
	float fpPretestTableFreq[100];
	float fNoiseThreshold;
	float fLoopCheckFreq;
	float fLoopCheckLevel;
	float fLoopCheckLimit;
	float fPretestInitLevel;
	int iFreqLine;
	int iAvgTimes;
	int iStopButtonCheck;
	int iGainCheck;
	float fLinerity;
	int CohCheck;
	float MinCohLevel;
	float fppTableFreq[8][101];
	float fppTableAcc[8][101];
	float fppTableVel[8][101];
	float fppTableDesp[8][101];
	float fppTableLeftSlp[8][101];
	float fppTableRightSlp[8][101];
	float fppTableLowAlm[8][101];
	float fppTableHighAlm[8][101];
	float fppTableLowAbt[8][101];
	float fppTableHighAbt[8][101];
	int ipProfileType[8];
	float fppUnitType[8];
	int ipChannelType[CHANNEL];
	int ipChannelCheck[CHANNEL];
	int ipRecogMode[CHANNEL];
	float fpWeighting[CHANNEL];
	int ipSenserType[CHANNEL];
	int ipCouplingType[CHANNEL];
	float fpSensitivity[CHANNEL];
	float fpSenserUnit[CHANNEL];
	float fpSenserRange[CHANNEL];
	int ipChannelAbort[CHANNEL];
	float fpChannellimit[CHANNEL];
	int ipReferType[CHANNEL];
	float fEqualRate;
	float fStopRate;
	float fLevelupRate;
	float fLeveldownRate;
	float fMaxDrive;
	float fSystemGain;
	int   iAbortLoops;
	float fCSLLevel;
	float fMaxShakerInput;
	float fMaxShakerD;
	float fMaxShakerV;
	float fMaxShakerSineA;
	float fMaxShakerRandA;
	float fMaxShakerShockA;
	float fMaxShakerSineF;
	float fMaxShakerRandF;
	float fMaxShakerShockF;
	float fDUT;
	float fFixture;
	float fArmature;
	float fExpander;
	float fMisc;
	float fppScheTable[101][6];
	int   iScheTableLen;
}InputPara;
InputPara SInputPara = 
{
	0,//int iTestMode;
	5,//int iTestSweeps;
	60.f,//float fTestTime;
	200,//int iTestCycles;
	4.0f,//float fSweepRate;
	20.f,//float fSweepTime;
	2000.f,//float fMaxFreq;
	5.f,//float fMinFreq;
	0.03f,//float fInitialLevel;
	0,//int iRunMode;
	0,//int iStartDirect;
	0,//int iDriveFix;
	{ 0 },//float fpHisUpFRF[2048];
	{0},//float fpHisDownFRF[2048];
	0.5,// fTheoryWeight;
	0,//int iWeightMode;
	50,//float fLowRadioFreq;
	250,//float fHighRadioFreq;
	0.16,//float fLowRadioTime;
	0.032,//float fHighRadioTime;
	8,//int fMidRadioCycles;
	0,//int iPretestType;
	0,//int iSignalType;
	{ 0 },//float fpPretestTableAmp[100];
	{ 0 },//float fpPretestTableFreq[100];
	0.05,//float fNoiseThreshold;
	5,//float fLoopCheckFreq;
	3,//float fLoopCheckLevel;
	0.1,//float fLoopCheckLimit;
	0.03,//float fPretestInitLevel;
	2048,//int iFreqLine;
	10,//int iAvgTimes;
	0,//int iStopButtonCheck;
	0,//int iGainCheck;
	0.f,//float fLinerity;
	0,//int CohCheck;
	0.0f,//float MinCohLevel;
	{ { 5.0f, 20.0f, 2000.0f } }, //float fppTableFreq[8][101];
	{ { 1.0f, 2.0f, 2.0f } }, //float fppTableAcc[8][101];
	{ { 0.0f, 0.0f, 0.0f } }, //float fppTableVel[8][101];
	{ { 0.0f, 0.0f, 0.0f } },//float fppTableDesp[8][101];
	{ { 0.0f, 0.0f, 0.0f } },//float fppTableLeftSlp[8][101];
	{ { 0.0f, 0.0f, 0.0f } },//float fppTableRightSlp[8][101];
	{ { 0.707f, 0.707f, 0.707f } },//float fppTableLowAlm[8][101];
	{ { 1.414f, 1.414f, 1.414f } },//float fppTableHighAlm[8][101];
	{ { 0.5f, 0.5f, 0.5f } },//float fppTableLowAbt[8][101];
	{ { 2.0f, 2.0f, 2.0f } }, //float fppTableHighAbt[8][101];
	{ 0, 0, 0, 0, 0, 0, 0, 0 },//int ipProfileType[8];
	{ 0, 0, 0, 0, 0, 0, 0, 0 },//float fppUnitType[8][3];
	{ 2, 2, 2, 2, 2, 2, 2, 2 }, //int ipChannelType[CHANNEL];0未激活，1观测，2控制，3限制
	{ 1, 1, 1, 1, 1, 1, 1, 1 },//int ipChannelCheck[CHANNEL];0禁用，1激活
	{ 0, 0, 0, 0, 0, 0, 0, 0 },//int ipRecogMode[CHANNEL];0滤波，1RMS，2峰值
	{ 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f },//float fpWeighting[CHANNEL];
	{ 0, 0, 0, 0, 0, 0, 0, 0 },//int ipSenserType[CHANNEL];0加速度，1EU
	{ 0, 0, 0, 0, 0, 0, 0, 0 },//int ipCouplingType[CHANNEL];
	{ 100, 100, 100, 100, 100, 100, 100, 100 },//float fpSensitivity[CHANNEL];
	{ 0, 0, 0, 0, 0, 0, 0, 0 },//float fpSenserUnit[CHANNEL];0:mv/g
	{ 10, 10, 10, 10, 10, 10, 10, 10 },//float fpSenserRange[CHANNEL];V
	{ 0, 0, 0, 0, 0, 0, 0, 0 },//int ipChannelAbort[CHANNEL];
	{ 0, 0, 0, 0, 0, 0, 0, 0 },//float fpChannellimit[CHANNEL];
	{ 0, 0, 0, 0, 0, 0, 0, 0 },// int ipReferType[CHANNEL];
	30.0f,//float fEqualRate;
	-30.0f,//float fStopRate;
	30.0f,//float fLevelupRate;
	-30.0f,//float fLeveldownRate;
	10.0f,//float fMaxDrive;
	0.03f,//float fSystemGain;
	1,//int iAbortLimilt
	0.25f,//float fCSLLevel;
	10.0f,//float fMaxShakerInput;
	50.0f,//float fMaxShakerD;
	50.0f,//float fMaxShakerV;
	100.0f,//float fMaxShakerSineA;
	100.0f,//float fMaxShakerRandA;
	100.0f,// float fMaxShakerShockA;
	100.0f,// float fMaxShakerSineF;
	100.0f,//float fMaxShakerRandF;
	100.0f,// float fMaxShakerShockF;
	100.0f,// float fDUT;
	100.0f,// float fFixture;
	100.0f,// float fArmature;
	100.0f,//float fExpander;
	100.0f,//float fMisc;
	{						//float fppScheTable[101][6];
		{ 5, 2000, 4, INF, 1, 0 },
		{ 2000, 5, 4, INF, 1, 0 },
		{ 5, 2000, 4, INF, 1, 0 },
		{ 2000, 5, 4, INF, 1, 0 },
		{ 5, 2000, 4, INF, 1, 0 }
	},
	5//int iScheTableLen;
};

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

void LevelRate(float *fRate, float *fOffset, float RealRate, float LowT, float HighT);
void InterpPoints(float *ifX, float *ifY, float *ifx, float *ify, int iXLenth, int ixLenth);
float OcttoRate(float Oct);
float Interp(float x1, float y1, float x2, float y2, float x);

UpdataPara SUpdataPara;
UpdataData SUpdataData;
double x;