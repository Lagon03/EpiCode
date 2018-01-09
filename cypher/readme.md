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
