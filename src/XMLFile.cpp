#include "XMLFile.h"

#include <iostream>

#include "XMLParser.h"

//load the entire json file into memory at once
static char* loadFile(const std::string& name, uint32_t& size) {
    char* ret;
    std::ifstream inputFile(name, std::ios::in | std::ios::binary | std::ios::ate);
    size = (uint32_t)inputFile.tellg();
    inputFile.seekg(0, std::ios::beg);

    ret = new char[size + 1];
    inputFile.read(ret, size);
    ret[size] = 0;

    inputFile.close();

    return ret;
}

//load the json file and convert to json format
XMLFile::XMLFile(const std::string& name) :
    values(nullptr)
{
    uint32_t size = 0;
    char* fileContents = loadFile(name, size);
    loadXML(fileContents, size);
    delete[] fileContents;
}

//save the json as a file
void XMLFile::save(const std::string& name, bool beautiful) {
    //write contents to outputStream
    std::ofstream outputFile(name, std::ios::out);

    //if the values are null, there was an error loading the file
    if(values != nullptr) {
        //values->createJsonString(outputFile, beautiful);
        //TODO
    }

    outputFile.close();
}

void XMLFile::loadXML(char* fileContents, uint32_t size) {
    values = new XMLNode();
    XMLParser parser(fileContents, size);
    parser.parseXML(values);
}