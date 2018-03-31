#include <iostream>
#include <vector>
#include <stdio.h>
using namespace std;

#define NUM_STATIONS 1024  // N

// Global variables
int stationsArr[NUM_STATIONS];
int numReadyStationsArr[] = {1,2,4,8,16,32,64,128,256,512,1024};  // K
int numReadyStationsN = 11;
int probeLevelsArr[] = {0,2,4,6,8,10};  // I
int probeLevelsN = 6;

struct ATW_Statistics {
  int totalProbes;
  int successfulProbes;
};

////////////////////////////////////////////////////////////////////////////////
// Functions

ATW_Statistics initializeATW_Statistics() {
  ATW_Statistics atw;
  atw.totalProbes = 0;
  atw.successfulProbes = 0;
  return atw;
}

// Randomly readies k stations for sending
void generateRandomFrames(int k) {
  // Set all stations to 0
  for (int i = 0; i < NUM_STATIONS; i++)
    stationsArr[i] = 0;

  // Randomly ready k stations
  vector<int> randomStationsVec(NUM_STATIONS);
  for (int i = 0; i < randomStationsVec.size(); i++)
    randomStationsVec[i] = i;
  int numReady = 0;
  for (int j = 0; numReady != k; j++) {
    j = j % randomStationsVec.size();
    int selected = rand() % randomStationsVec.size();
    stationsArr[randomStationsVec[selected]] = 1;
    randomStationsVec.erase(randomStationsVec.begin()+selected);
    numReady++;
  }

  // Sanity check because I'm scared of the rand function
  int errorcheck = 0;
  for (int i = 0; i < NUM_STATIONS; i++)
    if (stationsArr[i] == 1) errorcheck++;
  if (errorcheck != k) {
    cout << "\nERROR in generateRandomFrames, k stations not ready!\n" << endl;
    exit(1);
  }
}

// Probes tree and tracks: collisions, # probes, # idle
int probeTree(int startingLevel) {

}

void runSimulation() {
  // For each different k stations ready
  for (int a = 0; a < numReadyStationsN; a++) {
    int k = numReadyStationsArr[a];
    printf("Testing 100 cases of k=%d\n", k);
    // 100 random combinations of k stations
    for (int b = 0; b < 100; b++) {
      // if (b%10==0) printf("  test #%d\n", b);
      generateRandomFrames(k);
      // Probe starting on different levels i
      for (int c = 0; c < probeLevelsN; c++) {
        int startingLevel = probeLevelsArr[c];
        probeTree(startingLevel);
      }
    }
  }
}

// End of functions
////////////////////////////////////////////////////////////////////////////////

int main() {
  cout << "Starting simulation..." << endl;
  srand(time(NULL)); // Makes rand() more random
  runSimulation();
}
