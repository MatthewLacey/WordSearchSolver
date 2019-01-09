#include <iostream>
#include <string>
#include <fstream>
#include <map>
#include <unordered_set>
#include <vector>
#include <algorithm>

using namespace std;

class PuzzleSolver
{
public:
    PuzzleSolver();

    void build_dict(string file);
    string remove_spaces(string line);
    void set_rows(string file);
    void set_col();
    void set_diag();
    void forward();
    void backward();
    void report();
    void search(string puzzleFile, string dictFile);

private:
    // Stores dictionary
    unordered_set<string> dict;
    unordered_set<string>::iterator itr;

    // Stores answers in alphabetical order
    multimap<string, string> answers;
    multimap<string, string>::iterator it;

    // Counters
    int num_rows;
    int num_columns;
    int num_diagonals;
    int diag1_half;
    int diag2_half;

    // Stores rows/columns/diagonals
    vector<string> rows;
    vector<string> columns;
    vector<string> diag1;   // Diagonals in back slash direction
    vector<string> diag2;   // Diagonals in forward slash direction
};

PuzzleSolver::PuzzleSolver(){
    num_rows = 0;
    num_columns = 0;
    num_diagonals = 0;
    diag1_half = 0;
    diag2_half = 0;
}

/* Builds dictionary of lowercase words with no punctuation and
 * length 6 or greater from a word file.
 * @param a string indicating word file
 * @return none
 */
void PuzzleSolver::build_dict(string file) {
    ifstream inFile(file);
    string line;

    cout << "Building dictionary..." << endl;

    while(getline(inFile,line)){
        // Remove punctuation
        line.erase(remove_if(line.begin(), line.end(), ::ispunct), line.end());
        // Words length 6 or longer
        if (line.length() >= 6) {
            // Convert to lowercase
            for (int i = 0; i < line.length(); i++) {
                line[i] = tolower(line[i]);
            }
            dict.insert(line);
        }
    }
    inFile.close();
}

/* Removes spaces from a string
 * @param desired string
 * @return a string with spaces removed
 */
string PuzzleSolver::remove_spaces(string line) {
    line.erase(remove(line.begin(), line.end(), ' '), line.end());
    return line;
}

/* Stores puzzle rows (w/o spaces) in vector and counts number of rows
 * @param a string indicating word file
 * @return none
 */
void PuzzleSolver::set_rows(string file) {
    ifstream inFile(file);
    string line;

    cout << "Setting rows..." << endl;

    while(getline(inFile,line)){
        if(line.length() != 0 ) {
            rows.push_back(remove_spaces(line));
            num_rows++;
        }
    }

    inFile.close();
}

/* Stores puzzle columns (w/o spaces) in vector and counts number of
 * columns
 * @param none
 * @return none
 */
void PuzzleSolver::set_col() {
    string line, temp;

    cout << "Setting columns..." << endl;

    // Get number of columns
    num_columns = rows[0].length();

    // Store columns in vector
    if (num_rows > 0) {
        line = rows[0];
        for(int i = 0; i < line.length(); i++) {
            temp = line[i];
            columns.push_back(temp);
        }
        if (num_rows > 1) {
            for(int i = 1; i < num_rows; i++) {
                line = rows[i];
                for (int j = 0; j < line.length(); j++) {
                    columns[j] = columns[j] + line[j];
                }
            }
        }
    }
}

/* Stores puzzle diagonals (w/o spaces) in vectors if possible
 * and counts number of diagonals
 * @param none
 * @return none
 */
void PuzzleSolver::set_diag() {
    string line, temp;
    int next_char, end;

    // If diagonals >= 6 exist
    if(num_rows >= 6 && num_columns >= 6) {

        cout << "Setting diagonals..." << endl;

        // Set diagonals in backward slash direction (diag1)
        // Upper triangular
        line = rows[0];
        for(int i = 0; i < line.length(); i++) {
            temp = line[i];
            diag1.push_back(temp);
        }
        for(int i = 0; i < num_columns; i++) {
            for(int j = 1; j < num_rows; j++) {
                if (i+j < num_columns) {
                    diag1[i] = diag1[i] + rows[j][i+j]; }
            }
        }
        diag1_half = diag1.size();

        //Lower triangular
        line = columns[0];
        for(int i = 1; i < line.length(); i++) {
            temp = line[i];
            diag1.push_back(temp);
        }
        for(int i = 1; i < num_rows-1; i++) {
            for (int j = 1; j < num_rows-i; j++) {
                diag1[diag1_half+i-1] =
                        diag1[diag1_half+i-1] + rows[i+j][j];
            }
        }

        // Set diagonals in forward slash direction (diag2)
        // Upper triangular
        line = rows[0];
        for(int i = line.length()-1; i >=0 ; i--) {
            temp = line[i];
            diag2.push_back(temp);
        }
        for(int i = 0; i < num_columns; i++) {
            for(int j = 1; j < num_rows; j++) {
                next_char = num_columns-1-j-i;
                if(next_char >= 0) {
                    diag2[i] = diag2[i] + rows[j][next_char];
                }
            }
        }
        diag2_half = diag2.size();

        //Lower triangular
        line = columns[num_columns-1];
        for(int i = 1; i < line.length(); i++) {
            temp = line[i];
            diag2.push_back(temp);
        }
        for(int i = 1; i < num_rows-1; i++) {
            for (int j = 1; j < num_rows-i; j++) {
                next_char = num_columns-1-j;
                diag2[diag2_half+i-1] =
                        diag2[diag2_half+i-1] + rows[i+j][next_char];
            }
        }

        // Get number of diagonals
        num_diagonals = diag1.size();

    }
}

