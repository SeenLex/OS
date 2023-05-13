#!/bin/bash

if [ $# -eq 0 ]; then
  echo "Usage: $0 <input_file>"
  exit 1
fi

input_file=$1

error_warnings=$(gcc -o /dev/null -Wall -Wextra $input_file 2>&1 | grep -E 'error|warning')

error_count=$(echo "$error_warnings" | grep -c "error")
warning_count=$(echo "$error_warnings" | grep -c "warning")

echo "$error_count" > info.txt
echo "$warning_count" >> info.txt
