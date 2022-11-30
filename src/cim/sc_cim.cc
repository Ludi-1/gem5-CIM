#include <vector>

#include "base/trace.hh"

// #include <systemc.h>
#include "systemc/ext/systemc"

using namespace gem5;

class Printer : public sc_core::sc_module
{
  public:
    sc_core::sc_in<const char *> input;

    SC_CTOR(Printer)
    {
        SC_THREAD(print);
    }

    using sc_core::sc_object::print;
    void
    print()
    {
        int i = 0;
        while (true) {
            wait(input.value_changed_event());
            DPRINTFN("CIM Word %d: %s\n", i++, input.read());
        }
    }
};

class Feeder : public sc_core::sc_module
{
  public:
    sc_core::sc_in<bool> clk;
    sc_core::sc_out<const char *> output;

    std::vector<const char *> strings;

    SC_HAS_PROCESS(Feeder);
    Feeder(sc_core::sc_module_name, std::vector<const char *> _strings) :
        strings(_strings)
    {
        SC_THREAD(feed);
        sensitive << clk.pos();
    }

    void
    feed()
    {
        int i = 0;
        while (true) {
            wait();
            if (i >= strings.size())
                sc_core::sc_stop();
            else
                output = strings[i];
            i++;
        }
    }
};

int
sc_main(int argc, char *argv[])
{
    std::vector<const char *> strings;
    for (int i = 0; i < argc; i++)
        strings.push_back(argv[i]);

    sc_core::sc_clock clk;
    sc_core::sc_buffer<const char *> buf;

    Feeder feeder("feeder", strings);
    feeder.clk(clk);
    feeder.output(buf);

    Printer printer("printer");
    printer.input(buf);

    DPRINTFN("CIM SC model works!!!!!\n");
    sc_core::sc_start();

    return 0;
}
