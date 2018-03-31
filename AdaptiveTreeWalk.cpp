#include <iostream>
using namespace std;

#define NUM_STATIONS 1024  // N

// Global variables
int bus[NUM_STATIONS];
int readyStations; // K
int probeLevel; // I

////////////////////////////////////////////////////////////////////////////////
// Functions

void generateRandomFrames() {
  for (int i = 0; i < NUM_STATIONS; i++)
    bus[NUM_STATIONS] = rand() % 2;
}



// End of functions
////////////////////////////////////////////////////////////////////////////////
int main() {
  cout << "Starting simulation..." << endl;
  srand(time(NULL)); // Makes rand() more random
  generateFrames();

}
