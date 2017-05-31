#!/usr/bin/env python
# -*- coding: utf-8 -*-
import sys
from subprocess import call


for i in range(3):
	call("python3 hocus_pocus_test_generation.py", shell = True)