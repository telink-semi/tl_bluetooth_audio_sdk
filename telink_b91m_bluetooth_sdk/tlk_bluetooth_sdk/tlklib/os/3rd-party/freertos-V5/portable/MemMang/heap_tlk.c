//for debug
#include "../../../../tlkos_config.h"
#include "../../include/FreeRTOS.h"
#include "tlklib/mem/tlkmem1.h"

#if TLKOS_CFG_FREERTOS_ENABLE && (TLKOS_CFG_OS_HEAP_ALG_SELECT == TLKOS_CFG_OS_HEAP_ALG_TLK)
extern uint8_t ucHeap[ configTOTAL_HEAP_SIZE ];

void tlkos_mem_init(void)
{
    tlkmem1_init(ucHeap, configTOTAL_HEAP_SIZE);
}

_attribute_os_core_code_ram_sec_
void * pvPortMalloc( size_t xWantedSize )
{
    void *ret = tlkmem1_malloc(ucHeap, xWantedSize);
    #if ( configUSE_MALLOC_FAILED_HOOK == 1 )
        {
            if( ret == NULL ) {
                extern void vApplicationMallocFailedHook( void );
                vApplicationMallocFailedHook();
            }
        }
    #endif /* if ( configUSE_MALLOC_FAILED_HOOK == 1 ) */
    return ret;
}

_attribute_os_core_code_ram_sec_
void vPortFree( void * pv )
{
    tlkmem1_free(ucHeap, pv);
}

void tlkos_mem_print(void)
{
    tlkmem1_print(ucHeap);
}
#endif //TLKOS_CFG_FREERTOS_ENABLE
