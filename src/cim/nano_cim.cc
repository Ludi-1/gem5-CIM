#include "cim/nano_cim.hh"

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
#include "cim/cim_mmap.h"
#include "cim/nano_cim_driver.hh"
#include "debug/NanoCIM.hh"
#include "mem/packet.hh"
#include "mem/packet_access.hh"
#include "params/NanoCIM.hh"

namespace gem5
{

NanoCIM::NanoCIM(const Params &params) :
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
        DPRINTF(NanoCIM, "NanoCIM Exit Callback\n");
    });

    DPRINTF(NanoCIMDriver, "NanoCIM memory content file: %s\n",
        params.memory_data);
    DPRINTF(NanoCIM, "NanoCIM instantiated\n");
}

void
NanoCIM::init()
{
    PioDevice::init();
    DPRINTF(NanoCIM, "NanoCIM initialized\n");
}

Tick
NanoCIM::read(PacketPtr pkt)
{
    Addr addr = pkt->getAddr() - pioAddr;
    DPRINTF(NanoCIM,"NanoCIM: read (address: %#x, size: %#x, value: %#x)\n",
            addr, pkt->getSize(), pkt->getUintX(byteOrder));

    pkt->setUintX(deviceStorage[addr], byteOrder); // Sets return value of read
    pkt->makeResponse();
    cim_stats.cim_reads++; //Increment cim_reads (scalar) stat by 1
    return pioDelay;
}

Tick
NanoCIM::write(PacketPtr pkt)
{
    Addr addr = pkt->getAddr() - pioAddr;
    DPRINTF(NanoCIM,"NanoCIM: write (address: %#x, size: %#x, value: %#x)\n",
            addr, pkt->getSize(), pkt->getUintX(byteOrder));
    switch (addr) {
        case CIM_ADDR_EXECUTE:
        {
            DPRINTF(NanoCIMDriver, "NanoCIM mmap: EXECUTE\n");
            // Execute SystemC model
            system("./cim_tile.exe > m5out/systemc-logs.txt");
            // Read result from SystemC model to deviceStorage
            std::ifstream output_result_file;
            output_result_file.open(
                "systemc-output/outputfile_after_addition.txt"
            );
            for (int i = 0; i < 16; i++){
                for (int j = 0; j < 16; j++){
                    output_result_file >> deviceStorage[
                        CIM_ADDR_OUTPUT_MEM + 32*i + 30 - 2*j];
                }
            }
        }
        break;
        default:
        {
            DPRINTF(NanoCIMDriver, "NanoCIM mmap: DEFAULT\n");
        }
        break;
    }
    deviceStorage[addr] = pkt->getUintX(byteOrder);
    pkt->makeResponse();
    cim_stats.cim_writes++; //Increment cim_writes (scalar) stat by 1
    return pioDelay;
}

Addr
NanoCIM::devAddr()
{
    return pioAddr;
}

void
NanoCIM::attachDriver(NanoCIMDriver *cim_driver)
{
    fatal_if(_driver, "Should not overwrite driver.");
    // TODO: GPU Driver inheritance hierarchy doesn't really make sense.
    // Should get rid of the base class.
    _driver = cim_driver;
    assert(_driver);
}

void
NanoCIM::exitCallback(std::vector<uint16_t> deviceStorage)
{
    DPRINTF(NanoCIM, "NanoCIM destroyed\n");
    std::ofstream outputFile("m5out/cim_device_memory.txt");
    for (std::vector<uint16_t>::const_iterator i = deviceStorage.begin();
        i != deviceStorage.end();
        i++){
        outputFile << *i << "\n";
    }
    outputFile.close();
}

} // namespace gem5