/* Forward searches words from dictionary in rows, columns, diagonals
 * and inserts answers and their locations in multimap
 * @param none
 * @return none
 */
void PuzzleSolver::forward() {
    string word;
    string begin, end, location;
    int first_pos, start, depth;

    cout << "Searching forward..." << endl;

    for ( itr = dict.begin(); itr != dict.end(); itr++) {
        word = *itr;

        // Search rows
        // --------->
        // --------->
        // --------->
        for(int i = 0; i < num_rows; i++) {
            if (rows[i].find(word) != string::npos) {
                first_pos = rows[i].find(word);
                begin = "(" + to_string(i) + "," +
                        to_string(first_pos) + ")";
                end = "(" + to_string(i) + "," +
                        to_string(first_pos + word.length() - 1) + ")";
                location = begin + " to " + end;
                answers.insert(make_pair(word,location));
            }
        }
        // Search columns
        // | | |
        // | | |
        // V V V
        for(int i = 0; i < num_columns; i++) {
            if (columns[i].find(word) != string::npos) {
                first_pos = columns[i].find(word);
                begin = "(" + to_string(first_pos) + "," +
                        to_string(i) + ")";
                end = "(" + to_string(first_pos + word.length() - 1)
                        + "," + to_string(i) + ")";
                location = begin + " to " + end;
                answers.insert(make_pair(word,location));
            }
        }
        // Search diagonals, if possible
        //  \ \ \              / / /
        //   \ \ \    and     / / /
        //    V V V          V V V
        if (num_diagonals > 0) {

            // Diag1
            for(int i = 0; i < num_diagonals; i++){
                if(diag1[i].length() >= 6 &&
                   diag1[i].find(word) != string::npos)
                {
                    first_pos = diag1[i].find(word);

                    if ( i < diag1_half) {
                        begin = "(" + to_string(first_pos) + "," +
                                to_string(i+first_pos) + ")";
                        end = "(" + to_string(first_pos + word.length() - 1)
                                + "," + to_string(i+first_pos + word.length() - 1)
                                + ")";
                    } else {
                        start = i-diag1_half+1;
                        begin = "(" + to_string(start+first_pos) + "," +
                                to_string(first_pos) + ")";
                        end = "(" + to_string(start + first_pos +
                                word.length() - 1) + "," +
                                to_string(first_pos + word.length() - 1) + ")";
                    }
                    location = begin + " to " + end;
                    answers.insert(make_pair(word,location));
                }
            }

            // Diag2
            for(int i = 0; i < num_diagonals; i++){
                if(diag2[i].length() >= 6 &&
                   diag2[i].find(word) != string::npos)
                {
                    first_pos = num_columns-1-diag2[i].find(word);
                    depth = diag2[i].find(word);

                    if ( i < diag2_half) {
                        begin = "(" + to_string(depth) + "," +
                                to_string(first_pos-i) + ")";
                        end = "(" + to_string(depth + word.length() - 1)
                              + "," + to_string(first_pos-i- word.length()
                              + 1) + ")";
                    } else {
                        start = i-diag2_half+1;
                        begin = "(" + to_string(start+depth) + "," +
                                to_string(first_pos) + ")";
                        end = "(" + to_string(start + depth + word.length() - 1)
                              + "," + to_string(first_pos - word.length() + 1) + ")";
                    }
                    location = begin + " to " + end;
                    answers.insert(make_pair(word,location));
                }
            }
        }

    }

}

/* Backward searches words from dictionary in rows, columns, diagonals
 * and inserts answers and their locations in multimap
 * @param none
 * @return none
 */
