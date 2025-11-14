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
    
    // CRITICAL: Enable backup domain access
    HAL_PWR_EnableBkUpAccess();

    // Small delay for backup domain to stabilize
    os_delay_ms(100);

    // Read backup register to check if RTC was previously initialized
    u32 backup_value = os_rtc_backup_read(hm->os.rtc, RTC_BKP_DR0);
    
    char debug_msg[100];

    if (backup_value != RTC_INIT_MAGIC)
    {
        // First time initialization or after backup power loss
        snprintf(debug_msg, sizeof(debug_msg), "RTC: First boot or backup lost\r\n");
        os_uart_send(hm->os.data_streem, (u8*)debug_msg, strlen(debug_msg));

        // Force LSE to start (if using external crystal)
        __HAL_RCC_LSE_CONFIG(RCC_LSE_ON);

        // Wait for LSE to be ready (timeout after 5 seconds)
        u32 lse_timeout = 5000;
        while(__HAL_RCC_GET_FLAG(RCC_FLAG_LSERDY) == RESET && lse_timeout > 0)
        {
            os_delay_ms(1);
            lse_timeout--;
        }

        if (lse_timeout == 0)
        {
            snprintf(debug_msg, sizeof(debug_msg), "RTC: LSE failed to start!\r\n");
            os_uart_send(hm->os.data_streem, (u8*)debug_msg, strlen(debug_msg));
        }
        
        // Set date first - CRITICAL: Date must be set before time
        b32 date_ok = 0;
        u8 retries = 3;
        while (retries > 0 && !date_ok)
        {
            // Date: 2025-11-12 (Year=25, Month=11, Day=12, Weekday=Wednesday)
            date_ok = os_rtc_set_date(hm->os.rtc, 25, 11, 14, RTC_WEEKDAY_WEDNESDAY);
            if (!date_ok)
            {
                os_delay_ms(100);
                retries--;
            }
        }
        
        // Set time
        b32 time_ok = 0;
        retries = 3;
        while (retries > 0 && !time_ok)
        {
            // Time: 14:23:00
            time_ok = os_rtc_set_time(hm->os.rtc, 16, 30, 0);
            if (!time_ok)
            {
                os_delay_ms(100);
                retries--;
            }
        }
        
        // Mark RTC as initialized ONLY if both date and time were set successfully
        if (date_ok && time_ok)
        {
            os_rtc_backup_write(hm->os.rtc, RTC_BKP_DR0, RTC_INIT_MAGIC);
            snprintf(debug_msg, sizeof(debug_msg), "RTC: Initialized successfully\r\n");
        }
        else
        {
            snprintf(debug_msg, sizeof(debug_msg), "RTC: Init FAILED - Date:%s Time:%s\r\n",
                     date_ok ? "OK" : "FAIL", time_ok ? "OK" : "FAIL");
        }
        os_uart_send(hm->os.data_streem, (u8*)debug_msg, strlen(debug_msg));
    }
    else
    {
        // RTC was previously initialized - time should be maintained
        snprintf(debug_msg, sizeof(debug_msg), "RTC: Already initialized, reading current time\r\n");
        os_uart_send(hm->os.data_streem, (u8*)debug_msg, strlen(debug_msg));
    }
    


}

// NOTE(ARUN): @rtc_reset_function
// Call this function to force RTC reset on next boot
internal void
hm_rtc_force_reset()
{
    // Clear the backup register to force re-initialization
    os_rtc_clear_backup(hm->os.rtc, RTC_BKP_DR0);
    
    char msg[] = "RTC Reset Flag Set - Please Reboot\r\n";
    os_uart_send(hm->os.data_streem, (u8*)msg, strlen(msg));
}

internal void
hm_send_time_date_uart()
{
    char buffer[64];
    
    // Check if date values are valid (month and day should not be 0)
    if (hm->month == 0 || hm->date == 0)
    {
        snprintf(buffer, sizeof(buffer),
                 "RTC NOT SET - %02d:%02d:%02d\r\n",
                 hm->hour, hm->minute, hm->second);
    }
    else
    {
        snprintf(buffer, sizeof(buffer),
                 "20%02d-%02d-%02d %02d:%02d:%02d\r\n",
                 hm->year, hm->month, hm->date,
                 hm->hour, hm->minute, hm->second);
    }
    os_uart_send(hm->os.data_streem, (u8*)buffer, strlen(buffer));
}



internal void
hm_rtc_debug_status()
{
    char msg[200];

    // Check LSE status
    b32 lse_ready = __HAL_RCC_GET_FLAG(RCC_FLAG_LSERDY);

    // Check if backup domain is accessible
    b32 backup_access = (PWR->CR & PWR_CR_DBP) != 0;

    // Check RTC clock source
    u32 rtc_source = __HAL_RCC_GET_RTC_SOURCE();

    snprintf(msg, sizeof(msg),
             "RTC Debug:\r\n"
             "  LSE Ready: %s\r\n"
             "  Backup Access: %s\r\n"
             "  Clock Source: 0x%lx (LSE=0x100, LSI=0x200)\r\n"
             "  Magic Value: 0x%lx\r\n",
             lse_ready ? "YES" : "NO",
             backup_access ? "YES" : "NO",
             rtc_source,
             os_rtc_backup_read(hm->os.rtc, RTC_BKP_DR0));

    os_uart_send(hm->os.data_streem, (u8*)msg, strlen(msg));
}



internal void
hm_frame()
{
    static u32 last_send_second = 0;
    
       os_rtc_get_time(hm->os.rtc, &hm->hour, &hm->minute, &hm->second);
       os_rtc_get_date(hm->os.rtc, &hm->year, &hm->month, &hm->date, &hm->weekday);
    
    if (hm->second != last_send_second && (hm->second % 3 == 0))
    {
        hm_send_time_date_uart();
        last_send_second = hm->second;
    }
}




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
