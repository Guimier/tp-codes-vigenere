#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <array>
#include <functional>
#include <queue>
#include <cmath>
#include <cstdlib>
#include <limits>

#include "../common/enc_dec.h"

using namespace std;

int verbose = 1;
 
typedef array<pair<char, double>, 26> FreqArray;

template <typename T>
class Serie
{
	private:
		const string& v;
		int freq;
		size_t offset;
	
	public:
		Serie( const string &v, int freq, size_t offset ):
			v( v ), freq( freq ), offset( offset )
		{}
		
		inline size_t size() const { return ( v.size() - offset ) / freq; }
		
		inline T operator [] ( size_t i ) const { return v[ offset + i * freq ]; }
		inline const T& operator [] ( size_t i ) { return v[ offset + i * freq ]; }
	
};

struct CharSequenceStatisticsComputer {

	vector<size_t> countOccurences( Serie<char>& serie )
	{
		vector<size_t> occurences( 26 );
		
		for ( size_t i = 0; i < serie.size(); ++i ) {
			++( occurences[ serie[i] - 'A' ] );
		}
		
		return occurences;
	}

	double indexOfCoincidence( Serie<char> chars )
	{
		vector<size_t> occurences = countOccurences( chars );

		unsigned long int sum = 0;

		for ( int i = 0; i < 26; ++i ) {
			if ( verbose > 2 ) cout << char( i + 'A' ) << ": " << occurences[i] << "; ";
			sum += occurences[i] * ( occurences[i] - 1 );
		}
		if ( verbose > 2 ) cout << endl;

		return sum / double( chars.size() * ( chars.size() - 1 ) );
	}
	
	double averageIndexOfCoincidence( const string list, int freq )
	{
		double sum = 0;
		for ( int i = 0; i < freq; ++i ) {
			sum += indexOfCoincidence( Serie<char>( list, freq, i ) );
		}
		
		return sum / freq;
	}
	
	double xhiSquared( Serie<char> chars, const array<double, 26>& target, int offset )
	{
		vector<size_t> occurences = countOccurences( chars );
		
		double res = 0;
		
		for ( int i = 0; i < 26; ++i ) {
			double eq = chars.size() * target[i + offset % 26];
			res += pow( occurences[i] - eq, 2 ) / eq;
		}
		
		return res;
	}

};

string normalizeString( const string in ) {
	string out;
	
	for ( size_t i = 0; i < in.size(); ++i ) {
		if ( in[i] >= 'A' && in[i] <= 'Z' ) {
			out += in[i];
		} else if ( in[i] >= 'a' && in[i] <= 'z' ) {
			out += in[i] + 'A' - 'a';
		}
	}
	
	return out;
}

template <typename T, typename U, typename C>
struct PairSecondSort {
	typedef pair<T,U> couple;
	C comp;
	bool operator () ( const couple& a, const couple& b ) {
		return comp( a.second, b.second );
	}
};

class VigenereCryptanalysis: CharSequenceStatisticsComputer
{
	private:
		array<double, 26> targets;
		array<double, 26> sortedTargets;

	public:
		VigenereCryptanalysis(const array<double, 26>& targetFreqs) 
		{
			targets = targetFreqs;
			sortedTargets = targets;
			sort(sortedTargets.begin(), sortedTargets.end());
		}
		
		int guessFrequency( string input )
		{

			priority_queue<pair<int,double>, vector<pair<int,double>>, PairSecondSort<int,double,less<double> > > pq;
			for ( int f = 1; f <= 10; ++ f ) {
				pq.push( make_pair( f, averageIndexOfCoincidence( input, f ) ) );
				if ( verbose > 1 ) cout << "f = " << f << "; ic = " << averageIndexOfCoincidence( input, f ) << endl;
			}
			
			int res = pq.top().first;
			double xhi = pq.top().second;
			pq.pop();
			
			if ( verbose > 0 ) cout << "Found frequency \"" << res << "\" (" << xhi << ", alternative is \"" << pq.top().first << "\", " << pq.top().second << ")" << endl;
			
			return res;
		}

		pair<string, string> analyze( string input, int freq ) 
		{
			input = normalizeString( input );

			if ( freq <= 0 ) {
				freq = guessFrequency( input );
				if ( verbose > 0 ) cout << "Using guessed frequency: " << freq << endl;
			} else {
				if ( verbose > 0 ) cout << "Using provided frequency: " << freq << endl;
			}
			
			string key;
			string alternateKey;
			
			bool deepDebug = verbose > 1;
			
			for ( int i = 0; i < freq; ++i ) {
				if ( deepDebug ) cout << "Substring offset: " << i << endl << "\t";
				Serie<char> serie( input, freq, i );
				
				priority_queue<pair<char,double>, vector<pair<char,double> >, PairSecondSort<char,double,greater<double> > > pq;
				
				for ( int offset = 0; offset < 26; ++offset ) {
					double xhi = xhiSquared( serie, targets, offset );
					char c = 'A' + offset;
					if ( deepDebug ) cout << c << ": " << xhi << ' ';
					pq.push( make_pair( c, xhi ) );
				}
				
				if ( deepDebug ) cout << endl;
				
				key += pq.top().first;
				pq.pop();
				alternateKey += pq.top().first;
			}
			
			if ( verbose > 0 ) cout << "Found key \"" << key << "\" (alternative \"" << alternateKey << "\")" << endl;

			Vigenere dec( key );
	
			return make_pair( dec.decrypt( input ), key );
		}
};
 
