import argparse
import m5
import sys

from m5.objects import SystemC_Kernel, Root

# pylint:disable=unused-variable

# The python version of the systemc kernel acts as an interface to sc_main. The
# c++ version of the kernel object has a lot of important jobs supporting
# systemc objects and needs to exist in simulations using systemc.
kernel = SystemC_Kernel()
root = Root(full_system=True, systemc_kernel=kernel)

parser = argparse.ArgumentParser()
parser.add_argument('--word', action="append", default=[],
        help='Add a word to the list of words to print. Can be repeated.')

args = parser.parse_args()

# Tell gem5 to run the c++ sc_main function. If one isn't defined, gem5 will
# detect that and report an error. If gem5 isn't finding your sc_main, make
# sure its signature matches exactly so your compiler doesn't think it's a
# different function.
#
# The arguements passed to this function will be treated as the argv values
# passed to the c++ sc_main, with the argc value set appropriately.
m5.systemc.sc_main(*args.word);

# Construct the SimObject hierarchy. Anything sc_main built has already been
# constructed.
m5.instantiate(None)

# Run the simulation until something kicks us back to the config file. sc_main
# will be at the point it first called sc_start and may keep executing as the
# simulation runs, or it may be completed if it never called sc_start.
cause = m5.simulate(m5.MaxTick).getCause()

# If sc_main finished, extract what it returned and do something with it.
result = m5.systemc.sc_main_result()
if result.code != 0:
    sys.exit(int(result.code))
