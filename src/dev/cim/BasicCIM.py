from m5.params import Param
from m5.objects.Device import BasicPioDevice

class BasicCIM(BasicPioDevice):
    type = 'BasicCIM'
    cxx_header = "dev/cim/basic_cim.hh"
    cxx_class = 'gem5::BasicCIM'

    pio_size = Param.Addr(0x1000, "PIO Size")