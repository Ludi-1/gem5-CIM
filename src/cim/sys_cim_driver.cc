#include "cim/sys_cim_driver.hh"

#include "base/trace.hh"
#include "cpu/thread_context.hh"

//#include "systemc/tlm_bridge/gem5_to_tlm.hh"
#include "cim/cim_ioctl.h"
#include "debug/CIMDriver.hh"
#include "mem/port_proxy.hh"
#include "mem/se_translating_port_proxy.hh"
#include "params/SysCIMDriver.hh"
#include "sim/process.hh"
#include "sim/se_workload.hh"
#include "sim/syscall_emul_buf.hh"

namespace gem5
{

SysCIMDriver::SysCIMDriver(const Params &params)
    : EmulatedDriver(params), deviceAddr(params.device_addr)
{
    DPRINTF(CIMDriver, "Constructing SysCIMDriver\n");
}

int
SysCIMDriver::open(ThreadContext *tc, int mode, int flags)
{
    DPRINTF(CIMDriver, "CIMDriver open: Opened %s\n", filename);
    auto process = tc->getProcessPtr();
    auto device_fd_entry = std::make_shared<DeviceFDEntry>(this, filename);
    int tgt_fd = process->fds->allocFD(device_fd_entry);
    return tgt_fd;
}

Addr
SysCIMDriver::mmap(ThreadContext *tc, Addr start, uint64_t length,
                       int prot, int tgt_flags, int tgt_fd, off_t offset)
{
    auto process = tc->getProcessPtr();
    auto mem_state = process->memState;

    DPRINTF(CIMDriver, "cim mmap (start: %p, length: 0x%x, "
            "offset: 0x%x)\n", start, length, offset);

    start = mem_state->extendMmap(length);
    process->pTable->map(start, deviceAddr,
                    length, false);
  return start;
}

int
SysCIMDriver::ioctl(ThreadContext *tc, unsigned req, Addr ioc_buf)
{
    SETranslatingPortProxy se_proxy(tc);
    PortProxy &virt_proxy = se_proxy;
    auto process = tc->getProcessPtr();
    auto mem_state = process->memState;
    DPRINTF(CIMDriver, "ioctl code: %ld\n", req);
    switch (req) {
        case CIMTEST:
        {
            DPRINTF(CIMDriver, "CIMDriver ioctl: CIMTEST\n");
        }
        break;
        case CIMWRITE:
        {
            TypedBufferArg<cim_ioctl_write_args> args(ioc_buf);
            DPRINTF(CIMDriver, "CIMDriver ioctl: CIMWRITE\n");
            DPRINTF(CIMDriver, "argument: %p\n", args->base_adress);
            args.copyIn(virt_proxy);
        }
        break;
        default:
        fatal("%s: bad ioctl %p\n", req, ioc_buf);
        break;
    }
    return 0;
}

} // namespace gem5
