#include <iostream>
#include <string>
using namespace std;

class Vigenere
{
public:
  string key;
 
  Vigenere(string key)
  {
    // Modifying all characters other than uppercase : lowercase -> uppercase, other -> delete
    for(unsigned int i = 0; i < key.size(); ++i)
    {
      if(key[i] >= 'A' && key[i] <= 'Z')
        this->key += key[i];
      else if(key[i] >= 'a' && key[i] <= 'z')
        this->key += key[i] + 'A' - 'a';
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

    // ADD THE VIGENERE CRYPTION 	
 
    return out;
  }
 
  string decrypt(string text)
  {
    string out;

    // TO REMOVE
    out = text;

    // ADD THE VIGENERE DECRYPTION 
    return out;
  }
};

//////////////////////////////////////////////////////////////////
//                             MAIN                             //
//////////////////////////////////////////////////////////////////

int main()
{
  Vigenere cipher("MYKEY");
 
  string original_en  = "Kerckhoffs's principle - A cryptosystem should be secure even if everything about the system, except the key, is public knowledge.";
  string encrypted_en = cipher.encrypt(original_en);
  string decrypted_en = cipher.decrypt(encrypted_en);
 
  cout << original_en << endl;
  cout << "Encrypted: " << encrypted_en << endl;
  cout << "Decrypted: " << decrypted_en << endl;

  string original_fr  = "Principe de Kerckhoffs - Toute la securite d'un systeme cryptographique doit reposer sur la clef et pas sur le systeme lui meme.";

  string encrypted_fr = cipher.encrypt(original_fr);
  string decrypted_fr = cipher.decrypt(encrypted_fr);
 
  cout << original_fr << endl;
  cout << "Encrypted: " << encrypted_fr << endl;
  cout << "Decrypted: " << decrypted_fr << endl;

}

