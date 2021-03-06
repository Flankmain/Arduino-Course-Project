#!/bin/python

import subprocess
import os
import os.path

from configparser import ConfigParser

import sys
import traceback

COMMAND_DEPENDENCIES = [
    'avr-gcc',
    'avr-size',
    'avr-objcopy',
    'avrdude',

    'make',
    
    'lsusb'
]

def unimplemented():
    print("NOT IMPLEMENTED YET, SORRY")

def listDependencies():
    print('''NOTE:
    This list is probably not exhaustive and probably doesn't work on every distribution.
    If you get errors about missing functions, you just have to find a way to install them, unfortunately.

    On Arch and derived distributions the AUR packages 'arduino-avr-core' and 'make' should be enough.

    0) a linux environment, preferrably Arch-based.
        The reason is the device ports are programmatically extracted by this wizard.
        On windows you can just use device manager instead. 
        
        LINUX:   dev/ttyACM... and dev/ttyUSB...
        WINDOWS: COM1, COM2 or something similar.
        

    1) makefile parameters
        You have to manually edit the DEVICE parameter of MEGA/makefile and UNO/makefile
        On windows this argument can be found by the WIZARD or by running ls -l /dev/tty{ACM*,USB*}
        On linux you also might need to give yourself permissions on the device port.
    
    1) avr-gcc is needed for compilation. These commands are used:
        avr-gcc
        avr-size
        avr-objcopy
        avrdude

    2) make
        required by the build scripts (makefiles)

    3) commands probably needed for setup
        lsusb
    ''')


def printFlashConfigGuide():
    print('In order to flash you will have to manually write the DEVICE parameter of each makefile to match an address on your system.')
    print('These are usually /dev/ttyACM0 and /dev/tty/USB0 for the MEGA and UNO respectively')
    print('The makefiles are /UNO/makefile and /MEGA/makefile')
    print('(I didn\'t have enough time to do this programmatically, sorry.)\n')

    print(f'To confirm the same device address across reboots and usb ports, you can try this:')
    print('ls -l /dev/tty{ACM*,USB*}')

    print('''
    crw-rw---- 1 root uucp 166, 0  5. 5. 12:21 /dev/ttyACM0
    crw-rw---- 1 root uucp 188, 0  5. 5. 13:06 /dev/ttyUSB0
    ''')

    print('copy the group name (in this case it\'s "uucp") and run:')
    
    print('sudo usermod -a -G group_name your_username')

    print('After this, you have to restart or re-login. Sorry for that too.')
    print('Alternatively, the WIZARD can generate single session use codes that might work.')
    print('I haven\'t tested that.')


def writeFlashPathToConfig(deviceModel, devicePath):
    unimplemented()

def runCommand(command:str, arguments=[]):
    command = [command] #list of words used
    command.extend(arguments)

    print(command)

    temp = subprocess.Popen(command, stdout = subprocess.PIPE)
    shellOutput = temp.communicate()[0].decode('utf-8') #ascii 128 to utf-8
    return shellOutput

