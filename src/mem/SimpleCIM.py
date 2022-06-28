from m5.params import *
from m5.objects.AbstractMemory import *

class SimpleCIM(AbstractMemory):
    type = 'SimpleCIM'
    cxx_header = "mem/simple_cim.hh"
    cxx_class = 'gem5::memory::SimpleCIM'

    port = ResponsePort("This port sends responses and receives requests")
    latency = Param.Latency('30ns', "Request to response latency")
    latency_var = Param.Latency('0ns', "Request to response latency variance")
    # The memory bandwidth limit default is set to 12.8GiB/s which is
    # representative of a x64 DDR3-1600 channel.
    bandwidth = Param.MemoryBandwidth('12.8GiB/s',
                                      "Combined read and write bandwidth")

    def controller(self):
        # Simple memory doesn't use a MemCtrl
        return self