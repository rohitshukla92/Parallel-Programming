#!/bin/bash

#SBATCH -n 4 # Number of cores
#SBATCH -N 1 # Ensure that all cores are on one machine
#SBATCH -t 0-00:05 # Runtime in D-HH:MM
#SBATCH -p development # Partition to submit to


./Question1
