#include <random>
#include <iostream>
#include <vector>
#include <list>

int rows = 8;
int columns = 8;

std::list<int> distances = {};

std::vector<std::vector<String>> matrix(rows, std::vector<String>(columns, " "));
int random_int; 

std::random_device rd; 
  
  // 2. Initialize the standard Mersenne Twister engine with the seed
std::mt19937 gen(rd()); 
  
  // 3. Define the distribution mapping
std::uniform_int_distribution<int> int_dist(1, 200); 

void setup() {
  Serial.begin(115200);
}

void loop() {
  distances.clear();


  String low = "░";
  String medium = "▒";
  String high = "▓";
  String veryHigh = "█";


  for (int i=0; i<64; i++) {
    random_int = int_dist(gen);
    distances.push_back(random_int);
  }
  auto it = distances.begin();
  for (int index = 0; index < 64; index++) {
    int value = *it;
    
    int x = index / 8;
    int y = index % 8;

    if (value < 50) {
      matrix[x][y] = low;
    } else if (value < 100) {
      matrix[x][y] = medium;
    } else if (value < 150) {
      matrix[x][y] = high;
    } else {
      matrix[x][y] = veryHigh;
    }

    it++;
    

  }

  for (int r=0; r<rows; r++) {
    for (int c= 0; c<columns; c++) {
      Serial.print(matrix[r][c]);
    }
    Serial.println();
  }
  Serial.println();

  // put your main code here, to run repeatedly:
  //Serial.println("░▓▒░▓██");
  //Serial.println("██▓▒░░▓");
  Serial.println();
  delay(500);


}
