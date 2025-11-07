#!/bin/bash

SITES_FILE="sites.txt"
OUTPUT_DIR="task3_lb_results"

mkdir -p $OUTPUT_DIR

echo "正在檢測 DNS Load Balancing..."

while read site; do
    if [ -z "$site" ]; then
        continue
    fi

    echo "測試: $site"

    # 查 10 次，比較明顯
    echo "查詢結果：" > "$OUTPUT_DIR/$site.txt"
    for i in {1..10}; do
        dig +short $site A | head -n 1 >> "$OUTPUT_DIR/$site.txt"
    done
done < "$SITES_FILE"

echo "完成！結果已輸出至 $OUTPUT_DIR/"