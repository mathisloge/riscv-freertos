#include "FreeRTOS.h"
#include "task.h"

#include "irq.h"
#include "clib.h"

#define IRQ_TABLE_NUM_ENTRIES 53
#define PLIC_BASE_ADDRESS 0x40000000
static volatile uint32_t * const PLIC_ENABLE_REGISTER_START = (uint32_t * const)(PLIC_BASE_ADDRESS + 0x2000);
static volatile uint32_t * const PLIC_PRIOTIRY_REGISTER_START = (uint32_t * const)(PLIC_BASE_ADDRESS); 
static volatile uint32_t * const PLIC_CLAIM_AND_RESPONSE_REGISTER = (uint32_t * const)(PLIC_BASE_ADDRESS + 0x200004); 

static BaseType_t irq_empty_handler() { return pdFALSE; }

static irq_handler_t irq_handler_table[IRQ_TABLE_NUM_ENTRIES] = { [ 0 ... IRQ_TABLE_NUM_ENTRIES-1 ] = irq_empty_handler };


BaseType_t xExternalInterruptHandler() {
	uint32_t irq_id = *PLIC_CLAIM_AND_RESPONSE_REGISTER;

	BaseType_t ans = irq_handler_table[irq_id]();

	*PLIC_CLAIM_AND_RESPONSE_REGISTER = irq_id;
	return ans;
}

void register_interrupt_handler(uint32_t irq_id, irq_handler_t fn, uint8_t prio) {
	configASSERT (irq_id < IRQ_TABLE_NUM_ENTRIES);
	irq_handler_table[irq_id] = fn;
	if(irq_id < 32)
		*PLIC_ENABLE_REGISTER_START = (*PLIC_ENABLE_REGISTER_START )| (1 << irq_id);
	else
		*(PLIC_ENABLE_REGISTER_START + 1) = (*(PLIC_ENABLE_REGISTER_START + 1) )| (1 << (irq_id - 32));
	*(PLIC_PRIOTIRY_REGISTER_START + irq_id) = prio;
}
