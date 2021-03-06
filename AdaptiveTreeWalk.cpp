#include <iostream>
#include <vector>
#include <cmath>
#include <iomanip>
using namespace std;

#define NUM_STATIONS 1024  // N

// Global variables
int stationsArr[NUM_STATIONS] = {0};
int numReadyStationsArr[] = {1,2,4,8,16,32,64,128,256,512,1024};  // K
int numReadyStationsN = 11;
int probeLevelsArr[] = {0,2,4,6,8,10};  // I
int probeLevelsN = 6;
double statsArrSuccess[6][11];
double statsArrCollision[6][11];
double statsArrIdle[6][11];
double statsArrTotal[6][11];

struct ATW_Statistics {
  int totalProbes;
  int successfulProbes;
  int collisionProbes;
  int idleProbes;
  int startingLevel;
  int readyStations;
};

////////////////////////////////////////////////////////////////////////////////
// Functions

ATW_Statistics initializeATW_Statistics(int startingLevel, int readyStations) {
  ATW_Statistics atw;
  atw.totalProbes = 0;
  atw.successfulProbes = 0;
  atw.collisionProbes = 0;
  atw.idleProbes = 0;
  atw.startingLevel = startingLevel/2;
  atw.readyStations = log2(readyStations);
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
  ATW_Statistics atw = initializeATW_Statistics(startingLevel, stationsReady);
  int tempStationsArr[NUM_STATIONS];
  copy(begin(stationsArr), end(stationsArr), begin(tempStationsArr));

  int fractionOfTree = NUM_STATIONS / (pow(2, startingLevel));
  int ignoreSubtreeArr[NUM_STATIONS] = {0};
  while (atw.successfulProbes != stationsReady) {
    for (int subtree = 0; subtree < (NUM_STATIONS/fractionOfTree); subtree++) {
      int framesReady = 0;
      int successIndex = 0;
      // Go through subtree and check for ready frames
      for (int i = subtree*fractionOfTree; i < (subtree*fractionOfTree+fractionOfTree); i++) {
        framesReady += tempStationsArr[i];
        if (tempStationsArr[i] == 1) successIndex = i; // Saves index to remove ready staton if probe success
        if (framesReady >= 2) break; // Collision
      }

      if (ignoreSubtreeArr[subtree*fractionOfTree] != 1) atw.totalProbes++;
      if (framesReady == 0 && ignoreSubtreeArr[subtree*fractionOfTree] != 1) {
        atw.idleProbes++;
      } else if (framesReady == 1) {
        atw.successfulProbes += 1;
        tempStationsArr[successIndex] = 0;
        for (int i = subtree*fractionOfTree; i < (subtree*fractionOfTree+fractionOfTree); i++) {
          ignoreSubtreeArr[i] = 1;
        }
      } else if (framesReady == 2) {
        atw.collisionProbes++;
      }
    }
    fractionOfTree /= 2;
  }
  return atw;
}

// Runs testing simulation for combinations of k, i, n, Adaptive Tree Walk Protocol
void runSimulation() {
  // For each different k stations ready
  for (int a = 0; a < numReadyStationsN; a++) {
    int k = numReadyStationsArr[a];
    cout << "Testing 100 cases of k=" << k;
    ATW_Statistics atwArr[600];
    int atwIndex = 0;
    // 100 random combinations of k stations
    for (int b = 0; b < 100; b++) {
      generateRandomFrames(k);
      // Probe starting on different levels i
      for (int c = 0; c < probeLevelsN; c++) {
        int startingLevel = probeLevelsArr[c];
        ATW_Statistics atw = probeTree(startingLevel, k);
        atwArr[atwIndex++] = atw;
      }
    }

    // Track global statistics for each combination of i and k
    for (int i = 0; i < 600; i++) {
      statsArrSuccess[atwArr[i].startingLevel][atwArr[i].readyStations] += atwArr[i].successfulProbes;
      statsArrTotal[atwArr[i].startingLevel][atwArr[i].readyStations] += atwArr[i].totalProbes;
      statsArrCollision[atwArr[i].startingLevel][atwArr[i].readyStations] += atwArr[i].collisionProbes;
      statsArrIdle[atwArr[i].startingLevel][atwArr[i].readyStations] += atwArr[i].idleProbes;
    }
    cout << "\tSuccess!" << endl;
  }
  cout << endl;
}

void printOnlySuccessStats() {
  cout << "       |\t\t\t\t  Ready stations\t\t\t\t    |" << endl;
  cout << "Level  |";
  for (int i = 0; i < 11; i++) {
    cout << numReadyStationsArr[i];
    if (i != 10) cout << "\t";
  }
  cout << "|";
  cout << "\n---------------------------------------------------------------------------------------------\n";
  for (int r = 0; r < 6; r++) {
    cout << probeLevelsArr[r];
    for (int c = 0; c < 11; c++) {
      cout << "\t" << setprecision(3) << fixed << statsArrSuccess[r][c]/statsArrTotal[r][c];
    }
    cout << endl;
  }
}

void printAllStats() {
  cout << "\nCSV:\n\n";
  cout << "i,k,% idle,% success,% collision,total" << endl;
  for (int r = 0; r < 6; r++) {
    for (int c = 0; c < 11; c++) {
      cout << probeLevelsArr[r];
      cout << "," << numReadyStationsArr[c];
      cout << "," << statsArrIdle[r][c]/statsArrTotal[r][c];
      cout << "," << statsArrSuccess[r][c]/statsArrTotal[r][c];
      cout << "," << statsArrCollision[r][c]/statsArrTotal[r][c];
      cout << "," << statsArrTotal[r][c]/100;
      cout << endl;
    }
  }
}

// End of functions
////////////////////////////////////////////////////////////////////////////////

int main() {
  cout << "Starting simulation..." << endl;
  srand(time(NULL)); // Makes rand() more random
  runSimulation();
  printOnlySuccessStats();
  // printAllStats();
}
