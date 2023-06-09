// Ben Hepditch
// 260976774

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <list>
#include <sstream>

using namespace std;

class Version
{
public:
    Version(int pVersionNumber, string pContent)
        : aVersionNumber(pVersionNumber), aContent(pContent), aHashCode(hash_it(pContent)), next(nullptr), prev(nullptr)
    {
    }

    // Method to hash a string
    std::size_t hash_it(std::string someString)
    {
        std::hash<std::string> hashCode;
        return hashCode(someString);
    }

    // Getter methods
    int getVersionNumber()
    {
        return this->aVersionNumber;
    }
    string getContent()
    {
        return this->aContent;
    }
    size_t getHashCode()
    {
        return this->aHashCode;
    }
    Version *getNext()
    {
        return this->next;
    }
    Version *getPrev()
    {
        return this->prev;
    }

    // Setter methods
    void setVersionNumber(int pVersionNumber)
    {
        this->aVersionNumber = pVersionNumber;
    }
    void setContent(string pContent)
    {
        this->aContent = pContent;
    }
    void setNext(Version *pNext)
    {
        this->next = pNext;
    }
    void setPrev(Version *pPrev)
    {
        this->prev = pPrev;
    }

private:
    int aVersionNumber;
    string aContent;
    size_t aHashCode;
    Version *next;
    Version *prev;
};

class VersionLinkedList
{
public:

    VersionLinkedList()
        : head(nullptr), tail(nullptr), versionCount(0)
    {
    }

    // Getter methods
    int getVersionCount()
    {
        return this->versionCount;
    }

    // Setter methods
    void updateVersionCount()
    {
        this->versionCount++;
    }

    void addVersion(string content)
    {
        string newVersionContent;
        // Read the contents of the file
        ifstream file("file.txt");
        if (file.is_open())
        {
            newVersionContent.assign((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());
            file.close();
        }
        else
        {
            cout << "Unable to open the file." << endl;
            return;
        }

        // Check if the file has changed since the last version
        if (this->getVersionCount() > 0 && this->tail->getContent() == newVersionContent)
        {
            cout << "git322 did not detect any change to your file and will not create a new version." << endl;
        }
        else
        {
            // If it has changed, create a new version
            updateVersionCount();
            // Initialize the new version with its version number, hash value, and content
            Version *newVersion = new Version(this->getVersionCount(), newVersionContent);

            // Add the new version to the linked list
            if (this->head == NULL)
            {
                this->head = newVersion;
            }
            else
            {
                this->tail->setNext(newVersion);
            }
            this->tail = newVersion;

            cout << "Your content has been added successfully." << endl;
        }
    }

    void removeVersion(int version)
    {
        bool version_found = false;
        Version *current = this->head;
        Version *previous = NULL;

        // Search through the linked list for the specified version number
        while (current != NULL)
        {
            if (current->getVersionNumber() == version)
            {
                // If a version with the matching version number is found, remove it from the list
                if (previous == NULL) // If the head node is the one to be removed
                {
                    this->head = current->getNext();
                    if (this->tail == current)
                    {
                        this->tail = NULL;
                    }
                }
                else
                {
                    previous->setNext(current->getNext());
                    if (this->tail == current)
                    {
                        this->tail = previous;
                    }
                }
                delete current;
                version_found = true;
                cout << "Version " << version << " deleted successfully." << endl;
                break;
            }
            previous = current;
            current = current->getNext();
        }

        if (!version_found)
        {
            // If no version with the matching version number is found, print an error message to the user
            cout << "Please enter a valid version number." << endl;
        }
    }

    void loadVersion(int version)
    {
        string filename = "file.txt";
        bool version_found = false;
        Version *current = this->head;

        // Check all the previous versions for the matching version number
        while (current != NULL)
        {
            if (current->getVersionNumber() == version)
            {
                // If a version with the matching version number is found, write its content to file.txt
                ofstream file(filename);
                if (file.is_open())
                {
                    file << current->getContent();
                    file.close();
                    version_found = true;
                    cout << "Version " << version << " loaded successfully. Please refresh your text editor to see the changes." << endl;
                }
                else
                {
                    // If file.txt cannot be opened, print an error message to the user
                    cout << "Unable to open the file." << endl;
                }
                break;
            }
            current = current->getNext();
        }

        if (!version_found)
        {
            // If no version with the matching version number is found, print an error message to the user
            cout << "Please enter a valid version number. If you are not sure please press 'p' to list all valid version numbers." << endl;
        }
    }
    void printVersionList(void)
    {
        cout << "Number of versions: " << versionCount << endl;
        int count = 0;
        Version *current = this->head;

        // Count the number of versions and print their version number, hash value, and content
        while (current != NULL)
        {
            count++;
            cout << "Version number: " << current->getVersionNumber() << endl;
            cout << "Hash value: " << current->getHashCode() << endl;
            cout << "Content: " << current->getContent() << endl;
            cout << endl;

            current = current->getNext();
        }

        
    }

    void compareVersions(int version1, int version2)
    {
        string content1, content2;

        // Find the content of the specified versions in the linked list
        Version *current = this->head;
        while (current != NULL)
        {
            if (current->getVersionNumber() == version1)
            {
                content1 = current->getContent();
            }
            if (current->getVersionNumber() == version2)
            {
                content2 = current->getContent();
            }
            current = current->getNext();
        }

        if (content1.empty() || content2.empty())
        {
            cout << "Please enter valid version numbers." << endl;
            return;
        }
        // Split content into lines
        vector<string> lines1, lines2;
        istringstream iss1(content1), iss2(content2);
        string line;

        // Load and store each line of each file separately
        while (getline(iss1, line))
        {
            lines1.push_back(line);
        }

        while (getline(iss2, line))
        {
            lines2.push_back(line);
        }

        // Compare the lines of the two versions
        int maxLines = max(lines1.size(), lines2.size());

        // Loop through the lines of the two versions and print the differences
        for (int i = 0; i < maxLines; ++i)
        {
            bool inLines1 = i < lines1.size();
            bool inLines2 = i < lines2.size();

            // If both versions have the same number of lines, compare the lines
            if (inLines1 && inLines2)
            {
                string comparison = lines1[i] == lines2[i] ? "<Identical>" : lines1[i] + " <<>> " + lines2[i];
                cout << "Line " << (i + 1) << ": " << comparison << endl;
                // If one of the versions has fewer lines than the other, print "<Empty line>"
            }
            else if (inLines1)
            {
                cout << "Line " << (i + 1) << ": " << lines1[i] << " <<>> <Empty line>" << endl;
            }
            else
            {
                cout << "Line " << (i + 1) << ": <Empty line> <<>> " << lines2[i] << endl;
            }
        }
    }
    void searchVersionList(string keyword)
    {
        // Keep track of whether the keyword was found in any version
        bool found = false;

        cout << "The keyword '" << keyword << "' has been found in the following versions:" << endl;
        // Loop through all versions in the linked list and print the ones that contain the keyword
        Version *current = this->head;
        while (current != NULL)
        {
            if (current->getContent().find(keyword) != string::npos)
            {
                found = true;
                cout << "Version number: " << current->getVersionNumber() << endl;
                cout << "Hash value: " << current->getHashCode() << endl;
                cout << "Content: " << current->getContent() << endl;
                cout << endl;
            }
            current = current->getNext();
        }
        // If the keyword was not found, indicate so to the user
        if (!found)
        {
            cout << "Your keyword '" << keyword << "' was not found in any version." << endl;
        }
    }

private:
    Version *head;
    Version *tail;
    int versionCount;
};

class Git322
{
    // Version class
public:

