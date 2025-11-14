#ifndef OS_STM32_CORE_H
#define OS_STM32_CORE_H


// NOTE(ARUN):@general
#define os_delay_ms(ms) HAL_Delay(ms)

// NOTE(ARUN): @GPIO

typedef struct OS_Gpio OS_Gpio;
struct OS_Gpio
{
	GPIO_TypeDef *port;
    i32 pin;
};

#define stm32_gpio(name) (u32 *)(name##_GPIO_Port), (i32)(name##_Pin)
internal inline void
os_gpio_init(OS_Gpio *gpio, u32 *port, i32 pin)
{
	Assert(gpio != 0);
	gpio->port = (GPIO_TypeDef *)port;
	gpio->pin = pin;
}

internal inline void
os_gpio_write(OS_Gpio *gpio, b32 val)
{
	HAL_GPIO_WritePin(gpio->port, gpio->pin, (val == 0) ? GPIO_PIN_RESET : GPIO_PIN_SET);
}

internal inline b32
os_gpio_read(OS_Gpio *gpio)
{
	b32 result = HAL_GPIO_ReadPin(gpio->port, gpio->pin);
	return result;
}

internal inline void
os_gpio_toggle(OS_Gpio *gpio)
{
	HAL_GPIO_TogglePin(gpio->port, gpio->pin);
}

#if 0
// I2C

typedef struct OS_I2c OS_I2c;
struct OS_I2c
{
	I2C_HandleTypeDef *handle;
	u16 timeout_ms;
};

internal inline void 
os_i2c_init(OS_I2c *i2c, I2C_HandleTypeDef *handle, u16 timeout_ms)
{
	Assert(i2c != 0);
    i2c->handle = handle;
	i2c->timeout_ms = timeout_ms;
}

internal inline b32
os_i2c_write8(OS_I2c *i2c, u16 device_address, u8* data, u16 count)
{
	HAL_StatusTypeDef result = HAL_I2C_Master_Transmit(i2c->handle, device_address, data, count, i2c->timeout_ms);
	return result == HAL_OK;
};

internal inline b32
os_i2c_read8(OS_I2c *i2c, u16 device_address, u8* data, u16 count)
{
	HAL_StatusTypeDef result = HAL_I2C_Master_Receive(i2c->handle, device_address, data, count, i2c->timeout_ms);
	return result == HAL_OK;
};

internal inline b32
os_i2c_write_register8(OS_I2c *i2c, u8 device_address, u16 register_address, u8 *val, u16 count)
{
	HAL_StatusTypeDef result = HAL_I2C_Mem_Write(i2c->handle, device_address, register_address, I2C_MEMADD_SIZE_8BIT, val, count, i2c->timeout_ms);
	return result == HAL_OK;
}

internal inline b32
os_i2c_read_register8(OS_I2c *i2c, u8 device_address, u16 register_address, u8 *val, u16 count)
{
	HAL_StatusTypeDef result = HAL_I2C_Mem_Read(i2c->handle, device_address, register_address, I2C_MEMADD_SIZE_8BIT, val, count, i2c->timeout_ms);
	return result == HAL_OK;
}
#endif


// NOTE(ARUN): @Timer

typedef HAL_StatusTypeDef OS_Timer_Control(TIM_HandleTypeDef *, u32);
typedef struct OS_Timer OS_Timer;
struct OS_Timer
{
    TIM_HandleTypeDef *handle;
    i32 channel;
    OS_Timer_Control *start;
    OS_Timer_Control *stop;
};

#define stm32_timer(pre, post) pre##Start##post, pre##Stop##post
#define stm32_timer_it stm32_timer(HAL_TIM_Base_, _IT)
#define stm32_timer_encoder TIM_CHANNEL_ALL, stm32_timer(HAL_TIM_Encoder_, )
internal inline void
os_timer_init(OS_Timer *timer, TIM_HandleTypeDef *handle, i32 channel, OS_Timer_Control *start, OS_Timer_Control *stop)
{
	Assert(timer != 0);
    timer->handle = handle;
    timer->channel = channel;
    timer->start = start;
    timer->stop = stop;
}

