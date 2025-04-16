#include <bits/stdc++.h>
using namespace std;

string encryptMessage(string msg, int a, int b)
{
	string cipher = ""; 
	for (int i = 0; i < msg.length(); i++)
	{
		if(msg[i] != ' ') 
			cipher += (char) ((((a * (msg[i] - 'A')) + b) % 26) + 'A');
		else
			cipher += msg[i];	 
	}
	return cipher;
}

string decryptCipher(string cipher, int a, int b)
{
	string msg = "";
	int a_inv = 0;
	int flag = 0;
	
	for (int i = 0; i < 26; i++)
	{
		if ((a * i) % 26 == 1)
		{ 
			a_inv = i;
			break;
		}
	}

	for (int i = 0; i < cipher.length(); i++)
	{
		if(cipher[i] != ' ')
		{
			int decrypted = a_inv * ((cipher[i] - 'A' - b + 26) % 26);
			msg += (char)(decrypted % 26 + 'A');
		}
		else
			msg += cipher[i]; 
	}

	return msg;
}

int main()
{
	string msg;
	int a, b;

	cout << "Enter a message in UPPERCASE (no special characters): ";
	getline(cin, msg);

	cout << "Enter key 'a' (must be coprime with 26): ";
	cin >> a;

	cout << "Enter key 'b': ";
	cin >> b;

	// Validate if 'a' is coprime with 26
	auto gcd = [](int x, int y) {
		while (y != 0) {
			int temp = y;
			y = x % y;
			x = temp;
		}
		return x;
	};

	if (gcd(a, 26) != 1) {
		cout << "'a' must be coprime with 26. Encryption won't work properly.\n";
		return 1;
	}

	string cipherText = encryptMessage(msg, a, b);
	cout << "Encrypted Message is : " << cipherText << endl;
	cout << "Decrypted Message is : " << decryptCipher(cipherText, a, b) << endl;

	return 0;
}