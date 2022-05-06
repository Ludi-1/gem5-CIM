#include "learning_gem5/part2/hello_object.hh"
#include "debug/HelloExample.hh"
#include "base/trace.hh"

namespace gem5
{
HelloObject::HelloObject(const HelloObjectParams &params) :
    SimObject(params), event([this]{processEvent();}, name()),
    latency(100),timesLeft(10)
{
    DPRINTF(HelloExample, "Created the hello object\n");
}

void
HelloObject::processEvent()
{
    timesLeft--;
    DPRINTF(HelloExample, "Hello world! Processing the event! %d left\n", timesLeft);

    if (timesLeft <= 0) {
        DPRINTF(HelloExample, "Done firing!\n");
    } else {
        schedule(event, curTick() + latency);
    }
}

void
HelloObject::startup()
{
    schedule(event, latency);
}

} // namespace gem5