internal inline void
os_timer_start(OS_Timer *timer)
{
    timer->start(timer->handle, timer->channel);
}

internal inline void
os_timer_stop(OS_Timer *timer)
{
    timer->stop(timer->handle, timer->channel);
}

internal inline i32
os_timer_get_counter_32(OS_Timer *timer)
{
	i16 result = __HAL_TIM_GET_COUNTER(timer->handle);
	return result;
}

internal inline void
os_timer_set_counter_32(OS_Timer *timer, i32 val)
{
	__HAL_TIM_SET_COUNTER(timer->handle, val);
}

internal inline TIM_HandleTypeDef *
os_timer_handle(OS_Timer *timer)
{
    return timer->handle;
}

// NOTE(ARUN): @Timer_IT

typedef HAL_StatusTypeDef OS_Timer_It_Control(TIM_HandleTypeDef *);
typedef struct OS_Timer_It OS_Timer_It;
struct OS_Timer_It
{
	TIM_HandleTypeDef *handle;
    OS_Timer_It_Control *start;
    OS_Timer_It_Control *stop;
};

internal inline void 
os_timer_it_init(OS_Timer_It *timer, TIM_HandleTypeDef *handle, OS_Timer_It_Control *start, OS_Timer_It_Control *stop)
{
	Assert(timer != 0);
    timer->handle = handle;
    timer->start = start;
    timer->stop = stop;
}

internal inline void 
os_timer_it_start(OS_Timer_It *timer)
{
    timer->start(timer->handle);
}

internal inline void 
os_timer_it_stop(OS_Timer_It *timer)
{
    timer->stop(timer->handle);
}

internal inline TIM_HandleTypeDef *
os_timer_it_handle(OS_Timer_It *timer)
{
    return timer->handle;
}

// NOTE(ARUN): @Uart

typedef struct OS_Uart OS_Uart;
struct OS_Uart
{
	UART_HandleTypeDef *handle;
    u16 timeout;
};

internal inline void
os_uart_init(OS_Uart *uart, UART_HandleTypeDef *handle, u16 timeout)
{
	Assert(uart != 0);
    uart->handle = handle;
    uart->timeout = timeout;
}

internal inline b32
os_uart_send(OS_Uart *uart, u8 *data, u16 size)
{
	HAL_StatusTypeDef status = HAL_UART_Transmit(uart->handle, data, size, uart->timeout);
	return (status == HAL_OK) ? 1 : 0;
}

internal void
os_uart_receive(OS_Uart *uart, u8 *data, u16 size)
{
	HAL_UART_Receive(uart->handle, data, size, uart->timeout);
}

internal inline b32
os_uart_send_dma(OS_Uart *uart, u8 *data, u16 size)
{
	HAL_StatusTypeDef status = HAL_UART_Transmit_DMA(uart->handle, data, size );
	return (status == HAL_OK) ? 1 : 0;
}

internal void
os_uart_receive_to_idle_dma(OS_Uart *uart, u8 *data, u16 size)
{
	HAL_UARTEx_ReceiveToIdle_DMA(uart->handle, data, size);
}

internal inline UART_HandleTypeDef *
os_uart_handle(OS_Uart *uart)
{
    return uart->handle;
}

// NOTE(ARUN): @QSPI

typedef struct OS_Qspi_Commands OS_Qspi_Commands;
struct OS_Qspi_Commands
{
    u8 write_enable;      // Default: 0x06
    u8 sector_erase;      // Default: 0x20
    u8 fast_read;         // Default: 0x0B
};

typedef struct OS_Qspi OS_Qspi;
struct OS_Qspi
{
    QSPI_HandleTypeDef *handle;
    u16 timeout_ms;
    OS_Qspi_Commands commands;
};

internal inline void
os_qspi_init(OS_Qspi *qspi, QSPI_HandleTypeDef *handle, u16 timeout_ms, 
             u8 write_enable_cmd, u8 sector_erase_cmd, u8 fast_read_cmd)
{
    Assert(qspi != 0);
    qspi->handle = handle;
    qspi->timeout_ms = timeout_ms;
    qspi->commands.write_enable = write_enable_cmd;
    qspi->commands.sector_erase = sector_erase_cmd;
    qspi->commands.fast_read = fast_read_cmd;
}

