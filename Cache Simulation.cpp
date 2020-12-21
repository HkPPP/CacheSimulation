/*
Name: Hank Pham, Brennan Miller-Klugman
Professor: Marisha Rawlins
Course: Computer Architecture

Direct Mapped Cache and Two-way Set Associative Cache Simulator
*/

#include <iostream>
#include <stdio.h>
#include <string>
#include <fstream>
#include <cmath>

/*
**** Direct Mapped Cache *****
Offset: 
64B block = 2^6 
=> offset = 6 bits

Index: (which block to use)
#blocks = cache size/ block size = 512/64 = 2^9/2^6 = 2^3 = 8 blocks
=> Index = 3 bits

Tag: 16 - 6 - 3 = 7 bits

|---Tag---|-Index-|Off|
15        8       5   0
*/

/*
**** 2-way Set Associative  Cache ****
Offset: 
64B block = 2^6 
=> offset = 6 bits

Index: (which set to use)
#blocks = cache size/ block size = 512/(2 x 64) = 2^9/(2^1 x 2^6) = 2^2 = 4 per set
=> Index = 2 bits

Tag: 16 - 6 - 2 = 8 bits

|---Tag---|-Index-|Off|
15        7       5   0
*/

std::string HexToBinary(std::string &addressInHex);
std::string TableOfHexToBinary(char HexDigit);

std::string getOffset(std::string &BinaryAddr, int LSB, int MSB);
std::string getTag(std::string &BinaryAddr, int LSB, int MSB);
std::string getIndex(std::string &BinaryAddr, int LSB, int MSB);

struct cache
{
    std::string Offset, Index, Tag;
    int value = 0;
    bool valid = false;
};

int main()
{
    std::string memLocation, binaryAddr, tag, stringIndex;
    int index, value, hitCount = 0, missCount = 0;
    cache directMap[8];
    cache twoWayMap[8];
    int leastUsed[4];

    for (int i = 0; i < 8; i++)
    {
        switch (i)
        {
        case 0:
        case 1:
            twoWayMap[i].Index = "00";
            break;
        case 3:
        case 4:
            twoWayMap[i].Index = "01";
            break;
        case 5:
        case 6:
            twoWayMap[i].Index = "10";
            break;
        case 7:
        case 8:
            twoWayMap[i].Index = "11";
            break;
        }
    }

    std::ifstream inputFile("input.txt"); //declaration of ifstream

    std::cout << "Direct Mapped:\n\n";

    while (inputFile >> memLocation)
    {
        //read memory location into memLocation if the entry is valid
        inputFile >> value; //read memory location value into memory variable
        binaryAddr = HexToBinary(memLocation);
        tag = getTag(binaryAddr, 8, 15);
        stringIndex = getIndex(binaryAddr, 5, 8);

        if (stringIndex == "000")
        { //turn binary index into decimal
            index = 0;
        }
        else if (stringIndex == "001")
        {
            index = 1;
        }
        else if (stringIndex == "010")
        {
            index = 2;
        }
        else if (stringIndex == "011")
        {
            index = 3;
        }
        else if (stringIndex == "100")
        {
            index = 4;
        }
        else if (stringIndex == "101")
        {
            index = 5;
        }
        else if (stringIndex == "110")
        {
            index = 6;
        }
        else if (stringIndex == "111")
        {
            index = 7;
        }

        // Check valid bit first
        if (!directMap[index].valid)
        {
            directMap[index].valid = true;
            directMap[index].Tag = tag;
            directMap[index].value = value;
            std::cout << memLocation << ": MISS" << std::endl;
            missCount++;
        }
        else
        { // otherwise, compare tags
            if (tag == directMap[index].Tag)
            {
                std::cout << memLocation << ": HIT" << std::endl;
                hitCount++;
            }
            else
            { // replace if tag not equal
                directMap[index].Tag = tag;
                directMap[index].value = value;
                std::cout << memLocation << ": MISS" << std::endl;
                missCount++;
            }
        }
    }

    std::cout << "\nDirect Mapped Cache Final Contents:\n";
    std::cout << "Index\tV\tTag\tData\n";
    std::cout << "000\t" << directMap[0].valid << "\t" << directMap[0].Tag << "\t" << directMap[0].value << std::endl;
    std::cout << "001\t" << directMap[1].valid << "\t" << directMap[1].Tag << "\t" << directMap[1].value << std::endl;
    std::cout << "010\t" << directMap[2].valid << "\t" << directMap[2].Tag << "\t" << directMap[2].value << std::endl;
    std::cout << "011\t" << directMap[3].valid << "\t" << directMap[3].Tag << "\t" << directMap[3].value << std::endl;
    std::cout << "100\t" << directMap[4].valid << "\t" << directMap[4].Tag << "\t" << directMap[4].value << std::endl;
    std::cout << "101\t" << directMap[5].valid << "\t" << directMap[5].Tag << "\t" << directMap[5].value << std::endl;
    std::cout << "110\t" << directMap[6].valid << "\t" << directMap[6].Tag << "\t" << directMap[6].value << std::endl;
    std::cout << "111\t" << directMap[7].valid << "\t" << directMap[7].Tag << "\t" << directMap[7].value << std::endl;

    std::cout << std::endl
              << "Hits: " << hitCount << std::endl
              << "Misses: " << missCount << std::endl;

    index = 0;    //reset the index
    hitCount = 0; //reset hit and miss counts
    missCount = 0;
    std::cout << "\n\n2 Way Set Associative:\n\n";

    std::ifstream twoWayText("input.txt"); //declaration of ifstream
    while (twoWayText >> memLocation)
    {
        twoWayText >> value; //read memory location value into memory variable
        binaryAddr = HexToBinary(memLocation);
        tag = getTag(binaryAddr, 7, 15);
        stringIndex = getIndex(binaryAddr, 5, 7);

        if (stringIndex == "00")
        { //turn binary index into decimal
            index = 0;
        }
        else if (stringIndex == "01")
        {
            index = 1;
        }
        else if (stringIndex == "10")
        {
            index = 2;
        }
        else if (stringIndex == "11")
        {
            index = 3;
        }

        if ((!twoWayMap[index * 2].valid) && (!twoWayMap[(index * 2) + 1].valid)) //if index is 3, check if twoWayMap 6 & 7 not valid
        {
            twoWayMap[index * 2].Tag = tag; //set index*2 in the twoWayMap Cache tag, valid, and valid bit
            twoWayMap[index * 2].value = value;
            twoWayMap[index * 2].valid = true;

            leastUsed[index] = 1; //if index is 3, twoWayMap[7] is stored in the LRU

            std::cout << memLocation << ": MISS" << std::endl;
            missCount++;
        }
        else if ((twoWayMap[index * 2].Tag == tag) || (twoWayMap[(index * 2) + 1].Tag == tag)) //check if either of the cache's have a matching tag
        {
            if (index % 2 == 0)
            { // if index is 0, 2, 4, 6, the least used is the next index 1, 3, 5, 7
                leastUsed[index] = 1;
            }
            else
            { // otherwise, the least used index is 0, 2, 4, or 6
                leastUsed[index] = 0;
            }

            std::cout << memLocation << ": HIT" << std::endl;
            hitCount++;
        }
        else
        {
            std::cout << memLocation << ": MISS" << std::endl;
            missCount++;

            // Replace the least used tag with the new tag
            twoWayMap[(index * 2) + leastUsed[index]].Tag = tag;

            if (leastUsed[index] == 0)
            {
                leastUsed[index] = 1;
            }
            else
            {
                leastUsed[index] = 0;
            }
        }
    }

    std::cout << "\n2-Way Set Associative - Final Contents:\n";
    std::cout << "Index\tV\tTag\tData\n";
    std::cout << "00\t" << directMap[0].valid << "\t" << directMap[0].Tag << "\t" << directMap[0].value << std::endl;
    std::cout << "00\t" << directMap[1].valid << "\t" << directMap[1].Tag << "\t" << directMap[1].value << std::endl;
    std::cout << "01\t" << directMap[2].valid << "\t" << directMap[2].Tag << "\t" << directMap[2].value << std::endl;
    std::cout << "01\t" << directMap[3].valid << "\t" << directMap[3].Tag << "\t" << directMap[3].value << std::endl;
    std::cout << "10\t" << directMap[4].valid << "\t" << directMap[4].Tag << "\t" << directMap[4].value << std::endl;
    std::cout << "10\t" << directMap[5].valid << "\t" << directMap[5].Tag << "\t" << directMap[5].value << std::endl;
    std::cout << "11\t" << directMap[6].valid << "\t" << directMap[6].Tag << "\t" << directMap[6].value << std::endl;
    std::cout << "11\t" << directMap[7].valid << "\t" << directMap[7].Tag << "\t" << directMap[7].value << std::endl;

    std::cout << std::endl
              << "Hits: " << hitCount << std::endl
              << "Misses: " << missCount << std::endl;
}

