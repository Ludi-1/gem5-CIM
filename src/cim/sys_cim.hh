#ifndef __CIM_SYS_CIM_HH__
#define __CIM_SYS_CIM_HH__

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

  private:
    //unsigned char mem[512];
    unsigned char *mem;

  public:
    SC_HAS_PROCESS(SysCIM);
    SysCIM(sc_module_name name) :
         sc_module(name),
         tSocket("tSocket"),
         wrapper(tSocket, std::string(name) + ".tlm", InvalidPortID)
    {
        tSocket.register_b_transport(this, &SysCIM::b_transport);
        mem = (unsigned char *)malloc(16*1024*1024);
        std::cout << "SysCIM Online" << std::endl;
    }

    gem5::Port &gem5_getPort(const std::string &if_name, int idx=-1) override;

    virtual void b_transport(tlm::tlm_generic_payload& trans,
                             sc_time& delay);

    void executeTransaction(tlm::tlm_generic_payload& trans);
};

#endif // __CIM_SYS_CIM_HH__
