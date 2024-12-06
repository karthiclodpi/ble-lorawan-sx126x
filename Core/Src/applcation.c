/* Includes ------------------------------------------------------------------*/
#include "application.h"
#include "systime.h"
#include "timer.h"
#include "LmHandler.h"
#include "app_conf.h"
#include "lora_app.h"
#include "lora_info.h"
#include "stm32_seq.h"
#include "main.h"
#include "stm32wb0x_hal.h"

VTIMER_HandleType TestTimer_timer_Id;

static TimerEvent_t TxTimer;



//void printReadableTime(time_t epoch_time) {
//	struct tm *local_time = localtime(&epoch_time);
//	char time_str[100];
//
//	// Manually format the date and time
//	snprintf(time_str, sizeof(time_str), "%04d-%02d-%02d %02d:%02d:%02d",
//			local_time->tm_year + 1900, // Year since 1900
//			local_time->tm_mon + 1,     // Month (0-based)
//			local_time->tm_mday,        // Day of the month
//			local_time->tm_hour,        // Hours
//			local_time->tm_min,         // Minutes
//			local_time->tm_sec);        // Seconds
//
//	// Print the formatted time
//	APP_DBG_MSG("Readable TStamp: %s\n", time_str);
//}
//
//static void OnTxTimerEvent(void *context) {
//	TimerStop(&TxTimer);
//
//	//  IsTxFramePending = 1;
//
//	// Schedule next transmission
//
//	APP_DBG_MSG("Test_Time_Req\n");
//	SysTime_t sysTime = SysTimeGet();
//	APP_DBG_MSG("sysTime %d \n", sysTime.Seconds);
//	printReadableTime((time_t) sysTime.Seconds);
//	TimerSetValue(&TxTimer, 15000);
//	TimerStart(&TxTimer);
//}
//
//static void Test_Time_Req(void *arg) {
//	HAL_RADIO_TIMER_StopVirtualTimer(&TestTimer_timer_Id);
//	APP_DBG_MSG("Test_Time_Req\n");
//	SysTime_t sysTime = SysTimeGet();
//	APP_DBG_MSG("sysTime %d \n", sysTime.Seconds);
//	//printReadableTime((time_t) sysTime.Seconds);
//
//	TimerInit(&TxTimer, OnTxTimerEvent);
//	TimerSetValue(&TxTimer, 15000);
//
//	// HAL_RADIO_TIMER_StartVirtualTimer(&TestTimer_timer_Id, 15000);
//
//	return;
//}
//
//void InitApp(void) {
//	TestTimer_timer_Id.callback = Test_Time_Req;
//	SysTime_t sysTime;
//	sysTime.Seconds = 1732887883;
//	SysTimeSet(sysTime);
//
//	TimerInit(&TxTimer, OnTxTimerEvent);
//	TimerSetValue(&TxTimer, 15000);
//	TimerStart(&TxTimer);
//
//	/* Init the Lora Stack*/
////	  LmHandlerInit(&LmHandlerCallbacks);
////
////	  LmHandlerConfigure(&LmHandlerParams);
//	// HAL_RADIO_TIMER_StartVirtualTimer(&TestTimer_timer_Id, 15000);
//}

//******LoRa-----------//

static uint8_t AppDataBuffer[LORAWAN_APP_DATA_BUFFER_MAX_SIZE];

/**
 * @brief User application data structure
 */
static LmHandlerAppData_t AppData = { 0, 0, AppDataBuffer };

static void OnTxTimerEvent(void *context) {
	/* USER CODE BEGIN OnTxTimerEvent_1 */

	/* USER CODE END OnTxTimerEvent_1 */
	UTIL_SEQ_SetTask((1 << CFG_SEQ_Task_LoRaSendOnTxTimerOrButtonEvent),
			CFG_SEQ_PRIO_0);

	/*Wait for next tx slot*/
	TimerStart(&TxTimer);
	/* USER CODE BEGIN OnTxTimerEvent_2 */

	/* USER CODE END OnTxTimerEvent_2 */
}

static ActivationType_t ActivationType = LORAWAN_DEFAULT_ACTIVATION_TYPE;
uint8_t GetBatteryLevel() {
	return 0;
}

uint16_t GetTemperatureLevel() {
	return 0;
}

