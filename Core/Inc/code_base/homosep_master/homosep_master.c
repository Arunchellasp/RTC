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

//
// NOTE(ARUN): @rtc_functions - Simplified application-level RTC functions
//

internal void
hm_rtc_set_time(u8 hours, u8 minutes, u8 seconds,
                u8 year, u8 month, u8 date, u8 weekday)
{
    char msg[100];

    // Validate inputs
    if (!os_rtc_validate_time(hours, minutes, seconds) ||
        !os_rtc_validate_date(year, month, date))
    {
        snprintf(msg, sizeof(msg), "RTC: Invalid date/time values\r\n");
        os_uart_send(hm->os.data_streem, (u8*)msg, strlen(msg));
        return;
    }

    HAL_PWR_EnableBkUpAccess();
    os_delay_ms(10);

    // Use OS layer function with built-in retry logic
    b32 success = os_rtc_set_datetime(hm->os.rtc, hours, minutes, seconds,
                                      year, month, date, weekday);

    if (success)
    {
        snprintf(msg, sizeof(msg), "RTC: Time set to 20%02d-%02d-%02d %02d:%02d:%02d\r\n",
                 year, month, date, hours, minutes, seconds);
    }
    else
    {
        snprintf(msg, sizeof(msg), "RTC: Failed to set date/time\r\n");
    }

    os_uart_send(hm->os.data_streem, (u8*)msg, strlen(msg));
}

internal void
hm_rtc_get_time()
{
    os_rtc_get_datetime(hm->os.rtc,
                        &hm->hour, &hm->minute, &hm->second,
                        &hm->year, &hm->month, &hm->date, &hm->weekday);
}

internal void
hm_rtc_print_time()
{
    char buffer[64];
    b32 date_valid = (hm->month > 0 && hm->date > 0);

    os_rtc_format_datetime(buffer, sizeof(buffer),
                          hm->year, hm->month, hm->date,
                          hm->hour, hm->minute, hm->second,
                          date_valid);

    strcat(buffer, "\r\n");
    os_uart_send(hm->os.data_streem, (u8*)buffer, strlen(buffer));
}

internal void
hm_rtc_force_reset()
{
    os_rtc_clear_backup(hm->os.rtc, RTC_BKP_DR0);
    char msg[] = "RTC: Reset flag cleared - will reinitialize on next boot\r\n";
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

    // NOTE(ARUN): @com_controll_init
    hm->os.rs485_com_control = push_array(arena, OS_Gpio, 1);
    os_gpio_init(hm->os.rs485_com_control, stm32_gpio(LOAD_EN));
    
    // NOTE(ARUN): @motor_com
    hm->os.data_streem = push_array(arena, OS_Uart, 1);
    os_uart_init(hm->os.data_streem, &huart6, 1000);
    
    os_gpio_write(hm->os.rs485_com_control, 1);
    
    // NOTE(ARUN): @panel_timer
    hm->os.panel_cc_timer = push_array(arena, OS_Timer_It, 1);
    os_timer_it_init(hm->os.panel_cc_timer, &htim3, stm32_timer_it);
    os_timer_start(hm->os.panel_cc_timer);
    
    // NOTE(ARUN): @rtc_init - Optimized RTC initialization
    hm->os.rtc = push_array(arena, OS_Rtc, 1);
    os_rtc_init(hm->os.rtc, &hrtc);
    
    HAL_PWR_EnableBkUpAccess();
    os_delay_ms(100);

    // Uncomment this line when you want to force a new time setting
    // hm_rtc_force_reset();

    char msg[100];

    if (!os_rtc_is_initialized(hm->os.rtc, RTC_BKP_DR0, OS_RTC_INIT_MAGIC))
    {
        // First boot - initialize RTC
        snprintf(msg, sizeof(msg), "RTC: First boot - initializing...\r\n");
        os_uart_send(hm->os.data_streem, (u8*)msg, strlen(msg));

        // Initialize LSE oscillator
        if (!os_rtc_init_lse(5000))
        {
            snprintf(msg, sizeof(msg), "RTC: LSE initialization failed!\r\n");
            os_uart_send(hm->os.data_streem, (u8*)msg, strlen(msg));
            return;
        }
        
        // Set initial date and time: 2025-11-14 17:33:00 Thursday
        hm_rtc_set_time(18, 17, 0, 25, 11, 14, RTC_WEEKDAY_THURSDAY);
        
        // Mark as initialized
        os_rtc_mark_initialized(hm->os.rtc, RTC_BKP_DR0, OS_RTC_INIT_MAGIC);
        
        snprintf(msg, sizeof(msg), "RTC: Initialization complete\r\n");
        os_uart_send(hm->os.data_streem, (u8*)msg, strlen(msg));
    }
    else
    {
        // RTC already initialized
        snprintf(msg, sizeof(msg), "RTC: Already initialized, keeping time\r\n");
        os_uart_send(hm->os.data_streem, (u8*)msg, strlen(msg));

        // Read and display current time
        hm_rtc_get_time();
        hm_rtc_print_time();
    }
}

//
// NOTE(ARUN): @frame
//

internal void
hm_frame()
{
    static u32 last_print_second = 0;
    
    // Get current time
    hm_rtc_get_time();
    
    // Print time every 3 seconds
    if (hm->second != last_print_second && (hm->second % 3 == 0))
    {
        hm_rtc_print_time();
        last_print_second = hm->second;
    }

    // TODO(ARUN): Add CSV logging every 5 seconds for gas sensor data
    // Example structure for future implementation:
    // static u32 last_log_second = 0;
    // if (hm->second != last_log_second && (hm->second % 5 == 0))
    // {
    //     hm_log_gas_sensor_data();
    //     last_log_second = hm->second;
    // }
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
        // Read all panel buttons every timer interrupt
        for (u32 it = 0; it < Button_COUNT; it++)
        {
            hm->panel_buttons[it] = os_gpio_read(hm->os.panel_buttons + it);
        }
    }
}