    VersionLinkedList *versionList;
    Git322()
    {
        versionList = new VersionLinkedList();
    }

    // Read the contents of file.txt
    std::string read_file()
    {

        std::ifstream file("file.txt");
        std::string content;
        // Open and save the contents as a string, unless the file doesn't exist
        if (file.is_open())
        {
            std::string line;
            while (std::getline(file, line))
            {
                content += line;
                content.push_back('\n');
            }
            file.close();
        }
        else
        {
            std::cout << "Unable to open file: "
                      << "file.txt" << std::endl;
        }

        return content;
    }

    // Add a new version to the list of versions
    void add(string content)
    {
        // Add a new version to the linked list
        this->versionList->addVersion(content);  
    }
    void remove(int version)
    {
        // Remove the version with the matching version number from the linked list
        this->versionList->removeVersion(version);
    }
    void load(int version)
    {
        // Loadd the version with the matching version number from the linked list
        this->versionList->loadVersion(version);
    }
    void print(void)
    {
        // Print all versions in the linked list
        this->versionList->printVersionList();
    }
    void compare(int version1, int version2)
    {
        // Compare the versions with the matching version numbers from the linked list
        this->versionList->compareVersions(version1, version2);
    }
    void search(string keyword)
    {
        // Search for the keyword in the linked list
        this->versionList->searchVersionList(keyword);
    }

};

int main()
{
    // Initialize a new Git322 object
    Git322 git322 = Git322();

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
    while (true)
    {
        // Run program until user exits, and ask for input
        cout << "Enter your option: ";
        cin >> option;

        switch (option)
        {
        case 'a':
        {
            // Add the content of the file to version control
            string content = git322.read_file();
            git322.add(content);
            break;
        }
        case 'r':
        {
            // Remove a version
            int version;
            cout << "Enter the number of the version that you want to delete: ";
            cin >> version;
            git322.remove(version);
            break;
        }
        case 'l':
        {
            // Load a version
            int version;
            cout << "Which version would you like to load? ";
            cin >> version;
            git322.load(version);
            break;
        }
        case 'p':
        {
            // Print the detailed list of all versions
            git322.print();
            break;
        }
        case 'c':
        {
            // Compare any 2 versions
            int version1, version2;
            cout << "Please enter the number of the first version to compare: ";
            cin >> version1;
            cout << "Please enter the number of the second version to compare: ";
            cin >> version2;
            git322.compare(version1, version2);
            break;
        }
        case 's':
        {
            // Implement search functionality
            string keyword;
            cout << "Please enter the keyword that you are looking for: ";
            cin.ignore();
            getline(cin, keyword);
            git322.search(keyword);
            break;
        }
        case 'e':
        {
            // Exit
            return 0;
        }
        default:
        {
            cout << "Invalid option. Please try again." << endl;
            break;
        }
        }
    }

    return 0;
}