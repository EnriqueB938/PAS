#!/bin/bash


cat access.log | cut -d ' ' -f 1,7 | sort -k1 | uniq -c