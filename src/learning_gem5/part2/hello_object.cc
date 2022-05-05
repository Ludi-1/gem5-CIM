#include "learning_gem5/part2/hello_object.hh"
#include "debug/HelloExample.hh"
#include "base/trace.hh"

namespace gem5
{
HelloObject::HelloObject(const HelloObjectParams &params) :
    SimObject(params)
{
    DPRINTF(HelloExample, "Created the hello object\n");
}

} // namespace gem5