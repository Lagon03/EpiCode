# EpiCode by the Team NotABarCode

## Major task to be done

- [x] QR-Code encoding (at least from version 1 to 20)
- [x] Better interface (either two completes ones or one which can switch)
- [ ] Epicode working

## Instruction / How to use
For debugging purpose of the encoding part follow the next instruction :

- ./main :
	- -d : stand for data, put after this option your data beside quote
  	- -c : stand for correction, put the level L(Low)/M(Medium)/Q(Quartile)/H(High)
  	- -v : stand for version, put any number from 1 to 40 (if v is inferior to the minimum version needed, v will be ignored)

Example of command :

- ./main                    	: will test encoding with HELLO WORLD and correction level Low
- ./main -d "HEllo BananA"  	: will test with HEllo Banana and correction Low
- ./main -d "Hello" -c Q    	: will test with Hello and correction Quartile
- ./main -d "Hello" -c Q -v 12  : will test with Hello and correction Quartile

## Tasks

### - Epicode -
#### - Epicode's specifications -

Epicode must be our own version of a QrCode, though it might be a simple modification it need to be different and only readable by us (our program), this way that said QrCode will truly be ours.

Todo :

- [x] Encoding Epicode
- [ ] Decoding Epicode

#### - Interface -

Todo :

- [x] Option to save Qrcode to specified path
- [x] Complete interface (both encoding / decoding finished)

### - Testing -

Todo :

- [ ] Test for the presentation
- [ ] Test of installation/package