internal inline b32
os_qspi_write(OS_Qspi *qspi, u32 address, u8 *data, u32 size)
{
    HAL_StatusTypeDef status = HAL_QSPI_Transmit(qspi->handle, data, qspi->timeout_ms);
    return (status == HAL_OK) ? 1 : 0;
}

internal inline b32
os_qspi_read(OS_Qspi *qspi, u32 address, u8 *data, u32 size)
{
    HAL_StatusTypeDef status = HAL_QSPI_Receive(qspi->handle, data, qspi->timeout_ms);
    return (status == HAL_OK) ? 1 : 0;
}

internal inline b32
os_qspi_erase_sector(OS_Qspi *qspi, u32 sector_address)
{
    QSPI_CommandTypeDef cmd = {0};
    cmd.InstructionMode   = QSPI_INSTRUCTION_1_LINE;
    cmd.Instruction      = qspi->commands.sector_erase;
    cmd.AddressMode      = QSPI_ADDRESS_1_LINE;
    cmd.AddressSize      = QSPI_ADDRESS_24_BITS;
    cmd.Address         = sector_address;
    cmd.DataMode        = QSPI_DATA_NONE;
    cmd.DummyCycles     = 0;
    
    HAL_StatusTypeDef status = HAL_QSPI_Command(qspi->handle, &cmd, qspi->timeout_ms);
    return (status == HAL_OK) ? 1 : 0;
}

internal inline b32
os_qspi_write_enable(OS_Qspi *qspi)
{
    QSPI_CommandTypeDef cmd = {0};
    cmd.InstructionMode   = QSPI_INSTRUCTION_1_LINE;
    cmd.Instruction      = qspi->commands.write_enable;
    cmd.AddressMode      = QSPI_ADDRESS_NONE;
    cmd.DataMode        = QSPI_DATA_NONE;
    cmd.DummyCycles     = 0;
    
    HAL_StatusTypeDef status = HAL_QSPI_Command(qspi->handle, &cmd, qspi->timeout_ms);
    return (status == HAL_OK) ? 1 : 0;
}

internal inline b32
os_qspi_memory_mapped(OS_Qspi *qspi)
{
    QSPI_CommandTypeDef cmd = {0};
    QSPI_MemoryMappedTypeDef cfg = {0};
    
    cmd.InstructionMode   = QSPI_INSTRUCTION_1_LINE;
    cmd.Instruction      = qspi->commands.fast_read;
    cmd.AddressMode      = QSPI_ADDRESS_1_LINE;
    cmd.AddressSize      = QSPI_ADDRESS_24_BITS;
    cmd.DataMode        = QSPI_DATA_1_LINE;
    cmd.DummyCycles     = 8;
    
    cfg.TimeOutActivation = QSPI_TIMEOUT_COUNTER_DISABLE;
    
    HAL_StatusTypeDef status = HAL_QSPI_MemoryMapped(qspi->handle, &cmd, &cfg);
    return (status == HAL_OK) ? 1 : 0;
}

internal inline QSPI_HandleTypeDef *
os_qspi_handle(OS_Qspi *qspi)
{
    return qspi->handle;
}


// NOTE(ARUN): @RTC

typedef struct OS_Rtc OS_Rtc;
struct OS_Rtc
{
    RTC_HandleTypeDef *handle;
};

// RTC initialization check magic number
#define OS_RTC_INIT_MAGIC 0xBEEFCAFE

internal inline void
os_rtc_init(OS_Rtc *rtc, RTC_HandleTypeDef *handle)
{
    Assert(rtc != 0);
    rtc->handle = handle;
}

internal inline b32
os_rtc_set_time(OS_Rtc *rtc, u8 hours, u8 minutes, u8 seconds)
{
    RTC_TimeTypeDef time = {0};
    time.Hours = hours;
    time.Minutes = minutes;
    time.Seconds = seconds;
    time.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
    time.StoreOperation = RTC_STOREOPERATION_RESET;
    time.SubSeconds = 0;
    time.SecondFraction = 0;

    HAL_StatusTypeDef status = HAL_RTC_SetTime(rtc->handle, &time, RTC_FORMAT_BIN);
    return (status == HAL_OK) ? 1 : 0;
}

