#include <iostream>
#include <string>
#include <fstream>
using namespace std;

class Song {
private:
    string title;
    string artist;
    Song* next;
    Song* prev;

    friend class Playlist;

public:
    Song(const string& t, const string& a) : title(t), artist(a), next(nullptr), prev(nullptr) {}
};

class Playlist {
private:
    Song* head;
    Song* tail;
    Song* current;

public:
    Playlist() : head(nullptr), tail(nullptr), current(nullptr) {}

    ~Playlist() {
        Song* temp = head;
        while (temp) {
            Song* next = temp->next;
            delete temp;
            temp = next;
        }
    }

    void loadFromFile(const string& filename) {
        ifstream file(filename);
        if (!file) {
            cout << "No existing playlist found. Starting a new one.\n";
            return;
        }

        string title, artist;
        while (getline(file, title) && getline(file, artist)) {
            addSong(title, artist, false);
        }

        file.close();
        current = head;
    }

    void saveToFile(const string& filename) {
        ofstream file(filename);
        Song* temp = head;
        while (temp) {
            file << temp->title << "\n" << temp->artist << "\n";
            temp = temp->next;
        }
        file.close();
    }

    void addSong(const string& title, const string& artist, bool save = true) {
        Song* newSong = new Song(title, artist);
        if (!head) {
            head = tail = current = newSong;
        } else {
            tail->next = newSong;
            newSong->prev = tail;
            tail = newSong;
        }
        cout << "Added: " << title << " by " << artist << endl;
        if (save) saveToFile("playlist.txt");
    }

    void playCurrent() {
        if (current) {
            cout << "Playing: " << current->title << " by " << current->artist << endl;
        } else {
            cout << "No song is currently selected.\n";
        }
    }

    void nextSong() {
        if (current && current->next) {
            current = current->next;
            playCurrent();
        } else {
            cout << "You are at the end of the playlist.\n";
        }
    }

    void prevSong() {
        if (current && current->prev) {
            current = current->prev;
            playCurrent();
        } else {
            cout << "You are at the beginning of the playlist.\n";
        }
    }

    void deleteCurrentSong() {
        if (!current) {
            cout << "No song to delete.\n";
            return;
        }

        cout << "Deleting: " << current->title << " by " << current->artist << endl;

        if (current == head) head = head->next;
        if (current == tail) tail = tail->prev;
        if (current->prev) current->prev->next = current->next;
        if (current->next) current->next->prev = current->prev;

        Song* temp = current;
        current = current->next ? current->next : current->prev;
        delete temp;

        saveToFile("playlist.txt");
    }

    void showPlaylist() {
        if (!head) {
            cout << "Playlist is empty.\n";
            return;
        }
        cout << "Playlist:\n";
        Song* temp = head;
        int idx = 1;
        while (temp) {
            cout << idx++ << ". " << temp->title << " by " << temp->artist;
            if (temp == current) cout << " <-- current";
            cout << endl;
            temp = temp->next;
        }
    }
};

int main() {
    Playlist myPlaylist;
    myPlaylist.loadFromFile("playlist.txt");

    int choice;
    string title, artist;

    do {
        cout << "\n--- Music Playlist Manager ---\n";
        cout << "1. Add Song\n2. Play Current Song\n3. Next Song\n4. Previous Song\n5. Delete Current Song\n6. Show Playlist\n0. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;
        cin.ignore();

        switch (choice) {
            case 1:
                cout << "Enter song title: ";
                getline(cin, title);
                cout << "Enter artist name: ";
                getline(cin, artist);
                myPlaylist.addSong(title, artist);
                break;
            case 2:
                myPlaylist.playCurrent();
                break;
            case 3:
                myPlaylist.nextSong();
                break;
            case 4:
                myPlaylist.prevSong();
                break;
            case 5:
                myPlaylist.deleteCurrentSong();
                break;
            case 6:
                myPlaylist.showPlaylist();
                break;
            case 0:
                cout << "Goodbye!" << endl;
                break;
            default:
                cout << "Invalid choice.\n";
        }
    } while (choice != 0);

    return 0;
}
