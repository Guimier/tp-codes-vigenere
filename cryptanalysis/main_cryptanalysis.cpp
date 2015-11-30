#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <array>
#include <queue>

using namespace std;
 
typedef array<pair<char, double>, 26> FreqArray;

template <typename T>
class Serie
{
	private:
		string& v;
		int freq;
		size_t offset;
	
	public:
		Serie( string &v, int freq, size_t offset ):
			v( v ), freq( freq ), offset( offset )
		{}
		
		inline size_t size() const { return ( v.size() - offset ) / freq; }
		
		inline T operator [] ( size_t i ) const { return v[ offset + i * freq ]; }
		inline T& operator [] ( size_t i ) { return v[ offset + i * freq ]; }
	
};

struct CharSequenceStatisticsComputer {

	double indexOfCoincidence( Serie<char> chars ) {
		size_t apparitions[26] = {0};

		for ( size_t i = 0; i < chars.size(); ++i ) {
			// cout << chars[i];
			++( apparitions[ chars[i] - 'A' ] );
		}

		unsigned long int sum = 0;

		for ( int i = 0; i < 26; ++i ) {
			//cout << char( i + 'A' ) << ": " << apparitions[i] << "; ";
			sum += apparitions[i] * ( apparitions[i] - 1 );
		}
		//cout << endl;

		return sum / double( chars.size() * ( chars.size() - 1 ) );
	}
	
	double averageIndexOfCoincidence( const string list, int freq ) {
		double sum = 0;
		for ( int i = 0; i < freq; ++i ) {
			sum += indexOfCoincidence( Serie<char>( list, freq, i ) );
		}
		
		return sum / freq;
	}

};

string normalizeString( const string in ) {
	string out;
	
	for ( size_t i = 0; i < in.size(); ++i )
	{
		if ( in[i] >= 'A' && in[i] <= 'Z' ) {
			out += in[i];
		} else if ( in[i] >= 'a' && in[i] <= 'z' ) {
			out += in[i] + 'A' - 'a';
		}
	}
	
	return out;
}

struct ICSort {
	typedef pair<int,double> couple;
	bool operator () ( const couple& a, const couple& b ) {
		return a.second < b.second;
	}
};

class VigenereCryptanalysis: CharSequenceStatisticsComputer
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

		pair<string, string> analyze( string input ) 
		{
			input = normalizeString( input );
			string key = "ISIMA PERHAPS";
			string result = "I CAN NOT DECRYPT THIS TEXT FOR NOW :-)" + input;

			priority_queue<ICSort::couple, vector<ICSort::couple>, ICSort> pq;
			for ( int f = 1; f <= 10; ++ f ) {
				pq.push( make_pair( f, averageIndexOfCoincidence( input, f ) ) );
				cout << "f = " << f << "; ic = " << averageIndexOfCoincidence( input, f ) << endl;
			}

			cout << pq.top().first << endl;

			return make_pair(result, key);
		}
};
 
int main() 
{
	string input = "zbpuevpuqsdlzgllksousvpasfpddggaqwptdgptzweemqzrdjtddefekeferdprrcyndgluaowcnbptzzzrbvpssfpashpncotemhaeqrferdlrlwwertlussfikgoeuswotfdgqsyasrlnrzppdhtticfrciwurhcezrpmhtpuwiyenamrdbzyzwelzucamrptzqseqcfgdrfrhrpatsepzgfnaffisbpvblisrplzgnemswaqoxpdseehbeeksdptdttqsdddgxurwnidbdddplncsd";

	array<double, 26> english = {
		0.08167, 0.01492, 0.02782, 0.04253, 0.12702, 0.02228,
		0.02015, 0.06094, 0.06966, 0.00153, 0.00772, 0.04025,
		0.02406, 0.06749, 0.07507, 0.01929, 0.00095, 0.05987,
		0.06327, 0.09056, 0.02758, 0.00978, 0.02360, 0.00150,
		0.01974, 0.00074
	};

	array<double, 26> french = {
		0.0811,  0.0081,  0.0338,  0.0428,  0.1769,  0.0113,
		0.0119,  0.0074,  0.0724,  0.0018,  0.0002,  0.0599, 
		0.0229,  0.0768,  0.0520,  0.0292,  0.0083,  0.0643,
		0.0887,  0.0744,  0.0523,  0.0128,  0.0006,  0.0053,
		0.0026,  0.0012
	};	

	VigenereCryptanalysis vc_en(english);
	pair<string, string> output_en = vc_en.analyze(input);

	cout << "Key: "  << output_en.second << endl;
	cout << "Text: " << output_en.first << endl;

	VigenereCryptanalysis vc_fr(french);
	pair<string, string> output_fr = vc_fr.analyze(input);

	cout << "Key: "  << output_fr.second << endl;
	cout << "Text: " << output_fr.first << endl;

}
