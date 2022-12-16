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
                            sc_core::sc_time& delay)
{
    cout << "b_transport called" << endl;
    executeTransaction(trans);
}

unsigned int SysCIM::transport_dbg(tlm::tlm_generic_payload& trans)
{
    cout << "transport_dbg called" << endl;
    unsigned int len = trans.get_data_length();
    return len;
}

/* TLM-2 non-blocking transport method */
tlm::tlm_sync_enum SysCIM::nb_transport_fw(tlm::tlm_generic_payload& trans,
                                           tlm::tlm_phase& phase,
                                           sc_time& delay)
{
    cout << "nb_transport_fw called" << endl;
    /* Queue the transaction until the annotated time has elapsed */
    // m_peq.notify(trans, phase, delay);
    trans.set_response_status( tlm::TLM_OK_RESPONSE );
    return tlm::TLM_ACCEPTED;
}

void SysCIM::executeTransaction(tlm::tlm_generic_payload& trans)
{
    long phys_addr = 0x200000000;
    tlm::tlm_command cmd = trans.get_command();
    sc_dt::uint64    adr = trans.get_address() - phys_addr;
    unsigned char*   ptr = trans.get_data_ptr();
    unsigned int     len = trans.get_data_length();
    unsigned char*   byt = trans.get_byte_enable_ptr();

    cout << "ptr addr: " << reinterpret_cast<void *>(ptr) << endl;
    cout << "value in ptr: " << *reinterpret_cast<int*>(ptr) << endl;

    if (adr >= 16*1024*1024) {
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
        cout << "read command" << endl;
        memcpy(ptr,      // destination
                mem + adr, // source
                trans.get_data_length());  // size
    }
    else if (cmd == tlm::TLM_WRITE_COMMAND)
    {
        cout << "write command" << endl;
        memcpy(mem + adr, // destination
                ptr,      // source
                trans.get_data_length());  // size
    }

    cout << name()
            << setfill(' ') << setw(12) << sc_time_stamp()
            << ": " << setw(12) << (cmd ? "Exec. Write " : "Exec. Read ")
            << "Addr = " << setfill('0') << setw(8) << hex << adr
            << " Data = " << *reinterpret_cast<int*>(ptr) << endl;

    trans.set_response_status( tlm::TLM_OK_RESPONSE );
}

// void
// SysCIM::peq_cb(tlm::tlm_generic_payload& trans,
//                const tlm::tlm_phase& phase)
// {
//     sc_core::sc_time delay;

//     if (phase == tlm::BEGIN_REQ) {

//         /* Increment the transaction reference count */
//         trans.acquire();

//         if ( !transaction_in_progress ) {
//             send_end_req(trans);
//         } else {
//             /* Put back-pressure on initiator by deferring END_REQ until
//              * pipeline is clear */
//             end_req_pending = &trans;
//         }
//     } else if (phase == tlm::END_RESP) {
//         /* On receiving END_RESP, the target can release the transaction and
//          * allow other pending transactions to proceed */
//         if (!response_in_progress) {
//             SC_REPORT_FATAL("TLM-2", "Illegal transaction phase END_RESP"
//                             "received by target");
//         }

//         transaction_in_progress = 0;

//         /* Target itself is now clear to issue the next BEGIN_RESP */
//         response_in_progress = false;
//         if (next_response_pending) {
//             send_response( *next_response_pending );
//             next_response_pending = 0;
//         }

//         /* ... and to unblock the initiator by issuing END_REQ */
//         if (end_req_pending) {
//             send_end_req( *end_req_pending );
//             end_req_pending = 0;
//         }

//     } else /* tlm::END_REQ or tlm::BEGIN_RESP */ {
//             SC_REPORT_FATAL("TLM-2", "Illegal transaction phase received by"
//                             "target");
//     }
// }

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
