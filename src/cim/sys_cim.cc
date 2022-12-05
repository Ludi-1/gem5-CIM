#include "cim/sys_cim.hh"

#include <tlm_utils/simple_target_socket.h>

#include <iomanip>
#include <iostream>
#include <map>
#include <queue>
#include <vector>

#include "base/trace.hh"
#include "params/SysCIM.hh"

#include "systemc/ext/systemc"
#include "systemc/ext/tlm"

using namespace std;
using namespace sc_core;
using namespace gem5;

void SysCIM::b_transport(tlm::tlm_generic_payload& trans,
                            sc_time& delay)
{
    executeTransaction(trans);
}

void SysCIM::executeTransaction(tlm::tlm_generic_payload& trans)
{
    tlm::tlm_command cmd = trans.get_command();
    sc_dt::uint64    adr = trans.get_address();
    unsigned char*   ptr = trans.get_data_ptr();
    unsigned int     len = trans.get_data_length();
    unsigned char*   byt = trans.get_byte_enable_ptr();

    if (trans.get_address() >= 16*1024*1024) {
        cout << "\033[1;31m("
             << "Address Error"
             << "\033[0m" << endl;
        trans.set_response_status( tlm::TLM_ADDRESS_ERROR_RESPONSE );
        return;
    }
    if (byt != 0) {
        cout << "\033[1;31m("
             << "Byte Enable Error"
             << "\033[0m" << endl;
        trans.set_response_status( tlm::TLM_BYTE_ENABLE_ERROR_RESPONSE );
        return;
    }
    if (len < 1 || len > 64) {
        cout << "\033[1;31m("
             << "Burst Error"
             << "\033[0m" << endl;
        trans.set_response_status( tlm::TLM_BURST_ERROR_RESPONSE );
        return;
    }

    if (cmd == tlm::TLM_READ_COMMAND)
    {
        memcpy(mem+trans.get_address(), // destination
                trans.get_data_ptr(),      // source
                trans.get_data_length());  // size
    }
    else if (cmd == tlm::TLM_WRITE_COMMAND)
    {
        memcpy(trans.get_data_ptr(),      // destination
                mem + trans.get_address(), // source
                trans.get_data_length());  // size
    }

    cout << "\033[1;32m("
            << name()
            << ")@"  << setfill(' ') << setw(12) << sc_time_stamp()
            << ": " << setw(12) << (cmd ? "Exec. Write " : "Exec. Read ")
            << "Addr = " << setfill('0') << setw(8) << hex << adr
            << " Data = " << "0x" << setfill('0') << setw(8) << hex
            << *reinterpret_cast<int*>(ptr)
            << "\033[0m" << endl;

    trans.set_response_status( tlm::TLM_OK_RESPONSE );
}

// This "create" method bridges the python configuration and the systemc
// objects. It instantiates the Printer object and sets it up using the
// parameter values from the config, just like it would for a SimObject. The
// systemc object could accept those parameters however it likes, for instance
// through its constructor or by assigning them to a member variable.
SysCIM *
gem5::SysCIMParams::create() const
{
    SysCIM *target = new SysCIM(name.c_str());
    return target;
}

gem5::Port
&SysCIM::gem5_getPort(const std::string &if_name, int idx)
{
    return wrapper;
}
