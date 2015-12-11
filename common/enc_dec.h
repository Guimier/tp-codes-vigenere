#include <string>

using namespace std;

struct Vigenere
{
	string key;

	Vigenere(string key)
	{
		// Modifying all characters other than uppercase : lowercase -> uppercase, other -> delete
		for(unsigned int i = 0; i < key.size(); ++i)
		{
			if(key[i] >= 'A' && key[i] <= 'Z')
				this->key += key[i] - 'A';
			else if(key[i] >= 'a' && key[i] <= 'z')
				this->key += key[i] - 'a';
		}
	}

	string encrypt(string text)
	{
		string out;

		// Modifying all characters other than uppercase : lowercase -> uppercase, other -> delete
		for(unsigned int i = 0; i < text.length(); ++i)
		{
			if(text[i] >= 'A' && text[i] <= 'Z')
				out += text[i];
			else if(text[i] >= 'a' && text[i] <= 'z')
				out += text[i] + 'A' - 'a';
		}

		for (size_t i = 0 ; i <out.size() ; ++i)
		{
			out[i] = ( ( out[i] - 'A' + key[i%key.size()] ) % 26 ) + 'A';
		}

		return out;
	}

	string decrypt(string text)
	{
		string out;

		for (size_t i = 0 ; i <text.size() ; ++i)
		{
			out += ( ( text[i] - 'A' - key[i%key.size()] + 26 ) % 26 ) + 'A';
		}

		return out;
	}
};
