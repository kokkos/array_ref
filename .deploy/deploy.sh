#!/bin/bash
set -e # Exit with nonzero exit code if anything fails

mkdir -p out/proposals
for i in proposals/*.rst; do
  rst2html.py $i out/proposals/$(basename -s .rst $i).html
done
for i in proposals/*.bs; do
  bikeshed spec $i out/proposals/$(basename -s .bs $i).html
done
