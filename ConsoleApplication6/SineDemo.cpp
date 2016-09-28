// ConsoleApplication5.cpp : 定义控制台应用程序的入口点。
//
#include "stdafx.h"
#include "SineDemo.h"

int _tmain(int argc, _TCHAR* argv[])

{
	int j;
	int i;
	int iExit = 0;
	int iTestCase = 0;
	int iCmd = 0;

	float fpData[CHANNEL];
	float fDriveSignal;
	long long int llPreSampleNums;
	long long int llSampleNums;

	float fFreq;
	float fPhase;
	float fColaSin;
	float fColaCos;	

	float fDrive;

	int ipAcquCh[CHANNEL];
	int iAcquChLen;
	int ipCtrCh[CHANNEL];
	int iCtrLen;
	int ipCalCh[CHANNEL];
	int iCalLen;

	float fpNoiseLevel[CHANNEL];
	float fMaxNoise;
	int iNoiseResult;


	int iLoopResult;
	float fLoopCheckFreq;
	float fLoopCheckLevel;
	float fLoopCheckLimit;
	


	float fInitialLevel;

	float fFreqRate;
	float iSweepDirect;
	float fTempFreq = 0;

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

	float fTempPhase;
	int iZeroPass;
	int iRefresh;
	int iCtrCycles;
	int iPassTimes;
	float fLowFreq;
	float fHighFreq;
	float fLowTime;
	float fHighTime;
	int iMidCycles;
	float fCtrRadio;

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

	float fSumX[CHANNEL];
	float fSumX2[CHANNEL];

	float fpMaxX[CHANNEL];
	float fpMinX[CHANNEL];

	float fpChannelResp[CHANNEL+1];
	float fpFourPhase[CHANNEL];
	float fpRecogResp[CHANNEL*3];
	int	  ipRecongMode[CHANNEL];

	float fpWeightResp[3] = { 0, 0, 10000 };
	int   iWeightMode;
	float fWeightSum;
	float fpChannelWeight[CHANNEL];

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

	float fppReferFreq[CHANNEL][TABLELEN];
	float fppReferAmp[CHANNEL][TABLELEN];
	int   ipReferType[CHANNEL + 1];
	int   ipFreqMark[CHANNEL + 1];
	float fpChannelRefer[CHANNEL + 1];

	float fCalDrive[CHANNEL + 1];
	float fFilterDrive[CHANNEL + 1];
	float fPreCalDrive[CHANNEL + 1];
	int   iEffectCh;
	int   iLimitEnable;


	while (iExit == 0)
	{
		switch (iTestCase)
		{
		case 0:												//等待初始化
			while (iTestCase == 0)
			{
				iExit = SBufCmd.iExit;
				iCmd = SBufCmd.iCmd;
				iTestCase = SBufCmd.iTestCase;
				iScheduleAdd = SBufCmd.iScheduleAdd;
				iSweepDirect = SBufCmd.iSweepDirect;
				fpReferGainGoal[iScheduleAdd] = SBufCmd.fTestlevel;
				fpScheduleRate[iScheduleAdd] = SBufCmd.fSweepRate;
				fCtrRadio = SBufCmd.fCtrRadio;
			}

			break;

		case 1:                                             //进行初始化


			for (i = 0; i < CHANNEL; i++)
			{
				fpData[i] = 0.0f;
			}

			iAcquChLen = 0;
			iCtrLen = 0;
			iCalLen = 0;
			for (i = 0; i < CHANNEL; i++)
			{
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
			ipCalCh[iCalLen] = CHANNEL;
			iCalLen++;

			for (i = 0; i < CHANNEL; i++)
			{
				fpNoiseLevel[i] = 0.0f;
			}

			fMaxNoise = SInputPara.fNoiseThreshold;
			iNoiseResult = 0;
			iLoopResult = 0;
			fLoopCheckFreq = SInputPara.fLoopCheckFreq;
			fLoopCheckLevel = SInputPara.fLoopCheckLevel;
			fLoopCheckLimit = SInputPara.fLoopCheckLimit;


			fInitialLevel = SInputPara.fInitialLevel;

			fFreqRate = 0;
			iSweepDirect = 1;

			fDriveSignal = 0.0f;
			llPreSampleNums = 0;
			llSampleNums = 0;

			fFreq = fLoopCheckFreq;
			fPhase = 0.0f;
			fColaSin = 0.0f;
			fColaCos = 1.0f;

			fDrive = 0.0f;

			iScheduleAdd = 0;
			for (i = 0; i < 100; i++)
			{
				fpFreqGoal1[i] = (SInputPara.fppScheTable[i][0] < SInputPara.fppScheTable[i][1]) ? SInputPara.fppScheTable[i][0] : SInputPara.fppScheTable[i][1];
				fpFreqGoal2[i] = (SInputPara.fppScheTable[i][0] > SInputPara.fppScheTable[i][1]) ? SInputPara.fppScheTable[i][0] : SInputPara.fppScheTable[i][1];
				fpScheduleRate[i] = OcttoRate(SInputPara.fppScheTable[i][2]);
				fpScheduleRate[i] = (SInputPara.fppScheTable[i][0] < SInputPara.fppScheTable[i][1]) ? fpScheduleRate[i] : (-fpScheduleRate[i]);
				llpTimeGoal[i] = SInputPara.fppScheTable[i][3] * SAMPINGRATE;
				fpReferGainGoal[i] = SInputPara.fppScheTable[i][4];
				iAddMove[i] = SInputPara.fppScheTable[i][5];
				llpDwellTime[i] = 0;
				
			}
			iGetEnd = 0;
			iGetEnd1 = 0;
			iGetEnd2 = 0;
			iGetEnd3 = 0;
			iScheTableLen = SInputPara.iScheTableLen;

			fTempPhase = 0;
			iZeroPass = 0;
			iRefresh = 0;;
			iCtrCycles = 0;
			iPassTimes = 0;

			fLowFreq = SInputPara.fLowRadioFreq;
			fHighFreq = SInputPara.fHighRadioFreq;
			fLowTime = SInputPara.fLowRadioTime;
			fHighTime = SInputPara.fHighRadioTime;
			iMidCycles = SInputPara.fMidRadioCycles;

			fCtrRadio = 1;

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
			}
			fpChannelResp[CHANNEL] = 0;

			fpWeightResp[0] = 0;
			fpWeightResp[1] = 0;
			fpWeightResp[2] = 10000;
			iWeightMode = SInputPara.iWeightMode;
			fWeightSum = 0;
			for (i = 0; i < CHANNEL; i++)
			{
				fpChannelWeight[i] = SInputPara.fpWeighting[i];
				fWeightSum += fpChannelWeight[i];
			}


			LevelRate(&fLevelupRate, &fLevelupOffset, SInputPara.fLevelupRate, SInputPara.fLowRadioTime, SInputPara.fHighRadioTime);
			LevelRate(&fLeveldownRate, &fLeveldownOffset, SInputPara.fLeveldownRate, SInputPara.fLowRadioTime, SInputPara.fHighRadioTime);


			fLevelRate = 1;
			fLevelOffset = 0;
			iLevelChanging = 0;
			fReferGain = 0.03f;
			fTempGain = NOTZERO;
			fCycletime = 1;

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

			for (i = 0; i < iCalLen; i++)
			{
				InterpPoints(fppReferFreq[ipReferType[i]], fppReferAmp[ipReferType[i]], SInputPara.fppTableFreq[ipReferType[i]], SInputPara.fppTableAcc[ipReferType[i]], TABLELEN, ipxLenth[ipReferType[i]]);
			}

			for (i = 0; i < CHANNEL + 1; i++)
			{

				ipFreqMark[i] = (fpScheduleRate[1]>0) ? 1 : 2048;
				fpChannelRefer[i] = NOTZERO;
				fCalDrive[i] = NOTZERO;;
				fFilterDrive[i] = NOTZERO;;
				fpChannelResp[i] = NOTZERO;;
				fPreCalDrive[i] = NOTZERO;;
			}
			iEffectCh = CHANNEL;
			iLimitEnable = 0;
			//send
			iTestCase = 6;
			fFreq = SInputPara.fppScheTable[1][0];
			break;

		case 2:												//时域数据预览
			while (iTestCase == 2)
			{
				iTestCase = SBufCmd.iTestCase;
			}
			break;

		case 3:
			fDriveSignal = NOTZERO;
			iNoiseResult = 0;

			for (j = 0; j < NOISECHECKTIME; j += 8)
			{
				for (i = 0; i < CHANNEL; i++)
				{
					fpData[i] = fDriveSignal;
				};
				for (i = 0; i < iAcquChLen; i++)
				{
					fSumX[ipAcquCh[i]] += fpData[ipAcquCh[i]];
					fSumX2[ipAcquCh[i]] += fpData[ipAcquCh[i]] * fpData[ipAcquCh[i]];
				}
			}

			for (i = 0; i < iAcquChLen; i++)
			{
				fpChannelResp[ipAcquCh[i]] = sqrtf((fSumX2[ipAcquCh[i]] - fSumX[ipAcquCh[i]] * fSumX[ipAcquCh[i]] / NOISECHECKTIME) / NOISECHECKTIME * 2);
				fSumX2[ipAcquCh[i]] = 0;
				fSumX[ipAcquCh[i]] = 0;
				fpNoiseLevel[ipAcquCh[i]] = fpChannelResp[ipAcquCh[i]];
				iNoiseResult |= ((fpChannelResp[ipAcquCh[i]]>fMaxNoise) << ipAcquCh[i]);
			}

			if (iNoiseResult > 0)
			{											//噪声结果大于0，则有通道噪声检查不通过
				iTestCase = 7;							//噪声检查不成功
			}
			else
			{
				iTestCase = 4;							//噪声检查成功
			}
			printf("流程=%d，噪声结果=&d,噪声量级=%f", iTestCase, iNoiseResult, iNoiseResult);
			break;

		case 4:
			fDriveSignal = NOTZERO;
			fDrive = 0.001f;

			while (iTestCase == 4)
			{
				iLoopResult = 0;

				llSampleNums++;
				for (i = 0; i < CHANNEL; i++)
				{
					fpData[i] = fDriveSignal;
				};

				fFreq = fLoopCheckFreq;

				fPhase += PI2*fFreq / SAMPINGRATE;
				iZeroPass = (fPhase > PI2);
				fTempPhase = fPhase - PI2;
				fPhase = iZeroPass ? fTempPhase : fPhase;

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

				for (i = 0; i < iAcquChLen; i++)
				{
					fSumX[ipAcquCh[i]] += fpData[ipAcquCh[i]];
					fSumX2[ipAcquCh[i]] += fpData[ipAcquCh[i]] * fpData[ipAcquCh[i]];
				}

				if (iRefresh == 1)
				{
					fLen = float(llSampleNums - llPreSampleNums);
					llPreSampleNums = llSampleNums;

					for (i = 0; i < iAcquChLen; i++)
					{
						fpChannelResp[ipAcquCh[i]] = sqrtf((fSumX2[ipAcquCh[i]] - fSumX[ipAcquCh[i]] * fSumX[ipAcquCh[i]] / fLen) / fLen * 2);
						fSumX2[ipAcquCh[i]] = 0;
						fSumX[ipAcquCh[i]] = 0;
						iLoopResult |= ((fpChannelResp[ipAcquCh[i]] < fLoopCheckLevel) << ipAcquCh[i]);
					}

					if (iLoopResult > 0)
					{
						if (fDrive > fLoopCheckLimit)
						{
							iTestCase = 7;
						}
						else fDrive *= 1.5;
					}
					else if (SPretestPara.iPretestType == 2)
					{
						iTestCase = 5;
					}
					else iTestCase = 6;
				}

			}

			break;

		case 5:;
			iTestCase = 6;

			break;

		case 6:
			fDriveSignal = NOTZERO;
			fDrive = 0.001f;

			while (iTestCase == 6)
			{
				llSampleNums++;
				for (i = 0; i < CHANNEL; i++)
				{
					fpData[i] = fDriveSignal;
				};

				fFreqRate = iSweepDirect * fpScheduleRate[iScheduleAdd];
				fTempFreq = (fFreqRate)* (fFreq)+fFreq;														//频率累乘的结果
				llpDwellTime[iScheduleAdd] += (fFreqRate == 0);												//驻留时间累加
				iGetEnd1 = (fFreq < fpFreqGoal1[iScheduleAdd]);												//当前频率是否到达下限
				iGetEnd2 = (fFreq > fpFreqGoal2[iScheduleAdd]);												//当前频率是否到达上限
				iGetEnd3 = (llpDwellTime[iScheduleAdd] >= llpTimeGoal[iScheduleAdd]);						//驻留时间到达时间目标
				fFreq = iGetEnd1 ? fpFreqGoal1[iScheduleAdd] : fTempFreq;									//达到目标1，频率=目标；未达到，频率=累乘结果
				fFreq = iGetEnd2 ? fpFreqGoal2[iScheduleAdd] : fTempFreq;									//达到目标2，频率=目标；未达到，频率=累乘结果
				iGetEnd = (iGetEnd1 | iGetEnd2 | iGetEnd3);
				
				fPhase += PI2*fFreq / SAMPINGRATE;
				iZeroPass = (fPhase > PI2);
				fTempPhase = fPhase - PI2;
				fPhase = iZeroPass ? fTempPhase : fPhase;

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
					fpDataRe[ipAcquCh[i]] = fpData[ipAcquCh[i]] * fColaCos;
					fpDataIm[ipAcquCh[i]] = fpData[ipAcquCh[i]] * fColaSin;
					fpSumRe[ipAcquCh[i]] += fpDataRe[ipAcquCh[i]];
					fpSumIm[ipAcquCh[i]] += fpDataIm[ipAcquCh[i]];
					fpLastDataRe[ipAcquCh[i]] = fpDataRe[ipAcquCh[i]];
					fpLastDataIm[ipAcquCh[i]] = fpDataIm[ipAcquCh[i]];

					fSumX[ipAcquCh[i]] += fpData[ipAcquCh[i]];
					fSumX2[ipAcquCh[i]] += fpData[ipAcquCh[i]] * fpData[ipAcquCh[i]];

					fpMaxX[ipAcquCh[i]] = fpMaxX[ipAcquCh[i]] >= fpData[ipAcquCh[i]] ? fpMaxX[ipAcquCh[i]] : fpData[ipAcquCh[i]];
					fpMinX[ipAcquCh[i]] = fpMinX[ipAcquCh[i]] <= fpData[ipAcquCh[i]] ? fpMinX[ipAcquCh[i]] : fpData[ipAcquCh[i]];
				}

				if (iRefresh == 1)
				{
					fCompLen = SAMPINGRATE * (fPhase) / (fFreq);
					fLen = (float)(llSampleNums - llPreSampleNums);
					fFourLen = fLen + fPreCompLen - fCompLen;
					fPreCompLen = fCompLen;
					llPreSampleNums = llSampleNums;
					fCycletime = fLen/SAMPINGRATE;

					for (i = 0; i < iAcquChLen; i++)
					{
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

						fpRecogResp[ipAcquCh[i] + CHANNEL] = sqrtf((fSumX2[ipAcquCh[i]] - fSumX[ipAcquCh[i]] * fSumX[ipAcquCh[i]] / fLen) / fLen * 2);
						fSumX2[ipAcquCh[i]] = 0;
						fSumX[ipAcquCh[i]] = 0;

						fpRecogResp[ipAcquCh[i] + CHANNEL * 2] = (fpMaxX[ipAcquCh[i]] - fpMinX[ipAcquCh[i]]) / 2;
						fpMaxX[ipAcquCh[i]] = 0;
						fpMinX[ipAcquCh[i]] = 10000;

						fpChannelResp[ipAcquCh[i]] = fpRecogResp[CHANNEL*ipRecongMode[ipAcquCh[i]] + ipAcquCh[i]];
					}

					for (i = 0; i < iCtrLen; i++)
					{//所有控制通道
						fpWeightResp[0] += fpChannelResp[ipCtrCh[i]] * fpChannelWeight[ipCtrCh[i]];										//乘以权重再累加
						fpWeightResp[1] = fpWeightResp[1] >= fpChannelResp[ipCtrCh[i]] ? fpWeightResp[1] : fpChannelResp[ipCtrCh[i]];	//与已有最大值比较，取较大者为最大值
						fpWeightResp[2] = fpWeightResp[2] <= fpChannelResp[ipCtrCh[i]] ? fpWeightResp[2] : fpChannelResp[ipCtrCh[i]];	//与已有最小值比较，取较小者为最小值
					}
					fpWeightResp[0] = fpWeightResp[0] / fWeightSum;																//累加和除以权重和，得加权平均值
					fpChannelResp[CHANNEL] = fpWeightResp[iWeightMode];																//根据加权模式，选择加权结果{平均值or最大值or最小值}
					fpWeightResp[0] = 0;
					fpWeightResp[1] = 0;
					fpWeightResp[2] = 10000;

					fLevelRate = (fpReferGainGoal[iScheduleAdd] >= fReferGain) ? fLevelupRate : fLeveldownRate;
					fLevelOffset = (fpReferGainGoal[iScheduleAdd] >= fReferGain) ? fLevelupOffset : fLeveldownOffset;
					fTempGain = (fLevelRate * fCycletime + fLevelOffset) * fReferGain;						  //计算当前控制周期的驱动抬升倍数*原增益（速度曲线拟合公式：k*控制周期数/当前频率+偏置）
					iLevelChanging = ((fTempGain < fpReferGainGoal[iScheduleAdd]) ^ (fReferGain > fpReferGainGoal[iScheduleAdd])) && (fReferGain != fpReferGainGoal[iScheduleAdd]);
					fReferGain = iLevelChanging ? fTempGain : fpReferGainGoal[iScheduleAdd];
					iScheduleAdd += ((~iLevelChanging) && iAddMove[iScheduleAdd]);

					for (i = 0; i < iCalLen; i++)
					{

						while ((fFreq > fppReferFreq[ipReferType[ipCalCh[i]]][ipFreqMark[ipCalCh[i]]]) && (fFreqRate > 0)){
							(ipFreqMark[ipCalCh[i]])++;
						}
						while ((fFreq < fppReferFreq[ipReferType[ipCalCh[i]]][ipFreqMark[ipCalCh[i]] - 1]) && (fFreqRate < 0)){
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

						iEffectCh = CHANNEL;
						iEffectCh = (fFilterDrive[ipCalCh[i]] < fFilterDrive[iEffectCh] ? ipCalCh[i] : iEffectCh);

					}
					printf("f=%f,Ref=%f,Resp=%f,Drive=%f\n", fFreq, fpChannelRefer[CHANNEL], fpChannelResp[CHANNEL], fDrive);
					fDrive = fFilterDrive[iEffectCh];

					iLimitEnable = (iEffectCh == CHANNEL) ? 0 : 1;
					//getCmd
					//SendData&State
					//Caseswich
					
					

				}

				fDriveSignal = fDrive * fColaSin;

				iScheduleAdd += iGetEnd;//达到目标，计划表地址+1；未达到，+0
				iTestCase = (iScheduleAdd == iScheTableLen) ? 7 : iTestCase;

			}
		

			break;

		case 7:
			while (iTestCase == 7)
			{
				for (i = 0; i < CHANNEL; i++)
				{
					fpData[i] = fDriveSignal;
				};

				fPhase += PI2*fFreq / SAMPINGRATE;
				iZeroPass = (fPhase > PI2);
				fTempPhase = fPhase - PI2;
				fPhase = iZeroPass ? fTempPhase : fPhase;

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

					fDrive *= fTempGain;
					printf("f=%f,Drive=%f\n", fFreq, fDrive);
				}

				fDriveSignal = fDrive * fColaSin;

			}

			break;
		}
	}
	return 0;
}



