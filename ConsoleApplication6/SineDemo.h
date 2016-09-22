

#include "math.h"

const float SAMPINGRATE = 200000;
const float NOTZERO = 0.00001f;
const int CHANNEL = 8;
const int NOISECHECKTIME =(int)(5 * SAMPINGRATE);
const long long int INF = 10000000000000;
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
	float fpTheoryTrans[2048];
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
	float fppProfileTable[8][101][10];
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
	float fpTheoryTrans[2048];
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
	float fpppProfileTable[8][101][10];
	int ipProfileType[8];
	float fppUnitType[8][3];
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
	float fEqualRate;
	float fStopRate;
	float fLevelupRate;
	float fLeveldownRate;
	float fMaxDrive;
	float fSystemGain;
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
}InputPara;
InputPara SInputPara;

typedef struct InputCmd
{
	int iExit;
	int iCmd;
	int iTestCase;
	int iScheduleAdd;
	int iSweepDirect;
	float fTestlevel;
	float fSweepRate;
	float fCtrRadio;
}InputCmd;

InputCmd SBufCmd;
typedef struct UpdataPara
{
	int iCmd;
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
	int iAlarmResult;
	int iAbortorNot;
	int iAbortResult;
	int iCSLorNot;
	int iCSLResult;
	int iChannelAbort;
	int iChannelAbortResult;
	int iLimitChannelEnable;
}UpdataPara;

typedef struct UpdataData
{
	float fFreq;
	float fpFourPhase[8];
	float fpFourResp[8];
	float fpRMSResp[8];
	float fpPeakResp[8];
	float fCtrRefer;
	float fCtrResp;
	float fCtrDrive;
}UpdataData;

void LevelRate(float *fRate, float *fOffset, float RealRate, float LowT, float HighT);