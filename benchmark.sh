#!/usr/bin/env bash
exes="
serial
with_boost_compute
threaded
with_openmp
"

for f in $exes
do
  echo "Benchmarking $f"
  total_time=0.0
  for i in `seq 1 10`;
  do
    run_time=$(/usr/bin/time 2>&1 -f '%e' ./$f > /dev/null)
    total_time=$(echo $total_time + $run_time | bc -l)
  done
  total_time=$(echo $total_time / 10.0 | bc -l)
  echo Average time: $total_time
done

