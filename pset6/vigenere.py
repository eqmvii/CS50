# vigenere.py
# re-implementing vigenere's cyper in python

import cs50
import sys # to work with command line arguments

def main():
    print ('Arguments:' + str(sys.argv))
    if ((len(sys.argv) != 2) or (sys.argv[1].isalpha() == False)):
        print("You have failed to enter exactly one argument, ya dingus")
        sys.exit()
    key = sys.argv[1]
    key = key.lower()
    print("plaintext:")
    plainText = cs50.get_string()
    cypherText = ""
    letterIndex = 0 # to skip spaces
    
    for i in range(len(plainText)):
        alphaIndexKey = ord(key[letterIndex % len(key)]) - 97
        if (plainText[i].isalpha()):
            letterIndex += 1
            caseCorrector = 97
            if (plainText[i].isupper()):
                caseCorrector = 65
            curPlain = ord(plainText[i]) - caseCorrector
            newLetter = ((alphaIndexKey + curPlain ) % 26 )
            newLetter += caseCorrector
            cypherText += chr(newLetter)
        else:
            cypherText += plainText[i]
        
    print("cipherText: " + cypherText)
        
    
if __name__ == "__main__":
    main()
    