void GetUniqueId(uint8_t *id) {
	/* USER CODE BEGIN GetUniqueId_1 */

	/* USER CODE END GetUniqueId_1 */
	uint32_t ID_1_3_val = HAL_GetUIDw0() + HAL_GetUIDw1();
	uint32_t ID_2_val = HAL_GetUIDw1();

	id[7] = (ID_1_3_val) >> 24;
	id[6] = (ID_1_3_val) >> 16;
	id[5] = (ID_1_3_val) >> 8;
	id[4] = (ID_1_3_val);
	id[3] = (ID_2_val) >> 24;
	id[2] = (ID_2_val) >> 16;
	id[1] = (ID_2_val) >> 8;
	id[0] = (ID_2_val);

	/* USER CODE BEGIN GetUniqueId_2 */

	/* USER CODE END GetUniqueId_2 */
}

uint32_t GetDevAddr(void) {
	return ((HAL_GetUIDw0()) ^ (HAL_GetUIDw1()) ^ (HAL_GetUIDw1()));
}

static void OnMacProcessNotify(void) {
	/* USER CODE BEGIN OnMacProcessNotify_1 */

	/* USER CODE END OnMacProcessNotify_1 */
	UTIL_SEQ_SetTask((1 << CFG_SEQ_Task_LmHandlerProcess), CFG_SEQ_PRIO_0);

	/* USER CODE BEGIN OnMacProcessNotify_2 */

	/* USER CODE END OnMacProcessNotify_2 */
}

static void OnTxData(LmHandlerTxParams_t *params) {
	/* USER CODE BEGIN OnTxData_1 */
	if ((params != NULL)) {
		/* Process Tx event only if its a mcps response to prevent some internal events (mlme) */
		if (params->IsMcpsConfirm != 0) {

			APP_DBG_MSG( "\r\n###### ========== MCPS-Confirm =============\r\n");
			APP_DBG_MSG( "###### U/L FRAME:%04d | PORT:%d | DR:%d | PWR:%d", params->UplinkCounter,
					params->AppData.Port, params->Datarate, params->TxPower);

			APP_DBG_MSG( " | MSG TYPE:");
			if (params->MsgType == LORAMAC_HANDLER_CONFIRMED_MSG) {
				APP_DBG_MSG( "CONFIRMED [%s]\r\n", (params->AckReceived != 0) ? "ACK" : "NACK");
			} else {
				APP_DBG_MSG( "UNCONFIRMED\r\n");
			}
		}
	}
	/* USER CODE END OnTxData_1 */
}

static void OnJoinRequest(LmHandlerJoinParams_t *joinParams) {

	if (joinParams != NULL) {
		if (joinParams->Status == LORAMAC_HANDLER_SUCCESS) {

			APP_DBG_MSG( "\r\n###### = JOINED = ");
			if (joinParams->Mode == ACTIVATION_TYPE_ABP) {
				APP_DBG_MSG( "ABP ======================\r\n");
			} else {
				APP_DBG_MSG( "OTAA =====================\r\n");
			}
		} else {
			APP_DBG_MSG( "\r\n###### = JOIN FAILED\r\n");
		}
	}

}

static void OnRxData(LmHandlerAppData_t *appData, LmHandlerRxParams_t *params) {

	if ((appData != NULL) || (params != NULL)) {

		static const char *slotStrings[] = { "1", "2", "C", "C Multicast",
				"B Ping-Slot", "B Multicast Ping-Slot" };

		APP_DBG_MSG( "\r\n###### ========== MCPS-Indication ==========\r\n");
		APP_DBG_MSG( "###### D/L FRAME:%04d | SLOT:%s | PORT:%d | DR:%d | RSSI:%d | SNR:%d\r\n",
				params->DownlinkCounter, slotStrings[params->RxSlot], appData->Port, params->Datarate, params->Rssi, params->Snr);
//    switch (appData->Port)
//    {
//      case LORAWAN_SWITCH_CLASS_PORT:
//        /*this port switches the class*/
//        if (appData->BufferSize == 1)
//        {
//          switch (appData->Buffer[0])
//          {
//            case 0:
//            {
//              LmHandlerRequestClass(CLASS_A);
//              break;
//            }
//            case 1:
//            {
//              LmHandlerRequestClass(CLASS_B);
//              break;
//            }
//            case 2:
//            {
//              LmHandlerRequestClass(CLASS_C);
//              break;
//            }
//            default:
//              break;
//          }
//        }
//        break;
//      case LORAWAN_USER_APP_PORT:
//        if (appData->BufferSize == 1)
//        {
//          AppLedStateOn = appData->Buffer[0] & 0x01;
//          if (AppLedStateOn == RESET)
//          {
//            APP_LOG(TS_OFF, VLEVEL_H,   "LED OFF\r\n");
//
//            LED_Off(LED_RED1);
//          }
//          else
//          {
//            APP_LOG(TS_OFF, VLEVEL_H, "LED ON\r\n");
//
//            LED_On(LED_RED1);
//          }
//        }
//        break;
//
//      default:
//
//        break;
//    }
	}

}

