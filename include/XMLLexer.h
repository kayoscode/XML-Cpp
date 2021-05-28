#ifndef INCLUDE_XML_LEXER_H
#define INCLUDE_XML_LEXER_H

#include <stdint.h>
#include <string>
#include <iostream>

#define XMLOPENSECTION_CODE 0
#define XMLCLOSESECTION_CODE 1
#define XMLAMPERSAND_CODE 2
#define XMLAPOSTROPE_CODE 3
#define XMLQUOTE_CODE 4
#define XMLSTRING_CODE 5
#define XMLEQUALS_CODE 6
#define XMLESC_APOSTROPE_CODE 8
#define XMLESC_QUOTE_CODE 9
#define XMLESC_AMPERSAND_CODE 10
#define XMLESC_GT_CODE 11
#define XMLESC_LT_CODE 12
#define XMLIDENTIFIER_CODE 13
#define XMLWORD_CODE 14
#define XMLCLOSE_TAG_CODE 15
#define XMLSELFCLOSE_TAG_CODE 16

/**
 * A class for tokenizing an XML file
 * @author Bryce Young 5/27/2021
 * */
class XMLLexer {
    public:
        XMLLexer(char* xmlContents, uint32_t size) 
            :xml(xmlContents), index(0), size(size)
        {
        }

        ~XMLLexer() {

        }

        /**
         * The various kinds of tokens for the XML file format
         * */
        enum class TokenType {
            RESERVE,
            STRING,
            IDENTIFIER,
            EQUALS,
            LESS_THAN,
            GREATER_THAN,
            AMPERSAND,
            APOSTROPHE,
            QUOTE,
            CHARACTER,
            UNDEFINED
        };

        /**
         * A class storing information about an individual XML token
         * @author Bryce Young 5/27/2021
         * */
        struct Token {
            char* begin = nullptr, *end = nullptr;
            TokenType type;
            int code;

            void print() {
                char tmp = *end;
                *end = 0;
                std::cout << begin << "\n";
                *end = tmp;
            }
        };

        bool getNextToken(XMLLexer::Token& token);

    private:
        char* xml;
        uint32_t index;
        uint32_t size;
};

#endif