#include <iostream>
#include <string>
#include <fstream>
#include "Header.h"
using namespace std;
char* inblock;
string * outblock;

int u16(int a, int b)
{
    int r;
    r = ((b * 256) + a);
    return r;
}

int u32(int a, int b, int c, int d)
{
    int r;
    r = ((b * 256) + a) + (((d * 256) + c) * 65536);
    return r;
}

int u_8(int a)
{
    int r;
    r = (a + 256) % 256;
    return r;
}

int readchk (int i, int offset, unsigned int chkbytes) {
    int r;
    switch (chkbytes) {
    case 4:
        return u32(u_8(inblock[i + offset]), u_8(inblock[i + offset + 1]), u_8(inblock[i + offset + 2]), u_8(inblock[i + offset + 3]));
        break;
    case 2:
        return u16(u_8(inblock[i + offset]), u_8(inblock[i + offset + 1]));
        break;
    default:
        return u_8(inblock[i + offset]);
        break;
    }
}

void seekname (int& length, int& offset, char* block, int size, char name[4], int wordlength) {
    int i = 0;
    int e = 0;
    int matching = 0;
    while (i < size) {
        if (matching == wordlength) {
            offset = i + 7;
            length = u32(u_8(inblock[i + 3]), u_8(inblock[i + 4]), u_8(inblock[i + 5]), u_8(inblock[i + 6]));
        }
        matching = 0;
        while (e < wordlength) {
            matching += inblock[i + e] == name[e];
            e++;
        }
        e = 0;
        i++;
    }
}

int main() {
    streampos size;
    char unitchar[] = { 'U', 'N', 'I', 'T' };
    char dim[] = { 'D', 'I', 'M' };
    char doodad[] = { 'D', 'D', '2' };
    char tile[] = { 'T', 'I', 'L', 'E' };
    int length;
    int offset;
    unsigned int mapX;
    unsigned int mapY;
    
    ifstream file("input.chk", ios::in | ios::binary | ios::ate);
    if (file.is_open())
    {
        size = file.tellg();
        inblock = new char[size];
        outblock = new string[size];
        file.seekg(0, ios::beg);
        file.read(inblock, size);
        file.close();
        int i = 0;
        int e = 1;
        seekname(length, offset, inblock, size, dim, 3);
        mapX = readchk(0, offset, 2);
        mapY = readchk(2, offset, 2);
        cout << mapX << "\n";
        cout << mapY << "\n";
        seekname(length, offset, inblock, size, unitchar, 4);
        cout << length << "\n";
        while (i < length) {
            //ID
            outblock[(e * 9) - 9] = readchk(i, offset, 4);
            i += 4;
            //X
            outblock[(e*9) - 8] = readchk(i, offset, 2) - (mapX * 16);
            i += 2;
            //Y
            outblock[(e * 9) - 7] = (readchk(i, offset, 2) - (mapY * 16)) * -1;
            i += 2;
            //Unit
            outblock[(e * 9) - 6] = readchk(i, offset, 4);
            i += 6;
            //Owner
            if (u_8(inblock[i + offset]) == 18) {
                outblock[(e * 9) - 3] = -8;
            }
            else
            {
                outblock[(e * 9) - 3] = u_8(inblock[i + offset]);
            }
            i += 22;
            e++;
            if (i > 3900) {
                length = 0;
            }
        }
            //1 : x
            //2 : y
            //3 : type
            //4 : moveable
            //5 : constructiontime
            //6 : player owner
            //7 : health
            //8 : armor
        e = e * 9;
        i = 0;
        ofstream myfile;
        myfile.open("UnitData.txt");
        while (i < e) {
            myfile << outblock[i] << "\n";
            i++;
        }
        myfile.close();
        i = 0;
        e = 1;
        seekname(length, offset, inblock, size, doodad, 3);
        cout << length << "\n";
        while (i < length) {
            //Unit
            outblock[(e * 5) - 5] = readchk(i, offset, 2);
            i += 2;
            //X
            outblock[(e * 5) - 4] = readchk(i, offset, 2) - (mapX * 16);
            i += 2;
            //Y
            outblock[(e * 5) - 3] = (readchk(i, offset, 2) - (mapY * 16)) * -1;
            i += 2;
            //Owner
            if (u_8(inblock[i + offset]) == 18) {
                outblock[(e * 9) - 2] = -8;
            }
            else
            {
                outblock[(e * 5) - 2] = u_8(inblock[i + offset]);
            }
            i += 1;
            //Enabled
            outblock[(e * 5) - 1] = u_8(inblock[i + offset]);
            i += 1;
            e++;
            if (i > 3900) {
                length = 0;
            }
        }
        e = e * 5;
        i = 0;
        myfile.open("Doodads.txt");
        while (i < e) {
            myfile << outblock[i] << "\n";
            i++;
        }
        myfile.close();
        i = 0;
        e = 1;
        seekname(length, offset, inblock, size, tile, 4);
        cout << length << "\n";
        while (i < length) {
            //Tile
            outblock[(e * 1) - 1] = "leeE";
            i += 16;
            if (( i % mapX ) == 0) {
                cout << "switch at " << e << "\n";
                i += (mapX * 1);
            }
            e++;
            if (e > 3900) {
                length = 0;
            }
        }
        i = 0;
        myfile.open("isometric.txt");
        while (i < e) {
            myfile << outblock[i] << "\n";
            i++;
        }
        myfile.close();
        delete[] inblock;
        delete[] outblock;
    }
    else cout << "Unable to open file";
    return 0;
}
