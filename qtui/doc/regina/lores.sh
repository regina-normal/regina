#!/bin/bash
set -e

for i in "$@"; do
  hires="$i@2x.png"
  lores="$i.png"
  if [ -e "$hires" ]; then
    # 28.35 ppcm == 72 dpi
    convert "$hires" -resize '50%' -density '28.35' "$lores"
  else
    echo "ERROR: missing hi-res image: $i"
    exit 1
  fi
done
