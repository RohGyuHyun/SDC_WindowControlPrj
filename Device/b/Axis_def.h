#ifndef _Axis_def_h_
#define _Axis_def_h_
/******************  Axis Basic Info *******************/
#define MOTION_SYNC_TYPE 1

#define CW				0
#define CCW				1

#define _MMC             0
#define _FASTECH         1
#define _ADLink			 2
#define _AJINXL			 3
#define _DNET			 4
#define _NMC			 5

#define MAX_HOME_ORDER   4
const int HOME_ERROR = -1;
/****************** 장비별 축관련 정의 *******************/
//Motor 
#define MAX_AJIN_BD_AXIS_NUM				0
#define _USE_MLII_START_NUM					0
#define USE_MLII_SERVOPACK_NUM				4

#define SHUTTLE1_MULTY_GROUP				0 //151103 JSLee
#define SHUTTLE2_MULTY_GROUP				1 //151103 JSLee
typedef enum
{
	AXIS_ALIGN_X,
	AXIS_ALIGN_Y,
	AXIS_ALIGN_T,
	AXIS_ALIGN_Z,
	AXIS_SHUTTLE_X,
	AXIS_SHUTTLE_Z,
	AXIS_UNLOAD_X,
	AXIS_UNLOAD_Z,
	AXIS_LOADING_CONVEYOR,
	MAX_AXIS_NUM		
};

#endif
