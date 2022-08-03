#ifndef __CIM_CIM_DRIVER_HH__
#define __CIM_CIM_DRIVER_HH__

#include "cim/basic_cim.hh"
#include "debug/CIMDriver.hh"
#include "params/CIMDriver.hh"
#include "sim/emul_driver.hh"

namespace gem5
{

class BasicCIM;

class CIMDriver : public EmulatedDriver
{
    public:
        PARAMS(CIMDriver);
        CIMDriver(const Params &params);

        int ioctl(ThreadContext *tc, unsigned req, Addr ioc_buf) override;

        int open(ThreadContext *tc, int mode, int flags) override;
        Addr mmap(ThreadContext *tc, Addr start, uint64_t length,
                int prot, int tgt_flags, int tgt_fd, off_t offset) override;

    private:
        BasicCIM *device;
};

} // namespace gem5

#endif // __CIM_CIM_DRIVER_HH__

