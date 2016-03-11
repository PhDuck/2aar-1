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
  tlb_exception_state_t state;
  _tlb_get_exception_state(&state);

  for (int i = 0; i < PAGETABLE_ENTRIES; ++i)
  {
    if (state.badvpn2 == pagetable->entries[i].VPN2)
    {
      *entry_index = i;
      return 0;
    }
  }
  return NO_PAGE_ENTRY_FOUND;
}

void tlb_modified_exception()
{
  pagetable_t* pagetable = thread_get_current_thread_entry() -> pagetable;

  if (pagetable->entries == NULL) {
    KERNEL_PANIC("TLBL: Kernel Mode, we must panic.\n");
  }
  process_exit(7);
}

void tlb_load_exception(void)
{
  pagetable_t* pagetable = thread_get_current_thread_entry() -> pagetable;

  if (pagetable->entries == NULL) {
    KERNEL_PANIC("TLBL: Kernel Mode, we must panic.\n");
  }

  int search_result, entry_index;
  search_result = find_matching_entry(pagetable, &entry_index);

  if (search_result == 0) {
    tlb_entry_t entry = pagetable->entries[entry_index];
    _tlb_write_random(&entry);
  } else {
    process_exit(search_result);
  }

}

void tlb_store_exception(void)
{
  tlb_load_exception();
}


