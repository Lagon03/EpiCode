# Cypher - Encoding / Decoding
We will use this branch to develop the encoding and decoding process.

To create the Qr-Code we do the following :
1. Data Analysis
2. Data Encoding
3. Error Correction Coding
4. Structure Final Message
5. Module Placement in Matrix
6. Data Masking
7. Format and Version Information

## Encoding

In the first part we need to find which mode is the most efficient one.
Modes which are appropriate for us :
- Numeric mode
- Alphanumeric mode
- Byte mode
- Kanji mode

#### The Numeric mode
Is designed for decimal digits from 0 to 9

#### Alphanumeric mode
Is designed for decimal 0 through 9 and uppercase characters (not lowercase).
It also contain the space character and $ % * + - . / :

#### Byte mode
By default, is for characters from the ISO-8859-1 character set.
However, some QR code scanners can automatically detect if UTF-8 is used in byte mode instead.

#### Kanji mode
Kanji mode is for double-byte characters from the Shift JIS character set. 
While UTF-8 can encode Kanji characters, it must use three or four bytes to do so.
Shift JIS, on the other hand, uses just two bytes to encode each Kanji character, so Kanji mode compresses Kanji characters more efficiently.
If the entire input string consists of characters in the double-byte range of Shift JIS, use Kanji mode.
It is also possible to use multiple modes within the same QR code, as described later on this page.

### To choose the most efficient mode
We have to check the following condition :
1. If the input only consist of decimal digits, use **numeric mode**.
2. If numeric isn't applicable and if all the characters of the input can be found in the alphanumeric, use **alphanumeric**. **REMARK** Alphanumeric only contain uppercase not lowercase.
3. If Alphanumeric is not applicable and the character can be encoded in ISO 8859-1 then use **byte mode**.
4. If all of the character are in the Shift JIS character set, use Kanji mode.

In our project we will only use one mode by Qr-Code.

### How to derive the number of usable data bits

Essentially the total number of data modules for a chosen symbol version would be the total symbol area less any function pattern modules and format/version information modules:

DataModules = Rows × Columns − ( FinderModules + AlignmentModules + TimingPatternModules ) − ( FormatInformationModules + VersionInformationModules )

The values of these parameters are constants defined per symbol version.

Some of these data modules are then allocated to error correction purposes as defined by the chosen error correction level. What remains is the usable data capacity of the symbol found by treating each remaining module as a single bit:

UsableDataBits = DataModules − ErrorCorrectionBits

How to derive the character capacity for each mode

Encoding of the input data begins with a 4-bit mode indicator followed by a character count value whose length depends on the version of the symbol and the mode. Then the data is encoded according to the rules for the particular mode resulting in the following data compaction:

    Numeric Groups of 3 characters into 10 bits; 2 remainders into 7 bits; 1 remainder into 4 bits.
    Alphanumeric Groups of 2 characters into 11 bits; 1 remainder into 6 bits.
    Byte Each character into 8 bits.
    Kanji Each wide-character into 13 bits.

Although it does not affect the symbol capacity, for completeness I'll point out that a 4-bit terminator pattern is appended which may be truncated or omitted if there is insufficient capacity in the symbol. Any remaining data bits are then filled with a padding pattern.

**Worked Example**

Given a version 40 symbol with error correction level L.

The size is 177×177 = 31329 modules

There are three 8×8 finder patterns (192 modules), forty six 5×5 alignment patterns (1150 modules) and 272 timing modules, totalling 1614 function pattern modules.

There are also 31 format information modules and 36 version information modules, totalling 67 modules.

DataModules = 31329 − 1614 − 67 = 29648

Error correction level L dictates that there shall be 750 8-bit error correction codewords (6000 bits):

UsableDataBits = 29648 − 6000 = 23648

The character count lengths for a version 40 symbol are specified as follows:

 *   Numeric 14 bits.
 *   Alphanumeric 13 bits.
 *   Byte 16 bits.
 *   Kanji 12 bits.

Consider alphanumeric encoding. From the derived UsableDataBits figure of 23648 bits available we take 4 bits for the mode indicator and 13 bits for the character count leaving just 23631 for the actual alphanumeric data (and truncatable terminator and padding.)

You quoted 4296 as the alphanumeric capacity of a version 40-L QR Code symbol. Now 4296 alphanumeric characters becomes exactly 2148 groups of two characters each converted to 11 bits, producing 23628 data bits which is just inside our symbol capacity. However 4297 characters would produce 2148 groups with one remainder character that would be encoded into 6 bits, which produces 23628 + 6 bits overall – exceeding the 23631 bits available. So 4296 characters is clearly the correct alphanumeric capacity of a type 40-L QR Code.

Similarly for numeric encoding we have 23648−4−14 = 23630 bits available. Your quoted 7089 is exactly 2363 groups of three characters each converted to 10 bits, producing 23630 bits – exactly filling the bits available. Clearly any further characters would not fit so we have found our limit.

Caveat

Whilst the character capacity can be derived using the above procedure in practise QR Code permits encoding the input using multiple modes within a single symbol and a decent QR Code generator will switch between modes as often as necessary to optimise the overall data density. This makes the whole business of considering the capacity limits much less useful for open applications since they only describe the pathological case.

## Module Placement in Matrix

### Overview of Functions Patterns

<img src="https://github.com/Lagon03/EpiCode/tree/cypher/resources/function-patterns.png"/>

- The finder patterns are the three blocks in the corners of the QR code at the top left, top right, and bottom left.

- The separators are areas of whitespace beside the finder patterns.

- The alignment patterns are similar to finder patterns, but smaller, and are placed throughout the code. They are used in versions 2 and larger, and their positions depend on the QR code version.

- The timing patterns are dotted lines that connect the finder patterns.

- The dark module is a single black module that is always placed beside the bottom left finder pattern.

- The sections below explain in greater detail how to position the function patterns.

[Check the images in resources for greater introspection](https://github.com/Lagon03/EpiCode/tree/cypher/resources)

## Localization and Segmentation of 2D High Capacity Color Barcodes

[Nice PDF about that! ](http://citeseerx.ist.psu.edu/viewdoc/download?doi=10.1.1.218.125&rep=rep1&type=pdf "COOL PDF") it is focused on color barcodes.

[Strictly for QRCodes](https://thesai.org/Downloads/Volume8No4/Paper_33-QR_%20Code_Recognition_based_on_Principal_Components.pdf) what we will be using for now