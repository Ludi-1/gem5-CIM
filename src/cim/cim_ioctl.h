#ifndef CIM_IOCTL_H_INCLUDED
#define CIM_IOCTL_H_INCLUDED

#include <linux/ioctl.h>
#include <linux/types.h>
#include <stdint.h>

namespace gem5
{
#define CIM_IOCTL_BASE 'a'
#define CIM_IO(nr) _IO(CIM_IOCTL_BASE, nr)
#define CIM_IOR(nr, type) _IOR(CIM_IOCTL_BASE, nr, type)
#define CIM_IOW(nr, type) _IOW(CIM_IOCTL_BASE, nr, type)
#define CIM_IOWR(nr, type) _IOWR(CIM_IOCTL_BASE, nr, type)

struct cim_ioctl_write_args
{
  uint32_t base_adress;
};

#define CIMTEST CIM_IO(0x01)
#define CIMWRITE CIM_IOW(0x02, struct cim_ioctl_write_args)

} // namespace gem5

#endif
