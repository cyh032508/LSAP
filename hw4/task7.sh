#!/bin/bash
SITES="sites.txt"
OUTPUT="task7_server_results.csv"

echo "Website,Server" > $OUTPUT

while read site; do
    if [ -z "$site" ]; then
        continue
    fi
    
    SERVER=$(curl -I -s https://$site | grep -i "^server:" | awk -F ': ' '{print $2}' | tr -d '\r')

    if [ -z "$SERVER" ]; then
        SERVER=$(curl -I -s http://$site | grep -i "^server:" | awk -F ': ' '{print $2}' | tr -d '\r')
    fi

    if [ -z "$SERVER" ]; then
        SERVER="Unknown"
    fi

    echo "$site,$SERVER" >> $OUTPUT
done < "$SITES"