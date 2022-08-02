#ifndef CIM_MMAP_H_INCLUDED
#define CIM_MMAP_H_INCLUDED

#include "cim/cim_ioctl.h"

namespace gem5
{

#define PAGE_SHIFT 12
#define CIM_MMAP_TYPE_SHIFT     (62 - PAGE_SHIFT)
#define CIM_MMAP_TYPE_MASK      (0x3ULL << CIM_MMAP_TYPE_SHIFT)
#define CIM_MMAP_TEST           (0x3ULL << CIM_MMAP_TYPE_SHIFT)

} // namespace gem5

#endif