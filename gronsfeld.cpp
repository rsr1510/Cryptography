#include <iostream>
#include <string>
using namespace std;

string vocabulary = " !\"#$%&'()*+,-./0123456789:<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";

string acipher(string key, string cipher) {
    string resulta = cipher;
    for (int i = 0; i < cipher.length(); i++) {
        int realOffset = vocabulary.find(cipher[i]) + (key[i % key.length()] - '0');
        if (realOffset < 0)
            realOffset += vocabulary.length();
        resulta[i] = vocabulary[realOffset];
    }
    return resulta;
}

string decipher(string key, string resulta) {
    string result = resulta;
    for (int i = 0; i < resulta.length(); i++) {
        int realOffset = vocabulary.find(resulta[i]) - (key[i % key.length()] - '0');
        if (realOffset < 0)
            realOffset += vocabulary.length();
        result[i] = vocabulary[realOffset];
    }
    return result;
}

int main() {
    string key, cipher;

    cout << "Enter the Gronsfeld key (digits only): ";
    getline(cin, key);

    cout << "Enter the plain text: ";
    getline(cin, cipher);
    string crypted = acipher(key, cipher);
    string decrypted = decipher(key, crypted);
    
    cout << "Encrypted Message: " << crypted << endl;
    cout << "Decrypted Message: " << decrypted << endl;

    return 0;
}
