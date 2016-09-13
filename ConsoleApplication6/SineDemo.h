
#include "stdafx.h"
#include "math.h"

const float SAMPINGRATE = 200000;
const float NOTZERO = 0.00001;
const int CHANNEL = 8;
const int NOISECHECKTIME = 5 * SAMPINGRATE;
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
};
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
};
PretestPara SPretestPara;

typedef struct ProfilePara
{
	float fppProfileTable[101][10];
	int iProfileType;
	float fpUnitType[3];
};
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
};
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
};
SaftyPara SSaftyPara;

typedef struct SchedulePara
{
	float fppScheTable[101][6];
};
SchedulePara SSchedulePara;


typedef struct InitPara
{
	float fNoiseThreshold;
	float fLoopCheckFreq;
	float fLowRadioFreq;
	float fHighRadioFreq;
	float fLowRadioTime;
	float fHighRadioTime;
	int fMidRadioCycles;
	float fpSinTable[2048];
	int ipAcquCh[CHANNEL];
	int iAcquChLen;
	float fppScheTable[101][6];
	int iScheLength;
};
InitPara SInitPara;

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
};
UpdataPara SUpdataPara;

typedef struct RunPara
{
	float fpData[CHANNEL];
	float fpNoiseLevel[CHANNEL];
	float fDriveSignal;
	float fFreq;
	float fPhase;
	float fColaSin;
	float fColaCos;
	int iZeroPass;
	int iRefresh;
	long long int llDwellTime;
	int iScheduleAdd;

}RunPara;


void InitRunPara();
void PreviewCase();
void NoiseCheck();
void LoopCheck();
void TransferRecg();
void FormalTest();
void IdleCase();