int main( int argc, char* argv[] ) 
{

	if ( argc < 2 ) {
		cerr << "Missing ciphertext id" << endl;
		exit( 1 );
	}
	
	string input;
	
	switch ( atoi( argv[1] ) ) {
		case 1:
			input = "zbpuevpuqsdlzgllksousvpasfpddggaqwptdgptzweemqzrdjtddefekeferdprrcyndgluaowcnbptzzzrbvpssfpashpncotemhaeqrferdlrlwwertlussfikgoeuswotfdgqsyasrlnrzppdhtticfrciwurhcezrpmhtpuwiyenamrdbzyzwelzucamrptzqseqcfgdrfrhrpatsepzgfnaffisbpvblisrplzgnemswaqoxpdseehbeeksdptdttqsdddgxurwnidbdddplncsd";
			break;
		case 2:
			input = "gmyxzoocxziancxktanmyolupjrztgxwshctzluibuicyzwxyqtvqxzukibkotuxkagbknmimmzzyajvjzampqyzloinoiqknaumbknknvkaiakgwtnilvvzvqydmvjcximrvzkilxzqtomrgqmdjrzyazvzmmyjgkoaknkuiaivknvvy";
			break;
		case 3:
			input = "iefomntuohenwfwsjbsfftpgsnmhzsbbizaomosiuxycqaelrwsklqzekjvwsivijmhuvasmvwjewlzgubzlavclhgmuhwhakookakkgmrelgeefvwjelksedtyhsgghbamiyweeljcemxsohlnzujagkshakawwdxzcmvkhuwswlqwtmlshojbsguelgsumlijsmlbsixuhsdbysdaolfatxzofstszwryhwjenuhgukwzmshbagigzzgnzhzsbtzhalelosmlasjdttqzeswwwrklfguzl";
			break;
		case 4:
			input = "MOMUDEKAPVTQEFMOEVHPAJMIICDCTIFGYAGJSPXYALUYMNSMYHVUXJELEPXJFXGCMJHKDZRYICUHYPUSPGIGMOIYHFWHTCQKMLRDITLXZLJFVQGHOLWCUHLOMDSOEKTALUVYLNZRFGBXPHVGALWQISFGRPHJOOFWGUBYILAPLALCAFAAMKLGCETDWVOELJIKGJBXPHVGALWQCSNWBUBYHCUHKOCEXJEYKBQKVYKIIEHGRLGHXEOLWAWFOJILOVVRHPKDWIHKNATUHNVRYAQDIVHXFHRZVQWMWVLGSHNNLVZSJLAKIFHXUFXJLXMTBLQVRXXHRFZXGVLRAJIEXPRVOSMNPKEPDTLPRWMJAZPKLQUZAALGZXGVLKLGJTUIITDSUREZXJERXZSHMPSTMTEOEPAPJHSMFNBYVQUZAALGAYDNMPAQOWTUHDBVTSMUEUIMVHQGVRWAEFSPEMPVEPKXZYWLKJAGWALTVYYOBYIXOKIHPDSEVLEVRVSGBJOGYWFHKBLGLXYAMVKISKIEHYIMAPXUOISKPVAGNMZHPWTTZPVXFCCDTUHJHWLAPFYULTBUXJLNSIJVVYOVDJSOLXGTGRVOSFRIICTMKOJFCQFKTINQBWVHGTENLHHOGCSPSFPVGJOKMSIFPRZPAASATPTZFTPPDPORRFTAXZPKALQAWMIUDBWNCTLEFKOZQDLXBUXJLASIMRPNMBFZCYLVWAPVFQRHZVZGZEFKBYIOOFXYEVOWGBBXVCBXBAWGLQKCMICRRXMACUOIKHQUAJEGLOIJHHXPVZWJEWBAFWAMLZZRXJEKAHVFASMULVVUTTGK";
			break;
		case 5:
			input = "BPSRAUNOHCWCBGITMPJQFMEXCXYCIAGPSXCPSXWWEROQCOPITRAEBENTGAYCPMSXPQNYWCDQEVJMAVIDQRZEQESBPCEWCXCYCVYGYCWI";
			break;
		default:
			cerr << "Invalid id \"" << argv[1] << "\"" << endl;
			exit( 1 );
			
	}

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

	char* ver = getenv( "VERBOSE" );
	if ( ver != NULL ) {
		verbose = atoi( ver );
	}

	int freq = 0;
	if ( argc > 2 ) freq = atoi( argv[2] ); 

	VigenereCryptanalysis vc_en(english);
	pair<string, string> output_en = vc_en.analyze(input, freq);

	cout << "Key: "  << output_en.second << endl;
	cout << "Text: " << output_en.first << endl;

	VigenereCryptanalysis vc_fr(french);
	pair<string, string> output_fr = vc_fr.analyze(input, freq);

	cout << "Key: "  << output_fr.second << endl;
	cout << "Text: " << output_fr.first << endl;

}
