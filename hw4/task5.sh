#!/bin/bash
SITES_FILE="sites.txt"
OUTPUT="task5_network_results.csv"

echo "Website,Avg_Latency(ms),Packet_Loss(%),Download_Throughput(Mbps)" > "$OUTPUT"

while read -r site; do
  [ -z "$site" ] && continue

  ip4=$(dig +short A "$site" | head -n 1)
  ip6=$(dig +short AAAA "$site" | head -n 1)

  target_ip=""
  ping_cmd=""
  if [ -n "$ip4" ]; then
    target_ip="$ip4"
    ping_cmd="ping -c 5 $target_ip"
  elif [ -n "$ip6" ]; then
    target_ip="$ip6"
    ping_cmd="ping6 -c 5 $target_ip"
  else
    echo "$site,N/A,N/A,N/A" >> "$OUTPUT"
    continue
  fi

  # ---- Latency / Loss ----
  ping_out=$($ping_cmd 2>/dev/null)
  packet_loss=$(echo "$ping_out" | grep -Eo '[0-9]+(\.[0-9]+)?% packet loss' | awk '{print $1}')
  [ -z "$packet_loss" ] && packet_loss="N/A"

  rtt_line=$(echo "$ping_out" | grep -E 'round-trip|rtt')
  if [ -n "$rtt_line" ]; then
    avg_latency=$(echo "$rtt_line" | awk -F'/' '{print $5}')
  else
    avg_latency="N/A"
  fi

  speed_bytes=$(curl -o /dev/null -L --silent --write-out '%{speed_download}\n' "https://$site")
  if [ -z "$speed_bytes" ]; then
    throughput="N/A"
  else
    throughput=$(echo "$speed_bytes / 125000" | bc -l)  # Bytes/s -> Mbps
  fi

  echo "$site,$avg_latency,$packet_loss,$throughput" >> "$OUTPUT"
  sleep 0.4
done < "$SITES_FILE"