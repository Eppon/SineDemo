

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
	float fppProfileTable[101][10];
	int iProfileType;
	float fpUnitType[3];
}ProfilePara;
ProfilePara SProfilePara[8];

typedef struct ChannelPara
{
	int iChannelType;
	int iChannelCheck;
	int iRecogMode;
	float fWeighting;
	int iSenserType;
	int iCouplingType;
	float fSensitivity;
	float fSenserUnit;
	float fSenserRange;
	int iChannelAbort;
	float fChannellimit;
	int iProfileType;
}ChannelPara;
ChannelPara SChannelPara[8];

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


typedef struct InputCommand
{
	int iExit;
	int iTestCase;
}InputCommand;
InputCommand SInputCmd, SBufCmd;

typedef struct UpdataPara
{
	float fpChannelResp[CHANNEL];
	int iNoiseResult;
	int iTestStep;
}UpdataPara;
UpdataPara SUpdataPara;

