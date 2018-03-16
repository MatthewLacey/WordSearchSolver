#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_set>
#include <queue>

using namespace std;

// declare functions
unordered_set<string> importDictionary();
ifstream openPuzzleFile();
vector<string> populatePuzzle();
queue<string> unOrderedSetToQueue(unordered_set<string> dictionary);
void checkLeft(int x, int y);
void checkRight(int x, int y);
void checkUp(int x, int y);
void checkDown(int x, int y);
void checkUpLeft(int x, int y);
void checkDownLeft(int x, int y);
void checkUpRight(int x, int y);
void checkDownRight(int x, int y);


// Global Constants
const int SPACE = 32;
const unordered_set<string> DICTIONARY = importDictionary();
const queue<string> DQ = unOrderedSetToQueue(DICTIONARY);
const vector<string> PUZZLE = populatePuzzle();
const auto PUZ_HEIGHT = static_cast<const int>(PUZZLE.size());
const auto PUZ_WIDTH = static_cast<const int>(PUZZLE[0].length());
const int MIN_WORD_LENGTH = 6;

//Each word has a starting point, ending point, and a string value
struct Word{
    string value;
    int startX, startY, endX, endY;

    Word(string value, int startX, int startY, int endX, int endY){
        this->value = std::move(value);
        this->startX = startX;
        this->startY = startY;
        this->endX = endX;
        this->endY = endY;
    }
};

//Compares two Word structs
class wordComparator{
public:
    int operator() (const Word &w1, const Word &w2){
        return w1.value > w2.value;
    }
};

//  Global Variable to store all words found because I am a bad programmer
priority_queue <Word, vector<Word>, wordComparator > foundWords;


void checkRight(const int x, const int y){
    queue<string> potentialWords = DQ;
    string prefix;
    prefix += PUZZLE[x][y];
    int currentX = x;
    int currentY = y + MIN_WORD_LENGTH - 1;
    int builtToY = y;

    while (currentX < PUZ_HEIGHT && currentX >= 0 && currentY < PUZ_WIDTH && currentY >= 0 && !potentialWords.empty()){
        while(builtToY < currentY){
            prefix += PUZZLE[x][++builtToY];
        }
        queue<string> buffer;
        while(!potentialWords.empty()){
            if(potentialWords.front() == prefix){
                foundWords.push(Word(potentialWords.front(),x,y,currentX,currentY));
                potentialWords.pop();
            } else if(potentialWords.front().find(prefix) == 0) {
                buffer.push(potentialWords.front());
                potentialWords.pop();
            } else
                potentialWords.pop();
        }
        potentialWords = buffer;
        currentY++;
    }
}

void checkLeft(const int x, const int y){
    queue<string> potentialWords = DQ;
    string prefix;
    prefix += PUZZLE[x][y];
    int currentX = x;
    int currentY = y - MIN_WORD_LENGTH + 1;
    int builtToY = y;

    while (currentX < PUZ_HEIGHT && currentX >= 0 && currentY < PUZ_WIDTH && currentY >= 0 && !potentialWords.empty()){
        while(builtToY > currentY){
            prefix += PUZZLE[x][--builtToY];
        }
        queue<string> buffer;
        while(!potentialWords.empty()){
            if(potentialWords.front() == prefix){
                foundWords.push(Word(potentialWords.front(),x,y,currentX,currentY));
                potentialWords.pop();
            } else if(potentialWords.front().find(prefix) == 0) {
                buffer.push(potentialWords.front());
                potentialWords.pop();
            } else
                potentialWords.pop();
        }
        potentialWords = buffer;
        currentY--;
    }
}

