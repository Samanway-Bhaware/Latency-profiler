#!/bin/bash
# Must be run with sudo or signed entitlement
echo "Recording Time Profiler trace..."

# XCTrace is the modern replacement for 'instruments' CLI
xcrun xctrace record --template "Time Profiler" \
    --launch -- ./profiler all --core 4 \
    --output "trace.trace"

echo "Trace saved to trace.trace. Open in Instruments.app"