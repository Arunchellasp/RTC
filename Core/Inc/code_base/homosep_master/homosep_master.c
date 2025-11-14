// TODO(ARUN): @main
/*
        [-] GPIO read for panel
                  .....[-] read all the input mode and update to panel state
                  .....[] functionalites call for the all triggeers
                  .....[] panal_state referase every 100ms
[] Bring all the internal timer
                  .....[]
                  .....[]
                  .....[]
[] GPIO write to control mosfet
                  .....[]
                  .....[]
                  .....[]
[] communication slave_1
                  .....[]
                  .....[]
[] communication slave_2
                  .....[]
                  .....[]
                  .....[]
[]
[]


  // NOTE(ARUN): @ST_

1. Create Filesystem in QSPI
2. Store bucket count in a  "BucketCount.Txt"File
3. Complete internal RTC logic
4. Set current time
5. For every 5 Seconds Read current time from internal RTC, store the current time and 3 float value (3 gas sensor values) with comma separation in a "GasSensorData.csv" file with new line after each write

*/

#define RTC_INIT_MAGIC 0xBEEFCAFE

//
// NOTE(ARUN): @rtc_functions
//

internal void
hm_rtc_set_time(u8 hours, u8 minutes, u8 seconds,
                u8 year, u8 month, u8 date, u8 weekday)
{
    char msg[100];

    HAL_PWR_EnableBkUpAccess();
    os_delay_ms(10);

    b32 date_ok = 0;
    u8 retries = 3;
    while (retries > 0 && !date_ok)
    {
        date_ok = os_rtc_set_date(hm->os.rtc, year, month, date, weekday);
        if (!date_ok) os_delay_ms(100);
        retries--;
    }

    b32 time_ok = 0;
    retries = 3;
    while (retries > 0 && !time_ok)
    {
        time_ok = os_rtc_set_time(hm->os.rtc, hours, minutes, seconds);
        if (!time_ok) os_delay_ms(100);
        retries--;
    }

    if (date_ok && time_ok)
    {
        snprintf(msg, sizeof(msg), "RTC: Time set to 20%02d-%02d-%02d %02d:%02d:%02d\r\n",
                 year, month, date, hours, minutes, seconds);
    }
    else
    {
        snprintf(msg, sizeof(msg), "RTC: Set FAILED - Date:%s Time:%s\r\n",
                 date_ok ? "OK" : "FAIL", time_ok ? "OK" : "FAIL");
    }
    os_uart_send(hm->os.data_streem, (u8*)msg, strlen(msg));
}

internal void
hm_rtc_get_time()
{
    os_rtc_get_time(hm->os.rtc, &hm->hour, &hm->minute, &hm->second);
    os_rtc_get_date(hm->os.rtc, &hm->year, &hm->month, &hm->date, &hm->weekday);
}

internal void
hm_rtc_print_time()
{
    char buffer[64];

    if (hm->month == 0 || hm->date == 0)
    {
        snprintf(buffer, sizeof(buffer), "RTC NOT SET - %02d:%02d:%02d\r\n",
                 hm->hour, hm->minute, hm->second);
    }
    else
    {
        snprintf(buffer, sizeof(buffer), "20%02d-%02d-%02d %02d:%02d:%02d\r\n",
                 hm->year, hm->month, hm->date, hm->hour, hm->minute, hm->second);
    }
    os_uart_send(hm->os.data_streem, (u8*)buffer, strlen(buffer));
}

internal void
hm_rtc_force_reset()
{
    os_rtc_clear_backup(hm->os.rtc, RTC_BKP_DR0);
    char msg[] = "RTC: Reset flag cleared\r\n";
    os_uart_send(hm->os.data_streem, (u8*)msg, strlen(msg));
}

//
// NOTE(ARUN): @init
//