void checkUp(const int x, const int y){
    queue<string> potentialWords = DQ;
    string prefix;
    prefix += PUZZLE[x][y];
    int currentX = x - MIN_WORD_LENGTH + 1;
    int currentY = y;
    int builtToX = x;

    while (currentX < PUZ_HEIGHT && currentX >= 0 && currentY < PUZ_WIDTH && currentY >= 0 && !potentialWords.empty()){
        while(builtToX > currentX){
            prefix += PUZZLE[--builtToX][y];
        }
        queue<string> buffer;
        while(!potentialWords.empty()){
            if(potentialWords.front() == prefix){
                foundWords.push(Word(potentialWords.front(),x,y,currentX,currentY));
                potentialWords.pop();
            } else if(potentialWords.front().find(prefix) == 0) {
                buffer.push(potentialWords.front());
                potentialWords.pop();
            } else
                potentialWords.pop();
        }
        potentialWords = buffer;
        currentX--;
    }
}

void checkDown(const int x, const int y){
    queue<string> potentialWords = DQ;
    string prefix;
    prefix += PUZZLE[x][y];
    int currentX = x + MIN_WORD_LENGTH - 1;
    int currentY = y;
    int builtToX = x;

    while (currentX < PUZ_HEIGHT && currentX >= 0 && currentY < PUZ_WIDTH && currentY >= 0 && !potentialWords.empty()){
        while(builtToX < currentX){
            prefix += PUZZLE[++builtToX][y];
        }
        queue<string> buffer;
        while(!potentialWords.empty()){
            if(potentialWords.front() == prefix){
                foundWords.push(Word(potentialWords.front(),x,y,currentX,currentY));
                potentialWords.pop();
            } else if(potentialWords.front().find(prefix) == 0) {
                buffer.push(potentialWords.front());
                potentialWords.pop();
            } else
                potentialWords.pop();
        }
        potentialWords = buffer;
        currentX++;
    }
}

void checkUpRight(const int x, const int y){
    queue<string> potentialWords = DQ;
    string prefix;
    prefix += PUZZLE[x][y];
    int currentX = x - MIN_WORD_LENGTH + 1;
    int currentY = y + MIN_WORD_LENGTH - 1;
    int builtToX = x;
    int builtToY = y;

    while (currentX < PUZ_HEIGHT && currentX >= 0 && currentY < PUZ_WIDTH && currentY >= 0 && !potentialWords.empty()){
        while(builtToX > currentX){
            prefix += PUZZLE[--builtToX][++builtToY];
        }
        queue<string> buffer;
        while(!potentialWords.empty()){
            if(potentialWords.front() == prefix){
                foundWords.push(Word(potentialWords.front(),x,y,currentX,currentY));
                potentialWords.pop();
            } else if(potentialWords.front().find(prefix) == 0) {
                buffer.push(potentialWords.front());
                potentialWords.pop();
            } else
                potentialWords.pop();
        }
        potentialWords = buffer;
        currentX--; currentY++;
    }
}

void checkDownRight(const int x, const int y){
    queue<string> potentialWords = DQ;
    string prefix;
    prefix += PUZZLE[x][y];
    int currentX = x + MIN_WORD_LENGTH - 1;
    int currentY = y + MIN_WORD_LENGTH - 1;
    int builtToX = x;
    int builtToY = y;

    while (currentX < PUZ_HEIGHT && currentX >= 0 && currentY < PUZ_WIDTH && currentY >= 0 && !potentialWords.empty()){
        while(builtToX < currentX){
            prefix += PUZZLE[++builtToX][++builtToY];
        }
        queue<string> buffer;
        while(!potentialWords.empty()){
            if(potentialWords.front() == prefix){
                foundWords.push(Word(potentialWords.front(),x,y,currentX,currentY));
                potentialWords.pop();
            } else if(potentialWords.front().find(prefix) == 0) {
                buffer.push(potentialWords.front());
                potentialWords.pop();
            } else
                potentialWords.pop();
        }
        potentialWords = buffer;
        currentX++; currentY++;
    }
}

