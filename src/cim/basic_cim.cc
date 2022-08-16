#include "cim/basic_cim.hh"

#include <cassert>
#include <fstream>
#include <functional>
#include <iterator>
#include <sstream>
#include <string>
#include <vector>

#include <sim/core.hh>

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
    BasicPioDevice(params, params.pio_size),
    cim_stats(this)
{
    std::stringstream iss(params.memory_data);
    uint64_t number;
    while (iss >> number)
        deviceStorage.push_back(number);

    deviceStorage.resize(params.pio_size);
    registerExitCallback([this]() {
        exitCallback(deviceStorage);
        DPRINTF(BasicCIM, "BasicCIM Exit Callback\n");
    });

    DPRINTF(CIMDriver, "BasicCIM memory content file: %s\n",
        params.memory_data);
    DPRINTF(BasicCIM, "BasicCIM instantiated\n");
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
    cim_stats.cim_reads++; //Increment cim_reads (scalar) stat by 1
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
    cim_stats.cim_writes++; //Increment cim_writes (scalar) stat by 1
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

void
BasicCIM::exitCallback(std::vector<uint64_t> deviceStorage)
{
    DPRINTF(BasicCIM, "BasicCIM destroyed\n");
    std::ofstream outputFile("m5out/cim_device_memory.txt");
    for (std::vector<uint64_t>::const_iterator i = deviceStorage.begin();
        i != deviceStorage.end();
        i++){
        outputFile << *i << "\n";
    }
    outputFile.close();
}

} // namespace gem5
