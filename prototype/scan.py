#!/usr/bin/env python

import os
import sys
import time
import datetime
import threading
import subprocess
import RPi.GPIO as GPIO
from pygame import camera


MOTOR = 17
POS_SENSOR = 22
WATCH_SENSOR = 23

PATH = '/mnt/tantus/raw'
FRAMES = 3
SKIPS = 2
MAX_IMAGES = 85 * 1000 / 4.01
#CMD_CAM = '/usr/bin/fswebcam --jpeg 100 -F 4 --no-banner -r 1600x1200 --skip 1 -d /dev/video0 -i 0  --save %s'
CMD_CAM = 'uvccapture -x1600 -y1200 -d/dev/video0 -q100 -m  -t0  -o%s'
CMD_LN = '/bin/ln -sf %s %s/current'
CMD_MKDIR = '/bin/mkdir %s'

def init():
    if os.geteuid() is not 0:
        print 'you need run scan as root!'
        sys.exit(1)

    GPIO.setmode(GPIO.BCM)
    GPIO.setup(POS_SENSOR, GPIO.IN)
    GPIO.setup(WATCH_SENSOR, GPIO.IN)
    GPIO.setup(MOTOR, GPIO.OUT, initial=GPIO.LOW)

def clear():
    sys.stderr.write("\x1b[2J\x1b[H")
    print '\rWellcome to 8mm  movie scanner'
    print '\rWritten by Samuel Riolo 2013'
    print '\rRaspberry PI %s' % GPIO.RPI_REVISION
    print '\rRPI GPIO %s' % GPIO.VERSION
    print '\n\n'    


def main():
    clear()
    options = dict(step=step, move=move, scan=scan, quit=quit)
    while True:
        mode = raw_input('Choose a mode: step, move, scan or quit\n')
        if mode in options:
            clear()
            options[mode]()
        else:
            clear()
            print 'Command not found'

def step():
    GPIO.output(MOTOR, True)
    #GPIO.wait_for_edge(POS_SENSOR, GPIO.RISING)
    #GPIO.wait_for_edge(POS_SENSOR, GPIO.FALLING)
    while GPIO.input(POS_SENSOR) == GPIO.HIGH:
        time.sleep(0.001)
    while GPIO.input(POS_SENSOR) == GPIO.LOW:
        time.sleep(0.001)
    GPIO.output(MOTOR, False)

def move():
    print 'Press Space to move or ESC to ecape'
    getch = _GetchUnix()
    while True:
        GPIO.output(MOTOR, False)
        sys.stdout.write('\rmove> stopped')
        input = getch()
        if chr(27) in input:
            sys.stdout.write('\r')
            break
        if '\r' in input or ' ' in input:
            GPIO.output(MOTOR, True)
            sys.stdout.write('\rmove> running')
            time.sleep(0.3)
