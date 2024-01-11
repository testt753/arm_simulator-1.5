#!/usr/bin/env bash
i=1

for fichier in Examples/*; do

    if [ -x "$fichier" ]; then
        ./arm_simulator --gdb-port 58000 --irq-port 59000 --trace-file "f${i}" --trace-registers --trace-memory --trace-state SVC --trace-position > "res_t_${i}" &
        gdb-multiarch -q -ex "file $fichier" -ex 'target remote 127.0.0.1:58000' -ex 'set end b' -ex 'load' -ex 'c' -ex 'q'

        ((i++))
    fi
done