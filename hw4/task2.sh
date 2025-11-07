#!/bin/bash
SITES_FILE="sites.txt"
OUTPUT_FILE="task2_dns_time.csv"

while read site; do
    if [ -z "$site" ]; then
        continue
    fi

    total=0
    count=5

    for i in $(seq 1 $count); do
        time=$(dig +stats $site | grep "Query time" | awk '{print $4}')
        total=$((total + time))
        sleep 0.3
    done

    avg=$((total / count))

    echo "$site,$avg" >> $OUTPUT_FILE
done < "$SITES_FILE"