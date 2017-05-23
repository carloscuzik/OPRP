#!/usr/bin/env python
# -*- coding: utf-8 -*-

import sys

img_input = []
img_input += ["asp75.pgm "]
img_input += ["asp100.pgm"]
img_input += ["asp150.pgm"]
img_input += ["asp200.pgm"]
img_input += ["asp250.pgm"]
# img_input += ["asp300.pgm"]
# img_input += ["asp350.pgm"]
# img_input += ["asp_500x375.pgm"]
# img_input += ["asp_800x600.pgm"]
# img_input += ["asp_1120X750.pgm"]
# img_input += ["asp_1500x900.pgm"]
# img_input += ["asp_1500x1000.pgm"]
# img_input += ["asp_1800x1000.pgm"]
# img_input += ["aspirina.pgm"]
# img_input += ["aspirina2.pgm"]
# img_input += ["aspirina8.pgm"]
# img_input += ["aspirinaO.pgm"]
# img_input += ["halloween-1600x1200.pgm"]
number_threads = [1,2,4,8,12,16,20,24]

SpeedUp = []

arquivo = open("speed_result")
arquivo = arquivo.readlines()
arquivo = arquivo[0].split(' ')
for number in range(len(number_threads)*len(img_input)):
	SpeedUp += [float(arquivo[number])]

print()
print()
print("Highcharts.chart('container', {\n", end="")

print("\ttitle: {\n", end="")
print("\t\ttext: 'Gráfico de comparação de SeepUps da parametrização do código Maxtree'\n", end="")
print("\t},\n", end="")

print("\txAxis: {\n", end="")
print("\t\tcategories: "+str(number_threads)+"\n", end="")
# print("\t\ttitle: {\n", end="")
# print("\t\t\ttext: 'Number of Threads'\n", end="")
# print("\t\t}\n", end="")
print("\t},\n", end="")

print("\tyAxis: {\n", end="")
print("\t\ttitle: {\n", end="")
print("\t\t\ttext: 'SpeedUp'\n", end="")
print("\t\t}\n", end="")
print("\t},\n", end="")
print("\tlegend: {\n", end="")
print("\t\tlayout: 'vertical',\n", end="")
print("\t\talign: 'right',\n", end="")
print("\t\tverticalAlign: 'middle'\n", end="")
print("\t},\n", end="")

print("\tplotOptions: {\n", end="")
print("\t},\n", end="")

print("\tseries: [\n", end="")
print("\t{\n", end="")

print("\t\tname: 'Theoretical SpeedUp',\n", end="")
print("\t\tdata: "+str(number_threads)+",");
print("\t},{\n", end="")

k=0
for i in img_input:
	print("\t\tname: '"+i+"',\n", end="")
	print("\t\tdata: [",end="");
	for j in range(len(number_threads)-1):
		print(str(SpeedUp[k])+",",end="")
		k = k + 1
	print(SpeedUp[k],end="")
	k = k + 1
	print("],")
	if(i!=img_input[len(img_input)-1]):	
		print("\t},{\n", end="")


print("\t}]\n", end="")
print("});\n", end="")





