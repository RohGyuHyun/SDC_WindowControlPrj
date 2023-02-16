#ifndef _DEFINE_H_
#define _DEFINE_H_

//AUTORUN Mode: Current Status define
#define MANUAL_MODE		100
#define AUTORUN_MODE	101
#define ERROR_STOP		200
#define STEP_STOP		201
#define START_RUN		202
#define RUN				203
#define IDLE_RUN		204
#define OP_CALL			205
#define DRY_RUN			206
#define COM_ERROR		207
#define EMO_STOP		208
#define NG_STOP         209

//TOP MENU
#define MAINVIEW		0
#define MANUALVIEW		1
#define TEACHVIEW		3
#define COMVIEW			4
#define DATAVIEW		5

//BOTTOM MENU
#define MAIN			0
#define TACTTIME		1
#define PRODUCT			2
#define ALARM			3


//SYSTEM		
#define SYSTEM_TITLE	                    _T("WINDOW 외관 검사기")
#define SYSTEM_VERSION	                    _T("Ver : 1.0.0")

#define LOG_IN_NAME_OPERATOR	_T("Operator")
#define LOG_IN_NAME_ENGINEER	_T("Engineer")
#define LOG_IN_NAME_MAKER		_T("Maker")

#define OPERATOR			0
#define ENGINEER			1
#define MAKER				2

#define DATA_BASIC_PATH			_T("D:\\DATABASE")
#define DATA_SYSTEM_PATH		_T("D:\\DATABASE\\SYSTEM")
#define DATA_SYSTEM_FILENAME	_T("SystemData.ini")
#define DATA_MODEL_PATH			_T("D:\\DATABASE\\MODEL")
#define SYSYEM_LOG_PATH			_T("D:\\DATABASE\\LOG\\SYSTEM")
#define LOGIN_LOG_PATH			_T("D:\\DATABASE\\LOG\\LOGIN")
#define ALARM_LOG_PATH			_T("D:\\DATABASE\\LOG\\ALARM")
#define PACKET_LOG_PATH			_T("D:\\DATABASE\\LOG\\PACKET")
#define RESULT_LOG_PATH			_T("D:\\DATABASE\\LOG\\RESULT")
#define TACTTIME_LOG_PATH		_T("D:\\DATABASE\\LOG\\TACTTIME")

#define SYSTEM_LOG			0
#define LOGIN_LOG				1
#define ALARM_LOG				2
#define PACKET_LOG			3
#define RESULT_LOG			4
#define TACTTIME_LOG		5
#define MODEL_MAX_NUM		50


#define ERR_CHAR			20
#define MAX_CHAR			512


//TIMER
#define TIMER_TOP_DATE				100
#define TIMER_IO_READ					200
#define TIMER_AXIS_STATE				500
#define TIMER_MAIN_VIEW				501
#define TIMER_TEACH_VIEW			503
#define TIMER_AXIS_MANAGE			506
#define TIMER_ORIGIN_DLG_DOUBLE			508
#define TIMER_ORIGIN_DLG				509
#define TIMER_MANUAL_UNIT1			510
#define TIMER_JOG_DLG					515
#define TIMER_VISION_CONNECT		516
#define TIMER_COM_VISION_CHECK		517
#define TIMER_MAIN_TACT_TIME_INFO	520
#define TIMER_TEACH_PROGRESS		521
#define TIMER_MAIN_ALARM_INFO		525

