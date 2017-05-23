#!/bin/bash
date > logs/runs.log
./maxtree imgs/asp75.pgm > logs/asp75.log 
gprof maxtree > logs/asp75.log-gmon 
mv imgs/out.pgm imgs/outasp75.pgm

./maxtree imgs/asp100.pgm > logs/asp100.log 
gprof maxtree > logs/asp100.log-gmon 
mv imgs/out.pgm imgs/outasp100.pgm

./maxtree imgs/asp150.pgm > logs/asp150.log 
gprof maxtree > logs/asp150.log-gmon 
mv imgs/out.pgm imgs/outasp150.pgm

./maxtree imgs/asp200.pgm > logs/asp200.log 
gprof maxtree > logs/asp200.log-gmon 
mv imgs/out.pgm imgs/outasp200.pgm

./maxtree imgs/asp300.pgm > logs/asp300.log 
gprof maxtree > logs/asp300.log-gmon 
mv imgs/out.pgm imgs/outasp300.pgm

./maxtree imgs/asp350.pgm > logs/asp350.log 
gprof maxtree > logs/asp350.log-gmon 
mv imgs/out.pgm imgs/outasp350.pgm

./maxtree imgs/asp_1500x900.pgm > logs/asp1500.log 
gprof maxtree > logs/asp1500.log-gmon 
mv imgs/out.pgm imgs/outasp1500.pgm

date >> logs/runs.log
