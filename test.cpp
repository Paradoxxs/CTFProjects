#include <iostream>

using namespace std;

string encryptDecrypt(string toEncrypt) {
    char key = 'n'; //Any char will work
    string output = toEncrypt;
    
    for (int i = 0; i < toEncrypt.size(); i++)
        output[i] = toEncrypt[i] ^ key;
    
    return output;
}

int main(int argc, const char * argv[])
{
    string encrypted = encryptDecrypt("HTB=bth;EXEC=cexe;ENCODE=ec;EXIT=tixe;");
    cout << "Encrypted:" << encrypted << "\n";
    
    string decrypted = encryptDecrypt(encrypted);
    cout << "Decrypted:" << decrypted << "\n";
    
    return 0;
}