//COLOR
// Color Sample
const COLORREF BACKGROUND_COLOR						= RGB(230, 230, 230);
const COLORREF CLOUDBLUE							= RGB(128, 184, 223);
const COLORREF WHITE								= RGB(255, 255, 255);
const COLORREF BLACK								= RGB(1, 1, 1);
const COLORREF DKGRAY								= RGB(128, 128, 128);
const COLORREF LTGRAY								= RGB(192, 192, 192);
const COLORREF LTYELLOW								= RGB(255, 255, 128);
const COLORREF YELLOW								= RGB(255, 255, 0);
const COLORREF DKYELLOW								= RGB(128, 128, 0);
const COLORREF RED									= RGB(255, 0, 0);
const COLORREF DKRED								= RGB(128, 0, 0);
const COLORREF BLUE									= RGB(0, 0, 255);
const COLORREF DKBLUE								= RGB(0, 0, 220);
const COLORREF CYAN									= RGB(0, 255, 255);
const COLORREF DKCYAN								= RGB(0, 128, 128);
const COLORREF GREEN								= RGB(0, 255, 0);
const COLORREF DKGREEN								= RGB(0, 128, 0);
const COLORREF MAGENTA								= RGB(255, 0, 255);
const COLORREF DKMAGENTA							= RGB(128, 0, 128);
const COLORREF BASIC								= RGB(212, 208, 200);
const COLORREF LTSKYBLUE							= RGB(128,255,255);
const COLORREF LTVIOLET								= RGB(157,157,255);
const COLORREF DATA_ITEM_CHANGE_BACKGROUND_COLOR	= CYAN;
const COLORREF DATA_ITEM_CHANGE_TEXT_COLOR			= BLACK;
const COLORREF DATA_ITEM_TEXT_COLOR					= BLACK;
const COLORREF DATA_ITEM_BACKGROUND_COLOR			= CYAN;

///////////////////////////////////////////////////////////////
//ALARM CODE
#define ALARM_CODE_EMO			"Err000"
#define ALARM_CODE_SAFETYKEY "Err001"
#define ALARM_CODE_GPSPOWER "Err002"
#define ALARM_CODE_SERVOMC "Err003"
#define ALARM_CODE_DOOR_FL	"Err004"
#define ALARM_CODE_DOOR_FR	"Err005"
#define ALARM_CODE_DOOR_RL	"Err006"
#define ALARM_CODE_DOOR_RR	"Err007"
#define ALARM_CODE_MAINAIR "Err008"
#define ALARM_CODE_LIGHTCURTAIN "Err009"

#define ALARM_CODE_PICKERUP "Err010"
#define ALARM_CODE_PICKERDOWN "Err011"
#define ALARM_CODE_PICKER_VAC "Err012"

#define ALARM_CODE_SHUTTLE_VAC "Err015"

#define ALARM_CODE_JIGB_VAC_FAIL "Err018"
#define ALARM_CODE_JIGB_EMPTY "Err019"
#define ALARM_CODE_JIGB_IN "Err020"
#define ALARM_CODE_JIGB_OUT "Err021"
#define ALARM_CODE_JIGB_CLAMP_FORWARD "Err022"
#define ALARM_CODE_JIGB_CLAMP_BACK "Err023"
#define ALARM_CODE_JIGB_CLAMP_UP "Err024"
#define ALARM_CODE_JIGB_CLAMP_DOWN "Err025"
#define ALARM_CODE_JIGB_TILT_UP "Err026"
#define ALARM_CODE_JIGB_TILT_DOWN "Err027"
#define ALARM_CODE_JIGB_PLATE_UP "Err028"
#define ALARM_CODE_JIGB_PLATE_DOWN "Err029"

#define ALARM_CODE_JIGA_IN "Err030"
#define ALARM_CODE_JIGA_OUT "Err031"
#define ALARM_CODE_JIGA_VAC_FAIL "Err032"

#define ALARM_CODE_AXIS_Y_MOVEFAIL "Err050"
#define ALARM_CODE_AXIS_X_MOVEFAIL "Err051"

#define ALARM_CODE_READY_TIMEOUT "Err070"
#define ALARM_CODE_START_TIMEOUT "Err071"

#define ALARM_CODE_MUTE_PUSH_COMPLETE "Err080"
#define ALARM_CODE_MUTE_AUTO_FAIL "Err081"

