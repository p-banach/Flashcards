#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <random>
#include <ctime>        // std::time
#include <chrono>      // std::rand, std::srand


/* ___________________TO DO______________
 * stworzyc modul do zapisywania wynikow, i eliminowac slowka jezeli maja wysoki
 * (np 100% przy 5> odpowiedziach wynik
 * Menu do wyboru modulów przed rozpoczeciem
 * (dodam tu angielski, francuski, duzo muzyki, etc)
 */
using namespace std;

#define CARD_ARRAY_SIZE 2000

size_t loaded_cards = 0;

struct card {
    string question;
    string answer;
    string type;
    bool correct_answer;
};

card card_array[CARD_ARRAY_SIZE];

void print(){
    printf("Czesc");
}

bool PromptForChar(const char *prompt, char &readch) {
    std::string tmp;
    std::cout << prompt << std::endl;
    if ( std::getline(std::cin, tmp)) {
        // Only accept single character input
        if ( tmp.length() == 1 ) {
            readch = tmp[0];
        } else {
            // For most input, char zero is an appropriate sentinel
            readch = '\0';
        }
        return true;
    }
    return false;
}

void initialize() {
    for ( auto &i : card_array ) {
        i.correct_answer = false;
    }
}

void parseInputUtil(string name) {
    ifstream myfile;
    myfile.open(name.c_str());

    string temp;
    while ( loaded_cards < CARD_ARRAY_SIZE && !myfile.eof()) {
        getline(myfile, card_array[loaded_cards].question);
        getline(myfile, card_array[loaded_cards].answer);
        getline(myfile, card_array[loaded_cards].type);
        //pomijanie linii oddzielającej wpisy, ktora jst tam dla czytelnosci
        getline(myfile, temp);
        loaded_cards++;
    }
    myfile.close();
}

void parseInput() {
    parseInputUtil("_tempa.txt");
    parseInputUtil("_dynamika.txt");
    parseInputUtil("_misc.txt");
    parseInputUtil("_skale.txt");

    for ( size_t i = loaded_cards; i < CARD_ARRAY_SIZE; i++ ) {
        card_array[i].question = "empty";
    }
}

void startGame() {
    long long seed = std::chrono::system_clock::now().time_since_epoch().count();
    shuffle(card_array, card_array + CARD_ARRAY_SIZE, std::default_random_engine(seed));

    string answer;
    char was_correct;
    size_t count = 0;
    size_t correct_count = 0;
    for ( int i = 0; i < CARD_ARRAY_SIZE; i++ ) {
        if ( card_array[i].question != "empty" && card_array[i].correct_answer == false ) {
            count++;
            cout << card_array[i].question << endl;
            cout << "Twoja odpowiedz: ";
            getline(cin, answer);
            cout << "Poprawna odpowiedz: ";
            cout << card_array[i].answer << endl;
            cout << "Typ: ";
            cout << card_array[i].type << endl;
            while ( PromptForChar("Poprawna odpowiedz? [t/n]", was_correct)) {
                if ( was_correct == 't' || was_correct == 'n' ) {
                    break;
                }
            }
            if ( was_correct == 't' ) {
                correct_count++;
                card_array[i].correct_answer = true;
            }
            cout << correct_count << "/" << count << "----------" << count << "/" << loaded_cards << "----------"
                 << endl;
        }
    }
    loaded_cards -= correct_count;
    if ( loaded_cards != 0 ) {
        cout << "Kolejna runda, pozostało " << loaded_cards << " pytań" << endl;
        startGame();
    } else {
        cout << "Udalo sie, wszystkie odpowiedzi byly poprawne!" << endl;
    }
}

int main() {
    initialize();
    parseInput();
    startGame();

    return 0;
}
