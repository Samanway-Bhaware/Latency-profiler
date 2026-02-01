#!/bin/bash
# Records performance data ensuring stack traces are captured
# -F 99: Sample at 99Hz (avoids lockstep with 100Hz timers)
# -g: Call graphs (stack traces)
# --: Separator before command
echo "Recording perf data..."
perf record -F 99 -g -- ./build/profiler all
echo "Done. Data saved to perf.data"