/*void ControlLost(int *flag, int *Result, float SystemGain[], float Num, float Resp[], float Drive, float Safebox1, float Safebox2, int Enable, int ActiveCh[], int ActiveLen)
{
	int i = 0;
	int j = 0;
	float TempSystemGain[CHANNEL];
	float DevGain;
	*Result = 0;																		//结果清零
	for (i = 0; i < ActiveLen; i++){													//所有控制通道+限制通道
		TempSystemGain[ActiveCh[i]] = Resp[ActiveCh[i]] / Drive;						//当前系统增益计算（响应/驱动）
		DevGain = abs(TempSystemGain[ActiveCh[i]] - SystemGain[ActiveCh[i]]) / Num;		//当前系统增益的导数计算
		SystemGain[ActiveCh[i]] = TempSystemGain[ActiveCh[i]];							//保存当前系统增益
		j = (Resp[ActiveCh[i]] / Drive< Safebox1);										//系统增益（响应/驱动）是否超标
		j &= (DevGain<Safebox2);														//系统增益的一阶导（响应/驱动）是否超标
		*Result |= (j << ActiveCh[i]);													//超标是,通道对应位,置1
	}
	*flag = (*Result > 0) && Enable;													//结果大于0有通道控制信号丢失，功能激活时，开环标志置1，未激活时，开环标志置0
}
void AlarmAbort(int *flag, int *Result, float *Resp, float Refer, float Highbox, float Lowbox, float Enable, int ActiveCh[], int ActiveLen)
{
	int i = 0;
	int j = 0;
	int k = 0;
	*Result = 0;																		//结果清零
	for (i = 0; i < ActiveLen; i++){													//所有控制通道+限制通道
		j = (Resp[ActiveCh[i]] / Refer> Highbox);										//量级误差是否上限
		k = (Resp[ActiveCh[i]] / Refer< Lowbox) && Enable;								//量级误差是否下限，下限可被禁用
		j |= k;																			//上下限判断结果相与
		*Result |= (j << ActiveCh[i]);													//量级误差超限,通道对应位,置1
	}
	*flag = *Result > 0;																//结果大于0有通道控制信号超限
}*/

