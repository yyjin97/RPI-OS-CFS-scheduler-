
# RPI-OS-CFS-scheduler

Raspberry pi os with CFS(_Completely Fair Scheduler_), the default Linux scheduler.

This code is the actual Linux source code analyzed and applied to the Raspberry Pi OS.

## Getting started


### Prerequisites

 1. Raspberry Pi 3 Model B 
 2. USB to TTL serial cable
 3. SD card with installed Raspbian OS

### Download image file to SD card

Install code and make kernel8.img file 
If the execution environment is ubuntu 14.04 version, add -std=99 option to Makefile before make. 

```

git clone https://github.com/yyjin97/rpi-os-cfs-sched.git
make

```

Copy the created kernel8.img file to the SD card's boot directory.
At this time, all kernel image files such as kernel.img, kernel7.img, kernel8.img, etc. existing on the SD card must be deleted.

Delete all the existing contents of the config.txt file in the sd card's boot directory and modify as follows.

```

kernel_old=1
disable_commandline_tags=1

```

## Raspberry Pi connection

Raspberry Pi GPIO and UART cable connection
Never connect the power cable and uart cable at the same time.

![rpi connect](https://user-images.githubusercontent.com/26455575/92269622-81dd2580-ef1f-11ea-85e8-a57ea92752e7.PNG)


Insert the sd card into the raspberry pi and connect the uart cable to the laptop.

![rpi to laptop](https://user-images.githubusercontent.com/26455575/92269797-ded8db80-ef1f-11ea-8156-1d49fa898bb5.PNG)

```

sudo screen /dev/ttyUSB0 115200

```

## Execution result

![exe result](https://user-images.githubusercontent.com/26455575/92270681-6f63eb80-ef21-11ea-84c2-55918a398743.PNG)