void checkUpLeft(const int x, const int y){
    queue<string> potentialWords = DQ;
    string prefix;
    prefix += PUZZLE[x][y];
    int currentX = x - MIN_WORD_LENGTH + 1;
    int currentY = y - MIN_WORD_LENGTH + 1;
    int builtToX = x;
    int builtToY = y;

    while (currentX < PUZ_HEIGHT && currentX >= 0 && currentY < PUZ_WIDTH && currentY >= 0 && !potentialWords.empty()){
        while(builtToX > currentX){
            prefix += PUZZLE[--builtToX][--builtToY];
        }
        queue<string> buffer;
        while(!potentialWords.empty()){
            if(potentialWords.front() == prefix){
                foundWords.push(Word(potentialWords.front(),x,y,currentX,currentY));
                potentialWords.pop();
            } else if(potentialWords.front().find(prefix) == 0) {
                buffer.push(potentialWords.front());
                potentialWords.pop();
            } else
                potentialWords.pop();
        }
        potentialWords = buffer;
        currentX--; currentY--;
    }
}

void checkDownLeft(const int x, const int y){
    queue<string> potentialWords = DQ;
    string prefix;
    prefix += PUZZLE[x][y];
    int currentX = x + MIN_WORD_LENGTH - 1;
    int currentY = y - MIN_WORD_LENGTH + 1;
    int builtToX = x;
    int builtToY = y;

    while (currentX < PUZ_HEIGHT && currentX >= 0 && currentY < PUZ_WIDTH && currentY >= 0 && !potentialWords.empty()){
        while(builtToX > currentX){
            prefix += PUZZLE[++builtToX][--builtToY];
        }
        queue<string> buffer;
        while(!potentialWords.empty()){
            if(potentialWords.front() == prefix){
                foundWords.push(Word(potentialWords.front(),x,y,currentX,currentY));
                potentialWords.pop();
            } else if(potentialWords.front().find(prefix) == 0) {
                buffer.push(potentialWords.front());
                potentialWords.pop();
            } else
                potentialWords.pop();
        }
        potentialWords = buffer;
        currentX++; currentY--;
    }
}


int main() {
    for (int i = 0; i < PUZ_HEIGHT; i++){
        for (int j = 0; j < PUZ_WIDTH; j++){
            checkUp(i,j);
            checkDown(i,j);
            checkRight(i,j);
            checkLeft(i,j);
            checkUpLeft(i,j);
            checkUpRight(i,j);
            checkDownLeft(i,j);
            checkDownRight(i,j);
        }
    }
    while(!foundWords.empty()){
        cout << foundWords.top().value << " from [" << foundWords.top().startX <<
             "," << foundWords.top().startY << "] to [" << foundWords.top().endX << "," << foundWords.top().endY << "]" << endl;
        foundWords.pop();
    }
    return 0;
}



unordered_set<string> importDictionary() {
    ifstream dictionaryFile;
    string word;
    unordered_set<string> dictionarySet;
    dictionaryFile.open("words.txt");
    if (dictionaryFile.is_open())
    {
        while ( getline (dictionaryFile, word) )
        {
            if(word.length() > MIN_WORD_LENGTH - 1)
                dictionarySet.insert(word);
        }
        dictionaryFile.close();
    }

    else cout << "Unable to open file\n";
    return dictionarySet;
}

ifstream openPuzzleFile(){
    ifstream puzzleFile;
    string word;
    puzzleFile.open("puzzle.txt");
    if (!puzzleFile.is_open())
        cout << "Unable to open file\n";
    return puzzleFile;
}

vector<string> populatePuzzle(){
    vector<string> grid;
    ifstream puzzleFile = openPuzzleFile();
    string row;
    string rowNoSpaces;
    while ( getline (puzzleFile, row) )
    {
        rowNoSpaces = "";
        for (char &c : row){
            if(c != SPACE){
                rowNoSpaces += c;
            }
        }
        grid.push_back(rowNoSpaces);
    }
    puzzleFile.close();
    return grid;
}

queue<string> unOrderedSetToQueue(unordered_set<string> dictionary){
    queue<string> dictQ;
    for (const auto& elem: dictionary) {
        dictQ.push(elem);
    }
    return dictQ;
}