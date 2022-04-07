#include <iostream>
#include <cstdlib>
#include <fstream>
#include <vector>


using namespace std;
const int MAX_SIZE = 1000;

//==========================================================
//CACHE CLASS
//==========================================================
class Entry {
public:
  Entry(int addr){
    set_valid(true);
    set_data(addr);
  };
  ~Entry();
  void display(ofstream& outfile);

  void set_tag(int _tag) { tag = _tag; }
  int get_tag() { return tag; }

  void set_valid(bool _valid) { valid = _valid; }
  bool get_valid() { return valid; }

  void set_index(int _index) { index = _index; }
  int get_index() { return index; }

  void set_data(int _data) { data = _data; }
  int get_data() { return data; }

private:  
  bool valid;
  unsigned tag;
  int index;
  int data; 
};

class Cache {
public:
  Cache(int num_entries, int associativity) {
    set_assoc(associativity);
    set_num_entries(num_entries);
  };
  ~Cache();

  void display(ofstream& outfile);

  void set_assoc(int _assoc) { assoc = _assoc; }
  void set_num_entries(int _entries) { num_entries = _entries; }


  int get_index(unsigned long addr);
  int get_tag(unsigned long addr);

  unsigned long retrieve_addr(int way, int index);
  
  bool hit(ofstream& outfile, unsigned long addr);

  void update(ofstream& outfile, unsigned long addt);

  
private:
  int assoc;
  unsigned num_entries;
  int num_sets;
  Entry **entries;
};
//==========================================================
//MAIN METHOD
//==========================================================

int main(int argc, char*argv[]) {

  //FILE I/O
  //check if the correct number of arguments have been passed; bail otherwise
  if (argc < 4) {
    cout << "Usage: " << endl;
    cout << "   ./cache num_entries associativity filename" << endl;
    return 0;
  }
  //get args
  unsigned entries = atoi(argv[1]);
  unsigned assoc = atoi(argv[2]);
  string input_filename = argv[3];
  //create output filename
  string output_filename = "cache_sim_output";

  //print the args (TESTING)
  cout << "Number of entries: " << entries << endl;
  cout << "Associativity: " << assoc << endl;
  cout << "Input File Name: " << input_filename << endl;
  //return 0;

  //create input and output files streams 
  ofstream output;
  ifstream input;
  //open input stream for reading 
  input.open(input_filename);
  //check if input stream has been succesfully opened; bail otherwise 
  if (!input.is_open()) {
    cerr << "Could not open input file " << input_filename << ". Exiting ..." << endl;
    exit(0);
  }
  //populate input vector nums
  vector<int> nums;
  int count = 0;
  while (!input.eof() && count < MAX_SIZE) {
    input >> nums[count];
    count++;
  }
  //close the input stream
  input.close();
  //open output file stream for writing
  output.open(output_filename);

  //Create Cache
  Cache* myCache = new Cache(entries,assoc);
  //Loop through input
  int i = 0;
  for (i  = 0; i < nums.size(); i++) {
    //Create Entry
    Entry* myEntry = new Entry(nums[i]);
    //Calculate/Set Index
    int index = nums[i] % assoc;
    myEntry->set_index(index);
    //Calculate/Set Tag
    int tag = nums[i] / assoc;
    myEntry->set_tag(tag);
    //Set Valid Bit
  }


  
  //write to output file
  for (int i = 0; i < count; i++)
    output << "ADDR : " << nums[i] << endl; 
  //close output stream
  output.close();
  //end simulation
  return 0;
}


