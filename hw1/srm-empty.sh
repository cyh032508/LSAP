#!/usr/bin/env bash
set -euo pipefail

if [[ -n "${SUDO_USER-}" && "$SUDO_USER" != "root" ]]; then
  HOME_DIR="$(eval echo "~$SUDO_USER")"
else
  HOME_DIR="$HOME"
fi
TRASH_ROOT="$HOME_DIR/.trash"
FILES_DIR="$TRASH_ROOT/files"
META_DIR="$TRASH_ROOT/meta"
AGE="${TRASH_MAX_AGE_DAYS:-7}"

purged=0
now_epoch="$(date +%s)"

shopt -s nullglob
for meta in "$META_DIR"/*.meta.csv; do
  key="$(basename -- "$meta")"; key="${key%.meta.csv}"
  IFS=, read -r orig ts sz < "$meta" || ts=""
  if [[ -z "$ts" ]]; then
    echo "WARN: cannot parse timestamp in $meta" >&2
    continue
  fi
  # GNU date 能吃 ISO8601；若失敗則跳過
  if ! del_epoch="$(date -d "$ts" +%s 2>/dev/null)"; then
    echo "WARN: invalid timestamp: $ts ($meta)" >&2
    continue
  fi
  diff_days=$(( (now_epoch - del_epoch) / 86400 ))
  if (( diff_days >= AGE )); then
    rm -f -- "$FILES_DIR/$key.tar.gz" "$meta" && ((purged++))
  fi
done
shopt -u nullglob

echo "Purged $purged item(s) older than $AGE day(s)."