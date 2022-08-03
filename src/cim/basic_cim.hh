#ifndef __CIM_BASIC_CIM_HH__
#define __CIM_BASIC_CIM_HH__

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

  private:
    CIMDriver *_driver;
    std::vector<uint64_t> deviceStorage;

  protected:
    const ByteOrder byteOrder = ByteOrder::little;

    struct CIMStats : public Stats::Group
    {
        Stats::Scalar cim_writes;
        Stats::Scalar cim_reads;
        Stats::Vector cim_storage;

        CIMStats(Stats::Group *parent) :
            Stats::Group(parent),
            ADD_STAT(cim_writes, "Amount of writes on CIM device"),
            ADD_STAT(cim_reads, "Amount of reads on CIM device"),
            ADD_STAT(cim_storage, "Data content BasicCIM")
        {
        }
    } cim_stats;
};

} // namespace gem5

#endif // __DEV_CIM_BASIC_CIM_HH__
