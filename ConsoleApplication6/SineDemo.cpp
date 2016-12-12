// ConsoleApplication5.cpp : �������̨Ӧ�ó������ڵ㡣
//
#include "stdafx.h"
#include "SineDemo.h"

#define GETCMD 	iCmdCount = SBufCmd.iCmdCount;\
				iExit = SBufCmd.iExit; \
				iCmd = SBufCmd.iCmd; \
				iTestCase = SBufCmd.iTestCase; \
				iSweepDirect = SBufCmd.iSweepDirect; \
				fTestLevel = SBufCmd.fTestlevel; \
				fSweepRate = SBufCmd.fSweepRate; \
				iScheduleAdd = SBufCmd.iScheduleAdd; \
				fCtrRadio = SBufCmd.fCtrRadio;

#define SENDSTATE	SUpdataPara.iCmd = iCmd;\
					SUpdataPara.iTestCase = iTestCase;\
					SUpdataPara.fCtrRadio = fCtrRadio;\
					SUpdataPara.fSweepRate = dFreqRate;\
					SUpdataPara.fTestlevel = fLevelGoal;\
					SUpdataPara.iChannelAbort = iChannelAbort;\
					SUpdataPara.iChannelAbortResult = iChannelAbortResult;\
					SUpdataPara.iAbortorNot = iAbortorNot;\
					SUpdataPara.iAlarmorNot = iAlarmorNot;\
					SUpdataPara.iCSLorNot = iCSLorNot;\
					SUpdataPara.iCSLResult = iCSLResult;\
					SUpdataPara.iLimitChannelEnable = iLimitEnable;\
					SUpdataPara.iLoopResult = iLoopResult;\
					SUpdataPara.iNoiseResult = iNoiseResult;\
					SUpdataPara.iScheduleAdd = iScheduleAdd;\
					SUpdataPara.iStopReason = iStopReason;\
					SUpdataPara.iSweepDirect = iSweepDirect;\
					SUpdataPara.iDriveCliped = iDriveCliped;\
					SUpdataPara.iDataCliped = iDataCliped;

#define SENDDATA 	SUpdataData.fCtrDrive = fDrive;\
					SUpdataData.fCtrRefer = fpChannelRefer[CHANNEL];\
					SUpdataData.fCtrResp = fpChannelRefer[CHANNEL];\
					SUpdataData.fFreq = fFreq;\
					memcpy(SUpdataData.fpFourPhase, fpFourPhase, sizeof(float)* 8);\
					memcpy(SUpdataData.fpFourResp, fpRecogResp, sizeof(float)* 8);\
					memcpy(SUpdataData.fpRMSResp, fpRecogResp + CHANNEL, sizeof(float)* 8);\
					memcpy(SUpdataData.fpPeakResp, fpRecogResp + CHANNEL * 2, sizeof(float)* 8);



int _tmain(int argc, _TCHAR* argv[])

