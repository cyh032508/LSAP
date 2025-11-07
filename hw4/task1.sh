#!/bin/bash
SITES_FILE="sites.txt"
OUTPUT_CSV="task1_results.csv"

if [ ! -f "$SITES_FILE" ]; then
    exit 1
fi

echo "Website,A (IPv4),AAAA (IPv6),CNAME,MX,DNSSEC" > $OUTPUT_CSV


while read site; do
    if [ -z "$site" ]; then
        continue
    fi

    A_RECORD=$(dig +short $site A | tr '\n' ' ')
    AAAA_RECORD=$(dig +short $site AAAA | tr '\n' ' ')
    CNAME_RECORD=$(dig +short $site CNAME)
    MX_RECORD=$(dig +short $site MX | tr '\n' ' ')

    if dig +dnssec $site | grep -q "RRSIG"; then
        DNSSEC_STATUS="Enabled"
    else
        DNSSEC_STATUS="Disabled"
    fi

    echo "\"$site\",\"$A_RECORD\",\"$AAAA_RECORD\",\"$CNAME_RECORD\",\"$MX_RECORD\",\"$DNSSEC_STATUS\"" >> $OUTPUT_CSV

    sleep 0.5

done < "$SITES_FILE"