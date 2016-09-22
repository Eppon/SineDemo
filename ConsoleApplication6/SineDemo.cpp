// ConsoleApplication5.cpp : 定义控制台应用程序的入口点。
//
#include "stdafx.h"
#include "SineDemo.h"

int _tmain(int argc, _TCHAR* argv[])

{
	int j;
	int i;
	int iTestCase;

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
	float iLoopStop;


	float fInitialLevel;

	float fFreqRate;
	float iSweepDirect;

	int   iScheduleAdd;
	float fpScheduleRate[100];
	float fpFreqGoal1[100];
	float fpFreqGoal2[100];
	long long int llpTimeGoal[100];
    long long int llpDwellTime[100];
	int   iAddMove[100];
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

	float fReferGain;
	float fReferGainGoal;
	float fLevelupRate;
	float fLeveldownRate;
	float fLevelupOffset;
	float fLeveldownOffset;
	float fLevelRate;
	float fLevelOffset;
	int   iLevelChanging;
	float fTempGain;
	float fCycletime;

	float fppReferFreq[CHANNEL][2048];
	float fppReferAmp[CHANNEL][2048];
	int   ipReferType[CHANNEL + 1];
	int   ipFreqMark[CHANNEL + 1];
	float fpChannelRefer[CHANNEL + 1];

	float fCalDrive[CHANNEL + 1];
	float fFilterDrive[CHANNEL + 1];
	float fChannelResp[CHANNEL + 1];
	float fPreCalDrive[CHANNEL + 1];
	int   iEffectCh;
	int   iLimitEnable;


	while (SInputCmd.iExit != 0)
	{
		switch (iTestCase)
		{
		case 0:												//等待初始化
			while (iTestCase == 0)
			{
				iTestCase = SBufCmd.iTestCase;
			}

			break;

		case 1:                                             //进行初始化
			fMaxNoise = SInitPara.fNoiseThreshold;
			
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
			printf("流程=%d，噪声结果=&d,噪声量级=%f", iTestCase,iNoiseResult,iNoiseResult);
			break;

		case 4:
			fDriveSignal = NOTZERO;
			iLoopStop = 0;
			fDrive = 0.03f*fInitialLevel;

			while (iTestCase == 4)
			{
				iLoopResult = 0;

				llSampleNums++;
				for (i = 0; i < CHANNEL; i++)
				{
					fpData[i] = fDriveSignal;
				};
				
				fFreq = fLoopCheckFreq;
								
				fPhase += fFreq / SAMPINGRATE;
				iZeroPass = (fPhase > 1);
				fTempPhase = fPhase - 1;
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
						iLoopResult |= ((fpChannelResp[ipAcquCh[i]]<fLoopCheckLevel) << ipAcquCh[i]);
					}

					if (iLoopStop == 0)
					{
						if (iLoopResult > 0)
						{
							if (fDrive > fLoopCheckLimit)
							{
								iLoopStop = 1;
								fDrive *= 0.707f;
							}
							else fDrive *= 1.5;
						}
						else if (SPretestPara.iPretestType == 2)
						{
							iTestCase = 5;
						}
						else iTestCase = 6;
					}
					else if (fDrive <= fInitialLevel)
						iTestCase = 7;
					else fDrive *= 0.707f;
				}
			}

			break;

		case 5:;
			iTestCase = 6;

			break;

		case 6:
			while (iTestCase == 6)
			{
				fFreqRate = iSweepDirect * fpScheduleRate[iScheduleAdd];
				fFreq += (fFreqRate)* (fFreq);															//频率累乘的结果
				llpDwellTime[iScheduleAdd] += (fFreqRate == 0);											//驻留时间累加
				iGetEnd1 = (fFreq < fpFreqGoal1[iScheduleAdd]);											//当前频率是否小于下限
				iGetEnd2 = (fFreq > fpFreqGoal2[iScheduleAdd]);											//当前频率是否大于上限
				iGetEnd3 = (llpDwellTime[iScheduleAdd] >= llpTimeGoal[iScheduleAdd]);					//驻留时间到达时间目标
				fFreq = iGetEnd1 ? fpFreqGoal1[iScheduleAdd] : fFreq;									//达到目标1，频率=目标；未达到，频率=累乘结果
				fFreq = iGetEnd2 ? fpFreqGoal2[iScheduleAdd] : fFreq;									//达到目标2，频率=目标；未达到，频率=累乘结果
				iGetEnd = (iGetEnd1 | iGetEnd2 | iGetEnd3);
				iScheduleAdd += iGetEnd;																//达到目标，计划表地址+1；未达到，+0

				fPhase += fFreq / SAMPINGRATE;
				iZeroPass = (fPhase > 1);
				fTempPhase = fPhase - 1;
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
																								
						for (i = 0; i < iCtrLen; i++){																						//所有控制通道
							fpWeightResp[0] += fpChannelResp[ipCtrCh[i]] * fpChannelWeight[ipCtrCh[i]];										//乘以权重再累加
							fpWeightResp[1] = fpWeightResp[1] >= fpChannelResp[ipCtrCh[i]] ? fpWeightResp[1] : fpChannelResp[ipCtrCh[i]];	//与已有最大值比较，取较大者为最大值
							fpWeightResp[2] = fpWeightResp[2] >= fpChannelResp[ipCtrCh[i]] ? fpWeightResp[2] : fpChannelResp[ipCtrCh[i]];	//与已有最小值比较，取较小者为最小值
						}
						fpWeightResp[0] = fpWeightResp[0] / fWeightSum;																		//累加和除以权重和，得加权平均值
						fpChannelResp[CHANNEL] = fpWeightResp[iWeightMode];																	//根据加权模式，选择加权结果{平均值or最大值or最小值}
						fpWeightResp[0] = 0;
						fpWeightResp[1] = 0;
						fpWeightResp[2] = 10000;

						fLevelRate = (fReferGainGoal > fReferGainGoal) ? fLevelupRate : fLeveldownRate;
						fLevelOffset = (fReferGainGoal > fReferGainGoal) ? fLevelupOffset : fLeveldownOffset;
						iLevelChanging = (fReferGainGoal == fReferGainGoal);
						fTempGain = (fLevelRate * fCycletime + fLevelOffset) * fReferGain;                                       //计算当前控制周期的驱动抬升倍数*原增益（速度曲线拟合公式：k*控制周期数/当前频率+偏置）
						fReferGain = iLevelChanging ? fTempGain : fReferGain;
						iScheduleAdd += (iLevelChanging && iAddMove[iScheduleAdd]);

						for (i = 0; i < iCalLen; i++){

							while ((fFreq > fppReferFreq[ipReferType[ipCalCh[i]]][ipFreqMark[ipCalCh[i]]]) && (fFreqRate > 0)){
								(ipFreqMark[ipCalCh[i]])++;
							}
							while ((fFreq < fppReferFreq[ipReferType[ipCalCh[i]]][ipFreqMark[ipCalCh[i]] - 1]) && (fFreqRate < 0)){
								(ipFreqMark[ipCalCh[i]])--;
							}
							fpChannelRefer[ipCalCh[i]] = fppReferAmp[ipReferType[ipCalCh[i]]][ipFreqMark[ipCalCh[i]]] - (fppReferAmp[ipReferType[ipCalCh[i]]][ipFreqMark[ipCalCh[i]]] - fppReferAmp[ipReferType[ipCalCh[i]]][ipFreqMark[ipCalCh[i]] - 1])\
								*(fppReferFreq[ipReferType[ipCalCh[i]]][ipFreqMark[ipCalCh[i]]] - fFreq) / (fppReferFreq[ipReferType[ipCalCh[i]]][ipFreqMark[ipCalCh[i]]] - fppReferFreq[ipReferType[ipCalCh[i]]][ipFreqMark[ipCalCh[i]] - 1]);
							
							fpChannelRefer[ipCalCh[i]] *= fReferGain;


							fCalDrive[ipCalCh[i]] = fDrive / fChannelResp[ipCalCh[i]] * fpChannelRefer[ipCalCh[i]];
							fCalDrive[ipCalCh[i]] = (fCalDrive[ipCalCh[i]]>NOTZERO ? fCalDrive[ipCalCh[i]] : NOTZERO);
							fFilterDrive[ipCalCh[i]] = sqrtf(fCalDrive[ipCalCh[i]] * fPreCalDrive[ipCalCh[i]]);
							fFilterDrive[ipCalCh[i]] = (fFilterDrive[ipCalCh[i]]>NOTZERO ? fCalDrive[ipCalCh[i]] : NOTZERO);
							fPreCalDrive[ipCalCh[i]] = fCalDrive[ipCalCh[i]];

							iEffectCh = (fFilterDrive[ipCalCh[i]] < fFilterDrive[iEffectCh] ? ipCalCh[i] : iEffectCh);
						}

						fDrive = fFilterDrive[iEffectCh];

						iLimitEnable = (iEffectCh == CHANNEL) ? 0 : 1;
					}

					fDriveSignal = fDrive * fColaSin;
	
				}
			}

			break;

		case 7:
			while (iTestCase == 7)
			{
				for (i = 0; i < CHANNEL; i++)
				{
					fpData[i] = fDriveSignal;
				};

				fPhase += fFreq / SAMPINGRATE;
				iZeroPass = (fPhase > 1);
				fTempPhase = fPhase - 1;
				fPhase = iZeroPass ? fTempPhase : fPhase;

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
					iLevelChanging = (fDrive <= 0.001f);
					fTempGain = fLevelRate * fCycletime + fLevelOffset;
					fTempGain = iLevelChanging ? fTempGain : 1;

					fDrive *= fTempGain;

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

