//#include "Stats.h"
//#include "Game/Pickup.h"
//
//StatsGameStruct statsStruct;
//
//void AddFile(void)
//{
//	statsStruct.filesCollectedLvl++;
//}
//
//void LooseAllFiles(void)
//{
//	statsStruct.filesCollectedLvl = 0;
//	statsStruct.filesCollectedAll = 0;
//	statsStruct.allFilesAllLvl = 0;
//}
//
//void GetAllFiles(void)
//{
//	statsStruct.filesCollectedAll += statsStruct.filesCollectedLvl;
//	statsStruct.filesCollectedLvl = 0;
//}
//
//float PercentFilesCollected(void)
//{
//	float value1 = (float)statsStruct.filesCollectedAll;
//	float value2 = (float) statsStruct.allFilesAllLvl;
//	return value1 / value2;
//}
//
//void AddEnemyKilled(void)
//{
//	statsStruct.EnemyKilled++;
//}
//
//void AddNewDeath(void)
//{
//	statsStruct.deathTotal++;
//}
//
//void AddFilesAllOfLvl(void)
//{
//	statsStruct.allFilesAllLvl += GetPickupNb();
//}
//
//void UpdateTime(float _dt)
//{
//	statsStruct.time.seconds += _dt;
//	if (statsStruct.time.seconds > 60.0f)
//	{
//		statsStruct.time.minutes++;
//		statsStruct.time.seconds = _dt;
//
//		if (statsStruct.time.minutes == 60)
//		{
//			statsStruct.time.hours++;
//			statsStruct.time.minutes = 0;
//		}
//	}
//}
