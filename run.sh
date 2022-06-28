#build/RISCV/gem5.opt --debug-flags=SimpleCIM configs/cim/test_simple.py | head -n 100
# build/RISCV/gem5.opt --debug-flags=SimpleCIM configs/cim/test_simple.py
#build/RISCV/gem5.opt configs/cim/test_simple.py
#build/RISCV/gem5.opt configs/cim/lupv_test.py --max-ticks=50000000 timing 1
#build/RISCV/gem5.opt configs/example/se.py -c="tests/test-progs/riscv/bin/forloop.out;tests/test-progs/riscv/bin/forloop.out;tests/test-progs/riscv/bin/forloop.out;tests/test-progs/riscv/bin/forloop.out;tests/test-progs/riscv/bin/forloop.out;" --l1d_size=16kB --l1i_size=64kB --caches --cpu-type=TimingSimpleCPU -n=3
#build/RISCV/gem5.opt configs/cim/simple_cim.py
#build/RISCV/gem5.opt configs/example/se.py -c="tests/test-progs/riscv/bin/forloop.out" --l1d_size=16kB --l1i_size=64kB --caches --cpu-type=TimingSimpleCPU -n=1

#build/RISCV/gem5.opt --debug-flags=BasicCIM configs/cim/basic_cim.py > logs/sim.txt
#build/X86/gem5.opt --debug-flags=BasicCIM configs/cim/basic_cim.py > logs/sim.txt

#build/RISCV/gem5.opt configs/example/lupv/run_lupv.py timing 1
#build/RISCV/gem5.opt configs/example/fs.py --kernel ./bootloader-vmlinux-5.10 --disk-image ./riscv_disk.img --mem-size 8192MB --mem-type DDR3_1600_8x8 --cpu-type TimingSimpleCPU --script tests/test-progs/hello/bin/riscv/linux/hello