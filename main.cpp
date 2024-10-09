#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <limits>

using namespace std;

struct song_item {
    string title = "";
    string artist = "";
    string album = "";
    int year = 0;

    song_item() : title(""), artist(""), album(""), year(0) {}

    song_item(string aTitle, string aArtist, string aAlbum, int aYear) :
    title(aTitle), artist(aArtist), album(aAlbum), year(aYear) {}
};

class MusicSpace {
private:
    struct song_item* musicLibrary;
    int numSongs;
    int maxSongs;

public:
    // Constructor
    MusicSpace(int maxSongs = 20) : numSongs(0), maxSongs(maxSongs) {
        musicLibrary = new song_item[maxSongs];
        loadFromFile(); // Load from file when initializing
    }

    // Destructor
    ~MusicSpace() {
        saveToFile(); // Save to file before destruction
        delete[] musicLibrary;
    }

    // Resize the dynamic array
    void resize() {
        maxSongs *= 2;
        song_item* newLibrary = new song_item[maxSongs];
        for (int i = 0; i < numSongs; i++) {
            newLibrary[i] = musicLibrary[i];
        }
        delete[] musicLibrary;
        musicLibrary = newLibrary;
    }

    // Add a new song to the library
    void addMusic(const song_item& newSong) {
        if (numSongs == maxSongs) {
            resize();
        }
        musicLibrary[numSongs++] = newSong;
    }

    // Remove a song by its index
    void removeSong(int index) {
        if (index >= 0 && index < numSongs) {
            for (int i = index; i < numSongs - 1; ++i) {
                musicLibrary[i] = musicLibrary[i + 1];
            }
            --numSongs;
        } else {
            cout << "Invalid index!" << endl;
        }
    }

    // Show all songs in the library
    void display() const {
        for (int i = 0; i < numSongs; ++i) {
            cout << endl << "Title: " << musicLibrary[i].title
                 << ", Artist: " << musicLibrary[i].artist
                 << ", Album: " << musicLibrary[i].album
                 << ", Year: " << musicLibrary[i].year << endl;
        }
    }

    // Show menu options
    void showOptions() const {
        cout << endl << "===== Music Library =====" << endl;
        cout << "1. Add a song" << endl;
        cout << "2. Remove a song" << endl;
        cout << "3. Show library" << endl;
        cout << "4. Quit" << endl;
        cout << "Choose an number: ";
    }

    // Save song data to a file
    void saveToFile() const {
        ofstream outputFile("songs.csv");
        if (outputFile.is_open()) {
            for (int i = 0; i < numSongs; ++i) {
                outputFile << musicLibrary[i].title << ","
                        << musicLibrary[i].artist << ","
                        << musicLibrary[i].album << ","
                        << musicLibrary[i].year << endl;
            }
            outputFile.close();
        } else {
            cout << "Can't open file." << endl;
        }
    }

    // Load song data from a file
    void loadFromFile() {
        ifstream inputFile("songs.csv");
        if (inputFile.is_open()) {
            string line;
            while (getline(inputFile, line)) {
                stringstream stream(line);
                string title, artist, album;
                int year;

                getline(stream, title, ',');
                getline(stream, artist, ',');
                getline(stream, album, ',');
                stream >> year;

                // Clear the rest of the line to prepare for the next read
                stream.ignore(numeric_limits<streamsize>::max(), '\n');

                struct song_item newSong(title, artist, album, year);
                addMusic(newSong);
            }
            inputFile.close();
        } else {
            cout << "Data not found. Enter data into library!" << endl;
        }
    }
};

    // Function to create a new song by taking user input
    struct song_item createSong() {
        string title, artist, album;
        int year;

        cout << "Enter title: "; cin.ignore(); getline(cin, title);
        cout << "Enter artist: "; getline(cin, artist);
        cout << "Enter album: "; getline(cin, album);
        cout << "Enter year: "; cin >> year;
        return song_item(title, artist, album, year);
    }

    // Function to manage the switch-case menu logic
    void manageMenu(MusicSpace &library) {
        int option;
        do {
            library.showOptions();
            cin >> option;
            switch (option) {
                case 1: {
                    struct song_item newSong = createSong();
                    library.addMusic(newSong);
                    break;
                }
                case 2: {
                    int index;
                    cout << "Enter the index of the song to remove: ";
                    cin >> index;
                    library.removeSong(index - 1);  // Adjust for 0-based index
                    break;
                }
                case 3: {
                    library.display();
                    break;
                }
                case 4:
                    cout << "Exiting..." << endl;
                    break;
                default:
                    cout << "Invalid option. Please try again." << endl;
            }
        } while (option != 4);
    }



int main() {
    MusicSpace library;
    manageMenu(library);  // Call the menu function
    return 0;
}