{

	/*���̲���*/
	int j;
	int i;
	int iPoints;
	int iExit = 0;
	int iTestCase = 1;
	int iCmd = 0;
	int iStopReason;
	int iCmdCount=0;

	/*ʱ�����*/
	float fppSampleBuf1[SAMPLETIMES][CHANNEL];
	float fppSampleBuf2[SAMPLETIMES][CHANNEL];
	float fppSampleBuf3[SAMPLETIMES] = {0};
	float fppSampleBuf4[SAMPLETIMES] = {0};
	float *fppImportData=NULL;
	float *fppExpertData=NULL;
	float fpData[CHANNEL];
	float fDriveSignal;
	long long int llPreSampleNums;
	long long int llSampleNums;
	float fpSensitivity[CHANNEL];
	int iDataCliped = 0;

	/*Ƶ����λ����*/
	float fFreq;
	double dTempFreq;
	double dFreqRate;
	float fSweepRate;
	float fSetRate;
	int   iSweepDirect;
	float fPhase;
	float fTempPhase;
	float fDeltaPhase;
	int   iZeroPass;
	int   iRefresh;
	int   iCtrCycles;
	int   iPassTimes;
	float fColaSin;
	float fColaCos;	

	

	/*��ͨ���������*/
	int ipCheckCh[CHANNEL];
	int iCheckChLen;
	int ipAcquCh[CHANNEL];
	int iAcquChLen;
	int ipCtrCh[CHANNEL];
	int iCtrLen;
	int ipCalCh[CHANNEL];
	int iCalLen;

	/*Ԥ�������*/
	float fpNoiseLevel[CHANNEL];
	float fMaxNoise;
	int iNoiseResult;
	int iLoopResult;
	float fLoopCheckFreq;
	float fLoopCheckLevel;
	float fLoopCheckLimit;
	int iFRFCheck;
	int ipChannelCheck[CHANNEL];
		
	/*�ƻ������*/
	int   iScheduleAdd;
	float fpScheduleRate[100];
	float fpFreqGoal1[100];
	float fpFreqGoal2[100];
	long long int llpTimeGoal[100];
    long long int llpDwellTime[100];
	float fpReferGainGoal[100];
	int   iAddMove[100];
	int   iScheTableLen;
	int   iGetEnd;
	int   iGetEnd1;
	int   iGetEnd2;
	int   iGetEnd3;

	/*ѹ�����Բ���*/
	float fLowFreq;
	float fHighFreq;
	float fLowTime;
	float fHighTime;
	int iMidCycles;
	float fCtrRadio;

	/*����Ҷ�˲�����*/
	float fLen = 0;
	float fFourLen = 0;
	float fCompLen = 1;
	float fpDataRe[CHANNEL] = { 0 };
	float fpDataIm[CHANNEL] = { 0 };
	float fpCompAreaRe[CHANNEL] = { 0 };
	float fpCompAreaIm[CHANNEL] = { 0 };
	float fpCompDataRe[CHANNEL] = { 0 };
	float fpCompDataIm[CHANNEL] = { 0 };
	float fpRespRe[CHANNEL] = { 0 };
	float fpRespIm[CHANNEL] = { 0 };

	float fpSumRe[CHANNEL] = { 0 };
	float fpSumIm[CHANNEL] = { 0 };
	float fpPreDataRe[CHANNEL] = { 0 };
	float fpPreDataIm[CHANNEL] = { 0 };
	float fpLastDataRe[CHANNEL] = { 0 };
	float fpLastDataIm[CHANNEL] = { 0 };
	float fpPreCompAreaRe[CHANNEL] = { 0 };
	float fpPreCompAreaIm[CHANNEL] = { 0 };
	float fPreCompLen = 0;

	/*RMS����*/
	float fSumX[CHANNEL];
	float fSumX2[CHANNEL];
	
	/*��ֵ������*/
	float fpMaxX[CHANNEL];
	float fpMinX[CHANNEL];

	/*ʶ����*/
	float fpChannelResp[CHANNEL+1];
	float fpFourPhase[CHANNEL];
	float fpRecogResp[CHANNEL*3];
	int	  ipRecongMode[CHANNEL];
	float fpPreChannelResp[CHANNEL];
	float fpRespSlope[CHANNEL];
	
	/*��ͨ����Ȩ����*/
	float fpWeightResp[3] = { 0, 0, 10000 };
	int   iWeightMode;
	float fWeightSum;
	float fpChannelWeight[CHANNEL];
	float fpSenserRange[CHANNEL];

	/*ͨ����ֵ�жϣ��ջ��жϲ���*/
	int   ipChannelAbort[CHANNEL+1];
	float fpChannellimit[CHANNEL+1];
	int   iChannelAbortResult;
	int   iChannelAbort;
	int   iCSLorNot;
	int   iCSLResult;
	int   fCSLLevel;
	float fCSLSlope = 0;

	/*��������*/
	float fLevelGoal;
	float fTestLevel;
	float iRunMode;
	float fReferGain=0;
	float fLevelupRate;
	float fLeveldownRate;
	float fLevelupOffset;
	float fLeveldownOffset;
	float fLevelRate;
	float fLevelOffset;
	int   iLevelChanging;
	float fTempGain;
	float fCycletime;
	float fInitialLevel;
	float fMaxSystemGain = 0;
	
	/*�ο��ײ���*/
	float fppReferFreq[CHANNEL][TABLELEN];
	float fppReferAmp[CHANNEL][TABLELEN];
	int   ipReferType[CHANNEL + 1];
	int   ipFreqMark[CHANNEL + 1];
	float fpChannelRefer[CHANNEL + 1];

	/*��������ֹ�޲���*/
	int iAlarmorNot;
	int iAbortOnce;
	int iAbortorNot;
	int iAbortCount;
	int   iAbortLoops;
	float fpUpAlarm[TABLELEN];
	float fpDownAlarm[TABLELEN];
	float fpUpAbort[TABLELEN];
	float fpDownAbort[TABLELEN];

	/*��������*/
	float fCalDrive[CHANNEL + 1];
	float fFilterDrive[CHANNEL + 1];
	float fPreCalDrive[CHANNEL + 1];
	int   iEffectCh;
	int   iLimitEnable;
	float fDrive;
	float fMaxDrive;
	int iDriveCliped = 0;

	/*��ʷƵ����������*/
	float fInvFreqResp;
	float fPreInvFreqResp;
	float fpIFRFFreq[TABLELEN];
	float fDeltaFreq;
	float fpUpIFRF[TABLELEN];
	float fpDownIFRF[TABLELEN];
	int   iIFRFMark=1;
	float fPreFreq=0;
	float fPrePreFreq;

	float fTheoryIFRF;
	float fTheoryDrive;
	float fTheoryWeight;
	float fTheorySetWeight;
	float fHighIFRF;
	float fLowIFRF;

	while (iExit == 0)
	{
		switch (iTestCase)
		{
		case 0:												
			while (iTestCase != 1)
			{
				
				/*����ָ��*/
				if (iCmdCount != SBufCmd.iCmdCount)
				{
					GETCMD
				}
				/*����״̬*/
				SENDSTATE
				SENDDATA
			}

			break;

		case 1:                                             
			iStopReason = 0;
			/*����״̬*/
			SENDSTATE
			SENDDATA
			/*�ɼ���������*/
			for (i = 0; i < CHANNEL; i++)
			{
				fpData[i] = 0.0f;
			}
			llPreSampleNums = 0;
			llSampleNums = 0;
			fppImportData = fppSampleBuf3;
			fppExpertData = fppSampleBuf4;


			/*�ɼ�ͨ��ͳ�ƣ�����ͨ��ͳ�ƣ�����ͨ��ͳ��*/
			iAcquChLen = 0;
			iCtrLen = 0;
			iCalLen = 0;
			iCheckChLen = 0;
			for (i = 0; i < CHANNEL; i++)
			{
				if (SInputPara.ipChannelCheck[i] == 1)
				{
					ipCheckCh[iCheckChLen] = i;
					iCheckChLen++;
				}
				if (SInputPara.ipChannelType[i] == 1)
				{
					ipAcquCh[iAcquChLen] = i;
					iAcquChLen++;
				}
				else if (SInputPara.ipChannelType[i] == 2)
				{
					ipAcquCh[iAcquChLen] = i;
					iAcquChLen++;
					ipCtrCh[iCtrLen] = i;
					iCtrLen++;
				}
				else if (SInputPara.ipChannelType[i] == 3)
				{
					ipAcquCh[iAcquChLen] = i;
					iAcquChLen++;
					ipCalCh[iCalLen] = i;
					iCalLen++;
				}
			}

			/*��������*/
			for (i = 0; i < CHANNEL; i++)
			{
				fpNoiseLevel[i] = 0.0f;
			}
			/*Ԥ���������ʼ��*/
			fMaxNoise = SInputPara.fNoiseThreshold;
			iNoiseResult = 0;
			iLoopResult = 0;
			fLoopCheckFreq = SInputPara.fLoopCheckFreq;
			fLoopCheckLevel = powf(10, (SInputPara.fLoopCheckLevel / 20.0f));
			fLoopCheckLimit = SInputPara.fLoopCheckLimit;
			iFRFCheck = 1;
			

			/*�����ʼ���ó�ʼ��*/
			iRunMode = SInputPara.iRunMode;
			fInitialLevel = SInputPara.fInitialLevel;
			dFreqRate = 0;
			iSweepDirect = 1;
			fMaxDrive = SInputPara.fMaxDrive * 1000;
			 

			/*Ƶ�ʡ���λ���������жϲ�����Cola�źų�ʼ��*/
			fFreq = 0.2f;
			fPhase = 0.0f;
			fTempPhase = 0.0f;
			iZeroPass = 0;
			iRefresh = 0;;
			iCtrCycles = 0;
			iPassTimes = 0;
			fColaSin = 0.0f;
			fColaCos = 1.0f;
			fSweepRate = OcttoRate(SInputPara.fSweepRate);


			/*���������źų�ʼ��*/
			fDrive = 0.0f;
			fDriveSignal = 0.0f;

			/*�ƻ����ʼ��*/
			iScheduleAdd = 0;
			if (SInputPara.iRunMode > 1)
			{
				for (i = 0; i < SInputPara.iScheTableLen; i++)
				{
					fpFreqGoal1[i+2] = (SInputPara.fppScheTable[i][0] < SInputPara.fppScheTable[i][1]) ? SInputPara.fppScheTable[i][0] : SInputPara.fppScheTable[i][1];
					fpFreqGoal2[i+2] = (SInputPara.fppScheTable[i][0] > SInputPara.fppScheTable[i][1]) ? SInputPara.fppScheTable[i][0] : SInputPara.fppScheTable[i][1];
					fpScheduleRate[i+2] = OcttoRate(SInputPara.fppScheTable[i][2]);
					fpScheduleRate[i+2] = (SInputPara.fppScheTable[i][0] < SInputPara.fppScheTable[i][1]) ? fpScheduleRate[i] : (-fpScheduleRate[i]);
					llpTimeGoal[i+2] = SInputPara.fppScheTable[i][3] * SAMPINGRATE;
					fpReferGainGoal[i+2] = SInputPara.fppScheTable[i][4];
					iAddMove[i+2] = 0;
					llpDwellTime[i+2] = 0;
				}
				iGetEnd = 0;
				iGetEnd1 = 0;
				iGetEnd2 = 0;
				iGetEnd3 = 0;
				iScheTableLen = SInputPara.iScheTableLen + 2;
			}
			else
			{
				for (i = 0; i < SInputPara.iTestSweeps; i++)
				{
					fpFreqGoal1[i + 2] = SInputPara.fMinFreq;
					fpFreqGoal2[i + 2] = SInputPara.fMaxFreq;
					fpScheduleRate[i + 2] = OcttoRate(SInputPara.fSweepRate);
					fpScheduleRate[i + 2] = ((SInputPara.iStartDirect == 0) ^ ((i % 2) == 1)) ? fpScheduleRate[i + 2] : (-fpScheduleRate[i + 2]);
					llpTimeGoal[i + 2] = (SInputPara.fSweepRate == 0) ? SInputPara.fTestTime * SAMPINGRATE : INF;
					fpReferGainGoal[i + 2] = 1;
					iAddMove[i + 2] = 0;
					llpDwellTime[i + 2] = 0;
				}
					iGetEnd = 0;
					iGetEnd1 = 0;
					iGetEnd2 = 0;
					iGetEnd3 = 0;
					iScheTableLen = SInputPara.iTestSweeps + 2;
			}
			/*�����������ʵ��*/
			fpFreqGoal1[0] = 0.5f * fpFreqGoal1[3];
			fpFreqGoal2[0] = 1.5f * fpFreqGoal1[3];
			fpScheduleRate[0] = 0.0f;
			llpTimeGoal[0] = INF;
			fpReferGainGoal[0] = 0.03;
			iAddMove[0] = 1;
			llpDwellTime[0] = 0;
			/*�����ʼ����ʵ��*/
			fpFreqGoal1[1] = 0.5f * fpFreqGoal1[3];
			fpFreqGoal2[1] = 1.5f * fpFreqGoal1[3];
			fpScheduleRate[1] = 0.0f;
			llpTimeGoal[1] = INF;
			fpReferGainGoal[1] = SInputPara.fInitialLevel;
			iAddMove[1] = (SInputPara.iRunMode != 1);
			llpDwellTime[1] = 0;
			/*��������ȴ�ʵ��*/
			fpFreqGoal1[iScheTableLen] = 0.5f *fpFreqGoal2[iScheTableLen - 1];
			fpFreqGoal2[iScheTableLen] = 1.5f * fpFreqGoal2[iScheTableLen - 1];
			fpScheduleRate[iScheTableLen] = 0.0f;
			llpTimeGoal[iScheTableLen] = INF;
			fpReferGainGoal[iScheTableLen] = SInputPara.fInitialLevel;
			iAddMove[iScheTableLen] = 0;
			llpDwellTime[iScheTableLen] = 0;
			/*ѹ�����Գ�ʼ��*/
			fLowFreq = SInputPara.fLowRadioFreq;
			fHighFreq = SInputPara.fHighRadioFreq;
			fLowTime = SInputPara.fLowRadioTime;
			fHighTime = SInputPara.fHighRadioTime;
			iMidCycles = SInputPara.fMidRadioCycles * 2;
			fCtrRadio = 1;

			/*ͨ��ʶ�������ʼ��*/
			fLen = 0;
			fFourLen = 0;
			fCompLen = 1;
			fPreCompLen = 0;
			for (i = 0; i < CHANNEL; i++)
			{
				fpDataRe[i] = 0;
				fpDataIm[i] = 0;
				fpCompAreaRe[i] = 0;
				fpCompAreaIm[i] = 0;
				fpCompDataRe[i] = 0;
				fpCompDataIm[i] = 0;
				fpRespRe[i] = 0;
				fpRespIm[i] = 0;

				fpSumRe[i] = 0;
				fpSumIm[i] = 0;
				fpPreDataRe[i] = 0;
				fpPreDataIm[i] = 0;
				fpLastDataRe[i] = 0;
				fpLastDataIm[i] = 0;
				fpPreCompAreaRe[i] = 0;
				fpPreCompAreaIm[i] = 0;

				fSumX[i] = 0;
				fSumX2[i] = 0;

				fpMaxX[i] = 0;
				fpMinX[i] = 0;

				fpChannelResp[i] = 0;
				fpFourPhase[i] = 0;
				fpRecogResp[CHANNEL * 2 + i] = 0;
				fpRecogResp[CHANNEL + i] = 0;
				fpRecogResp[i] = 0;
				ipRecongMode[i] = SInputPara.ipRecogMode[i];
				ipChannelAbort[i] = SInputPara.ipChannelAbort[i];
				fpChannellimit[i] = SInputPara.fpChannellimit[i];
			}
			fpChannelResp[CHANNEL] = 0;
			ipChannelAbort[CHANNEL] = 0;
			fpChannellimit[CHANNEL] = 0;

			fpWeightResp[0] = 0;
			fpWeightResp[1] = 0;
			fpWeightResp[2] = 10000;
			iWeightMode = SInputPara.iWeightMode;
			fWeightSum = 0;

			for (i = 0; i < CHANNEL; i++)
			{
				fpChannelWeight[i] = SInputPara.fpWeighting[i];
				fWeightSum += fpChannelWeight[i];
				fpSensitivity[i] = 1/SInputPara.fpSensitivity[i];
				fpSenserRange[i] = SInputPara.fpSenserRange[i] * 10000;
			}
			for (i = 0; i < CHANNEL; i++)
			{
				fpChannelWeight[i] /= fWeightSum;//Ȩ�ع�һ����Ȩ��=ԭȨ��/����Ȩ�صĺͣ���Ȩ�����Ϊ��ֵ*Ȩ�صĻ����ۼ�
			}

			/*�����仯������ʼ��*/
			LevelRate(&fLevelupRate, &fLevelupOffset, SInputPara.fLevelupRate, SInputPara.fLowRadioTime, SInputPara.fHighRadioTime);
			LevelRate(&fLeveldownRate, &fLeveldownOffset, SInputPara.fLeveldownRate, SInputPara.fLowRadioTime, SInputPara.fHighRadioTime);
			fLevelRate = 1;
			fLevelOffset = 0;
			iLevelChanging = 0;
			fReferGain = 0.03f;
			fTempGain = NOTZERO;
			iAbortLoops = SInputPara.iAbortLoops;
			fCycletime = 1;
			fCSLLevel = SInputPara.fCSLLevel;
			fTestLevel = 1;

			/*��ȫ��������*/
			fLevelGoal = 0;
			iAbortorNot = 0;
			iAlarmorNot = 0;
			iChannelAbort = 0;
			iChannelAbortResult = 0;
			iCSLorNot = 0;
			iCSLResult = 0;
			
			/*�׳�����������ͳ��*/
			float ipxLenth[CHANNEL];
			for (i = 0; i < CHANNEL; i++)
			{
				j = 0;
				ipxLenth[i] = 0;
				while ((SInputPara.fppTableFreq[i][j] <= SInputPara.fppTableFreq[i][j + 1]) && (j < 100))
				{
					(ipxLenth[i])++;
					j++;
				}
				ipReferType[i] = SInputPara.ipReferType[i];
			}
			ipReferType[CHANNEL] = 0;

			/*�ο��������ײ�ֵ*/
			for (i = 0; i < iCalLen+1; i++)
			{
				InterpPoints(fppReferFreq[ipReferType[i]], fppReferAmp[ipReferType[i]], SInputPara.fppTableFreq[ipReferType[i]], SInputPara.fppTableAcc[ipReferType[i]], TABLELEN, ipxLenth[ipReferType[i]]);
			}

			/*AbortAlarmȡֵ*/
			j = 0;
			for (i = 0; i < TABLELEN; i++)
			{
				while ((fppReferFreq[0][i]>SInputPara.fppTableFreq[0][j + 1]) && (j < ipxLenth[0]))
				{
					j++;
				}

				fpUpAbort[i] = SInputPara.fppTableHighAbt[0][j];
				fpDownAbort[i] = SInputPara.fppTableLowAbt[0][j];
				fpUpAlarm[i] = SInputPara.fppTableHighAlm[0][j];
				fpDownAlarm[i] = SInputPara.fppTableLowAlm[0][j];
			}

			/*�ο�����������*/
			for (i = 0; i < CHANNEL + 1; i++)
			{
				ipFreqMark[i] = (fpScheduleRate[2]>0) ? 1 : 2048;
				fpChannelRefer[i] = NOTZERO;
				fCalDrive[i] = NOTZERO;
				fFilterDrive[i] = NOTZERO;
				fpChannelResp[i] = NOTZERO;
				fPreCalDrive[i] = NOTZERO;
			}
			iEffectCh = CHANNEL;
			iLimitEnable = 0;

			/*��������������Ƶ����*/

			for (i = 0; i < TABLELEN; i++)
			{
				fpIFRFFreq[i] = fppReferFreq[0][i];
			}

			/*��������Ȩ��*/
			fTheorySetWeight = SInputPara.fTheoryWeight;
			
			/*��ʼ�������ת��case2*/
			//iTestCase = 2;
			/*��ʼ�������ת��case6*/
			iTestCase = 6;

			break;

		case 2:												
			/*����״̬*/
			iStopReason = 0;
			SENDSTATE
			SENDDATA
			/*�ȴ�����*/
			while (iTestCase == 2)
			{
				fppImportData = (fppImportData == fppSampleBuf3) ? fppSampleBuf4 : fppSampleBuf3;
				fppExpertData = (fppImportData == fppSampleBuf3) ? fppSampleBuf4 : fppSampleBuf3;
				for (iPoints = 0; iPoints < SAMPLETIMES; iPoints++)
				{
					for (i = 0; i < CHANNEL; i++)
					{
						iDataCliped = 0;
						iDataCliped |= ((abs(fppImportData[iPoints]) >= fpSenserRange[i]) << i);
						fpData[i] = fppImportData[iPoints] * fpSensitivity[i];
					}
					
					fppExpertData[iPoints]=fDriveSignal;
				}
				if (iCmdCount != SBufCmd.iCmdCount)
				{
					GETCMD
				}
				SENDSTATE
				SENDDATA

			}
			break;

		case 3:
			iStopReason = 0;
			SENDSTATE
			SENDDATA
			fDriveSignal = NOTZERO;
			iNoiseResult = 0;

			for (j = 0; j < NOISECHECKTIME; j += 100)
			{
				fppImportData = (fppImportData == fppSampleBuf3) ? fppSampleBuf4 : fppSampleBuf3;
				fppExpertData = (fppImportData == fppSampleBuf3) ? fppSampleBuf4 : fppSampleBuf3;
				for (iPoints = 0; iPoints < SAMPLETIMES; iPoints++)
				{
					for (i = 0; i < CHANNEL; i++)
					{
						iDataCliped = 0;
						iDataCliped |= ((abs(fppImportData[iPoints]) >= fpSenserRange[i]) << i);
						fpData[i] = fppImportData[iPoints];
					}

					for (i = 0; i < iCheckChLen; i++)
					{
						fSumX[ipCheckCh[i]] += fpData[ipCheckCh[i]];
						fSumX2[ipCheckCh[i]] += fpData[ipCheckCh[i]] * fpData[ipCheckCh[i]];
					}
					fppExpertData[iPoints] = 0;
				}
				if (iCmdCount != SBufCmd.iCmdCount)
				{
					GETCMD
				}
				SENDSTATE
				SENDDATA
			}
			for (i = 0; i < iCheckChLen; i++)
			{
				fpChannelResp[ipCheckCh[i]] = sqrtf((fSumX2[ipCheckCh[i]] - fSumX[ipCheckCh[i]] * fSumX[ipCheckCh[i]] / NOISECHECKTIME) / NOISECHECKTIME * 2);
				fSumX2[ipCheckCh[i]] = 0;
				fSumX[ipCheckCh[i]] = 0;
				fpNoiseLevel[ipCheckCh[i]] = fpChannelResp[ipCheckCh[i]];
				iNoiseResult |= ((fpChannelResp[ipCheckCh[i]]>fMaxNoise) << ipCheckCh[i]);
			}

			if (iNoiseResult > 0)
			{											//�����������0����ͨ��������鲻ͨ��
				iTestCase = 7;
				iStopReason = 1;						//������鲻�ɹ�
			}
			else
			{
				iTestCase = 4;							//�������ɹ�
			}
			printf("����=%d���������=&d,��������=%f", iTestCase, iNoiseResult, iNoiseResult);
			break;

		case 4:
			/*����״̬*/
			iStopReason = 0;
			fDriveSignal = NOTZERO;
			fDrive = 0.001f;
			iDataCliped = 0;
			SENDSTATE
			SENDDATA

			while (iTestCase == 4)
			{
				fppImportData = (fppImportData == fppSampleBuf3) ? fppSampleBuf4 : fppSampleBuf3;
				fppExpertData = (fppImportData == fppSampleBuf3) ? fppSampleBuf4 : fppSampleBuf3;
				for (iPoints = 0; iPoints < SAMPLETIMES; iPoints++)
				{

					llSampleNums++;
					for (i = 0; i < CHANNEL; i++)
					{
						iDataCliped |= ((abs(fppImportData[iPoints]) >= fpSenserRange[i]) << i);
						fpData[i] = fppImportData[iPoints] * fpSensitivity[i];;
					};

					fFreq = fLoopCheckFreq;

					fDeltaPhase=PI2*fFreq / SAMPINGRATE;
					fPhase += fDeltaPhase;
					fPhase = (fPhase > PI2) ? (fPhase - PI2) : fPhase;

					fTempPhase += fDeltaPhase;
					iZeroPass = (fTempPhase > PI);
					fTempPhase = iZeroPass ? (fTempPhase - PI) : fTempPhase;


					fColaSin = sinf(fPhase);
					fColaCos = cosf(fPhase);

					if (iZeroPass == 1)
					{
						iPassTimes++;
						if (fFreq <= fLowFreq)
							iCtrCycles = (int)ceilf(fLowTime * fFreq);
						else if (fFreq <= fHighFreq)
							iCtrCycles = iMidCycles;
						else iCtrCycles = (int)ceilf(fHighTime * fFreq);
						if (iPassTimes >= (iCtrCycles * fCtrRadio))
						{
							iRefresh = 1;
							iPassTimes = 0;
						}
					}
					else iRefresh = 0;

					for (i = 0; i < iCheckChLen; i++)
					{
						fSumX[ipCheckCh[i]] += fpData[ipCheckCh[i]];
						fSumX2[ipCheckCh[i]] += fpData[ipCheckCh[i]] * fpData[ipCheckCh[i]];
					}

					if (iRefresh == 1)
					{
						fLen = float(llSampleNums - llPreSampleNums);
						llPreSampleNums = llSampleNums;

						/*�ջ����ʶ��+�ж�*/
						iLoopResult = 0;
						for (i = 0; i < iCheckChLen; i++)
						{
							fpChannelResp[ipCheckCh[i]] = sqrtf((fSumX2[ipCheckCh[i]] - fSumX[ipCheckCh[i]] * fSumX[ipCheckCh[i]] / fLen) / fLen * 2);
							fSumX2[ipCheckCh[i]] = 0;
							fSumX[ipCheckCh[i]] = 0;
							iLoopResult |= ((fpChannelResp[ipCheckCh[i]] < (fLoopCheckLevel * fpNoiseLevel[ipCheckCh[i]])) << ipCheckCh[i]);
						}
						/*����״̬*/
						SENDSTATE
						/*��������*/
						SENDDATA
						iDataCliped = 0;
						if (iLoopResult > 0)
						{
							if (fDrive > fLoopCheckLimit)
							{
								iTestCase = 7;
								iStopReason = 2;
							}
							else fDrive *= 1.5;
						}
						else
						{
							iTestCase = 7;
							iStopReason = 3;
						}
					}
					/*��������*/
					if (iCmdCount != SInputCmd.iCmdCount)
					{
						GETCMD
					}

					fppExpertData[iPoints] = fDrive * fColaSin;
				}
			}
			break;

		case 5:
			/*����״̬*/
			iStopReason = 0;
			iTestCase =7;
			iStopReason = 4;	
			SENDSTATE
		    SENDDATA

			break;

		case 6:
			fDriveSignal = NOTZERO;
			fDrive = 0.001f;
			iAbortCount = 0;
			fFreq = 5;
			dTempFreq = fFreq;
			iStopReason = 0;
			printf("case=6\n");
			SENDSTATE
			SENDDATA
			iDataCliped = 0;
			while (iTestCase == 6)
			{
				fppImportData = (fppImportData == fppSampleBuf3) ? fppSampleBuf4 : fppSampleBuf3;
				fppExpertData = (fppImportData == fppSampleBuf3) ? fppSampleBuf4 : fppSampleBuf3;
				for (iPoints = 0; iPoints < SAMPLETIMES; iPoints++)
				{
					llSampleNums++;
					for (i = 0; i < CHANNEL; i++)
					{
						
						iDataCliped |= ((abs(fppImportData[iPoints]) >= fpSenserRange[i]) << i);
						fpData[i] = fppImportData[iPoints] * fpSensitivity[i];
					}
					fSetRate = (fpScheduleRate[iScheduleAdd] >= 0) ? fSweepRate : -fSweepRate;
					dFreqRate = ((iRunMode < 2) && (iScheduleAdd > 1) && (iScheduleAdd < iScheTableLen)) ? fSetRate : fpScheduleRate[iScheduleAdd];
					dFreqRate = (iTestCase == 7) ? 0 : dFreqRate;
					dFreqRate *= iSweepDirect;
					dTempFreq += (dFreqRate * dTempFreq);											//Ƶ���۳˵Ľ��
					llpDwellTime[iScheduleAdd] += (dFreqRate == 0);									//פ��ʱ���ۼ�
					iGetEnd1 = (fFreq < fpFreqGoal1[iScheduleAdd]);									//��ǰƵ���Ƿ񵽴�����
					iGetEnd2 = (fFreq > fpFreqGoal2[iScheduleAdd]);									//��ǰƵ���Ƿ񵽴�����
					iGetEnd3 = (llpDwellTime[iScheduleAdd] >= llpTimeGoal[iScheduleAdd]);			//פ��ʱ�䵽��ʱ��Ŀ��
					dTempFreq = iGetEnd1 ? fpFreqGoal1[iScheduleAdd] : dTempFreq;					//�ﵽĿ��1��Ƶ��=Ŀ�ꣻδ�ﵽ��Ƶ��=�۳˽��
					dTempFreq = iGetEnd2 ? fpFreqGoal2[iScheduleAdd] : dTempFreq;					//�ﵽĿ��2��Ƶ��=Ŀ�ꣻδ�ﵽ��Ƶ��=�۳˽��
					fFreq = dTempFreq;
					iGetEnd = (iGetEnd1 | iGetEnd2 | iGetEnd3);

					fDeltaPhase = PI2*fFreq / SAMPINGRATE;
					fPhase += fDeltaPhase;
					fPhase = (fPhase > PI2) ? (fPhase - PI2) : fPhase;

					fTempPhase += fDeltaPhase;
					iZeroPass = (fTempPhase > PI);
					fTempPhase = iZeroPass ? (fTempPhase - PI) : fTempPhase;

					fColaSin = sinf(fPhase);
					fColaCos = cosf(fPhase);

					//printf("f=%f,Phase=%f,sinx=%f,drive=%f,data=%f\n", fFreq, fPhase, fColaSin, fDriveSignal,fpData[0]);
					if (iZeroPass == 1)
					{
						iPassTimes++;
						if (fFreq <= fLowFreq)
							iCtrCycles = (int)ceilf(fLowTime * fFreq);
						else if (fFreq <= fHighFreq)
							iCtrCycles = iMidCycles;
						else iCtrCycles = (int)ceilf(fHighTime * fFreq);
						if (iPassTimes >= (iCtrCycles * fCtrRadio))
						{
							iRefresh = 1;
							iPassTimes = 0;

						}

					}
					else iRefresh = 0;

					for (i = 0; i < iAcquChLen; i++)
					{
						fpLastDataRe[ipAcquCh[i]] = fpDataRe[ipAcquCh[i]];
						fpLastDataIm[ipAcquCh[i]] = fpDataIm[ipAcquCh[i]];
						fpDataRe[ipAcquCh[i]] = fpData[ipAcquCh[i]] * fColaCos;
						fpDataIm[ipAcquCh[i]] = fpData[ipAcquCh[i]] * fColaSin;
						fpSumRe[ipAcquCh[i]] += fpDataRe[ipAcquCh[i]];
						fpSumIm[ipAcquCh[i]] += fpDataIm[ipAcquCh[i]];

						fSumX[ipAcquCh[i]] += fpData[ipAcquCh[i]];
						fSumX2[ipAcquCh[i]] += fpData[ipAcquCh[i]] * fpData[ipAcquCh[i]];

						fpMaxX[ipAcquCh[i]] = fpMaxX[ipAcquCh[i]] >= fpData[ipAcquCh[i]] ? fpMaxX[ipAcquCh[i]] : fpData[ipAcquCh[i]];
						fpMinX[ipAcquCh[i]] = fpMinX[ipAcquCh[i]] <= fpData[ipAcquCh[i]] ? fpMinX[ipAcquCh[i]] : fpData[ipAcquCh[i]];
					}

					if (iRefresh == 1)
					{
						fCompLen = (fTempPhase) / (fDeltaPhase);
						fLen = (float)(llSampleNums - llPreSampleNums);
						fFourLen = fLen + fPreCompLen - fCompLen;
						fPreCompLen = fCompLen;
						llPreSampleNums = llSampleNums;
						fCycletime = fLen*SAMPINGTIME;

						iChannelAbortResult = 0;
						iCSLResult = 0;
						for (i = 0; i < iAcquChLen; i++)
						{
							/*����Ҷ�˲���*/
							fpCompDataRe[ipAcquCh[i]] = fpDataRe[ipAcquCh[i]] - (fpDataRe[ipAcquCh[i]] - fpLastDataRe[ipAcquCh[i]]) * fCompLen;
							fpCompDataIm[ipAcquCh[i]] = fpDataIm[ipAcquCh[i]] - (fpDataIm[ipAcquCh[i]] - fpLastDataIm[ipAcquCh[i]]) * fCompLen;
							fpCompAreaRe[ipAcquCh[i]] = (fpCompDataRe[ipAcquCh[i]] + fpDataRe[ipAcquCh[i]]);
							fpCompAreaIm[ipAcquCh[i]] = (fpCompDataIm[ipAcquCh[i]] + fpDataIm[ipAcquCh[i]]);
							fpRespRe[ipAcquCh[i]] = (fpSumRe[ipAcquCh[i]] * 2 + fpPreCompAreaRe[ipAcquCh[i]] - fpCompAreaRe[ipAcquCh[i]] - fpDataRe[ipAcquCh[i]] + fpPreDataRe[ipAcquCh[i]]) / fFourLen;
							fpRespIm[ipAcquCh[i]] = (fpSumIm[ipAcquCh[i]] * 2 + fpPreCompAreaIm[ipAcquCh[i]] - fpCompAreaIm[ipAcquCh[i]] - fpDataIm[ipAcquCh[i]] + fpPreDataIm[ipAcquCh[i]]) / fFourLen;

							fpRecogResp[ipAcquCh[i]] = sqrtf(fpRespRe[ipAcquCh[i]] * fpRespRe[ipAcquCh[i]] + fpRespIm[ipAcquCh[i]] * fpRespIm[ipAcquCh[i]]);
							fpFourPhase[ipAcquCh[i]] = atanf(fpRespRe[ipAcquCh[i]] / fpRespIm[ipAcquCh[i]]);

							fpPreCompAreaRe[ipAcquCh[i]] = fpCompAreaRe[ipAcquCh[i]];
							fpPreCompAreaIm[ipAcquCh[i]] = fpCompAreaIm[ipAcquCh[i]];
							fpPreDataRe[ipAcquCh[i]] = fpDataRe[ipAcquCh[i]];
							fpPreDataIm[ipAcquCh[i]] = fpDataIm[ipAcquCh[i]];
							fpSumRe[ipAcquCh[i]] = 0;
							fpSumIm[ipAcquCh[i]] = 0;

							/*RMS��*/
							fpRecogResp[ipAcquCh[i] + CHANNEL] = sqrtf((fSumX2[ipAcquCh[i]] - fSumX[ipAcquCh[i]] * fSumX[ipAcquCh[i]] / fLen) / fLen * 2);
							fSumX2[ipAcquCh[i]] = 0;
							fSumX[ipAcquCh[i]] = 0;

							/*��ֵ��*/
							fpRecogResp[ipAcquCh[i] + CHANNEL * 2] = (fpMaxX[ipAcquCh[i]] - fpMinX[ipAcquCh[i]]) / 2;
							fpMaxX[ipAcquCh[i]] = 0;
							fpMinX[ipAcquCh[i]] = 10000;

							/*ͨ����Ӧѡȡ�����ַ�����ʶ������*/
							fpChannelResp[ipAcquCh[i]] = fpRecogResp[CHANNEL*ipRecongMode[ipAcquCh[i]] + ipAcquCh[i]];

							/*������ͨ����ֹ�ж�*/
							iChannelAbortResult |= (((fpChannelResp[ipAcquCh[i]] > fpChannellimit[ipAcquCh[i]] * fpChannelRefer[ipAcquCh[i]]) && (ipChannelAbort[i] == 1) & (iScheduleAdd > 0)) << ipAcquCh[i]);
							/*CSL�ж�*/
							fpRespSlope[ipAcquCh[i]] = (fpChannelResp[ipAcquCh[i]] - fpPreChannelResp[ipAcquCh[i]]) / fCycletime;
							iCSLResult |= (((fpRespSlope[ipAcquCh[i]]>fCSLSlope)&&(fpChannelResp[ipAcquCh[i]] < fCSLLevel * fpChannelRefer[ipAcquCh[i]]) && (iScheduleAdd > 0)) << ipAcquCh[i]);
							fpPreChannelResp[ipAcquCh[i]] = fpChannelResp[ipAcquCh[i]];
						}

						iChannelAbort = (iChannelAbortResult > 0);
						iCSLorNot = (iCSLResult > 0);

						/*����ͨ����Ȩ*/
						fpWeightResp[0] = 0;
						fpWeightResp[1] = 0;
						fpWeightResp[2] = 10000;
						for (i = 0; i < iCtrLen; i++)
						{
							fpWeightResp[0] += fpChannelResp[ipCtrCh[i]] * fpChannelWeight[ipCtrCh[i]];										//����Ȩ�����ۼ�
							fpWeightResp[1] = fpWeightResp[1] >= fpChannelResp[ipCtrCh[i]] ? fpWeightResp[1] : fpChannelResp[ipCtrCh[i]];	//���������ֵ�Ƚϣ�ȡ�ϴ���Ϊ���ֵ
							fpWeightResp[2] = fpWeightResp[2] <= fpChannelResp[ipCtrCh[i]] ? fpWeightResp[2] : fpChannelResp[ipCtrCh[i]];	//��������Сֵ�Ƚϣ�ȡ��С��Ϊ��Сֵ
						}
						fpChannelResp[CHANNEL] = fpWeightResp[iWeightMode];																//���ݼ�Ȩģʽ��ѡ���Ȩ���{ƽ��ֵor���ֵor��Сֵ}

						/*Alarm�ж�
						1.�ж��Ƿ��������
						2.�ж��Ƿ�С������
						3.����ͨ����Чʱ�������жϲ�������
						4.�ƻ���ִ�е������ʼ����֮ǰ����ַ=0����������������*/
						iAlarmorNot = (fpChannelResp[CHANNEL] > fpUpAlarm[ipFreqMark[CHANNEL]] * fpChannelRefer[CHANNEL]);
						iAlarmorNot |= ((fpChannelResp[CHANNEL]< fpDownAlarm[ipFreqMark[CHANNEL]] * fpChannelRefer[CHANNEL])&(iLimitEnable != 0));
						iAlarmorNot &= (iScheduleAdd > 0);
						/*Abort�ж�
						1.�ж��Ƿ��������
						2.�ж��Ƿ�С������
						3.����ͨ����Чʱ�������жϲ�������
						4.�ƻ���ִ�е������ʼ����֮ǰ����ַ=0������ֹ��������
						5.ÿ����һ�Σ���ֹ����+1
						6.��ֹ������������ֵʱ��������ֹ
						7.��ֹ������ִ�мƻ�����һ��ʱ������*/
						iAbortOnce = (fpChannelResp[CHANNEL] > fpUpAbort[ipFreqMark[CHANNEL]] * fpChannelRefer[CHANNEL]);
						iAbortOnce |= ((fpChannelResp[CHANNEL] < fpDownAbort[ipFreqMark[CHANNEL]] * fpChannelRefer[CHANNEL])&(iLimitEnable != 0));
						iAbortOnce &= (iScheduleAdd > 0);
						iAbortCount += iAbortOnce;
						iAbortorNot = (iAbortCount >= iAbortLoops);
						iAbortCount = (iGetEnd == 1) ? 0 : iAbortCount;

						/*������ֹ�ж�*/
						iTestCase = ((iAbortorNot == 0) && (iChannelAbort == 0) && (iCSLorNot == 0)) ? iTestCase : 7;
						iStopReason = iChannelAbort ? 5 : iStopReason;
						iStopReason = iAbortorNot ? 6 : iStopReason;
						iStopReason = iCSLorNot ? 7 : iStopReason;

						/*���������仯*/
						fLevelGoal = fTestLevel * fpReferGainGoal[iScheduleAdd];
						fLevelRate = (fLevelGoal >= fReferGain) ? fLevelupRate : fLeveldownRate;
						fLevelOffset = (fLevelGoal >= fReferGain) ? fLevelupOffset : fLeveldownOffset;
						fTempGain = (fLevelRate * fCycletime + fLevelOffset) * fReferGain;						  //���㵱ǰ�������ڵ�����̧������*ԭ���棨�ٶ�������Ϲ�ʽ��k*����������/��ǰƵ��+ƫ�ã�
						iLevelChanging = ((fTempGain < fLevelGoal) ^ (fReferGain > fLevelGoal)) && (fReferGain != fLevelGoal);
						fReferGain = iLevelChanging ? fTempGain : fpReferGainGoal[iScheduleAdd];
						iScheduleAdd += ((~iLevelChanging) && iAddMove[iScheduleAdd]);

						/*��ǰ��Ƶ�챣��*/
						fInvFreqResp = fDrive / fpChannelResp[CHANNEL];

						while ((fFreq > fpIFRFFreq[iIFRFMark]) && (dFreqRate > 0) && (fPrePreFreq != fPreFreq))
						{
							fpUpIFRF[iIFRFMark] = Interp(fPrePreFreq, fPreInvFreqResp, fPreFreq, fInvFreqResp, fpIFRFFreq[iIFRFMark]);
							(iIFRFMark)++;
						}
						while ((fFreq < fpIFRFFreq[iIFRFMark - 1]) && (dFreqRate < 0) && (fPrePreFreq != fPreFreq))
						{
							fpDownIFRF[iIFRFMark - 1] = Interp(fPrePreFreq, fPreInvFreqResp, fPreFreq, fInvFreqResp, fpIFRFFreq[iIFRFMark - 1]);
							(iIFRFMark)--;
						}
						fPrePreFreq = fPreFreq;
						fPreFreq = fFreq;
						fPreInvFreqResp = fInvFreqResp;

						/*��ʷ��Ƶ�����*/
						fHighIFRF = (dFreqRate >= 0) ? fpUpIFRF[iIFRFMark] : fpDownIFRF[iIFRFMark];
						fLowIFRF = (dFreqRate >= 0) ? fpUpIFRF[iIFRFMark - 1] : fpDownIFRF[iIFRFMark - 1];
						fTheoryWeight = ((fHighIFRF > 0) && (fLowIFRF > 0)) ? fTheorySetWeight : 0;
						fTheoryIFRF = Interp(fpIFRFFreq[iIFRFMark - 1], fLowIFRF, fpIFRFFreq[iIFRFMark], fHighIFRF, fFreq);

						/*���Ʋο��׼���*/
						while ((fFreq > fppReferFreq[0][ipFreqMark[CHANNEL]]) && (dFreqRate > 0))
						{
							(ipFreqMark[CHANNEL])++;
						}
						while ((fFreq < fppReferFreq[0][ipFreqMark[CHANNEL] - 1]) && (dFreqRate < 0))
						{
							(ipFreqMark[CHANNEL])--;
						}

						fpChannelRefer[CHANNEL] = fppReferAmp[0][ipFreqMark[CHANNEL]] - (fppReferAmp[0][ipFreqMark[CHANNEL]] - fppReferAmp[0][ipFreqMark[CHANNEL] - 1])\
							*(fppReferFreq[0][ipFreqMark[CHANNEL]] - fFreq) / (fppReferFreq[0][ipFreqMark[CHANNEL]] - fppReferFreq[0][ipFreqMark[CHANNEL] - 1]);

						fpChannelRefer[CHANNEL] *= fReferGain;

						/*������������*/
						fCalDrive[CHANNEL] = fInvFreqResp * fpChannelRefer[CHANNEL];
						fCalDrive[CHANNEL] = (fCalDrive[CHANNEL] > NOTZERO ? fCalDrive[CHANNEL] : NOTZERO);
						fFilterDrive[CHANNEL] = sqrtf(fCalDrive[CHANNEL] * fPreCalDrive[CHANNEL]);
						fPreCalDrive[CHANNEL] = fCalDrive[CHANNEL];
						iEffectCh = CHANNEL;
						/*��ʷ��������*/
						fTheoryDrive = fTheoryIFRF * fpChannelRefer[CHANNEL];
						/*��ʷ������Ȩ*/
						fFilterDrive[CHANNEL] = fFilterDrive[CHANNEL] + fTheoryWeight*(fTheoryDrive - fFilterDrive[CHANNEL]);


						/*��������ͨ�����׺�����*/
						for (i = 0; i < iCalLen; i++)
						{
							/*�׼���*/
							while ((fFreq > fppReferFreq[ipReferType[ipCalCh[i]]][ipFreqMark[ipCalCh[i]]]) && (dFreqRate > 0)){
								(ipFreqMark[ipCalCh[i]])++;
							}
							while ((fFreq < fppReferFreq[ipReferType[ipCalCh[i]]][ipFreqMark[ipCalCh[i]] - 1]) && (dFreqRate < 0)){
								(ipFreqMark[ipCalCh[i]])--;
							}

							fpChannelRefer[ipCalCh[i]] = fppReferAmp[ipReferType[ipCalCh[i]]][ipFreqMark[ipCalCh[i]]] - (fppReferAmp[ipReferType[ipCalCh[i]]][ipFreqMark[ipCalCh[i]]] - fppReferAmp[ipReferType[ipCalCh[i]]][ipFreqMark[ipCalCh[i]] - 1])\
								*(fppReferFreq[ipReferType[ipCalCh[i]]][ipFreqMark[ipCalCh[i]]] - fFreq) / (fppReferFreq[ipReferType[ipCalCh[i]]][ipFreqMark[ipCalCh[i]]] - fppReferFreq[ipReferType[ipCalCh[i]]][ipFreqMark[ipCalCh[i]] - 1]);

							fpChannelRefer[ipCalCh[i]] *= fReferGain;

							fCalDrive[ipCalCh[i]] = fDrive / fpChannelResp[ipCalCh[i]] * fpChannelRefer[ipCalCh[i]];
							fCalDrive[ipCalCh[i]] = (fCalDrive[ipCalCh[i]]>NOTZERO ? fCalDrive[ipCalCh[i]] : NOTZERO);
							fFilterDrive[ipCalCh[i]] = sqrtf(fCalDrive[ipCalCh[i]] * fPreCalDrive[ipCalCh[i]]);
							fFilterDrive[ipCalCh[i]] = (fFilterDrive[ipCalCh[i]] > NOTZERO ? fCalDrive[ipCalCh[i]] : NOTZERO);
							fPreCalDrive[ipCalCh[i]] = fCalDrive[ipCalCh[i]];
							/*����ͨ�������ͨ��ȡ��С����*/
							iEffectCh = (fFilterDrive[ipCalCh[i]] < fFilterDrive[iEffectCh] ? ipCalCh[i] : iEffectCh);
						}

						printf("Ff=%f,Ref=%f,Resp=%f,Drive=%f\n", fFreq, fpChannelRefer[CHANNEL], fpChannelResp[CHANNEL], fDrive);
						/*����ѡ��
						1���������������������У�ѡ����С������Ϊʵ�ʷ�������
						2��������󲻳�����ֵ
						3��ѡ������ͨ������ʱ������ͨ��ʹ�ܣ�iLimitEnable���棩*/
						fDrive = fFilterDrive[iEffectCh];
						iDriveCliped = 0;
						iDriveCliped = fFilterDrive[iEffectCh] > fMaxDrive;
						fDrive = (fFilterDrive[iEffectCh] < fMaxDrive) ? fDrive : fMaxDrive;
						iLimitEnable = (iEffectCh != CHANNEL);

						

						if ((iTestCase != 7) && (iCmdCount != SBufCmd.iCmdCount))
						{
							GETCMD
						}

						/*����״̬*/
						SENDSTATE
					    /*��������*/
					    SENDDATA
						iDataCliped = 0;
					}

					fppExpertData[iPoints] = fDrive * fColaSin;
					if (fFreq > 550)
					{
						printf("n=%d,f=%f,Phase=%f,sinx=%f,drive=%f,drivesignal=%f,data=%f\n", iPoints, fFreq, fPhase, fColaSin, fDrive, fppExpertData[iPoints], fpData[0]);
					}

					iScheduleAdd += iGetEnd;//�ﵽĿ�꣬�ƻ����ַ+1��δ�ﵽ��+0
					iTestCase = (iScheduleAdd == iScheTableLen) ? 7 : iTestCase;
					iStopReason = (iScheduleAdd == iScheTableLen) ? 8 : iStopReason;
				}

			}
			break;

		case 7:
			/*����״̬*/
			SENDSTATE
			SENDDATA
			iDataCliped = 0;
			while (iTestCase == 7)
			{
				fppImportData = (fppImportData == fppSampleBuf3) ? fppSampleBuf4 : fppSampleBuf3;
				fppExpertData = (fppImportData == fppSampleBuf3) ? fppSampleBuf4 : fppSampleBuf3;
				for (iPoints = 0; iPoints < SAMPLETIMES; iPoints++)
				{
					for (i = 0; i < CHANNEL; i++)
					{
						iDataCliped = 0;
						iDataCliped |= ((abs(fppImportData[iPoints]) >= fpSenserRange[i]) << i);
						fpData[i] = fppImportData[iPoints] * fpSensitivity[i];;
					};

					fDeltaPhase = PI2*fFreq / SAMPINGRATE;
					fPhase += fDeltaPhase;
					fPhase = (fPhase > PI2) ? (fPhase - PI2) : fPhase;

					fTempPhase += fDeltaPhase;
					iZeroPass = (fTempPhase > PI);
					fTempPhase = iZeroPass ? (fTempPhase - PI) : fTempPhase;

					fCycletime = 1 / fFreq;

					fColaSin = sinf(fPhase);

					if (iZeroPass == 1)
					{
						iPassTimes++;
						if (fFreq <= fLowFreq)
							iCtrCycles = (int)ceilf(fLowTime * fFreq);
						else if (fFreq <= fHighFreq)
							iCtrCycles = iMidCycles;
						else iCtrCycles = (int)ceilf(fHighTime * fFreq);
						if (iPassTimes >= (iCtrCycles * fCtrRadio))
						{
							iRefresh = 1;
							iPassTimes = 0;
						}
					}
					else iRefresh = 0;

					if (iRefresh == 1)
					{
						fLevelRate = fLeveldownRate;
						fLevelOffset = fLeveldownOffset;
						iLevelChanging = (fDrive > 0.001f);
						fTempGain = fLevelRate * fCycletime + fLevelOffset;
						fTempGain = iLevelChanging ? fTempGain : 1;


						printf("f=%f,Drive=%f\n", fFreq, fDrive);
						/*����״̬*/
						SENDSTATE
						/*��������*/
						SENDDATA
						fDrive *= fTempGain;
						iDataCliped = 0;

						if (iLevelChanging == 0)
						{
							iTestCase = ((iStopReason == 3) && (iFRFCheck == 1)) ? 5 : iTestCase;
							iTestCase = ((iStopReason == 3) && (iFRFCheck == 0) && (iRunMode != 1)) ? 6 : iTestCase;
							iTestCase = ((iStopReason == 4) && (iRunMode != 1)) ? 6 : iTestCase;
							if ((iCmdCount != SInputCmd.iCmdCount) && (iTestCase == 7))
							{
								GETCMD
							}

						}
					}

					fppExpertData[iPoints] = fDrive * fColaSin;

				}
			}
			break;
		}
	}
	return 0;
}

