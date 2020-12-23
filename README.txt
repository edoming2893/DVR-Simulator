Eduardo Dominguez-Arroyo
CS 4310 Project #1
Program Language: C++

***************** Files Needed **************

To run the program you need the following files in the
same directory.

Driver.cpp
DVRalgo.hpp
toplogyfile.txt (the topology files being used)


***************** Compiling the Program **************
g++ Driver.cpp

***************** Running the Out File ***************
./a.out


***************** Description ************************
Once the program out file runs you will be promoted to give the
topology file name that will be used to run the simulation in the following format: filename.txt

Then the user will be promoted through the terminal if they would like to run a packet routing simulation. In order to do this the user must now the source/sending node number and the recipient/receiving packet node number. If the user wants to perform a packet routing simulation they will be prompted for those two node numbers:
	Sending/Sender Node Number
	Recipient/Receiving Node Number

Otherwise the simulation will be done. An output text-file will be created and placed in the same directory as the the Drive and DVRalgo file are in. The output text file will have the initial routing tables, final converged routing tables, and if the user wanted routing packet simulation the information for routing the packet.

Once this is done for a topology file the user will be prompted again in the terminal if they want to run another topology file. If they choose YES the above will repeat if choose NO then program will end.
