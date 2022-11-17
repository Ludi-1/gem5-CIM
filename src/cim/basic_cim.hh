#ifndef __CIM_BASIC_CIM_HH__
#define __CIM_BASIC_CIM_HH__

#include "base/callback.hh"
#include "base/statistics.hh"
#include "cim/cim_driver.hh"
#include "debug/BasicCIM.hh"
#include "dev/io_device.hh"
#include "params/BasicCIM.hh"
#include "sim/system.hh"

namespace gem5
{

class CIMDriver;

class BasicCIM : public BasicPioDevice
{
  public:
    PARAMS(BasicCIM)
    BasicCIM(const Params &params);

    CIMDriver* driver();

    Tick write(PacketPtr pkt) override;
    Tick read(PacketPtr pkt) override;

    Addr devAddr();
    void attachDriver(CIMDriver *driver);

    void init() override;

    void functionalAccess(PacketPtr pkt);

    std::vector<uint16_t> deviceStorage;
    void exitCallback(std::vector<uint16_t> deviceStorage);

  private:
    CIMDriver *_driver;

  protected:
    const ByteOrder byteOrder = ByteOrder::little;

    struct CIMStats : public Stats::Group
    {
        Stats::Scalar cim_writes;
        Stats::Scalar cim_reads;

        CIMStats(Stats::Group *parent) :
            Stats::Group(parent),
            ADD_STAT(cim_writes, "Amount of writes on CIM device"),
            ADD_STAT(cim_reads, "Amount of reads on CIM device")
        {
        }
    } cim_stats;
};

} // namespace gem5

#endif // __DEV_CIM_BASIC_CIM_HH__
