#ifndef __DEV_CIM_BASIC_CIM_HH__
#define __DEV_CIM_BASIC_CIM_HH__

#include "debug/BasicCIM.hh"
#include "params/BasicCIM.hh"
#include "dev/io_device.hh"
#include "sim/system.hh"

namespace gem5
{

class BasicCIM : public BasicPioDevice
{
  public:
    PARAMS(BasicCIM);
    BasicCIM(const Params &params);

    /**
     * Implement BasicPioDevice virtual functions
     */
    Tick read(PacketPtr pkt) override;
    Tick write(PacketPtr pkt) override;

    void init() override;
};

} // namespace gem5

#endif // __DEV_CIM_BASIC_CIM_HH__