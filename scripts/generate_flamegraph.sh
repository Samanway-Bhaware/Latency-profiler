#!/bin/bash
# Assumes FlameGraph repo is cloned at ~/FlameGraph or in path
FLAMEGRAPH_DIR=~/FlameGraph

if [ ! -d "$FLAMEGRAPH_DIR" ]; then
    echo "Please clone FlameGraph: git clone https://github.com/brendangregg/FlameGraph ~/FlameGraph"
    exit 1
fi

echo "Generating Flamegraph..."
perf script > out.perf
$FLAMEGRAPH_DIR/stackcollapse-perf.pl out.perf > out.folded
$FLAMEGRAPH_DIR/flamegraph.pl out.folded > flamegraph.svg

echo "Done. Open flamegraph.svg in your browser."