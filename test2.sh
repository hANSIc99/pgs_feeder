#!/bin/bash
value=$(<test.json)
echo "$value" | ./pgs_feeder
