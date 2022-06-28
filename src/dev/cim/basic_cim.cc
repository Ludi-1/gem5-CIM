#include "dev/cim/basic_cim.hh"

#include "debug/BasicCIM.hh"
#include "params/BasicCIM.hh"

#include "mem/packet_access.hh"

namespace gem5
{

BasicCIM::BasicCIM(const Params &params) :
    BasicPioDevice(params, params.pio_size)
{
}

void
BasicCIM::init()
{
    PioDevice::init();

    DPRINTF(BasicCIM, "BasicCIM initialized\n");
}

Tick
BasicCIM::read(PacketPtr pkt)
{
    DPRINTF(BasicCIM,"basic cim read");
    return pioDelay;
}

Tick
BasicCIM::write(PacketPtr pkt)
{
    DPRINTF(BasicCIM,"basic cim write");
    return pioDelay;
}

} // namespace gem5