internal inline b32
os_rtc_get_time(OS_Rtc *rtc, u8 *hours, u8 *minutes, u8 *seconds)
{
    RTC_TimeTypeDef time = {0};
    HAL_StatusTypeDef status = HAL_RTC_GetTime(rtc->handle, &time, RTC_FORMAT_BIN);

    if (status == HAL_OK)
    {
        *hours = time.Hours;
        *minutes = time.Minutes;
        *seconds = time.Seconds;
    }

    return (status == HAL_OK) ? 1 : 0;
}

internal inline b32
os_rtc_set_date(OS_Rtc *rtc, u8 year, u8 month, u8 date, u8 weekday)
{
    RTC_DateTypeDef sDate = {0};
    sDate.Year = year;
    sDate.Month = month;
    sDate.Date = date;
    sDate.WeekDay = weekday;

    // Ensure the date values are valid before setting
    if (month < 1 || month > 12 || date < 1 || date > 31)
        return 0;

    HAL_StatusTypeDef status = HAL_RTC_SetDate(rtc->handle, &sDate, RTC_FORMAT_BIN);
    return (status == HAL_OK) ? 1 : 0;
}

internal inline b32
os_rtc_get_date(OS_Rtc *rtc, u8 *year, u8 *month, u8 *date, u8 *weekday)
{
    RTC_DateTypeDef sDate = {0};
    HAL_StatusTypeDef status = HAL_RTC_GetDate(rtc->handle, &sDate, RTC_FORMAT_BIN);

    if (status == HAL_OK)
    {
        *year = sDate.Year;
        *month = sDate.Month;
        *date = sDate.Date;
        *weekday = sDate.WeekDay;
    }

    return (status == HAL_OK) ? 1 : 0;
}

internal inline b32
os_rtc_set_alarm(OS_Rtc *rtc, u8 hours, u8 minutes, u8 seconds, u32 alarm)
{
    RTC_AlarmTypeDef sAlarm = {0};
    sAlarm.AlarmTime.Hours = hours;
    sAlarm.AlarmTime.Minutes = minutes;
    sAlarm.AlarmTime.Seconds = seconds;
    sAlarm.AlarmTime.SubSeconds = 0;
    sAlarm.AlarmTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
    sAlarm.AlarmTime.StoreOperation = RTC_STOREOPERATION_RESET;
    sAlarm.AlarmMask = RTC_ALARMMASK_DATEWEEKDAY;
    sAlarm.AlarmSubSecondMask = RTC_ALARMSUBSECONDMASK_ALL;
    sAlarm.AlarmDateWeekDaySel = RTC_ALARMDATEWEEKDAYSEL_DATE;
    sAlarm.AlarmDateWeekDay = 1;
    sAlarm.Alarm = alarm;

    HAL_StatusTypeDef status = HAL_RTC_SetAlarm_IT(rtc->handle, &sAlarm, RTC_FORMAT_BIN);
    return (status == HAL_OK) ? 1 : 0;
}

internal inline b32
os_rtc_deactivate_alarm(OS_Rtc *rtc, u32 alarm)
{
    HAL_StatusTypeDef status = HAL_RTC_DeactivateAlarm(rtc->handle, alarm);
    return (status == HAL_OK) ? 1 : 0;
}

internal inline RTC_HandleTypeDef *
os_rtc_handle(OS_Rtc *rtc)
{
    return rtc->handle;
}

internal inline void
os_rtc_backup_write(OS_Rtc *rtc, u32 backup_register, u32 data)
{
    // Ensure backup access is enabled
    HAL_PWR_EnableBkUpAccess();
    HAL_RTCEx_BKUPWrite(rtc->handle, backup_register, data);
}

internal inline u32
os_rtc_backup_read(OS_Rtc *rtc, u32 backup_register)
{
    // Ensure backup access is enabled
    HAL_PWR_EnableBkUpAccess();
    return HAL_RTCEx_BKUPRead(rtc->handle, backup_register);
}

