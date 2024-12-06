#include "timer.h"
//#include "stm32wb0x_hal_radio_timer.h"  // HAL_RADIO_TIMER APIs

//// Timer Object Type
//typedef struct {
//    VTIMER_HandleType handle;  // HAL_RADIO_TIMER virtual timer handle
//    uint32_t timeout;         // Timer timeout in ms
//} TimerEvent_t;
//
//// TimerTime_t is a 32-bit unsigned integer
//typedef uint32_t TimerTime_t;

/**
 * @brief Create a timer object
 * @param HANDLE: Pointer to the timer object
 * @param CB: Callback function
 */
void TimerInit(TimerEvent_t *HANDLE, VTIMER_CallbackType CB) {
    if (HANDLE == NULL || CB == NULL) return;

    HANDLE->timeout = 0;
    HANDLE->handle.callback = CB; // Link callback to HAL handle
    HANDLE->handle.userData = NULL;
    HANDLE->handle.active = false;
    HANDLE->handle.next = NULL;
}

/**
 * @brief Set the timer period
 * @param HANDLE: Pointer to the timer object
 * @param TIMEOUT: Timeout value in milliseconds
 */
void TimerSetValue(TimerEvent_t *HANDLE, TimerTime_t TIMEOUT) {
    if (HANDLE == NULL) return;

    HANDLE->timeout = TIMEOUT;
}

/**
 * @brief Start the timer
 * @param HANDLE: Pointer to the timer object
 */
void TimerStart(TimerEvent_t *HANDLE) {
    if (HANDLE == NULL) return;

    uint64_t expiryTime = HAL_RADIO_TIMER_GetCurrentSysTime() +
                          HAL_RADIO_TIMER_AddSysTimeMs(0, HANDLE->timeout);
    HAL_RADIO_TIMER_StartVirtualTimerSysTime(&HANDLE->handle, expiryTime);
}

/**
 * @brief Stop the timer
 * @param HANDLE: Pointer to the timer object
 */
void TimerStop(TimerEvent_t *HANDLE) {
    if (HANDLE == NULL) return;

    HAL_RADIO_TIMER_StopVirtualTimer(&HANDLE->handle);
}

/**
 * @brief Get the current system time in milliseconds
 * @return Current time in milliseconds
 */
TimerTime_t TimerGetCurrentTime(void) {
    return (TimerTime_t)(HAL_RADIO_TIMER_GetCurrentSysTime() / 1000); // Convert from STU to ms
}

/**
 * @brief Get the elapsed time between a reference and the current time
 * @param past: Reference time in milliseconds
 * @return Elapsed time in milliseconds
 */
TimerTime_t TimerGetElapsedTime(TimerTime_t past) {
    TimerTime_t current = TimerGetCurrentTime();
    return (current - past);
}