std::string HexToBinary(std::string &addressInHex)
{

    std::string Binary;

    /*
   Find length of the Hex string
   E.g. 0xABC will return 5 
   */
    int lengthOfHexString = addressInHex.length();

    /*
   The algorithm below converts a Hex string into a Binary string
   even in the case of 0x presenting in the string
   */
    if (addressInHex[0] == '0' && addressInHex[1] == 'x')
    {
        // Start at the third place and convert hex to binary
        for (int i = 2; i < lengthOfHexString; i++)
        {
            Binary = Binary + TableOfHexToBinary(addressInHex[i]);
        }
    }
    else
    { // else, start at the beginning
        for (int i = 0; i < lengthOfHexString; i++)
        {
            Binary = Binary + TableOfHexToBinary(addressInHex[i]);
        }
    }

    return Binary;
}

/*
Convert each Hex digit to a Binary string
Reference: https://www.geeksforgeeks.org/program-to-convert-hexadecimal-number-to-binary/ 
*/
std::string TableOfHexToBinary(char HexDigit)
{
    switch (HexDigit)
    {
    case '0':
        return "0000";
        break;
    case '1':
        return "0001";
        break;
    case '2':
        return "0010";
        break;
    case '3':
        return "0011";
        break;
    case '4':
        return "0100";
        break;
    case '5':
        return "0101";
        break;
    case '6':
        return "0110";
        break;
    case '7':
        return "0111";
        break;
    case '8':
        return "1000";
        break;
    case '9':
        return "1001";
        break;
    case 'A':
    case 'a':
        return "1010";
        break;
    case 'B':
    case 'b':
        return "1011";
        break;
    case 'C':
    case 'c':
        return "1100";
        break;
    case 'D':
    case 'd':
        return "1101";
        break;
    case 'E':
    case 'e':
        return "1110";
        break;
    case 'F':
    case 'f':
        return "1111";
        break;
    default:
        return 0;
        std::cout << "\nInvalid hexadecimal digit " << HexDigit << std::endl;
    }
}

std::string getOffset(std::string &BinaryAddr, int LSB, int MSB)
{
    return BinaryAddr.substr(15 - MSB, MSB - LSB + 1);
}
std::string getIndex(std::string &BinaryAddr, int LSB, int MSB)
{
    return BinaryAddr.substr(15 - MSB, MSB - LSB);
}
std::string getTag(std::string &BinaryAddr, int LSB, int MSB)
{
    return BinaryAddr.substr(15 - MSB, MSB - LSB);
}