void LevelRate(float *fRate, float *fOffset,float RealRate, float LowT, float HighT)
{
	float LowRate;
	float HighRate;
	LowRate = powf(10, RealRate* LowT / 20.0f);
	HighRate = powf(10, RealRate* HighT / 20.0f);
	*fRate = (LowRate - HighRate) / (LowT - HighT);
	*fOffset = 0.975f*HighRate;
}

void InterpPoints(float *ifX, float *ifY, float *ifx, float *ify, int iXLenth, int ixLenth)
{
	int i;
	float ifdx;
	int ixAdd;

	ifdx = (ifx[ixLenth] - ifx[0]) / (iXLenth - 3);
	ifX[0] = 0.5f * ifx[0];
	ifY[0] = ify[0];
	ifX[iXLenth-1] = 1.1f * ifx[ixLenth];
	ifY[iXLenth-1] = ify[ixLenth];
	ifX[1] = ifx[0];
	ifY[1] = ify[0];
	ixAdd = 1;

	for (i = 2; i < (iXLenth - 1); i++)
	{
		ifX[i] = ifX[i - 1] + ifdx;

		while ((ifX[i] > ifx[ixAdd]) && (ixAdd < ixLenth))
		{
			ixAdd++;
		}

		ifY[i] = (ifX[i] - ifx[ixAdd - 1])*(ify[ixAdd] - ify[ixAdd - 1]) / (ifx[ixAdd] - ifx[ixAdd - 1]) + ify[ixAdd - 1];
	}
}


float OcttoRate(float Oct)
{
	float Rate;
	Rate = powf(2.0f, Oct / 60 / SAMPINGRATE) - 1;
	return(Rate);
}
