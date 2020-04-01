/*
 * nbiot_env.h
 *
 *  Created on: 22 oct. 2019
 *      Author: nxf37702
 */

#ifndef PORT_NBIOT_ENV_H_
#define PORT_NBIOT_ENV_H_

#include <stdint.h>
#include <stdbool.h>
#include "FreeRTOS.h"
#include "semphr.h"
#include "event_groups.h"
#include "nbiot_env_port.h"
/* contains only prototypes and struct. no defines !!! */
#include "nbiot_common.h"
#include "fsl_usart_freertos.h"
#include "sqns_structs.h"

#define RTOS_USART_SEND_PENDING 0x1
#define RTOS_USART_SEND_COMPLETE 0x2
#define RTOS_USART_RECEIVE_PENDING 0x4
#define RTOS_USART_RECEIVE_COMPLETE 0x8

/*! @brief The event flags are cleared automatically or manually.*/
typedef enum _osa_event_clear_mode_t
{
    kEventAutoClear = 0U,  /*!< The flags of the event will be cleared automatically. */
    kEventManualClear = 1U /*!< The flags of the event will be cleared manually.      */
} osa_event_clear_mode_t;

/*! @brief Locks the task scheduler or disables interrupt in critical section. */
typedef enum _osa_critical_section_mode_t
{
    kCriticalLockSched = 0U, /*!< Lock scheduler in critical section.      */
    kCriticalDisableInt = 1U /*!< Disable interrupt in critical selection. */
} osa_critical_section_mode_t;

/*! @brief Type for an event group object in FreeRTOS OS */
// typedef EventGroupHandle_t event_freertos;

/*! @brief Type for an event group object */

typedef struct EventFreertos
{
    EventGroupHandle_t eventHandler;  /*!< FreeRTOS OS event handler       */
    osa_event_clear_mode_t clearMode; /*!< Auto clear or manual clear   */
} event_t;

typedef struct
{
	usart_transfer_t sendXfer;				/*!< TX Data structure */
	usart_transfer_t receiveXfer;			/*!< RX Data structure */
	SemaphoreHandle_t sendSemaphore; 		/*!< RX semaphore for resource sharing */
	SemaphoreHandle_t receiveSemaphore; 	/*!< TX semaphore for resource sharing */
	QueueHandle_t q_txQueue;				/*!< TX Data Queue */
	QueueHandle_t q_rxQueue;				/*!< RX Data Queue */
	EventGroupHandle_t e_UsartEvent;		/*!< USART Events */
	//usart_rtos_handle_t * p_UsartHandle;	/*!< USART Handler pointer */
} cust_context_t;

void nbiot_free(void *addr);
void *nbiot_malloc(int32_t size);
SQNS_STATUS nbiot_mutex_init(xSemaphoreHandle *pMutex);
SQNS_STATUS nbiot_mutex_acquire(xSemaphoreHandle *pMutex);
SQNS_STATUS nbiot_mutex_release(xSemaphoreHandle *pMutex);
bool nbiot_is_mutex_valid(xSemaphoreHandle *pMutex);
SQNS_STATUS nbiot_mutex_delete(xSemaphoreHandle *pMutex);
SQNS_STATUS nbiot_event_delete(event_t *pEvent);
SQNS_STATUS nbiot_event_init(event_t *pEvent, osa_event_clear_mode_t clearMode);
SQNS_STATUS nbiot_event_clear(event_t *pEvent, EventBits_t flagsToClear);
SQNS_STATUS nbiot_event_set(event_t *pEvent, EventBits_t flagsToSet);
SQNS_STATUS nbiot_event_wait(
    event_t *pEvent, EventBits_t flagsToWait, bool waitAll, uint32_t timeout, EventBits_t *setFlags);
uint32_t nbiot_time_get_msec(void);
void OS_nbiot_EnterCritical(osa_critical_section_mode_t mode);
void OS_nbiot_ExitCritical(osa_critical_section_mode_t mode);

#endif /* PORT_NBIOT_ENV_H_ */
