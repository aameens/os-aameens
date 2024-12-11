#include <iostream>
#include <fstream>
#include <vector>
#include <stdexcept>
#include <unordered_set>
#include <unordered_map>

using namespace std;

void raid0(const vector<string>& mockFile, const string& _split1, const string& _split2)
{
    ofstream split1(_split1);
    ofstream split2(_split2);

    for (size_t i = 0; i < mockFile.size(); ++i)
    {
        if (i % 2 == 0) split1 << mockFile[i] << endl;
        if (i % 2 == 1) split2 << mockFile[i] << endl;
    }

    split1.close();
    split2.close();
    cout << "RAID 0 Complete" << endl;
}

void raid1(const string& mockFile, const string& mirrorFile)
{
    ifstream source(mockFile);
    ofstream mirror(mirrorFile);

    string line = "";
    while (getline(source, line)) mirror << line << endl;

    source.close();
    mirror.close();
    cout << "RAID 1 Completed" << endl;
}

bool readFile(const string& fileName, vector<string>& data)
{
    ifstream file(fileName);
    if (!file.is_open()) return false;

    string line;
    while (getline(file, line)) data.push_back(line);

    file.close();
    return true;
}

void reconstructFile(vector<string>& reconstructedArray)
{
    // File names
    const string split1File = "split1.txt";
    const string split2File = "split2.txt";
    const string mirror1File = "mirror1.txt";
    const string mirror2File = "mirror2.txt";

    // Buffers to hold data from files
    vector<string> split1Data;
    vector<string> split2Data;
    vector<string> mirror1Data;
    vector<string> mirror2Data;

    // Read data from files
    bool split1Exists = readFile(split1File, split1Data);
    bool split2Exists = readFile(split2File, split2Data);
    bool mirror1Exists = readFile(mirror1File, mirror1Data);
    bool mirror2Exists = readFile(mirror2File, mirror2Data);

    // Reconstruct split1.txt if missing using mirror1.txt
    if (!split1Exists && mirror1Exists)
    {
        ofstream split1(split1File);
        for (const string& line : mirror1Data)
        {
            split1 << line << endl;
            split1Data.push_back(line);
        }
        split1.close();
        cout << "Recovered " << split1File << " from " << mirror1File << "." << endl;
    }

    // Reconstruct mirror1.txt if missing using split1.txt
    if (split1Exists && !mirror1Exists)
    {
        ofstream mirror1(mirror1File);
        for (const string& line : split1Data)
        {
            mirror1 << line << endl;
            mirror1Data.push_back(line);
        }
        mirror1.close();
        cout << "Recovered " << mirror1File << " from " << split1File << "." << endl;
    }

    // Reconstruct split2.txt if missing using mirror2.txt
    if (!split2Exists && mirror2Exists)
    {
        ofstream split2(split2File);
        for (const string& line : mirror2Data)
        {
            split2 << line << endl;
            split2Data.push_back(line);
        }
        split2.close();
        cout << "Recovered " << split2File << " from " << mirror2File << "." << endl;
    }

    // Reconstruct mirror2.txt if missing using split2.txt
    if (split2Exists && !mirror2Exists)
    {
        ofstream mirror2(mirror2File);
        for (const string& line : split2Data)
        {
            mirror2 << line << endl;
            mirror2Data.push_back(line);
        }
        mirror2.close();
        cout << "Recovered " << mirror2File << " from " << split2File << "." << endl;
    }

    // Check if reconstruction is possible
    if (!split1Exists && !mirror1Exists)
    {
        throw runtime_error("Error: Unable to reconstruct " + split1File + " and " + mirror1File + ".");
    }
    if (!split2Exists && !mirror2Exists)
    {
        throw runtime_error("Error: Unable to reconstruct " + split2File + " and " + mirror2File + ".");
    }

    // Combine split1 and split2 data into the original order
    size_t maxSize = max(split1Data.size(), split2Data.size());
    for (size_t i = 0; i < maxSize; ++i)
    {
        if (i < split1Data.size())
            reconstructedArray.push_back(split1Data[i]);
        if (i < split2Data.size())
            reconstructedArray.push_back(split2Data[i]);
    }

    cout << "Reconstruction successful. Original array has been rebuilt." << endl;
}

void raid10(const vector<string>& mockFile)
{
    // Perform RAID 0
    const string split1 = "split1.txt";
    const string split2 = "split2.txt";
    raid0(mockFile, split1, split2);

    // Perform RAID 1
    const string mirror1 = "mirror1.txt";
    const string mirror2 = "mirror2.txt";
    raid1(split1, mirror1);
    raid1(split2, mirror2);
    cout << "RAID 10 Complete; Data split and mirrored to files." << endl;
}

void cleanFiles()
{
    // Remove the RAID 10 files
    remove("split1.txt");
    remove("split2.txt");
    remove("mirror1.txt");
    remove("mirror2.txt");
    cout << "All RAID 10 files have been cleaned up." << endl;
}

int main(int argc, char* argv[])
{
    // Validate arguments
    if (argc < 2)
    {
        cerr << "Please provide a valid operation: raid10, reconstruct, clean" << endl;
        return 1;
    }

    vector<string> mockFile = \
    { // Each string is a "data block"
        "The",
        "Quick",
        "Brown",
        "Fox",
        "Jumped",
        "Over",
        "The",
        "Lazy",
        "Dog"
    };

    string operation = argv[1];

    // Perform RAID 10 operation
    if (operation == "raid10")
    {
        cout << "Performing RAID 10 on Mock File:" << endl;
        raid10(mockFile);
    }
    // Reconstruct the files
    else if (operation == "reconstruct")
    {
        cout << "Reconstructing Mock File from RAID 10 Files:" << endl;
        vector<string> reconstructedFile;
        try
        {
            reconstructFile(reconstructedFile);
            for (const string& item : reconstructedFile) cout << item << " ";
            cout << endl;
        }
        catch (const runtime_error& e)
        {
            cerr << e.what() << endl;
        }
    }
    // Clean the output files
    else if (operation == "clean")
    {
        cleanFiles();
    }
    else
    {
        cerr << "Invalid operation: " << operation << endl;
        return 1;
    }

    return 0;
}
