#ifndef __CIM_SYS_CIM_HH__
#define __CIM_SYS_CIM_HH__

#include <tlm_utils/peq_with_cb_and_phase.h>
#include <tlm_utils/simple_target_socket.h>

#include <iomanip>
#include <iostream>
#include <map>
#include <queue>
#include <vector>

#include "base/trace.hh"
#include "debug/SysCIM.hh"
#include "params/SysCIM.hh"
#include "systemc/ext/core/sc_module_name.hh"
#include "systemc/tlm_port_wrapper.hh"

#include "systemc/ext/systemc"
#include "systemc/ext/tlm"

using namespace std;
using namespace sc_core;
using namespace gem5;

SC_MODULE(SysCIM)
{
  public:
    tlm_utils::simple_target_socket<SysCIM> tSocket;
    sc_gem5::TlmTargetWrapper<32> wrapper;
    // tlm_utils::peq_with_cb_and_phase<SysCIM> m_peq;

  private:
    //unsigned char mem[512];
    unsigned char *mem;

  public:
    SC_HAS_PROCESS(SysCIM);
    SysCIM(sc_core::sc_module_name name) :
         sc_module(name),
         tSocket("tSocket"),
         wrapper(tSocket, std::string(name) + ".tlm", InvalidPortID)
    {
        tSocket.register_b_transport(this, &SysCIM::b_transport);
        tSocket.register_transport_dbg(this, &SysCIM::transport_dbg);
        tSocket.register_nb_transport_fw(this, &SysCIM::nb_transport_fw);
        mem = (unsigned char *)malloc(16*1024*1024);
        std::cout << "SysCIM Online" << std::endl;
    }

    gem5::Port &gem5_getPort(const std::string &if_name, int idx=-1);

    virtual void b_transport(tlm::tlm_generic_payload& trans,
                             sc_core::sc_time& delay);
    virtual unsigned int transport_dbg(tlm::tlm_generic_payload& trans);
    virtual tlm::tlm_sync_enum nb_transport_fw(
                tlm::tlm_generic_payload& trans,
                tlm::tlm_phase& phase,
                sc_core::sc_time& delay);

    /** Callback of Payload Event Queue: */
    // void peq_cb(tlm::tlm_generic_payload& trans,
    //             const tlm::tlm_phase& phase);
    void executeTransaction(tlm::tlm_generic_payload& trans);
};

#endif // __CIM_SYS_CIM_HH__