def runCommandInShell(command:str, arguments=[]):
    command = [command] #list of words used
    command.extend(arguments)
    process = subprocess.Popen(command, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    shellOutput = process.communicate(b'\n')

    return shellOutput

def checkDependencies():
    print('This subroutine uses the shell to check for available dependencies')
    print('It might not be exhaustive but should work for building and flashing.\n')
    
    try:
        summaries = ((command, runCommandInShell('which', [command])) for command in COMMAND_DEPENDENCIES)
    except FileNotFoundError:
        print("Can't use the command 'which' for some reason. Check that you have that installed from your package manager.")
        exit()
    except Exception:
        print("Something went wrong with the call, even though it appears that you have 'which' installed.")
        exit()

    successes = [summary for summary in summaries if not ('not found' in summary[1])]
    failures  = [summary for summary in summaries if     ('not found' in summary[1])]

    if len(successes) > 0:
        print('SUCCESSES:')
    else:
        print('\nNO DEPENDENCIES FOUND\n')
        return

    for item in successes:    
        command, outputs = item
        print('  '+command+': '+outputs[0].decode('utf-8'), end='')
    
    if len(failures) > 0:
        print('FAILURES:')
    else:
        print('\nAll depencies found!')

    for item in failures:
        command, outputs = item
        print(command, end=':')
        print('  '+outputs[1].decode('utf-8'), end='')

    print('')
    
    
def getDevicePath():
    shellOutput = runCommand(command='lsusb')
    devices = shellOutput.strip().split('\n') #parsing devices out of shell output
    
    for i, device in enumerate(devices):
        print(f'\t{i+1})\t {device}')
    print('')
    
    while True:
        try:
            bus = int(input('Select bus (e.g. 002): ').strip())
            break
        except KeyboardInterrupt:
            return
        except:
            print('Bad input.')

    while True:
        try:
            device = int(input('Select device (e.g. 001): ').strip())
            break
        except KeyboardInterrupt:
            return
        except:
            print('Bad input.')

    #padding number to have up to 2 leading zeroes: e.g.
    #     1,  13
    #   001, 013 
    
    bus    = str(bus)   .rjust(3, '0')
    device = str(device).rjust(3, '0')
    
    devicePath = f'/dev/bus/usb/{bus}/{device}'

    print("\nThe device's path is the following:")
    print(f'{devicePath}')
    print(f'The path {"exists!" if os.path.exists(devicePath) else "DOES NOT exist!"}\n')

    return devicePath


def makeFunction(arguments:list):
    def mf():
        shellOutput = runCommand(command='make', arguments=arguments)
        print(f'\n{shellOutput}\n')

    return mf

def pressEnterToContinue():
    input("\t<press Enter to continue>\n")

def wizard():
    print('You can press ctrl+c any time to abort flashing.')
    pressEnterToContinue()
    
    checkDependencies()
    pressEnterToContinue()

    printFlashConfigGuide()
    pressEnterToContinue()

    devices = ["UNO", "MEGA"]

    for device in devices:
        try:
            if 'y' in input(f"Do you want to flash your {device}? (y/n): "):
                pass
            else:
                continue
            
            print('Look for new (usb port based) device path of your {device}?')
            if 'y' in input(f"If you don't do this, use the flash config guide printed previously (y/n): "):
                path = getDevicePath()
            
            if 'y' in input(f"Do you want to use this path as a new default for {device}? (y/n): "):
                writeFlashPathToConfig(deviceModel=device, devicePath=path)

            if 'y' in input(f"Do you want to flash {device}? (y/n): "):
                flasherFunction = makeFunction([f'{device.lower()}flash'])
                flasherFunction()
        
        except KeyboardInterrupt:
            print(f"\nAborting flashing of your {device}")


choices = [
   #('1) Text will be displayed with the number', functionThatIsCalled)

    ('RUN SETUP WIZARD', wizard),
    ('\tCheck for existence of dependencies', checkDependencies),
    ('\tFind flash target boards', getDevicePath),
    ('\tWrite flash paths\n', writeFlashPathToConfig),
    
    ('FLASH BOTH BOARDS (with current firmware build)', makeFunction(['flashall'])),
    ('\tflash mega', makeFunction(['megaflash'])),
    ('\tflash uno\n', makeFunction(['unoflash'])),

    ('BUILD BOTH FIRMWARES (from local source code)', makeFunction(['buildall'])),
    ('\tbuild uno',  makeFunction(['mega'])),
    ('\tbuild mega\n', makeFunction(['uno'])),

    ("troubleshoot avrdude: ser_open(): can't open device \"/dev/tty....\": Permission denied", printFlashConfigGuide),
    ('read about required dependencies', listDependencies)
]

def printChoices():
    for i, choice in enumerate(choices):
        print(f' {i+1})'.ljust(5, ' ') + choice[0])


def menu():

    #myFunction = makeFunction(arguments=['test'])
    #print(f'shell output: {myFunction()}')

    while True:
        pressEnterToContinue()
        printChoices()

        try:
            choiceIndex = int(input(" >>"))
            print('')#prevents weird bugs from making the program hard to read
            function = choices[choiceIndex-1][1]
            
        except Exception:
            print("Wrong input.")

        try:
            function()
        except Exception as e:
            print('Something went wrong:')
            traceback.print_exception(*sys.exc_info())#print stack trace
        except KeyboardInterrupt as e:
            print('\nKeyboardInterrupts are supported, but be careful when flashing!')
            

def main():
    print('This script is designed to help you setup this custom build system.')
    print('It can also be used for building the project and flashing.')
    print('')
    
    while True:
        menu()


if __name__ == '__main__':
    try:
        main()
    except KeyboardInterrupt:
        print('\nThank you for using the program.')
        exit()