/*
 * TLB handling
 */

#include "kernel/panic.h"
#include "kernel/assert.h"
#include "kernel/thread.h"
#include "proc/process.h"
#include <tlb.h>
#include <pagetable.h>

int find_matching_entry(pagetable_t* pagetable, int* entry_index)
{
  tlb_exception_state_t* state;
  _tlb_get_exception_state(state);

  for (int i = 0; i < PAGETABLE_ENTRIES; ++i)
  {
    if (state.badvpn2 == pagetable->entries[i].VPN2)
    {
      entry_index* = i;
      return 0;
    }
  }
  return NO_PAGE_ENTRY_FOUND;
}

void tlb_modified_exception()
{

  pagetable_t pagetable = thread_get_current_thread() -> pagetable;

  if (pagetable.entries == NULL) {
    KERNEL_PANIC("TLBL: Kernel Mode, we must panic.\n");
  }
  proccess_exit(7);
}

void tlb_load_exception(void)
{
  pagetable_t pagetable = thread_get_current_thread() -> pagetable;

  if (pagetable.entries == NULL) {
    KERNEL_PANIC("TLBL: Kernel Mode, we must panic.\n");
  }

  int search_result, entry_index;

  search_result = find_matching_entry(pagetable, entry_index);

  if (search_result == 0) {
    tlb_entry_t entry = pagetable->entries[entry_index];
    _tlb_write_random(&entry);
  } else {
    proccess_exit(search_result);
  }

}

void tlb_store_exception(void)
{
  tlb_load_exception();
}

/**
 * Fill TLB with given pagetable. This function is used to set memory
 * mappings in CP0's TLB before we have a proper TLB handling system.
 * This approach limits the maximum mapping size to 128kB.
 *
 * @param pagetable Mappings to write to TLB.
 *
 */

void tlb_fill(pagetable_t *pagetable)
{
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
}
