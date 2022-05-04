#!/bin/python

import subprocess
import os
import os.path

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
    \tThis list is probably not exhaustive and probably doesn't work on every distribution.
    \tIf you get errors about missing functions, you just have to find a way to install them, unfortunately.

    On arch the AUR packages 'arduino-avr-core' and 'make' should be enough.

    0) a linux environment, preferrably arch or derivative
        The reason is that avrdude -P needs an argument, and I don't know what it is on windows. 
        If you do, you can manually edit the DEVICE parameter of MEGA/makefile and UNO/makefile 
    
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

    print('Would you like to check for dependencies?')
    answer = input('(y/n): ')
    
    if 'y' in answer.lower():
        checkDependencies()

def runCommand(command:str, arguments=[]):
    command = [command] #list of words used
    command.extend(arguments)

    temp = subprocess.Popen(command, stdout = subprocess.PIPE)
    shellOutput = temp.communicate()[0].decode('utf-8') #ascii 128 to utf-8
    return shellOutput


def checkDependencies():
    print('This subroutine uses the shell to check for available dependencies')
    print('It might not be exhaustive but should work for building and flashing.')

    #command -v mycommand returns all available locations for a given command
    for command in COMMAND_DEPENDENCIES:
        shellOutput = runCommand(command='command', arguments=['-v', command])
        if len(shellOutput) > 5:
            print(shellOutput, end='')
        else:
            print(f"{command} wasn't found.")
    print('')
    
    
def getDevicePath():
    shellOutput = runCommand(command='lsusb')
    devices = shellOutput.strip().split('\n') #parsing devices out of shell output
    
    for i, device in enumerate(devices):
        print(f'\t{i+1})\t {device}')
    print('')
    
    while True:
        try:
            bus = int(input('Select bus (e.g. BUS 002): ').strip())
            break
        except KeyboardInterrupt:
            return
        except:
            print('Bad input.')

    while True:
        try:
            device = int(input('Select device (e.g. Device 001): ').strip())
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


def makeFunction(arguments:list):
    def mf():
        shellOutput = runCommand(command='make', arguments=arguments)
        print(f'\n{shellOutput}\n')

    return mf

choices = [
   #('1) Text will be displayed with the number', functionThatIsCalled)
    ('Read a list of dependencies (recommended)', listDependencies),
    ('Check for existence of dependencies', checkDependencies),
    ('Find flash target boards', getDevicePath),
    #('Print flash configuration guide', unimplemented),
    
    ('Flash both boards', makeFunction(['flashall'])),
    ('Flash MEGA with current firmware build', makeFunction(['megaflash'])),
    ('Flash UNO  with current firmware build', makeFunction(['unoflash'])),

    ('Flash both firmwares from source', makeFunction(['flashall'])),
    ('Build MEGA firmware  from source',  makeFunction(['mega'])),
    ('Build UNO  firmware  from source',  makeFunction(['uno'])),

    ('Build and flash both boards', makeFunction(['all'])),

    ('TEST MAKE', makeFunction(arguments=['test']))
]

def printChoices():
    for i, choice in enumerate(choices):
        print(f' {i+1})'.ljust(5, ' ') + choice[0])


def menu():

    #myFunction = makeFunction(arguments=['test'])
    #print(f'shell output: {myFunction()}')

    while True:
        printChoices()

        try:
            choiceIndex = int(input(" >>"))
            print('')#prevents weird bugs from making the program hard to read
            function = choices[choiceIndex-1][1]
            
        except Exception:
            print("Wrong input.")
            print(e)


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