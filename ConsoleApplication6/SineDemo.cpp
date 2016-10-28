// ConsoleApplication5.cpp : 定义控制台应用程序的入口点。
//
#include "stdafx.h"
#include "SineDemo.h"

#define GETCMD 	iCmdCount = SBufCmd.iCmdCount;\
				iExit = SBufCmd.iExit;\
				iCmd = SBufCmd.iCmd;\
				iTestCase = SBufCmd.iTestCase;\
				iScheduleAdd = SBufCmd.iScheduleAdd;\
				iSweepDirect = SBufCmd.iSweepDirect;\
				fTestLevel = SBufCmd.fTestlevel;\
				fSweepRate = SBufCmd.fSweepRate;\
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
					SUpdataPara.iSweepDirect = iSweepDirect;

//                    iSendCount++;
//                    byt1.append((char*)&SUpdataData,sizeof(SUpdataData));
//                    if (iSendCount==5)
//                    {
//                        iSendCount=0;
//                        emit SendData(byt1);
//                        msleep(100);
//                        byt1.clear();
//                    }
//

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

	/*流程参数*/
	int j;
	int i;
	int iExit = 0;
	int iTestCase = 1;
	int iCmd = 0;
	int iStopReason;
	int iCmdCount;

	/*时域参数*/
	float fpData[CHANNEL];
	float fDriveSignal;
	long long int llPreSampleNums;
	long long int llSampleNums;

	/*频率相位参数*/
	float fFreq;
	double dTempFreq;
	double dFreqRate;
	float fSweepRate;
	int   iSweepDirect;
	float fPhase;
	float fTempPhase;
	int   iZeroPass;
	int   iRefresh;
	int   iCtrCycles;
	int   iPassTimes;
	float fColaSin;
	float fColaCos;	

	

	/*多通道计算参数*/
	int ipAcquCh[CHANNEL];
	int iAcquChLen;
	int ipCtrCh[CHANNEL];
	int iCtrLen;
	int ipCalCh[CHANNEL];
	int iCalLen;

	/*预试验参数*/
	float fpNoiseLevel[CHANNEL];
	float fMaxNoise;
	int iNoiseResult;
	int iLoopResult;
	float fLoopCheckFreq;
	float fLoopCheckLevel;
	float fLoopCheckLimit;
	int iFRFCheck;
		
	/*计划表参数*/
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

	/*压缩策略参数*/
	float fLowFreq;
	float fHighFreq;
	float fLowTime;
	float fHighTime;
	int iMidCycles;
	float fCtrRadio;

	/*傅里叶滤波参数*/
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

	/*RMS参数*/
	float fSumX[CHANNEL];
	float fSumX2[CHANNEL];
	
	/*峰值法参数*/
	float fpMaxX[CHANNEL];
	float fpMinX[CHANNEL];

	/*识别结果*/
	float fpChannelResp[CHANNEL+1];
	float fpFourPhase[CHANNEL];
	float fpRecogResp[CHANNEL*3];
	int	  ipRecongMode[CHANNEL];
	float fpPreChannelResp[CHANNEL];
	float fpRespSlope[CHANNEL];
	
	/*多通道加权参数*/
	float fpWeightResp[3] = { 0, 0, 10000 };
	int   iWeightMode;
	float fWeightSum;
	float fpChannelWeight[CHANNEL];

	/*通道峰值判断，闭环判断参数*/
	int   ipChannelAbort[CHANNEL+1];
	float fpChannellimit[CHANNEL+1];
	int   iChannelAbortResult;
	int   iChannelAbort;
	int   iCSLorNot;
	int   iCSLResult;
	int   fCSLLevel;

	/*量级参数*/
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
	
	/*参考谱参数*/
	float fppReferFreq[CHANNEL][TABLELEN];
	float fppReferAmp[CHANNEL][TABLELEN];
	int   ipReferType[CHANNEL + 1];
	int   ipFreqMark[CHANNEL + 1];
	float fpChannelRefer[CHANNEL + 1];

	/*报警限中止限参数*/
	int iAlarmorNot;
	int iAbortOnce;
	int iAbortorNot;
	int iAbortCount;
	int   iAbortLoops;
	float fpUpAlarm[TABLELEN];
	float fpDownAlarm[TABLELEN];
	float fpUpAbort[TABLELEN];
	float fpDownAbort[TABLELEN];

	/*驱动参数*/
	float fCalDrive[CHANNEL + 1];
	float fFilterDrive[CHANNEL + 1];
	float fPreCalDrive[CHANNEL + 1];
	int   iEffectCh;
	int   iLimitEnable;
	float fDrive;
	float fMaxDrive;

	/*历史频响修正参数*/
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
		case 0:												//等待初始化
			while (iTestCase != 1)
			{
				/*接收指令*/
				if (iCmdCount != SBufCmd.iCmdCount)
				{
					GETCMD
				}
				/*发送状态*/
				SENDSTATE	
			}

			break;

		case 1:                                             //进行初始化
			
			/*发送状态*/
			SENDSTATE
			/*采集数据清零*/
			for (i = 0; i < CHANNEL; i++)
			{
				fpData[i] = 0.0f;
			}
			llPreSampleNums = 0;
			llSampleNums = 0;

			/*采集通道统计，控制通道统计，限制通道统计*/
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

			/*噪声清零*/
			for (i = 0; i < CHANNEL; i++)
			{
				fpNoiseLevel[i] = 0.0f;
			}
			/*预试验参数初始化*/
			fMaxNoise = SInputPara.fNoiseThreshold;
			iNoiseResult = 0;
			iLoopResult = 0;
			fLoopCheckFreq = SInputPara.fLoopCheckFreq;
			fLoopCheckLevel = SInputPara.fLoopCheckLevel;
			fLoopCheckLimit = SInputPara.fLoopCheckLimit;
			iFRFCheck = 1;

			/*试验初始设置初始化*/
			iRunMode = SInputPara.iRunMode;
			fInitialLevel = SInputPara.fInitialLevel;
			dFreqRate = 0;
			iSweepDirect = 1;
			fMaxDrive = SInputPara.fMaxDrive;
			

			/*频率、相位、整周期判断参数、Cola信号初始化*/
			fFreq = 0.2f;
			fPhase = 0.0f;
			fTempPhase = 0;
			iZeroPass = 0;
			iRefresh = 0;;
			iCtrCycles = 0;
			iPassTimes = 0;
			fColaSin = 0.0f;
			fColaCos = 1.0f;

			/*驱动驱动信号初始化*/
			fDrive = 0.0f;
			fDriveSignal = 0.0f;

			/*计划表初始化*/
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

			/*压缩策略初始化*/
			fLowFreq = SInputPara.fLowRadioFreq;
			fHighFreq = SInputPara.fHighRadioFreq;
			fLowTime = SInputPara.fLowRadioTime;
			fHighTime = SInputPara.fHighRadioTime;
			iMidCycles = SInputPara.fMidRadioCycles;
			fCtrRadio = 1;

			/*通道识别参数初始化*/
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
			}
			for (i = 0; i < CHANNEL; i++)
			{
				fpChannelWeight[i] /= fWeightSum;//权重归一化，权重=原权重/所有权重的和，加权计算改为：值*权重的积的累加
			}

			/*量级变化参数初始化*/
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

			/*安全参数设置*/
			fLevelGoal = 0;
			iAbortorNot = 0;
			iAlarmorNot = 0;
			iChannelAbort = 0;
			iChannelAbortResult = 0;
			iCSLorNot = 0;
			iCSLResult = 0;
			/*谱长度谱谱类型统计*/
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

			/*参考谱限制谱插值*/
			for (i = 0; i < iCalLen+1; i++)
			{
				InterpPoints(fppReferFreq[ipReferType[i]], fppReferAmp[ipReferType[i]], SInputPara.fppTableFreq[ipReferType[i]], SInputPara.fppTableAcc[ipReferType[i]], TABLELEN, ipxLenth[ipReferType[i]]);
			}

			/*AbortAlarm取值*/
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

			/*参考谱驱动清零*/
			for (i = 0; i < CHANNEL + 1; i++)
			{
				ipFreqMark[i] = (fpScheduleRate[1]>0) ? 1 : 2048;
				fpChannelRefer[i] = NOTZERO;
				fCalDrive[i] = NOTZERO;
				fFilterDrive[i] = NOTZERO;
				fpChannelResp[i] = NOTZERO;
				fPreCalDrive[i] = NOTZERO;
			}
			iEffectCh = CHANNEL;
			iLimitEnable = 0;

			/*计算理论驱动的频率轴*/
			fpIFRFFreq[0] = SInputPara.fMinFreq;
			fDeltaFreq = powf(2, (log2f(SInputPara.fMaxFreq / SInputPara.fMinFreq)) / (TABLELEN - 3));
			for (i = 1; i < TABLELEN; i++)
			{
				fpIFRFFreq[i] = fpIFRFFreq[i - 1] * fDeltaFreq;
			}

			/*理论驱动权重*/
			fTheorySetWeight = SInputPara.fTheoryWeight;
			
			/*初始化完毕跳转至case2*/
			//iTestCase = 2;
			/*初始化完毕跳转至case6*/
			iTestCase = 6;
			break;

		case 2:												//时域数据预览
			/*发送状态*/
			SENDSTATE
			/*等待命令*/
			while (iTestCase == 2)
			{
				GETCMD
			}
			break;

		case 3:
			/*发送状态*/
			SENDSTATE
			fDriveSignal = NOTZERO;
			iNoiseResult = 0;

			for (j = 0; j < NOISECHECKTIME; j += 8)
			{
				for (i = 0; i < CHANNEL; i++)
				{
					fpData[i] = fDriveSignal;
				}

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
			{											//噪声结果大于0，有通道噪声检查不通过
				iTestCase = 7;
				iStopReason = 1;						//噪声检查不成功
			}
			else
			{
				iTestCase = 4;							//噪声检查成功
			}
			printf("流程=%d，噪声结果=&d,噪声量级=%f", iTestCase, iNoiseResult, iNoiseResult);
			break;

		case 4:
			/*发送状态*/
			SENDSTATE
			fDriveSignal = NOTZERO;
			fDrive = 0.001f;

			while (iTestCase == 4)
			{
				

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

					/*闭环检查识别+判断*/
					iLoopResult = 0;
					for (i = 0; i < iAcquChLen; i++)
					{
						fpChannelResp[ipAcquCh[i]] = sqrtf((fSumX2[ipAcquCh[i]] - fSumX[ipAcquCh[i]] * fSumX[ipAcquCh[i]] / fLen) / fLen * 2);
						fSumX2[ipAcquCh[i]] = 0;
						fSumX[ipAcquCh[i]] = 0;
						iLoopResult |= ((fpChannelResp[ipAcquCh[i]] < fLoopCheckLevel) << ipAcquCh[i]);
					}
					/*发送状态*/
					SENDSTATE
					/*发送数据*/
					SENDDATA
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
				/*接收命令*/
				if (iCmdCount != SInputCmd.iCmdCount)
				{
					GETCMD
				}

				fDriveSignal = fDrive * fColaSin;
			}
			break;

		case 5:
			/*发送状态*/
			SENDSTATE
			iTestCase =7;
			iStopReason = 4;
			break;

		case 6:
			fDriveSignal = NOTZERO;
			fDrive = 0.001f;
			iAbortCount = 0;
			dTempFreq = fFreq;
			/*发送状态*/
			SENDSTATE
			while (iTestCase == 6)
			{
				llSampleNums++;
				for (i = 0; i < CHANNEL; i++)
				{
					fpData[i] = fDriveSignal;
				};
				dFreqRate = (iRunMode == 1) ? fSweepRate : fpScheduleRate[iScheduleAdd];
				dFreqRate = (iTestCase == 7) ? 0 : dFreqRate;
				dFreqRate *= iSweepDirect;
				dTempFreq += dFreqRate * dTempFreq;												//频率累乘的结果
				llpDwellTime[iScheduleAdd] += (dFreqRate == 0);									//驻留时间累加
				iGetEnd1 = (fFreq < fpFreqGoal1[iScheduleAdd]);									//当前频率是否到达下限
				iGetEnd2 = (fFreq > fpFreqGoal2[iScheduleAdd]);									//当前频率是否到达上限
				iGetEnd3 = (llpDwellTime[iScheduleAdd] >= llpTimeGoal[iScheduleAdd]);			//驻留时间到达时间目标
				dTempFreq = iGetEnd1 ? fpFreqGoal1[iScheduleAdd] : dTempFreq;					//达到目标1，频率=目标；未达到，频率=累乘结果
				dTempFreq = iGetEnd2 ? fpFreqGoal2[iScheduleAdd] : dTempFreq;					//达到目标2，频率=目标；未达到，频率=累乘结果
				fFreq = dTempFreq;
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
					fCompLen = SAMPINGRATE * (fPhase) / (PI2*fFreq);
					fLen = (float)(llSampleNums - llPreSampleNums);
					fFourLen = fLen + fPreCompLen - fCompLen;
					fPreCompLen = fCompLen;
					llPreSampleNums = llSampleNums;
					fCycletime = fLen*SAMPINGTIME;

					iChannelAbortResult = 0;
					iCSLResult = 0;
					for (i = 0; i < iAcquChLen; i++)
					{
						/*傅里叶滤波法*/
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
						
						/*RMS法*/
						fpRecogResp[ipAcquCh[i] + CHANNEL] = sqrtf((fSumX2[ipAcquCh[i]] - fSumX[ipAcquCh[i]] * fSumX[ipAcquCh[i]] / fLen) / fLen * 2);
						fSumX2[ipAcquCh[i]] = 0;
						fSumX[ipAcquCh[i]] = 0;

						/*峰值法*/
						fpRecogResp[ipAcquCh[i] + CHANNEL * 2] = (fpMaxX[ipAcquCh[i]] - fpMinX[ipAcquCh[i]]) / 2;
						fpMaxX[ipAcquCh[i]] = 0;
						fpMinX[ipAcquCh[i]] = 10000;

						/*通道响应选取（三种方法的识别结果）*/
						fpChannelResp[ipAcquCh[i]] = fpRecogResp[CHANNEL*ipRecongMode[ipAcquCh[i]] + ipAcquCh[i]];

						/*独立的通道中止判断*/
						iChannelAbortResult |= (((fpChannelResp[ipAcquCh[i]] > fpChannellimit[ipAcquCh[i]] * fpChannelRefer[ipAcquCh[i]]) && (ipChannelAbort[i]==1) & (iScheduleAdd > 0)) << ipAcquCh[i]);
						/*CSL判断*/
						iCSLResult |= (((fpChannelResp[ipAcquCh[i]] < fCSLLevel * fpChannelRefer[ipAcquCh[i]])&(iScheduleAdd > 0)) << ipAcquCh[i]);
						fpRespSlope[ipAcquCh[i]] = (fpChannelResp[ipAcquCh[i]] - fpPreChannelResp[ipAcquCh[i]]) / fCycletime;
						fpPreChannelResp[ipAcquCh[i]] = fpChannelResp[ipAcquCh[i]];
					}

					iChannelAbort = (iChannelAbortResult > 0);
					iCSLorNot = (iCSLResult > 0);

					/*控制通道加权*/
					fpWeightResp[0] = 0;
					fpWeightResp[1] = 0;
					fpWeightResp[2] = 10000;
					for (i = 0; i < iCtrLen; i++)
					{
						fpWeightResp[0] += fpChannelResp[ipCtrCh[i]] * fpChannelWeight[ipCtrCh[i]];										//乘以权重再累加
						fpWeightResp[1] = fpWeightResp[1] >= fpChannelResp[ipCtrCh[i]] ? fpWeightResp[1] : fpChannelResp[ipCtrCh[i]];	//与已有最大值比较，取较大者为最大值
						fpWeightResp[2] = fpWeightResp[2] <= fpChannelResp[ipCtrCh[i]] ? fpWeightResp[2] : fpChannelResp[ipCtrCh[i]];	//与已有最小值比较，取较小者为最小值
					}
					fpChannelResp[CHANNEL] = fpWeightResp[iWeightMode];																//根据加权模式，选择加权结果{平均值or最大值or最小值}
					
					/*Alarm判断
					1.判断是否大于上限
					2.判断是否小于下限
					3.限制通道生效时，下限判断不起作用
					4.计划表执行到试验初始量级之前（地址=0），报警不起作用*/
					iAlarmorNot = (fpChannelResp[CHANNEL] > fpUpAlarm[ipFreqMark[CHANNEL]] * fpChannelRefer[CHANNEL]);
					iAlarmorNot |= ((fpChannelResp[CHANNEL]< fpDownAlarm[ipFreqMark[CHANNEL]] * fpChannelRefer[CHANNEL])&(iLimitEnable != 0));
					iAlarmorNot &= (iScheduleAdd > 0);
					/*Abort判断
					1.判断是否大于上限
					2.判断是否小于下限
					3.限制通道生效时，下限判断不起作用
					4.计划表执行到试验初始量级之前（地址=0），中止不起作用
					5.每触发一次，中止计数+1
					6.中止次数到达设置值时，试验中止
					7.中止次数在执行计划表下一项时，清零*/
					iAbortOnce = (fpChannelResp[CHANNEL] > fpUpAbort[ipFreqMark[CHANNEL]] * fpChannelRefer[CHANNEL]);
					iAbortOnce |= ((fpChannelResp[CHANNEL] < fpDownAbort[ipFreqMark[CHANNEL]] * fpChannelRefer[CHANNEL])&(iLimitEnable != 0));
					iAbortOnce &= (iScheduleAdd > 0);
					iAbortCount += iAbortOnce;
					iAbortorNot = (iAbortCount >= iAbortLoops);
					iAbortCount = (iGetEnd == 1) ? 0 : iAbortCount;
					
					/*试验中止判断*/
					iTestCase = ((iAbortorNot == 0) && (iChannelAbort == 0) && (iCSLorNot == 0)) ? iTestCase : 7;
					iStopReason = iChannelAbort ? 5 : iAbortorNot;
					iStopReason = iAbortorNot ? 6 : iAbortorNot;
					iStopReason = iCSLorNot ? 7 : iAbortorNot;

					/*试验量级变化*/
					fLevelGoal = (iRunMode != 1) ? fpReferGainGoal[iScheduleAdd] : fTestLevel;
					fLevelRate = (fLevelGoal >= fReferGain) ? fLevelupRate : fLeveldownRate;
					fLevelOffset = (fLevelGoal >= fReferGain) ? fLevelupOffset : fLeveldownOffset;
					fTempGain = (fLevelRate * fCycletime + fLevelOffset) * fReferGain;						  //计算当前控制周期的驱动抬升倍数*原增益（速度曲线拟合公式：k*控制周期数/当前频率+偏置）
					iLevelChanging = ((fTempGain < fLevelGoal) ^ (fReferGain > fLevelGoal)) && (fReferGain != fLevelGoal);
					fReferGain = iLevelChanging ? fTempGain : fpReferGainGoal[iScheduleAdd];
					iScheduleAdd += ((~iLevelChanging) && iAddMove[iScheduleAdd]);

					/*当前逆频响保存*/
					fInvFreqResp = fDrive / fpChannelResp[CHANNEL];

					while ((fFreq > fpIFRFFreq[iIFRFMark]) && (dFreqRate > 0))
					{
						fpUpIFRF[iIFRFMark] = Interp(fPrePreFreq, fPreInvFreqResp, fPreFreq, fInvFreqResp, fpIFRFFreq[iIFRFMark]);
						(iIFRFMark)++;
					}
					while ((fFreq < fpIFRFFreq[iIFRFMark- 1]) && (dFreqRate < 0))
					{
						fpDownIFRF[iIFRFMark-1] = Interp(fPrePreFreq, fPreInvFreqResp, fPreFreq, fInvFreqResp, fpIFRFFreq[iIFRFMark-1]);
						(iIFRFMark)--;
					}
					fPrePreFreq = fPreFreq;
					fPreFreq = fFreq;
					fPreInvFreqResp = fInvFreqResp;

					/*历史逆频响计算*/
					fHighIFRF = (dFreqRate >= 0) ? fpUpIFRF[iIFRFMark] : fpDownIFRF[iIFRFMark];
					fLowIFRF = (dFreqRate >= 0) ? fpUpIFRF[iIFRFMark - 1] : fpDownIFRF[iIFRFMark - 1];
					fTheoryWeight = ((fHighIFRF > 0) && (fLowIFRF > 0)) ? fTheorySetWeight : 0;
					fTheoryIFRF = Interp(fpIFRFFreq[iIFRFMark - 1], fLowIFRF, fpIFRFFreq[iIFRFMark], fHighIFRF, fFreq);
					
					/*控制参考谱计算*/
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

					/*控制驱动计算*/
					fCalDrive[CHANNEL] = fInvFreqResp * fpChannelRefer[CHANNEL];
					fCalDrive[CHANNEL] = (fCalDrive[CHANNEL]>NOTZERO ? fCalDrive[CHANNEL] : NOTZERO);
					fFilterDrive[CHANNEL] = sqrtf(fCalDrive[CHANNEL] * fPreCalDrive[CHANNEL]);
					fPreCalDrive[CHANNEL] = fCalDrive[CHANNEL];
					iEffectCh = CHANNEL;
					/*历史驱动计算*/
					fTheoryDrive = fTheoryIFRF * fpChannelRefer[CHANNEL];
					/*历史驱动加权*/
					fFilterDrive[CHANNEL] = fFilterDrive[CHANNEL] + fTheoryWeight*(fTheoryDrive - fFilterDrive[CHANNEL]);


					/*计算限制通道的谱和驱动*/
					for (i = 0; i < iCalLen; i++)
					{
						/*谱计算*/
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
						/*限制通道与控制通道取较小驱动*/
						iEffectCh = (fFilterDrive[ipCalCh[i]] < fFilterDrive[iEffectCh] ? ipCalCh[i] : iEffectCh);
					}

					/*驱动选择：
					1、控制驱动与限制驱动中，选择最小驱动作为实际发出驱动
					2、驱动最大不超过限值
					3、选择限制通道驱动时，限制通道使能（iLimitEnable置真）*/
					fDrive = fFilterDrive[iEffectCh]; 
					fDrive = (fFilterDrive[iEffectCh] < fMaxDrive) ? fDrive : fMaxDrive;
					iLimitEnable = (iEffectCh != CHANNEL);
															
					printf("Ff=%f,Ref=%f,Resp=%f,Drive=%f\n", fFreq, fpChannelRefer[CHANNEL], fpChannelResp[CHANNEL], fDrive);
					
					if ((iTestCase != 7) && (iCmdCount != SBufCmd.iCmdCount))
					{
						GETCMD
					}
					/*发送数据*/
					SENDDATA

					/*发送状态*/
					SENDSTATE
					//Caseswich
				}

				fDriveSignal = fDrive * fColaSin;

				iScheduleAdd += iGetEnd;//达到目标，计划表地址+1；未达到，+0
				iTestCase = (iScheduleAdd == iScheTableLen) ? 7 : iTestCase;
				iStopReason = (iScheduleAdd == iScheTableLen) ? 8 : iStopReason;
			}
		

			break;

		case 7:
			/*发送状态*/
			SENDSTATE
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

					
					printf("f=%f,Drive=%f\n", fFreq, fDrive);
					/*发送状态*/
					SENDSTATE
					/*发送数据*/
					SENDDATA
					fDrive *= fTempGain;

					if (iLevelChanging == 0)
					{
						iTestCase = ((iStopReason = 2) && (iFRFCheck == 1)) ? 5 : iTestCase;
						iTestCase = ((iStopReason = 2) && (iFRFCheck == 0) && (iRunMode != 1)) ? 6 : iTestCase;
						iTestCase = ((iStopReason = 4) && (iRunMode != 1)) ? 6 : iTestCase;
						if ((iCmdCount != SInputCmd.iCmdCount) && (iTestCase == 7))
							{
								GETCMD
							}

					}
				}

				fDriveSignal = fDrive * fColaSin;

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
