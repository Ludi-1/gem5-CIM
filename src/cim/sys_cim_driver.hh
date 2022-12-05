#ifndef __CIM_SYS_CIM_DRIVER_HH__
#define __CIM_SYS_CIM_DRIVER_HH__

// #include "systemc/tlm_bridge/gem5_to_tlm.hh"
// #include "debug/SysCIMDriver.hh"
#include "params/SysCIMDriver.hh"
#include "sim/emul_driver.hh"

namespace gem5
{

// class Gem5ToTlmBridge;

class SysCIMDriver : public EmulatedDriver
{
    public:
        PARAMS(SysCIMDriver);
        SysCIMDriver(const Params &params);

        int ioctl(ThreadContext *tc, unsigned req, Addr ioc_buf) override;

        int open(ThreadContext *tc, int mode, int flags) override;
        Addr mmap(ThreadContext *tc, Addr start, uint64_t length,
                int prot, int tgt_flags, int tgt_fd, off_t offset) override;

    private:
        Addr deviceAddr;
        // Gem5ToTlmBridge *device;
};

} // namespace gem5

#endif // __CIM_SYS_CIM_DRIVER_HH__

