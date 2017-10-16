// Greg Blair: Port of the PHP code in github.com/gingerbeardman/loveletter to C++
// October, 2017
// Christopher Strachey's "Loveletters" program (1952)
// The original was one of the earliest computer programs.
// Read more:
//     http://www.msimanchester.org.uk/en/whats-on/gallery/revolution-manchester
//     https://web.archive.org/web/20151018024547/http://www.mosi.org.uk/about-us/news/mosi-writes-love-letter-to-ferranti-(1).aspx
// References
//     https://grandtextauto.soe.ucsc.edu/2005/08/01/christopher-strachey-first-digital-artist/
//     http://gnoetrydaily.wordpress.com/2010/07/13/2-strachey-love-letters/
//     http://www.alpha60.de/art/love_letters/

#include <iostream>
#include <string>
#include <vector>
#include <random>
using namespace std;

vector<string> sals1 {"Beloved", "Darling", "Dear", "Dearest", "Fanciful", "Honey"};

vector<string> sals2 {"Chickpea", "Dear", "Duck", "Jewel", "Love", "Moppet", "Sweetheart"};

vector<string> adjs {"affectionate", "amorous", "anxious", "avid", "beautiful", "breathless", "burning", "covetous", "craving", "curious", "eager", "fervent", "fondest", "loveable", "lovesick", "loving", "passionate", "precious", "seductive", "sweet", "sympathetic", "tender", "unsatisfied", "winning", "wistful"};

vector<string> nouns {"adoration", "affection", "ambition", "appetite", "ardour", "being", "burning", "charm", "craving", "desire", "devotion", "eagerness", "enchantment", "enthusiasm", "fancy", "fellow feeling", "fervour", "fondness", "heart", "hunger", "infatuation", "little liking", "longing", "love", "lust", "passion", "rapture", "sympathy", "thirst", "wish", "yearning"};

vector<string> advs {"affectionately", "ardently", "anxiously", "beautifully", "burningly", "covetously", "curiously", "eagerly", "fervently", "fondly", "impatiently", "keenly", "lovingly", "passionately", "seductively", "tenderly", "wistfully"};

vector<string> verbs {"adores", "attracts", "clings to", "holds dear", "hopes for", "hungers for", "likes", "longs for", "loves", "lusts after", "pants for", "pines for", "sighs for", "tempts", "thirsts for", "treasures", "yearns for", "woos"};

void print_array(vector<string> arr) {
	for(auto el : arr) 
    cout <<  el << " \n";
}

int main()
{
  std::default_random_engine generator;                 // GB baseline RNG used to create a RN for a given distribution.
  std::uniform_int_distribution<int> distribution(0,9); // GB a uniform distribution with range [0,9].
  srand(static_cast<unsigned int>(time(0)));

		string optadj1;
		string noun1;
		string optadv;
		string verb;
		string optadj2;
		string noun2;
		string concat;
    string adv;
    string adj;
    string noun;
    string ll;

  //returns random value from array
  auto rel =  [] (vector<string>& arr) -> string& {
    return arr[rand() % arr.size()];
  };

// ll = sprintf("%s %s,\n     ", rel(sals1), rel(sals2));
  ll = rel(sals1) + " " + rel(sals2) + "\n\n";

  enum state_t { UNDEFINED, LONG, SHORT};
  state_t last = UNDEFINED;

  for(int i=0; i<7; i++) {
    if(distribution(generator) < 5) { //LONG
  		optadj1 = (distribution(generator) < 5) ? "" : rel(adjs);
  		noun1 = rel(nouns);
  		optadv = (distribution(generator) < 5) ? "" : rel(advs);
  		verb = rel(verbs);
  		optadj2 = (distribution(generator) < 5) ? "" : rel(adjs);
  		noun2 = rel(nouns);
  
    if (last != UNDEFINED || last == LONG) {
  			concat = ". ";
  		}
  
  		// ll .= sprintf("%s My %s %s %s %s your %s %s", concat, optadj1, noun1, optadv, verb, optadj2, noun2);
  		ll += concat + " My " + " "  + optadj1 + " " + noun1 + " " + optadv + " " + verb + " " + " your " + " " + optadj2 + " " + noun2;
  		last = LONG;
  
  	} else { //SHORT
  		adj = rel(adjs);
  		noun = rel(nouns);
  		if (last == SHORT) {
  			concat = ", ";
  		} else if (last == LONG) {
  			concat = ". You are";
  		} else {
  			concat = "You are ";
  		}
  
  		// ll .= sprintf("%s my %s %s", concat, adj, noun);
  		ll +=  concat + " my " + adj + " " + noun;
  		last = SHORT;
  	}
  }

  adv = rel(advs);
  ll +=  ".\n\n     Yours " + adv + ",\n     M.U.C.\n";
  
  // echo str_replace('  ', ' ', ll);
  string toReplace = "  ";
  string replaceWith = " ";
  while(true) {
    auto foundIt = ll.find(toReplace);
    if(foundIt == std::string::npos)
      break;
    ll.replace(foundIt, toReplace.size(), replaceWith);
  }
  
  cout << ll << "\n";
  
  
#if 0
  cout << "+++ salutations1:\n";
  print_array(sals1);
  
  cout << "+++ salutations2:\n";
  print_array(sals2);
  
  cout << "+++ adjectives:\n";
  print_array(adjs);
  
  cout << "+++ nouns:\n";
  print_array(nouns);
  
  cout << "+++ adverbs:\n";
  print_array(advs);
  
  cout << "+++ verbs:\n";
  print_array(verbs);
#endif
  
  return 0;
}
