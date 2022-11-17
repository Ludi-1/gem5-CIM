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
#include "cim/cim_mmap.h"
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
    std::ofstream empty_instr_file;
    empty_instr_file.open("instr_mem.txt");
    empty_instr_file.close();
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

    pkt->setUintX(deviceStorage[addr], byteOrder); // Sets return value of read
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
    switch (addr) {
        case CIM_ADDR_EXECUTE:
        {
            DPRINTF(CIMDriver, "BasicCIM mmap: EXECUTE\n");
            // Write WD mem from deviceStorage
            std::ofstream wd_file;
            wd_file.open("wd_mem_input.txt");
            for (int i = 0; i < 16; i++){
                for (int j = 0; j < 16; j++){
                    wd_file << deviceStorage[CIM_ADDR_WD_MEM + 32*i + 2*j];
                    wd_file << " ";
                }
                wd_file << "\n";
            }
            wd_file.close();

            // Write RD mem from deviceStorage
            std::ofstream rd_file;
            rd_file.open("rd_mem_input.txt");
            for (int i = 0; i < 16; i++){
                for (int j = 0; j < 16; j++){
                    rd_file << deviceStorage[CIM_ADDR_RD_MEM + 32*i + 2*j];
                    rd_file << " ";
                }
                rd_file << "\n";
            }
            rd_file.close();
            // Execute compiler
            system("./compiler.exe > m5out/compiler-logs.txt");
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
        case CIM_ADDR_INSTR_MEM:
        {
            DPRINTF(CIMDriver, "BasicCIM mmap: INSTR_MEM\n");
            std::ofstream instr_file;
            instr_file.open("instr_mem.txt", std::ios_base::app);
            switch (deviceStorage[CIM_ADDR_OPCODE]) {
                case CIM_OP_STORE:
                {
                    DPRINTF(CIMDriver,
                        "BasicCIM mmap: INSTR_MEM - CIM_OP_STORE\n");
                    instr_file << "store &B";
                    instr_file << "[" << deviceStorage[CIM_ADDR_OP1] << "]";
                    instr_file << "[" << deviceStorage[CIM_ADDR_OP2] << "]";
                    instr_file << " ";
                    instr_file << deviceStorage[CIM_ADDR_OP3] << " ";
                    instr_file << deviceStorage[CIM_ADDR_OP4] << " ";
                    instr_file << deviceStorage[CIM_ADDR_OP5] << " ";
                    instr_file << deviceStorage[CIM_ADDR_OP6] << " ";
                    instr_file << deviceStorage[CIM_ADDR_OP7] << std::endl;
                }
                break;
                case CIM_OP_MMM:
                {
                    DPRINTF(CIMDriver,
                        "BasicCIM mmap: INSTR_MEM - CIM_OP_MMM\n");
                    instr_file << "MMM &A";
                    instr_file << "[" << deviceStorage[CIM_ADDR_OP1] << "]";
                    instr_file << "[" << deviceStorage[CIM_ADDR_OP2] << "]";
                    instr_file << " ";
                    instr_file << deviceStorage[CIM_ADDR_OP3] << " ";
                    instr_file << deviceStorage[CIM_ADDR_OP4] << " ";
                    instr_file << deviceStorage[CIM_ADDR_OP5] << " ";
                    instr_file << deviceStorage[CIM_ADDR_OP6] << " ";
                    instr_file << deviceStorage[CIM_ADDR_OP7] << " ";
                    instr_file << deviceStorage[CIM_ADDR_OP8] << " ";
                    instr_file << deviceStorage[CIM_ADDR_OP9] << std::endl;
                }
                break;
                default:
                {
                    DPRINTF(CIMDriver,
                        "BasicCIM mmap: INSTR_MEM - BAD_OPCODE\n");
                }
            }
            instr_file.close();
        }
        break;
        default:
        {
            DPRINTF(CIMDriver, "BasicCIM mmap: DEFAULT\n");
        }
        break;
    }
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
BasicCIM::exitCallback(std::vector<uint16_t> deviceStorage)
{
    DPRINTF(BasicCIM, "BasicCIM destroyed\n");
    std::ofstream outputFile("m5out/cim_device_memory.txt");
    for (std::vector<uint16_t>::const_iterator i = deviceStorage.begin();
        i != deviceStorage.end();
        i++){
        outputFile << *i << "\n";
    }
    outputFile.close();
}

} // namespace gem5