void PuzzleSolver::backward() {
    string word;
    string begin, end, location;
    int first_pos, start, depth;

    cout << "Searching backward..." << endl;

    for ( itr = dict.begin(); itr != dict.end(); itr++) {

        // Reverse word for searching
        word = *itr;
        reverse(word.begin(), word.end());

        // Search rows
        // <---------
        // <---------
        // <---------
        for(int i = 0; i < num_rows; i++) {
            if (rows[i].find(word) != string::npos) {
                first_pos = rows[i].find(word);
                end = "(" + to_string(i) + "," +
                        to_string(first_pos) + ")";
                begin = "(" + to_string(i) + "," +
                        to_string(first_pos + word.length() - 1) + ")";
                location = begin + " to " + end;
                reverse(word.begin(), word.end());
                answers.insert(make_pair(word,location));
            }
        }
        // Search columns
        // ^ ^ ^
        // | | |
        // | | |
        for(int i = 0; i < num_columns; i++) {
            if (columns[i].find(word) != string::npos) {
                first_pos = columns[i].find(word);
                end = "(" + to_string(first_pos) + "," +
                        to_string(i) + ")";
                begin = "(" + to_string(first_pos + word.length() - 1)
                        + "," + to_string(i) + ")";
                location = begin + " to " + end;
                reverse(word.begin(), word.end());
                answers.insert(make_pair(word,location));
            }
        }
        // Search diagonals, if possible
        // ^ ^ ^                ^ ^ ^
        //  \ \ \     and      / / /
        //   \ \ \            / / /
        if (num_diagonals > 0) {

            // Diag1
            for(int i = 0; i < num_diagonals; i++){
                if(diag1[i].length() >= 6 &&
                   diag1[i].find(word) != string::npos)
                {
                    first_pos = diag1[i].find(word);

                    if ( i < diag1_half) {
                        end = "(" + to_string(first_pos) + "," +
                                to_string(i+first_pos) + ")";
                        begin = "(" + to_string(first_pos + word.length() - 1)
                                + "," + to_string(i+first_pos + word.length() - 1) + ")";
                    } else {
                        start = i-diag1_half+1;
                        end = "(" + to_string(start+first_pos) + "," +
                                to_string(first_pos) + ")";
                        begin = "(" + to_string(start + first_pos + word.length() - 1)
                                + "," + to_string(first_pos + word.length() - 1) + ")";
                    }
                    location = begin + " to " + end;
                    reverse(word.begin(), word.end());
                    answers.insert(make_pair(word,location));
                }
            }

            // Diag2
            for(int i = 0; i < num_diagonals; i++){
                if(diag2[i].length() >= 6 &&
                   diag2[i].find(word) != string::npos)
                {
                    first_pos = num_columns-1-diag2[i].find(word);
                    depth = diag2[i].find(word);

                    if ( i < diag2_half) {
                        end = "(" + to_string(depth) + "," +
                                to_string(first_pos-i) + ")";
                        begin = "(" + to_string(depth + word.length() - 1)
                                + "," + to_string(first_pos-i- word.length() + 1) + ")";
                    } else {
                        start = i-diag2_half+1;
                        end = "(" + to_string(start+depth) + "," +
                                to_string(first_pos) + ")";
                        begin = "(" + to_string(start + depth + word.length() - 1)
                                + "," + to_string(first_pos - word.length() + 1) + ")";
                    }
                    location = begin + " to " + end;
                    reverse(word.begin(), word.end());
                    answers.insert(make_pair(word,location));
                }
            }
        }

    }
}

/* Prints multimap of puzzle answers to report in alphabetical order
 * @param none
 * @return none
 */
void PuzzleSolver::report() {
    ofstream outFile;
    outFile.open("answers.txt");

    cout << "Generating report..." << endl;

    bool lastLine = true;

    for( it = answers.begin(); it != answers.end(); it++ )
    {
        if(!lastLine) {
            outFile << endl;
        }
        outFile << it -> first << " from " << it -> second;
        lastLine = false;
    }
    outFile.close();
}

/* Searches a puzzle for words from dictionary and generates a report
 * @param string indicating puzzle file and string indicating
 * dictionary file
 * @return none
 */
void PuzzleSolver::search(string puzzleFile, string dictFile) {
    build_dict(dictFile);
    set_rows(puzzleFile);
    set_col();
    set_diag();
    forward();
    backward();
    report();
    cout << "\nSearch complete!";
}

int main() {
    PuzzleSolver puzzle;
    puzzle.search("puzzle.txt","words.txt");
    return 0;
}
