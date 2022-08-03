#include "cim/basic_cim.hh"

#include <cassert>

#include "base/logging.hh"
#include "base/trace.hh"
#include "cim/cim_driver.hh"
#include "debug/BasicCIM.hh"
#include "mem/packet.hh"
#include "mem/packet_access.hh"
#include "params/BasicCIM.hh"

namespace gem5
{

BasicCIM::BasicCIM(const Params &params) :
    BasicPioDevice(params, params.pio_size)
{
    deviceStorage.resize(params.pio_size);
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
    Addr addr = pkt->getAddr() - pioAddr;
    DPRINTF(BasicCIM,"BasicCIM: read (address: %#x, size: %#x, value: %#x)\n",
            addr, pkt->getSize(), pkt->getUintX(byteOrder));

    pkt->setUintX(deviceStorage[addr], byteOrder);
    pkt->makeResponse();
    return pioDelay;
}

Tick
BasicCIM::write(PacketPtr pkt)
{
    Addr addr = pkt->getAddr() - pioAddr;
    DPRINTF(BasicCIM,"BasicCIM: write (address: %#x, size: %#x, value: %#x)\n",
            addr, pkt->getSize(), pkt->getUintX(byteOrder));

    deviceStorage[addr] = pkt->getUintX(byteOrder);
    pkt->makeResponse();
    return pioDelay;
}

Addr
BasicCIM::devAddr()
{
    return pioAddr;
}

void
BasicCIM::attachDriver(CIMDriver *cim_driver)
{
    fatal_if(_driver, "Should not overwrite driver.");
    // TODO: GPU Driver inheritance hierarchy doesn't really make sense.
    // Should get rid of the base class.
    _driver = cim_driver;
    assert(_driver);
}

} // namespace gem5