internal inline void
os_rtc_clear_backup(OS_Rtc *rtc, u32 backup_register)
{
    HAL_RTCEx_BKUPWrite(rtc->handle, backup_register, 0);
}

// NOTE(ARUN): @RTC_Extended - Extended RTC functions

// Set complete date and time with retry logic
internal b32
os_rtc_set_datetime(OS_Rtc *rtc, u8 hours, u8 minutes, u8 seconds,
                    u8 year, u8 month, u8 date, u8 weekday)
{
    b32 date_ok = 0;
    b32 time_ok = 0;
    u8 retries = 3;

    // Try to set date with retries
    while (retries > 0 && !date_ok)
    {
        date_ok = os_rtc_set_date(rtc, year, month, date, weekday);
        if (!date_ok) os_delay_ms(100);
        retries--;
    }

    // Try to set time with retries
    retries = 3;
    while (retries > 0 && !time_ok)
    {
        time_ok = os_rtc_set_time(rtc, hours, minutes, seconds);
        if (!time_ok) os_delay_ms(100);
        retries--;
    }

    return date_ok && time_ok;
}

// Get complete date and time in one call
internal b32
os_rtc_get_datetime(OS_Rtc *rtc, u8 *hours, u8 *minutes, u8 *seconds,
                    u8 *year, u8 *month, u8 *date, u8 *weekday)
{
    b32 time_ok = os_rtc_get_time(rtc, hours, minutes, seconds);
    b32 date_ok = os_rtc_get_date(rtc, year, month, date, weekday);
    return time_ok && date_ok;
}

// Check if RTC has been initialized
internal b32
os_rtc_is_initialized(OS_Rtc *rtc, u32 backup_register, u32 magic_value)
{
    HAL_PWR_EnableBkUpAccess();
    u32 backup_value = os_rtc_backup_read(rtc, backup_register);
    return (backup_value == magic_value);
}

// Mark RTC as initialized
internal void
os_rtc_mark_initialized(OS_Rtc *rtc, u32 backup_register, u32 magic_value)
{
    HAL_PWR_EnableBkUpAccess();
    os_rtc_backup_write(rtc, backup_register, magic_value);
}

// Initialize LSE oscillator with timeout
internal b32
os_rtc_init_lse(u32 timeout_ms)
{
    __HAL_RCC_LSE_CONFIG(RCC_LSE_ON);

    u32 elapsed = 0;
    while(__HAL_RCC_GET_FLAG(RCC_FLAG_LSERDY) == RESET && elapsed < timeout_ms)
    {
        os_delay_ms(1);
        elapsed++;
    }

    return (elapsed < timeout_ms);
}

// Format datetime to string buffer
internal u32
os_rtc_format_datetime(char *buffer, u32 buffer_size,
                       u8 year, u8 month, u8 date,
                       u8 hours, u8 minutes, u8 seconds,
                       b32 date_valid)
{
    if (date_valid && month > 0 && date > 0)
    {
        return snprintf(buffer, buffer_size, "20%02d-%02d-%02d %02d:%02d:%02d",
                       year, month, date, hours, minutes, seconds);
    }
    else
    {
        return snprintf(buffer, buffer_size, "RTC NOT SET - %02d:%02d:%02d",
                       hours, minutes, seconds);
    }
}

// Check if date values are valid
internal b32
os_rtc_validate_date(u8 year, u8 month, u8 date)
{
    if (month < 1 || month > 12) return 0;
    if (date < 1 || date > 31) return 0;

    // Check for months with fewer days
    if (month == 2) // February
    {
        u32 full_year = 2000 + year;
        b32 is_leap = ((full_year % 4 == 0) && (full_year % 100 != 0)) || (full_year % 400 == 0);
        return date <= (is_leap ? 29 : 28);
    }
    else if (month == 4 || month == 6 || month == 9 || month == 11)
    {
        return date <= 30;
    }

    return 1;
}

// Check if time values are valid
internal b32
os_rtc_validate_time(u8 hours, u8 minutes, u8 seconds)
{
    return (hours < 24) && (minutes < 60) && (seconds < 60);
}


#endif //OS_STM32_CORE_H
