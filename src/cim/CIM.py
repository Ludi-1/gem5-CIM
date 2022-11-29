from m5.params import *
from m5.objects.Process import EmulatedDriver
from m5.objects.Device import BasicPioDevice
from m5.objects.Device import DmaVirtDevice

# BasicCIM device
class BasicCIM(BasicPioDevice):
    type = 'BasicCIM'
    cxx_header = "cim/basic_cim.hh"
    cxx_class = 'gem5::BasicCIM'

    pio_size = Param.Addr(0x1000, "PIO Size")
    memory_data = Param.String(0, "Intial data content of device")

# NanoCIM device
class NanoCIM(BasicPioDevice):
    type = 'NanoCIM'
    cxx_header = "cim/nano_cim.hh"
    cxx_class = 'gem5::NanoCIM'

    pio_size = Param.Addr(0x1000, "PIO Size")
    memory_data = Param.String(0, "Intial data content of device")

# Fake drivers for BasicCIM device in SE mode
class CIMDriver(EmulatedDriver):
    type = 'CIMDriver'
    cxx_header = 'cim/cim_driver.hh'
    cxx_class = 'gem5::CIMDriver'
    device = Param.BasicCIM('CIM device controlled by this driver')

# Fake drivers for BasicCIM device in SE mode
class NanoCIMDriver(EmulatedDriver):
    type = 'NanoCIMDriver'
    cxx_header = 'cim/nano_cim_driver.hh'
    cxx_class = 'gem5::NanoCIMDriver'
    device = Param.NanoCIM('CIM device controlled by this driver')