filename = ''
def scan():
    camera.init()
    cam = camera.Camera(camera.list_cameras()[0], (1600,1200,))
    cam.start()
    
    ok = False
    while not ok:
        movie = raw_input('Name of the movie: ')
        startby = raw_input('Start movie by picture [default 0]: ')
        startat = raw_input('Start the scanner at "8:20" [default now]: ')
        
        try:
            if startat == '' or startat == 'now':
                startat = None
            else:
                h, m = startat.split(':')
                startat = datetime.datetime.now()
                startat = startat.replace(hour=int(h), minute=int(m))
                if(datetime.datetime.now() > startat):
                    startat = startat + datetime.timedelta(days=1)
        except:
            print 'Wrong time input'
            continue

        if startby:
            startby = int(startby)
        else:
            startby = 0
        f = '%s/%s' % (PATH, movie,)
        if (os.path.exists(f) and startby is 0) or (not os.path.exists(f) and startby is not 0):
            print 'Folder already exist'
            continue
        os.system(CMD_MKDIR % f)
        ok = True
    while startat is not None and datetime.datetime.now() < startat:
       clear()
       print 'Scanner will starts in %s:%s:%s ' % cal_time((startat - datetime.datetime.now()).total_seconds())
       time.sleep(0.5)



    params = dict(scanning=True, 
                  filename='',
                  stop=False,
                  stopwatch=datetime.timedelta(),
                  counter=1,
                  exit = False,
                  watchdog=True,
                  watchdog_diff=0)
    counter = startby
    stopwatch = datetime.datetime.now()
    threading.Thread(target=scan_display, args = ((params,))).start()
    threading.Thread(target=scan_stop, args = ((params,))).start()
    threading.Thread(target=scan_watchdog, args = ((params,))).start()
    #while GPIO.input(END_SENSOR) == GPIO.HIGH and counter <= MAX_IMAGES:
    while counter <= MAX_IMAGES and not params['exit']:
        while params['stop']:
            time.sleep(0.01)
        counter += 1
        params['counter'] = counter
        params['stopwatch'] = datetime.datetime.now() - stopwatch
        stopwatch = datetime.datetime.now()
        params['filename'] = '%s/%s/%s.raw' % (PATH, movie, ('0'*6 + str(counter))[-6:],)
        #subprocess.call(CMD_CAM % params['filename'], shell=True)
        f = open(params['filename'], 'w')
        # wait until the cameras frame is ready
        for i in range(FRAMES + SKIPS):
            while not cam.query_image():
                time.sleep(0.005)
            if i < SKIPS:
                cam.get_raw()
            else:
                f.write(cam.get_raw())
        f.close()
        subprocess.call(CMD_LN % (os.path.basename(params['filename']), '%s/%s' % (PATH, movie,),), shell=True)
        step()
    params['scanning'] = False
    cam.stop()
    open('%s/%s/ready' % (PATH, movie,), 'w').close()

def scan_display(params):
    print_lock = threading.Lock()
    def saveprint(args):
        with print_lock:
            print '\r' + args
    date = datetime.datetime.now()
    while params['scanning']:
        clear()
        saveprint('Started: %s' % date.strftime('%B %d, %Y %H:%M:%S'))
        saveprint('Time: %i:%s:%s' % cal_time((datetime.datetime.now() - date).total_seconds()))
        saveprint('Approximately finish: %i:%s:%s' % cal_time(params['stopwatch'].total_seconds() * (MAX_IMAGES - params['counter'])))
        saveprint('File: %s' % params['filename'])
        saveprint('Watchdog: %s' % (params['watchdog'] and 'ON' or 'OFF'))
        s = os.statvfs(PATH)
        saveprint('Free Space: %s MB' %  ((s.f_bavail * s.f_frsize) / 1024 / 1024))
        if params['stop']:
            saveprint('=== break ===   press SPACE to continue')
        time.sleep(0.5)

def scan_stop(params):
    getch = _GetchUnix()
    while params['scanning']:
        ge = getch()
        if ' ' in ge:
            params['stop'] = not params['stop']
        if 'w' in ge:
            params['watchdog'] = not params['watchdog']
        if 's' in ge:
            params['exit'] = True
            params['stop'] = False
        time.sleep(0.5) 

def scan_watchdog(params):
    while params['scanning']:
        time.sleep(1)
        if not params['watchdog']:
            continue
        if GPIO.input(WATCH_SENSOR) == GPIO.LOW:
            params['stop'] = True
            params['watchdog'] = False
    
def cal_time(second):
        second
        hours, remainder = divmod(second, 3600)
        minutes, seconds = divmod(remainder, 60)
        return (int(hours), ('00%i' % minutes)[-2:], ('00%i' % seconds)[-2:])

def quit():
    GPIO.cleanup()
    print 'bye bye'
    sys.exit(0)


# http://jennyandlih.com/reading-single-character-stdin
class _GetchUnix:
    def __init__(self):
        import tty, sys
        
    def __call__(self):
        import sys, tty, termios
        fd = sys.stdin.fileno()
        old_settings = termios.tcgetattr(fd)
        try:
            tty.setraw(sys.stdin.fileno())
            ch = sys.stdin.read(1)
        finally:
            termios.tcsetattr(fd, termios.TCSADRAIN, old_settings)
        return ch


if __name__ == "__main__":
    try:
        init()
        main()
    except KeyboardInterrupt:
        GPIO.cleanup()



