#!/bin/bash
SITES_FILE="sites.txt"
OUTPUT="task4_cdn_results.csv"

echo "Website,CDN Provider" > $OUTPUT

while read site; do
    if [ -z "$site" ]; then continue; fi


    ip=$(dig +short $site A | head -n 1)

    whois_info=$(whois $ip | grep -Ei "cloudflare|fastly|akamai|google|facebook|amazon|edge|cdn" | head -n 1)

    if echo "$whois_info" | grep -qi "cloudflare"; then provider="Cloudflare"
    elif echo "$whois_info" | grep -qi "fastly"; then provider="Fastly"
    elif echo "$whois_info" | grep -qi "akamai"; then provider="Akamai"
    elif echo "$whois_info" | grep -qi "google"; then provider="Google Global CDN"
    elif echo "$whois_info" | grep -qi "facebook"; then provider="Meta Edge CDN"
    elif echo "$whois_info" | grep -qi "amazon"; then provider="AWS CloudFront"
    else provider="No CDN / Direct Hosting"
    fi

    echo "$site,$provider" >> $OUTPUT

    sleep 0.3
done < "$SITES_FILE"