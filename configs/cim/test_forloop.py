import m5
from m5.objects import *

system = System()
system.clk_domain = SrcClockDomain()
system.clk_domain.clock = '1GHz'
system.clk_domain.voltage_domain = VoltageDomain()
system.mem_mode = 'timing'
system.mem_ranges = [AddrRange('8192MB')]

system.cpu = TimingSimpleCPU()

system.memobj = SimpleCIM()

system.cpu.icache_port = system.memobj.inst_port
system.cpu.dcache_port = system.memobj.data_port

system.membus = SystemXBar()

system.memobj.mem_side = system.membus.cpu_side_ports

system.cpu.createInterruptController()
# system.cpu.interrupts[0].pio = system.membus.cpu_side_ports
# system.cpu.interrupts[0].int_master = system.membus.cpu_side_ports
# system.cpu.interrupts[0].int_slave = system.membus.mem_side_ports

system.mem_ctrl = MemCtrl()
system.mem_ctrl.dram = DDR3_1600_8x8()
system.mem_ctrl.dram.range = system.mem_ranges[0]
system.mem_ctrl.port = system.membus.mem_side_ports

system.system_port = system.membus.cpu_side_ports

thispath = os.path.dirname(os.path.realpath(__file__))
print("current path:")
print(thispath)
binary = os.path.join(thispath, '../../',
    'tests/test-progs/riscv/bin/forloop.out')

system.workload = SEWorkload.init_compatible(binary)

# Create a process for a simple "Hello World" application
process = Process()
# Set the command
# cmd is a list which begins with the executable (like argv)
process.cmd = [binary]
# Set the cpu to use the process as its workload and create thread contexts
system.cpu.workload = process
system.cpu.createThreads()

# process = Process()
# thispath = os.path.dirname(os.path.realpath(__file__))
# print("current path:")
# print(thispath)
# binary = os.path.join(thispath, '../../',
#     'tests/test-progs/riscv/bin/forloop.out')
# system.cpu.workload = process
# system.cpu.createThreads()

root = Root(full_system = False, system = system)
m5.instantiate()

print ("Beginning simulation!")
exit_event = m5.simulate()
print('Exiting @ tick %i because %s' % (m5.curTick(), exit_event.getCause()))