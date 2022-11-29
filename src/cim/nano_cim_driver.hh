#ifndef __CIM_NANO_CIM_DRIVER_HH__
#define __CIM_NANO_CIM_DRIVER_HH__

#include "cim/nano_cim.hh"
#include "debug/NanoCIMDriver.hh"
#include "params/NanoCIMDriver.hh"
#include "sim/emul_driver.hh"

namespace gem5
{

class NanoCIM;

class NanoCIMDriver : public EmulatedDriver
{
    public:
        PARAMS(NanoCIMDriver);
        NanoCIMDriver(const Params &params);

        int ioctl(ThreadContext *tc, unsigned req, Addr ioc_buf) override;

        int open(ThreadContext *tc, int mode, int flags) override;
        Addr mmap(ThreadContext *tc, Addr start, uint64_t length,
                int prot, int tgt_flags, int tgt_fd, off_t offset) override;

    private:
        NanoCIM *device;
};

} // namespace gem5

#endif // __CIM_NANO_CIM_DRIVER_HH__