void LevelRate(float *fRate, float *fOffset,float RealRate, float LowT, float HighT)
{
	float LowRate;
	float HighRate;
	LowRate = powf(10, RealRate* LowT / 20.0f);
	HighRate = powf(10, RealRate* HighT / 20.0f);
	*fRate = (LowRate - HighRate) / (LowT - HighT);
	*fOffset = 0.975f*HighRate;
}

void InterpPoints(float *fpX, float *fpY, float *fpx, float *fpy, int iXLenth, int ixLenth)
{
	int i;
	float fdx;
	int ixAdd;
	
	fdx = powf(2,(log2f(fpx[ixLenth] / fpx[0])) / (iXLenth - 3));
	fpX[0] = 0.5f * fpx[0];
	fpY[0] = fpy[0];
	fpX[iXLenth-1] = 1.1f * fpx[ixLenth];
	fpY[iXLenth-1] = fpy[ixLenth];
	fpX[1] = fpx[0];
	fpY[1] = fpy[0];
	ixAdd = 1;

	for (i = 2; i < (iXLenth - 1); i++)
	{
		fpX[i] = fpX[i - 1] * fdx;

		while ((fpX[i] > fpx[ixAdd]) && (ixAdd < ixLenth))
		{
			ixAdd++;
		}
		fpY[i] = powf(2, ((log2f(fpX[i] / fpx[ixAdd - 1])*log2f(fpy[ixAdd] / fpy[ixAdd - 1]) / (log2f((fpx[ixAdd] / fpx[ixAdd - 1])))) + log2f(fpy[ixAdd - 1])));
	}
}

float OcttoRate(float Oct)
{
	float Rate;
	double deltaOct;
	deltaOct = Oct;
	deltaOct /= 60;
	deltaOct *= SAMPINGTIME;
	Rate = pow(2.0, deltaOct) - 1;
	return(Rate);
}


float Interp(float x1, float y1, float x2, float y2, float x)
{
	float y;
	y = (x - x2)*(y1 - y2) / (x1 - x2) + y2;
	return y;
}
