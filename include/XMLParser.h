#ifndef INCLUDE_XML_PARSER_H
#define INCLUDE_XML_PARSER_H

#include "XMLNode.h"
#include "XMLLexer.h"

/**
 * Class that parses XML into an XML node
 * contains invalid data if xml is invalid
 * */
class XMLParser {
    public:
        XMLParser(char* xml, uint32_t size);
        ~XMLParser();

        void parseXML(XMLNode* object);

    private:
        XMLLexer lexer;
};

#endif