*This project has been created as part of the 42 curriculum by ychabane.*

# Codexion

## Description

Codexion is a concurrency simulation where coders sit in a circle competing
for shared USB dongles to compile quantum code. Each coder needs two dongles
simultaneously to compile and must do so regularly to avoid burnout. Built
with POSIX threads, mutexes, and condition variables in C.

## Instructions

```bash
cd coders
make
./codexion number_of_coders time_to_burnout time_to_compile time_to_debug \
           time_to_refactor number_of_compiles_required dongle_cooldown scheduler
```

Example:
```bash
./codexion 4 800 200 200 200 5 50 fifo
```

## Blocking Cases Handled

- **Deadlock**: prevented via per-dongle request queues; a coder only
  proceeds when at the front of both queues simultaneously.
- **Starvation**: `fifo` serves in arrival order; `edf` prioritizes the
  coder closest to burnout.
- **Cooldown**: enforced by tracking `last_compile_start_saver` per dongle
  and busy-waiting until `time_to_compile + dongle_cooldown` has elapsed.
- **Burnout detection**: monitor thread polls every 500µs and prints the
  burnout log within the required 10ms window.
- **Log serialization**: all output is wrapped in `display_mutex` so lines
  never interleave.

## Thread Synchronization Mechanisms

- **`main_mutex`**: protects all shared state (flag, counters, timestamps).
- **`display_mutex`**: serializes stdout output.
- **`mutex_dongle`** (one per dongle): protects each dongle's priority queue.
- **`main_cond`**: coders waiting for dongles sleep on this condition;
  `pthread_cond_broadcast` wakes them after each compile or burnout.

## Resources

- [DEADLOCK](https://www.youtube.com/watch?v=ZE1B16ui7Gs&t=348s)
- [Threads course Playlist](https://www.youtube.com/watch?v=d9s_d28yJq0&list=PLfqABt5AS4FmuQf70psXrsMLEDQXNkLq2)
- [Dining Philosophers](https://www.youtube.com/watch?v=FYUi-u7UWgw&t=723s)

**AI usage**: ai was used to fix norminette errors and split function to a a lot functions for norminette
,also understand some concepts if you ask ai it gives you a lot of information and ai helps me to debug my code
because it's really hard to debug threads 