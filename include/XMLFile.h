#ifndef INCLUDE_XML_FILE_H
#define INCLUDE_XML_FILE_H

#include <string>
#include <ostream>
#include <fstream>
#include "XMLNode.h"

/**
 * Loads a file that follows the XML file format
 * @author Bryce Young 5/27/2021
 * */
class XMLFile {
    public:
        XMLFile(const std::string& fileName);
        ~XMLFile() {
            delete values;
        }

        XMLNode* getHead() {
            return values;
        }

        void save(const std::string& newFile, bool beautify);

    private:
        void loadXML(char* fileContents, uint32_t size);

        XMLNode* values;
};

#endif