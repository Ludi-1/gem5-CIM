from m5.params import *
from m5.objects.Process import EmulatedDriver
from m5.objects.Device import BasicPioDevice

#cim device
class BasicCIM(BasicPioDevice):
    type = 'BasicCIM'
    cxx_header = "cim/basic_cim.hh"
    cxx_class = 'gem5::BasicCIM'

    pio_size = Param.Addr(0x1000, "PIO Size")

#fake drivers for CIM device
class CIMDriver(EmulatedDriver):
    type = 'CIMDriver'
    cxx_header = 'cim/cim_driver.hh'
    cxx_class = 'gem5::CIMDriver'
    device = Param.BasicCIM('CIM device controlled by this driver')
