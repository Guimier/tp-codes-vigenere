#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <array>

using namespace std;
 
typedef array<pair<char, double>, 26> FreqArray;
 
class VigenereCryptanalysis
{
private:
  array<double, 26> targets;
  array<double, 26> sortedTargets;

  // TO COMPLETE
 
public:
  VigenereCryptanalysis(const array<double, 26>& targetFreqs) 
  {
    targets = targetFreqs;
    sortedTargets = targets;
    sort(sortedTargets.begin(), sortedTargets.end());
  }
 
  pair<string, string> analyze(string input) 
  {
    string key = "ISIMA PERHAPS";
    string result = "I CAN NOT DECRYPT THIS TEXT FOR NOW :-)" + input;
 
    return make_pair(result, key);
  }
};
 
int main() 
{
  string input = "YOU HAVE TO COPY THE CIPHERTEXT FROM THE ATTACHED FILES OR FROM YOUR CIPHER ALGORITHM";
 
  array<double, 26> english = {
    0.08167, 0.01492, 0.02782, 0.04253, 0.12702, 0.02228,
    0.02015, 0.06094, 0.06966, 0.00153, 0.00772, 0.04025,
    0.02406, 0.06749, 0.07507, 0.01929, 0.00095, 0.05987,
    0.06327, 0.09056, 0.02758, 0.00978, 0.02360, 0.00150,
    0.01974, 0.00074};

  array<double, 26> french = {
    0.0811,  0.0081,  0.0338,  0.0428,  0.1769,  0.0113,
    0.0119,  0.0074,  0.0724,  0.0018,  0.0002,  0.0599, 
    0.0229,  0.0768,  0.0520,  0.0292,  0.0083,  0.0643,
    0.0887,  0.0744,  0.0523,  0.0128,  0.0006,  0.0053,
    0.0026,  0.0012};	
 
  VigenereCryptanalysis vc_en(english);
  pair<string, string> output_en = vc_en.analyze(input);
 
  cout << "Key: "  << output_en.second << endl;
  cout << "Text: " << output_en.first << endl;

  VigenereCryptanalysis vc_fr(french);
  pair<string, string> output_fr = vc_fr.analyze(input);
 
  cout << "Key: "  << output_fr.second << endl;
  cout << "Text: " << output_fr.first << endl;

}
