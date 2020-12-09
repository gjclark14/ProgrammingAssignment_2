//
// Created by gc on 11/16/20.
//
#include <random>
#include <sys/stat.h>
#include "fstream"


namespace Utilities {
    const int MAX = 20;
    const int MIN = 11;
    const char LOG[] = "log.txt";

    void generateSparseMatrix(const std::string& fileName = "inputMatrix.txt");

    bool exists(const std::string& fileName);

    bool isLogged(const std::string& fileName);

    void logCreatedFile(const std::string& fileName);

    void clearLoggedFiles();

    std::vector<std::string> getLoggedFiles();

}

void Utilities::generateSparseMatrix(const std::string &fileName) {

    std::ofstream fout;
    fout.open(fileName.c_str());

    if(!isLogged(fileName))
        logCreatedFile(fileName);

    std::random_device rd;      // obtain a random number from hardware
    std::mt19937 gen(rd());     // seed the generator
    std::uniform_int_distribution<> max_range(Utilities::MIN, Utilities::MAX);   // inclusive

    const int M = max_range(gen);
    const int N = max_range(gen);

    std::uniform_int_distribution<> symbol_range(0,9); // i don't want two digit numbers

    for(int i = 0; i < M; i++) {
        for(int j = 0; j < N; j++) {
            int symbol = symbol_range(gen);
            symbol >= 7 ? fout << symbol_range(gen) << ' ' : fout << "0 ";
        }
        fout << '\n';
    }
    fout.close();
}

bool Utilities::exists(const std::string& fileName) {
    struct stat buffer;

    return (stat(fileName.c_str(), &buffer) == 0);

}

bool Utilities::isLogged(const std::string& fileName) {
    std::ifstream fin;
    fin.open(LOG);
    std::string loggedFile;
    while (getline(fin, loggedFile)){
        if(loggedFile == fileName) {
            fin.close();
            return true;
        }
    }

    fin.close();
    return false;
}

void Utilities::logCreatedFile(const std::string& fileName){
    std::ofstream fout;
    fout.open(LOG, std::ios::app);
    fout << fileName << '\n';
    fout.close();
}

void Utilities::clearLoggedFiles() {
    std::vector<std::string> loggedFiles = getLoggedFiles();
    for(const std::string& loggedFile: loggedFiles) {
        if(exists(loggedFile)) {
            std::remove(loggedFile.c_str());
        }
    }
    std::remove(LOG);

}

std::vector<std::string> Utilities::getLoggedFiles() {
    std::ifstream fin;
    fin.open(LOG);
    std::string loggedFile;
    std::vector<std::string> loggedFiles;
    while (getline(fin, loggedFile)){
        loggedFiles.push_back(loggedFile);
    }
    return loggedFiles;
}
