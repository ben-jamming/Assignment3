//Ben Hepditch
//260976774

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <list>
#include <sstream>

using namespace std;

// Use a struct to store information about the file versions
struct Version {
    int version_number;
    string hash_value;
    string content;
};

list<Version> version_list;

// Read the contents of file.txt
std::string read_file() {
    
    std::ifstream file("file.txt");
    std::string content;
    // Open and save the contents as a string, unless the file doesn't exist
    if (file.is_open()) {
        std::string line;
        while (std::getline(file, line)) {
            content += line;
            content.push_back('\n');
        }
        file.close();
    } else {
        std::cout << "Unable to open file: " << "file.txt" << std::endl;
    }

    return content;
}

// Define the functions available to the user
void add(string content);
void remove(int version);
void load(int version);
void print(void);
void compare(int version1, int version2);
void search(string keyword);

// Add a new version to the list of versions
void add(string content) {
    static int version_count = 0;
    string filename = "file.txt";
    string file_content;

    // Read the contents of the file
    ifstream file(filename);
    if (file.is_open()) {
        file_content.assign((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());
        file.close();
    } else {
        cout << "Unable to open the file." << endl;
        return;
    }
    // Check if the file has changed since the last version
    if (version_count > 0 && version_list.back().content == file_content) {
        cout << "git322 did not detect any change to your file and will not create a new version." << endl;
    } else {
        // If it has changed, create a new version
        version_count++;
        Version new_version;
        // Initialize the new version with its version number, hash value, and content
        new_version.version_number = version_count;
        new_version.hash_value = to_string(hash<string>{}(file_content)); // Generate a simple hash value
        new_version.content = file_content;
        // Add the new version to the list of versions
        version_list.push_back(new_version);
        cout << "Your content has been added successfully." << endl;
    }
}

// Remove a version from the list of versions
void remove(int version) {
    bool version_found = false;

    // Search through the list of versions for the specified version number
    list<Version>::iterator it;
    for (it = version_list.begin(); it != version_list.end(); ++it) {
        if (it->version_number == version) {
            // If a version with the matching version number is found, remove it from the list
            version_list.erase(it);
            version_found = true;
            cout << "Version " << version << " deleted successfully." << endl;
            break;
        }
    }

    if (!version_found) {
        // If no version with the matching version number is found, print an error message to the user
        cout << "Please enter a valid version number." << endl;
    } else {
        // Reassign version numbers
        int new_version_number = 1;
        for (it = version_list.begin(); it != version_list.end(); ++it) {
            it->version_number = new_version_number;
            new_version_number++;
        }
    }
}


// Load a previous version into file.txt
void load(int version) {
    string filename = "file.txt";
    bool version_found = false;
    // Check all the previous versions for the matching version number
    for (const Version& v : version_list) {
        if (v.version_number == version) {
            // If a version with the matching version number is found, write its content to file.txt
            ofstream file(filename);
            if (file.is_open()) {
                file << v.content;
                file.close();
                version_found = true;
                cout << "Version " << version << " loaded successfully. Please refresh your text editor to see the changes." << endl;
            } else {
                // If file.txt cannot be opened, print an error message to the user
                cout << "Unable to open the file." << endl;
            }
            break;
        }
    }

    if (!version_found) {
        // If no version with the matching version number is found, print an error message to the user
        cout << "Please enter a valid version number. If you are not sure please press 'p' to list all valid version numbers." << endl;
    }
}

// Print the current list of versions
void print() {
    cout << "Number of versions: " << version_list.size() << endl;

    // Loop through the list of versions and print their version number, hash value, and content
    for (const Version& v : version_list) {
        cout << "Version number: " << v.version_number << endl;
        cout << "Hash value: " << v.hash_value << endl;
        cout << "Content: " << v.content << endl;
        cout << endl;
    }
}

// Compare two versions of file.txt
void compare(int version1, int version2) {
    string content1, content2;

    // Find the content of the specified versions in the list of versions
    for (list<Version>::const_iterator it = version_list.begin(); it != version_list.end(); ++it) {
        if (it->version_number == version1) {
            content1 = it->content;
        }
        if (it->version_number == version2) {
            content2 = it->content;
        }
    }

    if (content1.empty() || content2.empty()) {
        cout << "Please enter valid version numbers." << endl;
        return;
    }

    // Split content into lines
    vector<string> lines1, lines2;
    istringstream iss1(content1), iss2(content2);
    string line;

    // Load and store each line of each file separately
    while (getline(iss1, line)) {
        lines1.push_back(line);
    }

    while (getline(iss2, line)) {
        lines2.push_back(line);
    }

    // Compare the lines of the two versions
    int max_lines = max(lines1.size(), lines2.size());

    // Loop through the lines of the two versions and print the differences
    for (int i = 0; i < max_lines; ++i) {
        bool inLines1 = i < lines1.size();
        bool inLines2 = i < lines2.size();

        // If both versions have the same number of lines, compare the lines
        if (inLines1 && inLines2) {
            string comparison = lines1[i] == lines2[i] ? "<Identical>" : lines1[i] + " <<>> " + lines2[i];
            cout << "Line " << (i + 1) << ": " << comparison << endl;
        // If one of the versions has fewer lines than the other, print "<Empty line>"
        } else if (inLines1) {
            cout << "Line " << (i + 1) << ": " << lines1[i] << " <<>> <Empty line>" << endl;
        } else {
            cout << "Line " << (i + 1) << ": <Empty line> <<>> " << lines2[i] << endl;
        }
    }
}


// Search for a keyword in all versions saved in the system
void search(const string keyword) {
    // Keep track of whether the keyword was found in any version
    bool found = false;

    cout << "The keyword '" << keyword << "' has been found in the following versions:" << endl;
    // Loop through all versions and print the ones that contain the keyword
    for (const Version& v : version_list) {
        if (v.content.find(keyword) != string::npos) {
            found = true;
            cout << "Version number: " << v.version_number << endl;
            cout << "Hash value: " << v.hash_value << endl;
            cout << "Content: " << v.content << endl;
            cout << endl;
        }
    }
    // If the keyword was not found, indicate so to the user
    if (!found) {
        cout << "Your keyword '" << keyword << "' was not found in any version." << endl;
    }
}

int main() {
    // Welcome message
    cout << "Welcome to the Comp322 file versioning system!" << endl;
    cout << "To add the content of your file to version control press 'a'" << endl;
    cout << "To remove a version press 'r'" << endl;
    cout << "To load a version press 'l'" << endl;
    cout << "To print to the screen the detailed list of all versions press 'p'" << endl;
    cout << "To compare any 2 versions press 'c'" << endl;
    cout << "To search versions for a keyword press 's'" << endl;
    cout << "To exit press 'e'" << endl;
    
    char option;
    while (true) {
        // Run program until user exits, and ask for input
        cout << "Enter your option: ";
        cin >> option;
        

        switch (option) {
            case 'a': {
                // Add the content of the file to version control
                string content = read_file();
                add(content);
                break;
            }
            case 'r': {
                // Remove a version
                int version;
                cout << "Enter the number of the version that you want to delete: ";
                cin >> version;
                remove(version);
                break;
            }
            case 'l': {
                // Load a version
                int version;
                cout << "Which version would you like to load? ";
                cin >> version;
                load(version);
                break;
            }
            case 'p': {
                // Print the detailed list of all versions
                print();
                break;
            }
            case 'c': {
                // Compare any 2 versions
                int version1, version2;
                cout << "Please enter the number of the first version to compare: ";
                cin >> version1;
                cout << "Please enter the number of the second version to compare: ";
                cin >> version2;
                compare(version1, version2);
                break;
            }
            case 's': {
                // Implement search functionality
                string keyword;
                cout << "Please enter the keyword that you are looking for: ";
                cin.ignore();
                getline(cin, keyword);
                search(keyword);
                break;
            }
            case 'e': {
                // Exit
                return 0;
            }
            default: {
                cout << "Invalid option. Please try again." << endl;
                break;
            }
        }
    }
    return 0;
}