internal void
hm_init()
{

    // NOTE(ARUN): @arena_init
    {
        for(u32 it = 0; it < ArrayCount(thread_arena); it += 1)
        {
            thread_arena[it] = arena_alloc(KB(1));
        }
    }
    


    Arena *arena = arena_alloc(KB(10));
    hm = push_array(arena, HM_State, 1);
    hm->arena = arena;
    
    // NOTE(ARUN): @panel_buttons_init
    hm->os.panel_buttons = push_array(arena, OS_Gpio, Button_COUNT);
    os_gpio_init(hm->os.panel_buttons + Button_MOTOR_CW_IN, stm32_gpio(MOTOR_CW_IN));
    os_gpio_init(hm->os.panel_buttons + Button_MOTOR_ACW_IN, stm32_gpio(MOTOR_ACW_IN));
    os_gpio_init(hm->os.panel_buttons + Button_BOOM_EXTEND_IN, stm32_gpio(BOOM_EXTEND_IN));
    os_gpio_init(hm->os.panel_buttons + Button_BUCKET_OPEN_IN, stm32_gpio(BUCKET_OPEN_IN));
    os_gpio_init(hm->os.panel_buttons + Button_GRIPPER_RIGHT_OPEN_IN, stm32_gpio(GRIPPER_RIGHT_OPEN_IN));
    os_gpio_init(hm->os.panel_buttons + Button_GRIPPER_LEFT_OPEN_IN, stm32_gpio(GRIPPER_LEFT_OPEN_IN));
    os_gpio_init(hm->os.panel_buttons + Button_MOSFET2_ALERT, stm32_gpio(MOSFET2_ALERT));
    os_gpio_init(hm->os.panel_buttons + Button_MOSFET3_ALERT, stm32_gpio(MOSFET3_ALERT));
    os_gpio_init(hm->os.panel_buttons + Button_WINCH_WIND_IN, stm32_gpio(WINCH_WIND_IN));
    os_gpio_init(hm->os.panel_buttons + Button_TIPPER_CLOSE_IN, stm32_gpio(TIPPER_CLOSE_IN));
    os_gpio_init(hm->os.panel_buttons + Button_TIPPER_OPEN_IN, stm32_gpio(TIPPER_OPEN_IN));
    os_gpio_init(hm->os.panel_buttons + Button_BLADE_ACW_IN, stm32_gpio(BLADE_ACW_IN));
    os_gpio_init(hm->os.panel_buttons + Button_BUCKET_CLOSE_IN, stm32_gpio(BUCKET_CLOSE_IN));
    os_gpio_init(hm->os.panel_buttons + Button_BLADE_CW_IN, stm32_gpio(BLADE_CW_IN));
    os_gpio_init(hm->os.panel_buttons + Button_STOP, stm32_gpio(STOP));
    os_gpio_init(hm->os.panel_buttons + Button_MOSFET1_ALERT, stm32_gpio(MOSFET1_ALERT));
    os_gpio_init(hm->os.panel_buttons + Button_GRIPPER_LEFT_CLOSE_IN, stm32_gpio(GRIPPER_LEFT_CLOSE_IN));
    os_gpio_init(hm->os.panel_buttons + Button_GRIPPER_RIGHT_CLOSE_IN, stm32_gpio(GRIPPER_RIGHT_CLOSE_IN));
    os_gpio_init(hm->os.panel_buttons + Button_WINCH_UNWIND_IN, stm32_gpio(WINCH_UNWIND_IN));
    os_gpio_init(hm->os.panel_buttons + Button_BOOM_RETRACT_IN, stm32_gpio(BOOM_RETRACT_IN));
    os_gpio_init(hm->os.panel_buttons + Button_BOOM_DOWN_IN, stm32_gpio(BOOM_DOWN_IN));
    os_gpio_init(hm->os.panel_buttons + Button_BOOM_UP_IN, stm32_gpio(BOOM_UP_IN));
    


    // NOTE(ARUN):  @com_controll_init
    hm->os.rs485_com_control = push_array(arena, OS_Gpio, 1);
    os_gpio_init(hm->os.rs485_com_control ,stm32_gpio(LOAD_EN));
    
    // NOTE(ARUN): @motor_com
    hm->os.data_streem = push_array(arena, OS_Uart, 1);
    os_uart_init(hm->os.data_streem, &huart6,1000);
    
    os_gpio_write(hm->os.rs485_com_control,1);
    
    // NOTE(ARUN): @panel_timer
    hm->os.panel_cc_timer = push_array(arena,OS_Timer_It,1);
    os_timer_it_init(hm->os.panel_cc_timer ,&htim3, stm32_timer_it);
    os_timer_start(hm->os.panel_cc_timer);
    

    // NOTE(ARUN): @rtc_init
    hm->os.rtc = push_array(arena, OS_Rtc, 1);
    os_rtc_init(hm->os.rtc, &hrtc);
    
    HAL_PWR_EnableBkUpAccess();
    os_delay_ms(100);

    // hm_rtc_force_reset(); //when ever you want set new time that only you need anable

    u32 backup_value = os_rtc_backup_read(hm->os.rtc, RTC_BKP_DR0);
    
    char msg[100];

    if (backup_value != RTC_INIT_MAGIC)
    {
        snprintf(msg, sizeof(msg), "RTC: First boot\r\n");
        os_uart_send(hm->os.data_streem, (u8*)msg, strlen(msg));

        __HAL_RCC_LSE_CONFIG(RCC_LSE_ON);

        u32 lse_timeout = 5000;
        while(__HAL_RCC_GET_FLAG(RCC_FLAG_LSERDY) == RESET && lse_timeout > 0)
        {
            os_delay_ms(1);
            lse_timeout--;
        }

        if (lse_timeout == 0)
        {
            snprintf(msg, sizeof(msg), "RTC: LSE failed!\r\n");
            os_uart_send(hm->os.data_streem, (u8*)msg, strlen(msg));
        }
        
        // Set initial date and time: 2025-11-14 16:30:00 Thursday
        hm_rtc_set_time(17, 33, 0, 25, 11, 14, RTC_WEEKDAY_THURSDAY);
        
        os_rtc_backup_write(hm->os.rtc, RTC_BKP_DR0, RTC_INIT_MAGIC);
        
        snprintf(msg, sizeof(msg), "RTC: Initialized\r\n");
        os_uart_send(hm->os.data_streem, (u8*)msg, strlen(msg));
    }
    else
    {
        snprintf(msg, sizeof(msg), "RTC: Already initialized\r\n");
        os_uart_send(hm->os.data_streem, (u8*)msg, strlen(msg));
    }
}

//
// NOTE(ARUN): @frame
//

internal void
hm_frame()
{
    static u32 last_send_second = 0;
    
    hm_rtc_get_time();
    
    if (hm->second != last_send_second && (hm->second % 3 == 0))
    {
        hm_rtc_print_time();
        last_send_second = hm->second;
    }
}

//
// NOTE(ARUN): @callbacks
//

void
HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    OS_State *os = &hm->os;
    if(htim == os_timer_it_handle(os->panel_cc_timer))
    {
        for (u32 it = 0; it < Button_COUNT; it++)
            hm->panel_buttons[it] = os_gpio_read(hm->os.panel_buttons + it);

    }
}
