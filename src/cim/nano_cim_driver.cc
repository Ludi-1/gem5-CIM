#include "cim/nano_cim_driver.hh"

#include "base/trace.hh"
#include "cim/cim_ioctl.h"
#include "cim/nano_cim.hh"
#include "debug/NanoCIMDriver.hh"
#include "mem/port_proxy.hh"
#include "mem/se_translating_port_proxy.hh"
#include "params/NanoCIMDriver.hh"
#include "sim/process.hh"
#include "sim/se_workload.hh"
#include "sim/syscall_emul_buf.hh"

namespace gem5
{

NanoCIMDriver::NanoCIMDriver(const Params &params)
    : EmulatedDriver(params), device(params.device)
{
    DPRINTF(NanoCIMDriver, "Constructing CIMDriver\n");
}

int
NanoCIMDriver::open(ThreadContext *tc, int mode, int flags)
{
    DPRINTF(NanoCIMDriver, "CIMDriver open: Opened %s\n", filename);
    auto process = tc->getProcessPtr();
    auto device_fd_entry = std::make_shared<DeviceFDEntry>(this, filename);
    int tgt_fd = process->fds->allocFD(device_fd_entry);
    return tgt_fd;
}

Addr
NanoCIMDriver::mmap(ThreadContext *tc, Addr start, uint64_t length,
                       int prot, int tgt_flags, int tgt_fd, off_t offset)
{
    auto process = tc->getProcessPtr();
    auto mem_state = process->memState;

    DPRINTF(NanoCIMDriver, "cim mmap (start: %p, length: 0x%x, "
            "offset: 0x%x)\n", start, length, offset);

    start = mem_state->extendMmap(length);
    process->pTable->map(start, device->devAddr(),
                    length, false);
  return start;
}

int
NanoCIMDriver::ioctl(ThreadContext *tc, unsigned req, Addr ioc_buf)
{
    SETranslatingPortProxy se_proxy(tc);
    PortProxy &virt_proxy = se_proxy;
    auto process = tc->getProcessPtr();
    auto mem_state = process->memState;
    DPRINTF(NanoCIMDriver, "ioctl code: %ld\n", req);
    switch (req) {
        case CIMTEST:
        {
            DPRINTF(NanoCIMDriver, "CIMDriver ioctl: CIMTEST\n");
        }
        break;
        case CIMWRITE:
        {
            TypedBufferArg<cim_ioctl_write_args> args(ioc_buf);
            DPRINTF(NanoCIMDriver, "CIMDriver ioctl: CIMWRITE\n");
            DPRINTF(NanoCIMDriver, "argument: %p\n", args->base_adress);
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