#define ALARM_CODE_VISION_GRAB_START_TIMEOUT "Err090"
#define ALARM_CODE_VISION_GRAB_END_TIMEOUT "Err091"
#define ALARM_CODE_VISION_RESULT_TIMEOUT "Err092"


///////////////////////////////////////////////////////////////
//IO
#define MAX_INPUT_NUM		64
#define MAX_OUTPUT_NUM		64
#define MAX_DIO_NUM		32

#define MAX_DIO_MODULE_NUM	4

#define DIO_INPUT_INDEX_1	0
#define DIO_OUTPUT_INDEX_1	1
#define DIO_INPUT_INDEX_2	2
#define DIO_OUTPUT_INDEX_2	3



//INPUT
#define SAFETY_KEY_AUTO_CHECK												0
#define GPS_POWER_FAILURE_ALARM_CHECK							2
#define SERVO_MC_ON_OFF_CHECK											3
#define FRONT_LEFT_DOOR_CHECK											4
#define FRONT_RIGHT_DOOR_CHECK											5
#define REAR_LEFT_DOOR_CHECK												6
#define REAR_RIGHT_DOOR_CHECK											7	
#define OP_EMS_CHECK																8
#define LIGHT_CURTAIN_DETECT_CHECK									9
#define LIGHT_CURTAIN_MUTING_START_CHECK_AUTO				10				//명칭변경  2016.04.07
#define LIGHT_CURTAIN_MUTING_END_CHECK							11				//신규 2016.03.29
#define LD_PNP_CYLINDER_UP_CHECK										12	
#define LD_PNP_CYLINDER_DN_CHECK										13	
#define LIGHT_CURTAIN_MUTING_START_CHECK_MANUAL			15				//신규  2016.03.30
#define START_SWITCH_1															32
#define READY_SWITCH_1															33
#define RESET_SWITCH																34
#define START_SWITCH_2															35
#define READY_SWITCH_2															36
#define MAIN_AIR_CHECK																37
#define LD_PNP_VAC_ON_CHECK												47
#define FILM_JIG_GLASS_CHECK													48
#define FILM_JIG_VAC_ON_CHECK												49
#define FILM_JIG_CYLINDER_OUT_CHECK									50				//자리변경 2016.04.07
#define FILM_JIG_CYLINDER_IN_CHECK										51				//자리변경 2016.04.07 
#define FILM_JIG_GLASS_CLAMP_CYLINDER_BACK_CHECK			52				//자리변경 2016.04.07
#define FILM_JIG_GLASS_CLAMP_CYLINDER_FEED_CHECK			53				//자리변경 2016.04.07
#define FILM_JIG_GLASS_CLAMP_CYLINDER_UNCLAMP_CHECK	54				//자리변경 2016.04.07
#define FILM_JIG_GLASS_CLAMP_CYLINDER_CLAMP_CHECK		55				//자리변경 2016.04.07
#define FILM_JIG_TILTING_CYLINDER_TILT_UP_CHECK				56
#define FILM_JIG_TILTING_CYLINDER_TILT_DN_CHECK				57	
#define FILM_JIG_VAC_PLATE_CYLINDER_UP_CHECK					58
#define FILM_JIG_VAC_PLATE_CYLINDER_DN_CHECK					59
#define GLASS_JIG_VAC_ON_CHECK											60
#define GLASS_JIG_CYLINDER_OUT_CHECK								61				//자리변경 2016.04.07
#define GLASS_JIG_CYLINDER_IN_CHECK									62				//자리변경 2016.04.07
#define TRAY_SHUTTLE_GLASS_VAC_ON_CHECK						63

