import m5
# import all of the SimObjects
from m5.objects import *

# create the system we are going to simulate
system = System()

# Set the clock fequency of the system (and all of its children)
system.clk_domain = SrcClockDomain()
system.clk_domain.clock = '1GHz'
system.clk_domain.voltage_domain = VoltageDomain()

# Set up the system
#system.mem_mode = 'atomic'               # Use timing accesses
system.mem_mode = 'timing'

# Create an address range
system.mem_ranges = [AddrRange('8192MB'), \
                    AddrRange(0x200000000, 0x200001000), \
                    ]

# Create a simple CPU
#system.cpu = AtomicSimpleCPU()
system.cpu = TimingSimpleCPU()

# Create a memory bus, a system crossbar, in this case
system.membus = SystemXBar()

# Hook the CPU ports up to the membus
system.cpu.icache_port = system.membus.cpu_side_ports
system.cpu.dcache_port = system.membus.cpu_side_ports

# create the interrupt controller for the CPU and connect to the membus
system.cpu.createInterruptController()

# For x86 only, make sure the interrupts are connected to the memory
# Note: these are directly connected to the memory bus and are not cached
if m5.defines.buildEnv['TARGET_ISA'] == "x86":
    system.cpu.interrupts[0].pio = system.membus.mem_side_ports
    system.cpu.interrupts[0].int_requestor = system.membus.cpu_side_ports
    system.cpu.interrupts[0].int_responder = system.membus.mem_side_ports

# Create a DDR3 memory controller and connect it to the membus
system.mem_ctrl = MemCtrl()
system.mem_ctrl.dram = DDR3_1600_8x8()
system.mem_ctrl.dram.range = system.mem_ranges[0]
system.mem_ctrl.port = system.membus.mem_side_ports

system.target = SysCIM()
system.transactor = Gem5ToTlmBridge32()
system.transactor.gem5 = system.membus.mem_side_ports
system.transactor.tlm = system.target.tlm
system.transactor.addr_ranges = system.mem_ranges[1]

#CIM Driver
cim_driver = SysCIMDriver(filename = "cim")
cim_driver.device_addr = 0x200000000

# Connect the system up to the membus
system.system_port = system.membus.cpu_side_ports

# get ISA for the binary to run.
isa = str(m5.defines.buildEnv['TARGET_ISA']).lower()

# grab the specific path to the binary
thispath = os.path.dirname(os.path.realpath(__file__))
binary = os.path.join(thispath, '../../',
                      'tests/test-progs/cim-tests/bin/', isa, 'open_cimdriver')

system.workload = SEWorkload.init_compatible(binary)
# Create a process for a simple "Hello World" application
process = Process()
# Set the command
# cmd is a list which begins with the executable (like argv)
process.drivers = [cim_driver]
process.cmd = [binary]

# Set the cpu to use the process as its workload and create thread contexts
system.cpu.workload = process
system.cpu.createThreads()

# set up the root SimObject and start the simulation
kernel = SystemC_Kernel(system=system)
root = Root(full_system = False, systemc_kernel = kernel)
# instantiate all of the objects we've created above
m5.instantiate()

print("Beginning simulation!")
exit_event = m5.simulate()
print('Exiting @ tick %i because %s' % (m5.curTick(), exit_event.getCause()))