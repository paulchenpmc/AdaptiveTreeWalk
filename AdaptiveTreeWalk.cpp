#include <iostream>
using namespace std;

#define NUM_STATIONS 8  // N

// Global variables
int stationsArr[NUM_STATIONS];
int readyStations; // K
int probeLevel; // I
int numProbes;

////////////////////////////////////////////////////////////////////////////////
// Functions

void generateRandomFrames(int k) {
  // Set all stations to 0
  for (int i = 0; i < NUM_STATIONS; i++)
    stationsArr[i] = 0;

  // int numLoops = 0;
  int numReady = 0;
  for (int i = 0; true; i++) {
    if (numReady == k) break;
    i = i % NUM_STATIONS; // Reset i when end of array reached, keep looping until k stations ready
    if (stationsArr[i]) continue; // Skip stations that are already set to 1
    stationsArr[i] = rand() % 2;
    if (stationsArr[i]) numReady++; // Track # stations ready
    // numLoops++;
  }
  // cout << "Num loops: " << numLoops << endl;
  // for (int i = 0; i < NUM_STATIONS; i++)
  //   cout << stationsArr[i] << endl;
}

int probeTree() {

}

void runSimulation() {

}

// End of functions
////////////////////////////////////////////////////////////////////////////////
int main() {
  cout << "Starting simulation..." << endl;
  srand(time(NULL)); // Makes rand() more random
  generateRandomFrames(0);
  runSimulation();
}
