#gcc -static tests/test-progs/cim-tests/src/access_cim.c -o tests/test-progs/cim-tests/bin/x86/access_cim
#gcc -S -static tests/test-progs/cim-tests/src/access_cim.c -o tests/test-progs/cim-tests/asm/x86/access_cim
#gcc -static tests/test-progs/cim-tests/src/cimdriver_test.c -o tests/test-progs/cim-tests/bin/x86/cimdriver_test
gcc -static tests/test-progs/cim-tests/src/open_cimdriver.c -o tests/test-progs/cim-tests/bin/x86/open_cimdriver