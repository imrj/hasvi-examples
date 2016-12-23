#!/usr/bin/python
# (C) Stephen Dade 2016
#Test for Hasvi

import sys
import Adafruit_DHT
import threading
import urllib2
import time

base_site = 'http://data.hasvi.com/insertData'

#repeated timer
#from http://stackoverflow.com/questions/8600161/executing-periodic-actions-in-python
def do_every(period,f):
    def g_tick():
        t = time.time()
        count = 0
        while True:
            count += 1
            yield max(t + count*period - time.time(),0)
    g = g_tick()
    while True:
        time.sleep(next(g))
        f()

#function to read the sensors and publish to Hasvi
def runRead():
    #read
    humid_out, temp_out = Adafruit_DHT.read_retry(Adafruit_DHT.DHT11, 4)
    humid_in, temp_in = Adafruit_DHT.read_retry(Adafruit_DHT.DHT11, 22)
    #print (for debugging)
    print('Out Temp={0:0.1f}*  Humidity={1:0.1f}%'.format(temp_out, humid_out))
    print('In Temp={0:0.1f}*  Humidity={1:0.1f}%'.format(temp_in, humid_in))
    #send to Hasvi
    urllib2.urlopen(base_site + '?token=wv23ibbx15sy1xk56m2v&data=' + str(int(temp_out)))
    urllib2.urlopen(base_site + '?token=re389r497its1rwj9p0s&data=' + str(int(humid_out)))
    urllib2.urlopen(base_site + '?token=1xsbdbv0favihirw6dg5&data=' + str(int(temp_in)))
    urllib2.urlopen(base_site + '?token=qwoyzkrz24mwgbzopb4k&data=' + str(int(temp_out)))


#main
if __name__ == '__main__':
    #run the runRead() function every 600sec
    do_every(600, runRead)