static LmHandlerCallbacks_t LmHandlerCallbacks = { .GetBatteryLevel =
		GetBatteryLevel, .GetTemperature = GetTemperatureLevel, .GetUniqueId =
		GetUniqueId, .GetDevAddr = GetDevAddr, .OnMacProcess =
		OnMacProcessNotify, .OnJoinRequest = OnJoinRequest,
		.OnTxData = OnTxData, .OnRxData = OnRxData, };

/**
 * @brief LoRaWAN handler parameters
 */
static LmHandlerParams_t LmHandlerParams = { .ActiveRegion = ACTIVE_REGION,
		.DefaultClass = LORAWAN_DEFAULT_CLASS, .AdrEnable = LORAWAN_ADR_STATE,
		.TxDatarate = LORAWAN_DEFAULT_DATA_RATE, .PingPeriodicity =
		LORAWAN_DEFAULT_PING_SLOT_PERIODICITY };

static void SendTxData(void);

void LoRaWAN_Init(void) {
	APP_DBG_MSG( "LoRaWAN_Init\r\n");
	UTIL_SEQ_RegTask((1 << CFG_SEQ_Task_LmHandlerProcess), UTIL_SEQ_RFU,
			LmHandlerProcess);
	UTIL_SEQ_RegTask((1 << CFG_SEQ_Task_LoRaSendOnTxTimerOrButtonEvent),
			UTIL_SEQ_RFU, SendTxData);

	/* Init Info table used by LmHandler*/
	LoraInfo_Init();
	APP_DBG_MSG( "LoraInfo_Init\r\n");

	/* Init the Lora Stack*/
	LmHandlerInit(&LmHandlerCallbacks);
	APP_DBG_MSG( "LmHandlerInit\r\n");

	LmHandlerConfigure(&LmHandlerParams);
	APP_DBG_MSG( "LmHandlerConfigure Apps\r\n");


	LmHandlerJoin(ActivationType);
	APP_DBG_MSG( "LmHandlerJoin\r\n");

	//UTIL_TIMER_Create(&TxTimer,  0xFFFFFFFFU, UTIL_TIMER_ONESHOT, OnTxTimerEvent, NULL);
	TimerInit(&TxTimer,  OnTxTimerEvent);
	   // UTIL_TIMER_SetPeriod(&TxTimer,  APP_TX_DUTYCYCLE);
	    //UTIL_TIMER_Start(&TxTimer);
	TimerSetValue(&TxTimer, 15000);
	TimerStart(&TxTimer);


}

static void SendTxData(void) {
	TimerTime_t nextTxIn;

	AppData.BufferSize = 0;
	AppData.Buffer[AppData.BufferSize++] = 0x01;
	AppData.Buffer[AppData.BufferSize++] = 0x02;
	AppData.Buffer[AppData.BufferSize++] = 0x03;

	if (LORAMAC_HANDLER_SUCCESS
			== LmHandlerSend(&AppData, LORAWAN_DEFAULT_CONFIRMED_MSG_STATE,
					&nextTxIn, false)) {
		APP_DBG_MSG("SEND REQUEST\r\n");
	} else if (nextTxIn > 0) {
		APP_DBG_MSG( "Next Tx in  : ~%d second(s)\r\n", (nextTxIn / 1000));
	}

	/* USER CODE END SendTxData_1 */
}

