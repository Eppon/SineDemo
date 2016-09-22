// ConsoleApplication5.cpp : �������̨Ӧ�ó������ڵ㡣
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
		case 0:												//�ȴ���ʼ��
			while (iTestCase == 0)
			{
				iTestCase = SBufCmd.iTestCase;
			}

			break;

		case 1:                                             //���г�ʼ��
			fMaxNoise = SInitPara.fNoiseThreshold;
			
			break;

		case 2:												//ʱ������Ԥ��
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
			{											//�����������0������ͨ��������鲻ͨ��
				iTestCase = 7;							//������鲻�ɹ�
			}
			else
			{
				iTestCase = 4;							//�������ɹ�
			}
			printf("����=%d���������=&d,��������=%f", iTestCase,iNoiseResult,iNoiseResult);
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
				fFreq += (fFreqRate)* (fFreq);															//Ƶ���۳˵Ľ��
				llpDwellTime[iScheduleAdd] += (fFreqRate == 0);											//פ��ʱ���ۼ�
				iGetEnd1 = (fFreq < fpFreqGoal1[iScheduleAdd]);											//��ǰƵ���Ƿ�С������
				iGetEnd2 = (fFreq > fpFreqGoal2[iScheduleAdd]);											//��ǰƵ���Ƿ��������
				iGetEnd3 = (llpDwellTime[iScheduleAdd] >= llpTimeGoal[iScheduleAdd]);					//פ��ʱ�䵽��ʱ��Ŀ��
				fFreq = iGetEnd1 ? fpFreqGoal1[iScheduleAdd] : fFreq;									//�ﵽĿ��1��Ƶ��=Ŀ�ꣻδ�ﵽ��Ƶ��=�۳˽��
				fFreq = iGetEnd2 ? fpFreqGoal2[iScheduleAdd] : fFreq;									//�ﵽĿ��2��Ƶ��=Ŀ�ꣻδ�ﵽ��Ƶ��=�۳˽��
				iGetEnd = (iGetEnd1 | iGetEnd2 | iGetEnd3);
				iScheduleAdd += iGetEnd;																//�ﵽĿ�꣬�ƻ����ַ+1��δ�ﵽ��+0

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
																								
						for (i = 0; i < iCtrLen; i++){																						//���п���ͨ��
							fpWeightResp[0] += fpChannelResp[ipCtrCh[i]] * fpChannelWeight[ipCtrCh[i]];										//����Ȩ�����ۼ�
							fpWeightResp[1] = fpWeightResp[1] >= fpChannelResp[ipCtrCh[i]] ? fpWeightResp[1] : fpChannelResp[ipCtrCh[i]];	//���������ֵ�Ƚϣ�ȡ�ϴ���Ϊ���ֵ
							fpWeightResp[2] = fpWeightResp[2] >= fpChannelResp[ipCtrCh[i]] ? fpWeightResp[2] : fpChannelResp[ipCtrCh[i]];	//��������Сֵ�Ƚϣ�ȡ��С��Ϊ��Сֵ
						}
						fpWeightResp[0] = fpWeightResp[0] / fWeightSum;																		//�ۼӺͳ���Ȩ�غͣ��ü�Ȩƽ��ֵ
						fpChannelResp[CHANNEL] = fpWeightResp[iWeightMode];																	//���ݼ�Ȩģʽ��ѡ���Ȩ���{ƽ��ֵor���ֵor��Сֵ}
						fpWeightResp[0] = 0;
						fpWeightResp[1] = 0;
						fpWeightResp[2] = 10000;

						fLevelRate = (fReferGainGoal > fReferGainGoal) ? fLevelupRate : fLeveldownRate;
						fLevelOffset = (fReferGainGoal > fReferGainGoal) ? fLevelupOffset : fLeveldownOffset;
						iLevelChanging = (fReferGainGoal == fReferGainGoal);
						fTempGain = (fLevelRate * fCycletime + fLevelOffset) * fReferGain;                                       //���㵱ǰ�������ڵ�����̧������*ԭ���棨�ٶ�������Ϲ�ʽ��k*����������/��ǰƵ��+ƫ�ã�
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
	*Result = 0;																		//�������
	for (i = 0; i < ActiveLen; i++){													//���п���ͨ��+����ͨ��
		TempSystemGain[ActiveCh[i]] = Resp[ActiveCh[i]] / Drive;						//��ǰϵͳ������㣨��Ӧ/������
		DevGain = abs(TempSystemGain[ActiveCh[i]] - SystemGain[ActiveCh[i]]) / Num;		//��ǰϵͳ����ĵ�������
		SystemGain[ActiveCh[i]] = TempSystemGain[ActiveCh[i]];							//���浱ǰϵͳ����
		j = (Resp[ActiveCh[i]] / Drive< Safebox1);										//ϵͳ���棨��Ӧ/�������Ƿ񳬱�
		j &= (DevGain<Safebox2);														//ϵͳ�����һ�׵�����Ӧ/�������Ƿ񳬱�
		*Result |= (j << ActiveCh[i]);													//������,ͨ����Ӧλ,��1
	}
	*flag = (*Result > 0) && Enable;													//�������0��ͨ�������źŶ�ʧ�����ܼ���ʱ��������־��1��δ����ʱ��������־��0
}
void AlarmAbort(int *flag, int *Result, float *Resp, float Refer, float Highbox, float Lowbox, float Enable, int ActiveCh[], int ActiveLen)
{
	int i = 0;
	int j = 0;
	int k = 0;
	*Result = 0;																		//�������
	for (i = 0; i < ActiveLen; i++){													//���п���ͨ��+����ͨ��
		j = (Resp[ActiveCh[i]] / Refer> Highbox);										//��������Ƿ�����
		k = (Resp[ActiveCh[i]] / Refer< Lowbox) && Enable;								//��������Ƿ����ޣ����޿ɱ�����
		j |= k;																			//�������жϽ������
		*Result |= (j << ActiveCh[i]);													//��������,ͨ����Ӧλ,��1
	}
	*flag = *Result > 0;																//�������0��ͨ�������źų���
}*/

