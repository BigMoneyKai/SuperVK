#include "job.h"

void Job::execute() {
    if (func)
        func(params);
    if (counter)
        counter->value.fetch_sub(1, std::memory_order_acq_rel);
    if (global_counter)
        global_counter->value.fetch_sub(1, std::memory_order_acq_rel);
}
