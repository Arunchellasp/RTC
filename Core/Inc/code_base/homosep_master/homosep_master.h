/* date = October 18th 2025 5:08 pm */

#ifndef HOMOSEP_MASTER_H
#define HOMOSEP_MASTER_H


typedef enum Boom_Action Boom_Action;
enum Boom_Action
{
    Boom_Down,
    Boom_Up,
    Boom_Extend,
    Boom_Retract,
    Boom_Right,
    Boom_left,
};

typedef enum Gripper_Action Gripper_Action;
enum Gripper_Action
{
    Gripper_close,
    Gripper_open,
    Gripper_right,
    Gripper_left,
};

typedef enum Mosfet_St Mosfet_St;
enum Mosfet_St
{
    Mosfet_1,
    Mosfet_2,
    Mosfet_3,
};

typedef enum Motor_Action Motor_Action;
enum Motor_Action
{
    motor_cw,
    motor_ccw,
};


typedef enum Bucket_Action Bucket_Action;
enum Bucket_Action
{
    bucket_open,
    bucket_close,
};

typedef enum Winch_Action Winch_Action;
enum Winch_Action
{
    winch_wind,
    winch_unwind,
};

typedef enum Tipper_Action Tipper_Action;
enum Tipper_Action
{
    tipper_open,
    tipper_close,
};
typedef enum Blade_Action Blade_Action;
enum Blade_Action
{
    blade_cw,
    blade_ccw,
};



typedef struct Panel_State Panel_State;
struct Panel_State
{
    Boom_Action boom_action;
    Gripper_Action gripper_action;
    Mosfet_St mosfet_st;
    Motor_Action motor_action;
    Bucket_Action bucket_action;
    Winch_Action winch_action;
    Tipper_Action tipper_action;
    Blade_Action blade_action;
    i32 stop_action;
};

enum
{
	Button_Control_Mode = 0,\
	Button_MOTOR_CW_IN,
	Button_MOTOR_ACW_IN,
	Button_BOOM_EXTEND_IN,
	Button_BUCKET_OPEN_IN,
	Button_GRIPPER_RIGHT_OPEN_IN,
	Button_GRIPPER_LEFT_OPEN_IN,
	Button_MOSFET2_ALERT,
	Button_MOSFET3_ALERT,
	Button_WINCH_WIND_IN,
	Button_TIPPER_CLOSE_IN,
	Button_TIPPER_OPEN_IN,
	Button_BLADE_ACW_IN,
	Button_BUCKET_CLOSE_IN,
	Button_BLADE_CW_IN,
	Button_STOP,
	Button_MOSFET1_ALERT,
	Button_GRIPPER_LEFT_CLOSE_IN,
	Button_GRIPPER_RIGHT_CLOSE_IN,
	Button_WINCH_UNWIND_IN,
	Button_BOOM_RETRACT_IN,
	Button_BOOM_DOWN_IN,
	Button_BOOM_UP_IN,
	Button_COUNT
};


typedef struct OS_State OS_State;
struct OS_State
{
	OS_Gpio *panel_buttons;
    OS_Timer_It *panel_cc_timer;
    OS_Rtc *rtc;
    OS_Gpio *rs485_com_control;
    OS_Uart *data_streem
};

typedef struct HM_State HM_State;
struct HM_State
{
    Arena *arena;
    OS_State os;
    
    u32 panel_buttons[Button_COUNT];
    Panel_State panel_state;
    
    // RTC data
    u8 hour;
    u8 minute;
    u8 second;
    u8 year;
    u8 month;
    u8 date;
    u8 weekday;
    
};

global_variable HM_State *hm = {0};
global_variable Arena *thread_arena[2] = {0};

#endif //HOMOSEP_MASTER_H
