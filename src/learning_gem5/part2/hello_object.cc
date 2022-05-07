#include "learning_gem5/part2/hello_object.hh"

#include "base/logging.hh"
#include "base/trace.hh"
#include "debug/HelloExample.hh"

namespace gem5
{

HelloObject::HelloObject(const HelloObjectParams &params) :
    SimObject(params),
    // This is a C++ lambda. When the event is triggered, it will call the
    // processEvent() function. (this must be captured)
    event([this]{ processEvent(); }, name() + ".event"),
    goodbye(params.goodbye_object),
    // Note: This is not needed as you can *always* reference this->name()
    myName(params.name),
    latency(params.time_to_wait),
    timesLeft(params.number_of_fires)
{
    DPRINTF(HelloExample, "Created the hello object\n");
    panic_if(!goodbye, "HelloObject must have a non-null GoodbyeObject");
}

void
HelloObject::startup()
{
    // Before simulation starts, we need to schedule the event
    schedule(event, latency);
}

void
HelloObject::processEvent()
{
    timesLeft--;
    DPRINTF(HelloExample, "Hello world! Processing the event! %d left\n",
                          timesLeft);

    if (timesLeft <= 0) {
        DPRINTF(HelloExample, "Done firing!\n");
        goodbye->sayGoodbye(myName);
    } else {
        schedule(event, curTick() + latency);
    }
}

} // namespace gem5