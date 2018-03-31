#include <iostream>
#include <vector>
#include <stdio.h>
using namespace std;

#define NUM_STATIONS 8  // N

// Global variables
int stationsArr[NUM_STATIONS] = {0};
int numReadyStationsArr[] = {1,2,4,8,16,32,64,128,256,512,1024};  // K
int numReadyStationsN = 11;
int probeLevelsArr[] = {0,2,4,6,8,10};  // I
int probeLevelsN = 6;

struct ATW_Statistics {
  int totalProbes;
  int successfulProbes;
  int collisionProbes;
  int idleProbes;
};

////////////////////////////////////////////////////////////////////////////////
// Functions

ATW_Statistics initializeATW_Statistics() {
  ATW_Statistics atw;
  atw.totalProbes = 0;
  atw.successfulProbes = 0;
  atw.collisionProbes = 0;
  atw.idleProbes = 0;
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
ATW_Statistics probeTree(int startingLevel, int stationsReady) {
  ATW_Statistics atw = initializeATW_Statistics();
  int fractionOfTree = NUM_STATIONS / (startingLevel+1);
  cout << "Entered probetree" << endl;
  int ignoreSubtreeArr[NUM_STATIONS] = {0};
  while (atw.successfulProbes != stationsReady) {
    cout << "Tree fraction: " << fractionOfTree << endl;
    for (int subtree = 0; subtree < (NUM_STATIONS/fractionOfTree); subtree++) {
      int framesReady = 0;
      int successIndex = 0;
      // Go through subtree and check for ready frames
      for (int i = subtree*fractionOfTree; i < (subtree*fractionOfTree+fractionOfTree); i++) {
        framesReady += stationsArr[i];
        if (stationsArr[i] == 1) successIndex = i; // Saves index to remove ready state if probe success
        if (framesReady >= 2) break; // Collision
      }

      if (ignoreSubtreeArr[subtree*fractionOfTree] != 1) atw.totalProbes++;
      if (framesReady == 0 && ignoreSubtreeArr[subtree*fractionOfTree] != 1) {
        atw.idleProbes++;
      } else if (framesReady == 1) {
        atw.successfulProbes++;
        stationsArr[successIndex] = 0;
        for (int i = subtree*fractionOfTree; i < (subtree*fractionOfTree+fractionOfTree); i++) {
          ignoreSubtreeArr[i] = 1;
        }
        cout << "success index: " << successIndex << endl;
      } else if (framesReady == 2) {
        atw.collisionProbes++;
      }
      cout << "chunk " << subtree+1 << " framesReady: " << framesReady << endl;
    }
    cout << "successfulProbes: " << atw.successfulProbes << endl << endl;
    fractionOfTree /= 2;
  }
  return atw;
}

// Runs testing simulation for combinations of k, i, n, Adaptive Tree Walk Protocol
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
        ATW_Statistics atw = probeTree(startingLevel, k);
        // Compute average
      }
    }
  }
}

// End of functions
////////////////////////////////////////////////////////////////////////////////

int main() {
  cout << "Starting simulation..." << endl;
  srand(time(NULL)); // Makes rand() more random
  //runSimulation();

  // Carey's example
  stationsArr[1] = 1;
  stationsArr[5] = 1;
  stationsArr[6] = 1;
  ATW_Statistics atw = probeTree(3, 3);
  cout << "\nSuccess probes: " << atw.successfulProbes << endl;
  cout << "Collision probes: " << atw.collisionProbes << endl;
  cout << "Idle probes: " << atw.idleProbes << endl;
  cout << "Total probes: " << atw.totalProbes << endl;
}
