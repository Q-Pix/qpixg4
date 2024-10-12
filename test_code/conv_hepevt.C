#include <string>
#include <vector>
#include <iostream>
#include <fstream>

struct hep_particle{

  hep_particle(std::string&, std::string&);
  friend ostream& operator<<(ostream& os, const hep_particle& hep);

  // meta PID stuff
  int index;
  int stat;
  int par;
  int children;
  int PID;
  int ents;
  int dren;

  // momentum
  double px;
  double py;
  double pz;
  // energy & mass
  double E;
  double M;
  // 4-position
  double x;
  double y;
  double z;
  double cT;
};

ostream& operator<<(ostream& os, const hep_particle& hep)
{
  os << "particle info: " << hep.index << std::endl;
  return os;
}

// find white space separated intgers in string
std::vector<int> find_ints(std::string int_vals)
{
  int cur_ind = 0;
  int max_ind = int_vals.size();

  std::vector<int> ints;

  while(cur_ind < max_ind-1){
    std::string sub_str = int_vals.substr(cur_ind);
    int num_ind = sub_str.find_first_not_of(" ");
    int blk_ind = sub_str.substr(num_ind).find_first_of(" ");
    std::string val = sub_str.substr(num_ind, num_ind+blk_ind);
    cur_ind += num_ind+blk_ind;

    int i = std::stoi(val);
    ints.push_back(i);
  }
  return ints;
}

std::vector<double> find_dbls(std::string dbl_vals)
{
  int cur_ind = 0;
  int max_ind = dbl_vals.size();

  std::vector<double> dbls;

  while(cur_ind < max_ind-1){
    std::string sub_str = dbl_vals.substr(cur_ind);
    int num_ind = sub_str.find_first_not_of(" ");
    int blk_ind = sub_str.substr(num_ind).find_first_of(" ");
    std::string val = sub_str.substr(num_ind, num_ind+blk_ind);
    cur_ind += num_ind+blk_ind;

    double dbl = std::stod(val);
    dbls.push_back(dbl);
  }
  return dbls;
}

// particle parsing
void particle_info_one(std::string& line, std::vector<int>& ints, std::vector<double>& dbls){
  int ind1 = line.find_first_not_of(" ");
  int paren1 = line.find("("); // first paren
  int paren2 = line.find(")");

  // integer values string
  std::string int_vals = line.substr(ind1, paren1-ind1);
  int_vals = int_vals.substr(0, int_vals.find_last_not_of(" ")+1)+" ";
  ints = find_ints(int_vals);

  // double values string
  std::string dbl_vals = line.substr(paren1+1, paren2-paren1-1)+" ";
  dbls = find_dbls(dbl_vals);
}


hep_particle::hep_particle(std::string& line1, std::string& line2)
{
  std::vector<int> ints1, ints2;
  std::vector<double> dbls1, dbls2;
  particle_info_one(line1, ints1, dbls1);
  particle_info_one(line2, ints2, dbls2);
  // first column ints
  index = ints1[0];
  stat = ints1[1];
  par = ints1[2];
  children = ints1[3];
  // second column ints
  PID = ints2[0];
  ents = ints2[1];
  dren = ints2[2];
  // first column ints
  px = dbls1[0];
  py = dbls1[1];
  pz = dbls1[2];
  E = dbls1[3];
  M = dbls1[4];
  // second column ints
  x = dbls2[0];
  y = dbls2[1];
  z = dbls2[2];
  cT = dbls2[3];
};
// event parsing
struct hep_evt{
  hep_evt(const int& evt_num, const int& np) : n_evt(evt_num), n_particles(np) {}; 
  int n_evt;
  int n_particles;
  std::vector<hep_particle> particles;
};

int evt_num(std::string header)
{
  std::string line = header.substr(header.find(":")+1, header.find(",")-header.find(":"));
  int evt_num = std::stoi(line);
  return evt_num;
}

int n_particles(std::string header)
{
  std::string line = header.substr(header.find(",")+1, header.find("p")-header.find(":"));
  int n_particles = std::stoi(line);
  return n_particles;
}


void conv_hepevt(std::string input_file_name="test_hepevt.txt")
{
  std::cout << "reading the input text file..\n";

  // open the file
  std::string line;
  std::ifstream input_file;
  input_file.open(input_file_name);

  std::vector<hep_evt> hepevts;
  int cur_particles = 0;

  while(std::getline(input_file, line)){
    // we can build the event
    if(line.substr(6,6) == "HEPEVT"){
      int nevt = evt_num(line);
      int nparticles = n_particles(line);
      hepevts.push_back(hep_evt(nevt, nparticles));
      std::cout << "found nevt: " << nevt << ", with particles: " << nparticles << std::endl;
      cur_particles = 0;
      // move past the header info we don't need
      std::getline(input_file, line);
      std::getline(input_file, line);
      std::getline(input_file, line);
    }
    // breakline
    else if(line[0] == '_'){
      // std::cout << "break line: " << line << std::endl;
      cur_particles += 1;
    }
    else if(line[0] == ' '){
      // particles come in two lines
      std::string line2;
      std::getline(input_file, line2);
      hep_particle particle(line, line2);
      hepevts.back().particles.push_back(particle);
    }else{
      std::cout << "WARNING unknown line type!\n";
    }
  }

  // print the whole file checking for errors in the particle construction count
  for(auto evt : hepevts){
    std::cout << "Event: " << evt.n_evt << " has " << evt.n_particles << " particles " << std::endl;
    if(evt.n_particles != evt.particles.size() - 1){
      std::cout << "ERROR!\n";
      std::cout << "evt.n_particles != evt.particles.size()" << std::endl;
      std::cout << evt.n_particles << "!=" << evt.particles.size() << std::endl;
      return -1;
    }
  }
}