//OUTPUT
#define TOWER_LAMP_RED															0
#define TOWER_LAMP_YELLOW													1
#define TOWER_LAMP_GREEN														2
#define BUZZER_ERROR_SIGNAL												3
#define SAFETY_KEY_LOCK_ON_OFF											4
#define SAFETY_DOOR_UNLOCK													5
#define LIGHT_CURTAIN_MUTING_ON											6
#define LD_PNP_VAC_ON_SOL														8
#define LD_PNP_VAC_OFF_SOL													9
#define LD_PNP_CYLINDER_DN_SOL											10				 //자리변경 2016.04.07	
#define LD_PNP_CYLINDER_UP_SOL											11				//자리변경  2016.04.07
#define START_SWITCH_LAMP_1													32
#define READY_SWITCH_LAMP_1													33
#define RESET_SWITCH_LAMP														34
#define START_SWITCH_LAMP_2													35
#define READY_SWITCH_LAMP_2													36
#define FILM_JIG_VAC_ON_SOL													40
#define FILM_JIG_VAC_OFF_SOL													41
#define FILM_JIG_CYLINDER_IN_SOL											42
#define FILM_JIG_CYLINDER_OUT_SOL										43
#define FILM_JIG_GLASS_CLAMP_CYLINDER_FEED_SOL				44
#define FILM_JIG_GLASS_CLAMP_CYLINDER_BACK_SOL				45
#define FILM_JIG_GLASS_CLAMP_CYLINDER_CLAMP_SOL			46
#define FILM_JIG_GLASS_CLAMP_CYLINDER_UNCLAMP_SOL		47
#define FILM_JIG_TILTING_CYLINDER_TILT_UP_SOL						48
#define FILM_JIG_TILTING_CYLINDER_TILT_DN_SOL						49
#define FILM_JIG_VAC_PLATE_CYLINDER_UP_SOL						50
#define FILM_JIG_VAC_PLATE_CYLINDER_DN_SOL						51
#define GLASS_JIG_VAC_ON_SOL												52
#define GLASS_JIG_VAC_OFF_SOL												53
//#define GLASS_JIG_CYLINDER_IN_SOL										54
//#define GLASS_JIG_CYLINDER_OUT_SOL									55
#define TRAY_SHUTTLE_GLASS_VAC_ON_SOL							56
#define TRAY_SHUTTLE_GLASS_VAC_OFF_SOL							57
//IO END
////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////
//MOTION
#define MAX_AXIS_NUM				2
#define MOT_PROCESS_MAX_NUM			1
#define MOT_PROCESS_MAX_NUM					1
#define MOT_STATUS_MAX_NUM					3
#define MOT_STATUS_SUB_MAX_NUM				2000
#define USER_MSG_MOT						WM_USER+21

#define TIMEOUT_MESSAGE						1
#define TIMEOUT_DECISION					10
#define TIMEOUT_SAFE_SENSOR					10
#define TIMEOUT_INPUT_BUTTON				700
#define TIMEOUT_CYL_SENSOR					5000
#define TIMEOUT_MOTOR_MOVE					15000
#define TIMEOUT_TCPIP_RCV					10000
#define TRIGGER_MOVE_POS					450000


#define AXIS_TRY_SHUTTLE			0
#define AXIS_LD_PNP					1

#define MAX_HOME_ORDER				4

#define NOT_USE 0
#define USE		1

#define HOME_ERROR -1

#define MAX_AJIN_BD_AXIS_NUM				0
#define CW				0
#define CCW				1

//Position Data
#define POS_DATA_CNT		 50
#define JOG_MODE			 0
#define STEP_MODE			 1

//MOTION END
////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////
//COM START
#define OK_RESPONSE			0
#define	TIMER_ACCURA_CONNECT	101
#define MSG_MAX_COUNT		100
#define STX					0x05
#define ETX					0x0a

#define BIN_NONE		-1
#define BIN_OK			0
#define BIN_REJECT		1
#define BIN_NG			2

#define MAX_PARAMETER		6

#define DATA_AXIS_CONFIG_FILE	_T("D:\\DATABASE\\SYSTEM\\Axis.ini")
#define SYSTEM_AXIS_AJINX_PATH	_T("D:\\DATABASE\\SYSTEM\\AJinx.mot")


//COM END
////////////////////////////////////////////////////////////////////////

#endif