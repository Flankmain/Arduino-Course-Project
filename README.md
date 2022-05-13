# Arduino-Course-Project

Made VERY hastily for the LUT University Embedded Systems Programming -course.

In its current state, it's a huge mess that shouldn't be copied by anyone.

For somewhat detailed help, you can run "python3 helper.py" in your terminal.

# How to compile and flash
Unfortunately I accidentally placed the output to .gitignore so manual compilation is required for now.
This project is catered towards Linux. Windows would probably work with proper PATH parameters, but I haven't tried it.

1) git clone this repository into some folder.

Basically, just running 'make all' in the root folder of the project should flash both boards, but you might have to edit the parameter DEVICE in both MEGA/makefile and UNO/makefile.
For Windows this parameter is something like COM3, for Linux it's something like dev/ttyACM0 or dev/ttyUSB0. The numbers can be different.

To see further details, go to the root folder of the project and run "python3 helper.py"
