#!/usr/bin/env python
# -*- coding: utf-8 -*-
import sys
import os
from subprocess import call

if(os.path.isfile("result.txt")):
	os.remove("result.txt")

if(os.path.isfile("log_out")):
	os.remove("log_out")

call("make clean", shell = True)
call("make", shell = True)
call("clear", shell = True)

def copia():
	arquivo = open("result2.txt")
	arquivo = arquivo.readlines()
	for line in arquivo:
		arquivo_out.write(line);
	os.remove("result2.txt")

def gera_media():
	arquivo = open("result.txt")
	arquivo = arquivo.readlines()
	arquivo = arquivo[0].split(' ')
	total = 0.0
	for number in range(repeticoes):
		subtotal = float(arquivo[number])
		total = total + subtotal
	os.remove("result.txt")
	return total/repeticoes

img_input = []
img_input += ["asp75.pgm        "]
img_input += ["asp100.pgm       "]
# img_input += ["asp150.pgm       "]
# img_input += ["asp200.pgm       "]
# img_input += ["asp250.pgm       "]
# img_input += ["asp300.pgm       "]
# img_input += ["asp350.pgm       "]
# img_input += ["asp_500x375.pgm  "]
# img_input += ["asp_800x600.pgm  "]
# img_input += ["asp_1120X750.pgm "]
# img_input += ["asp_1500x900.pgm "]
# img_input += ["asp_1500x1000.pgm"]
# img_input += ["asp_1800x1000.pgm"]
# img_input += ["aspirina.pgm     "]
# img_input += ["aspirina2.pgm    "]
# img_input += ["aspirina8.pgm    "]
# img_input += ["aspirinaO.pgm    "]
# img_input += ["halloween-1600x1200.pgm"]
number_threads = [1,2,4,8,12,16]
repeticoes = 10

media_single_core = 0

arquivo_media = open("media_result","w")
arquivo_speed = open("speed_result","w")
arquivo_out = open("log_out","a")

for i in img_input:
	for j in number_threads:
		for k in range(repeticoes):
			call("mpirun -np " + str(j) + " ./maxtree " + " imgs/" + i, shell = True)
		copia()
		media = round(gera_media(),6)
		if(j==1):
			media_single_core = media
		if(j<10):
			print("| " + i + " | 0" + str(j) + " thread(s) ",end="")
			arquivo_out.write("| " + i + " | 0" + str(j) + " thread(s) ");
		else:
			print("| " + i + " | " + str(j) + " thread(s) ",end="")
			arquivo_out.write("| " + i + " | " + str(j) + " thread(s) ");
		if(media<10):
			print("| M   {0:.6f} ".format(media),end="")
			arquivo_out.write("| M   {0:.3f} ".format(media));
		else:
			if(media>=100):
				print("| M {0:.6f} ".format(media),end="")
				arquivo_out.write("| M {0:.3f} ".format(media));
			if(media>=100):
				print("| M  {0:.6f} ".format(media),end="")
				arquivo_out.write("| M  {0:.3f} ".format(media));
			if(media>=10):
				print("| M   {0:.6f} ".format(media),end="")
				arquivo_out.write("| M   {0:.3f} ".format(media));
			else:
				print("| M    {0:.6f} ".format(media),end="")
				arquivo_out.write("| M    {0:.3f} ".format(media));
		print("| S {0:.6f}".format(media_single_core/media))
		arquivo_out.write("| S {0:.3f}\n".format(media_single_core/media));
		arquivo_media.write(str(media)+" ")
		arquivo_speed.write(str(media_single_core/media)+" ")
arquivo_media.close()
arquivo_speed.close()