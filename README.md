# EpiCode by the Team NotABarCode

This is the project we have done during the 4th semester at Epita. The project's aim was to be able to encode QrCodes and decode QrCodes, but aswell to create a new system of colorful QrCodes, which we have named Epicodes.

The Team nOtAbARcOdE :
	- Leader Stanislas Sokolov (Lagon03)
	- Lucas Martin (TNGitUser)
	- Jack Chouchani (jackchouchani)
	- Youness Suleiman (MachoMouse)

## Install

- This is the terminal version : Clone the git, enter the repository and write the following command.
```shell
cp install/MakefileT Makefile
```
- This is the version with the interface : Same as before but write this command instead. Uses GTK+.
```shell
cp install/MakefileI Makefile
``` 
- If you want to switch between Terminal use and Interface use, you just to make clean and use the commands above. 

## Instruction / How to use in a Terminal.
- ./encode OPTIONS:
  - -d : Stands for Data, write after the option the message you want to encode in between quotes
  - -c : Stands for Error Correction, write the letter corresponding to the level of correction you want, L(Low)|M(Medium)|Q(Quartile)|H(High)
  - -v : Stands for Version, write after the option the desired version (1 to 40)
  - -x : If you wish not to display your QrCode or EpiCode when encoded
  - -n : Stands for name, write after the option the name of your QrCode (It will be saved under that name)
  - -o : Saves the Code at desired location, the default location is output in the Repository
  - -t : Encodes the message in a Epicode
  - -h : Helps

  - ./decode File Name OPTIONS: By default, decodes A Qrcode from a file.
  	- -e : Decodes an Epicode instead of a QrCode
  	- -h : Helps

## Instruction / How to use with the interface

Should be pretty easy to understand.

## Details

Works in most cases, but sometimes doesn't work and horribly crashes.
And the error correction is local, thus it only corrects the QrCodes that are encoded by this program.
You shouldn't try to decode big QrCodes with this... ABORT
Otherwise we are pretty happy of the outcome, it was long and hard. but in the end we prevailed ;) 