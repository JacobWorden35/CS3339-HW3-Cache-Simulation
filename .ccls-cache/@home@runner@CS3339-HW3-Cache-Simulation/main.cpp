#include <iostream>
#include <cstdlib>
#include <fstream>
#include <vector>
#include <string>
#include <tuple>
#include <sstream>

using namespace std;
string str;
string strAddr;
string charDigit;
int count = 0;
int  ways;

//==========================================================
//                      ENTRY CLASS
//==========================================================
class Entry {
public:
  //CONSTRUCTORS
  Entry(int addr){
    set_data(addr);
  };
  ~Entry();

  //FUNCTIONS
  void display(ofstream& outfile);

  void set_tag(int _tag) { tag = _tag; }
  int get_tag() { return tag; }

  void set_valid(bool _valid) { valid = _valid; }
  int get_valid() { return valid; }

  void set_index(int _index) { index = _index; }
  int get_index() { return index; }

  void set_data(int _data) { data = _data; }
  int get_data() { return data; }

  void set_way(int _way) { data = _way; }
  int get_way() { return way; }

private:  
  int valid;
  unsigned tag;
  int index;
  int way;
  int data; 
};
//==========================================================
//                      CACHE CLASS
//==========================================================
class Cache {
public:
  //CONSTRUCTORS
  Cache(int num_entries, int associativity) {
    set_assoc(associativity);
    set_num_entries(num_entries);
  };
  ~Cache();
  //FUNCTIONS


  //      Insert an Entry to the Cache
  void insert(Entry* entry, string out) {
    //      Cache is empty
    if (cache.size() == 0) {
      cout << "Cache Empty, Miss: " << entry->get_data() << endl;
      miss(out,entry->get_data());
      entry->set_way(0);
      cache.emplace_back(entry->get_way(), entry->get_index(), true, entry->get_data());
      return;
    }
    //      Check cache
    for (int i = 0; i < cache.size(); i++) {
      //      HIT
      if (get<3>(cache[i]) == entry->get_data()) { 
        cout << "Found Address, HIT: " << entry->get_data() << endl;
        hit(out,entry->get_data());
        cache[i] = make_tuple(0,entry->get_index(),true,entry->get_data());
        //      Set other Valids to false
        for(int z = 0; z < cache.size(); z++) {
          if (z != i) {
            cache[z] = make_tuple(get<0>(cache[i]),get<1>(cache[i]),false,get<3>(cache[i]));
          }
        }
        return;
      } else if (get<1>(cache[i]) == entry->get_index()) { //REPLACE
          cout << "Cache Conflict, Miss: " << entry->get_data() << endl;
          miss(out,entry->get_data());
          //      Look for available ways
          for (int j = 0; i < ways; i++) {
            //Way is available
            if(get<0>(cache[i]) != j) {
              cout << "Set to new Way" << endl;
              entry->set_way(j);
              cache.emplace_back(j, entry->get_index(), true, entry->get_data());
              //      Set other Valids to false
              for(int z = 0; z < cache.size(); z++) {
                if (z != i) {
                  cache[z] = make_tuple(get<0>(cache[i]),get<1>(cache[i]),false,get<3>(cache[i]));
                }
              }
              return;
            }
          }
          //  Way not available IMPLEMENT LRU
          if (get<2>(cache[i]) == false) {
            cout << "1 is replaced with 5" << endl;
            //REPLACE BLOCK
            cache[i] = make_tuple(0,entry->get_index(),true,entry->get_data());
            //      Set other valids to false
            for(int z = 0; z < cache.size(); z++) {
              if (z != i) {
                cache[z] = make_tuple(get<0>(cache[i]),get<1>(cache[i]),false,get<3>(cache[i]));
              }             
            }
            return;
          }
      }
    }
    //      MISS
      cout << "Cache Open, Miss: " << entry->get_data() << endl;
      cache.emplace_back(0, entry->get_index(), true, entry->get_data());
      miss(out,entry->get_data());
      return;
  }

  //      Outputs a HIT
  void hit(string output, int addr) {
    output.append(to_string(addr) + " : HIT \n");
  };
  //      Outputs a MISS
  void miss(string output, int addr) {
    output.append(to_string(addr) + " : MISS \n");
  };
  //      Sets + Gets
  void set_assoc(int _assoc) { assoc = _assoc; }
  void set_num_entries(int _entries) { num_entries = _entries; }
  int get_index(unsigned long addr);
  int get_tag(unsigned long addr);
  //      NOT SURE YET
  void display(ofstream& outfile);
  unsigned long retrieve_addr(int way, int index);
  void update(ofstream& outfile, unsigned long addt);
  //      Cache
  vector<tuple<int, int, bool,int>> cache;
      /*      way  index  V  data  */
private:
  int assoc;
  unsigned num_entries;
  int num_sets;
  Entry **entries;
  
        
};
//==========================================================
//                        MAIN
//==========================================================

int main(int argc, char*argv[]) {

  //FILE I/O
  //      Check for the correct # of args 
  if (argc < 4) {
    cout << "Usage: " << endl;
    cout << "   ./cache num_entries associativity filename" << endl;
    return 0;
  }
  //      Get args
  unsigned entries = atoi(argv[1]);
  unsigned assoc = atoi(argv[2]);
  string input_filename = argv[3];
  //      Create output filename
  string output_filename = "cache_sim_output";
  string out = "";

  //      Print the args (TESTING)
  cout << "Number of entries: " << entries << endl;
  cout << "Associativity: " << assoc << endl;
  //return 0;
  int ways = assoc;
  //      Create input and output files streams 
  ofstream output;
  ifstream input;
  //      Open input stream for reading 
  input.open(input_filename);
  //      Check if input stream has been succesfully opened;
  if (!input.is_open()) {
    cerr << "Could not open input file " << input_filename << ". Exiting ..." << endl;
    exit(0);
  }
  //      Populate input vector nums
  vector<int> nums;
  int count = 0;
  while (!input.eof()) {
    getline(input, str);
    strAddr += str + " ";
  }
  stringstream iss(strAddr);
  int num;
  while (iss >> num) {
    nums.push_back(num);
  }
  cout << "Input Stream:" << strAddr << endl;
  //      Print the input stream (TESTING)
  /*for (int i = 0; i < nums.size(); i++) {
    cout << nums[i] << " ";  
  }*/
  //      Close the input stream
  input.close();
  //      Open output file stream for writing
  output.open(output_filename);
  //      Create Cache
  Cache* myCache = new Cache(entries,assoc);
  //      Loop through input
  //cout << nums.size() << endl;
  for (int i = 0; i < nums.size(); i++) {
    //      Create Entry
    Entry* myEntry = new Entry(nums[i]);
    //      Calculate/Set Index & Tag
    myEntry->set_index(nums[i] % assoc);
    myEntry->set_tag(nums[i] / assoc);
    //      Put Entry in Cache
    myCache->insert(myEntry, out);
  }
  //      Write to output file
  output << out;
  output.close();
  //      End simulation
  return 0;
}


