build/X86/gem5.opt --debug-flag=CIMALL configs/cim_configs/basic_cim.py -j 16 > m5out/logs.txt
#build/RISCV/gem5.opt --debug-flag=LupioTTY configs/example/lupv/run_lupv_big.py atomic 2 > m5out/logs.txt
#build/RISCV/gem5.opt --debug-flag=LupioTTY configs/example/lupv/run_lupv_test.py atomic 2 > m5out/logs.txt
#build/RISCV/gem5.opt configs/example/gem5_library/riscv-fs.py
#build/X86/gem5.opt --debug-flag=CIMDriver configs/example/apu_se.py > m5out/logs.txt