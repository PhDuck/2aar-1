/*
 * Process platform specific code.
 */
#include <arch.h>
#include "proc/process.h"
#include "kernel/thread.h"
#include "kernel/assert.h"
#include "kernel/interrupt.h"
#include "kernel/config.h"
#include "vm/memory.h"

void process_set_pagetable(pagetable_t *pagetable)
{
  interrupt_status_t intr_status;

  /* Put the mapped pages into TLB. Here we assume that the pages fit
     into the TLB. After writing proper TLB exception handling this
     call should be skipped, leaving the function empty. */
  intr_status = _interrupt_disable();
  if(pagetable == NULL)
  	return;

  /* Check that the pagetable can fit into TLB. This is needed until
	   we have proper VM system, because the whole pagetable must fit
	   into TLB. */
  KERNEL_ASSERT(pagetable->valid_count <= (_tlb_get_maxindex()+1));

  _tlb_write(pagetable->entries, 0, pagetable->valid_count);

	/* Set ASID field in Co-Processor 0 to match thread ID so that
	   only entries with the ASID of the current thread will match in
	   the TLB hardware. */
  _tlb_set_asid(pagetable->ASID);
  _interrupt_set_state(intr_status);
}
