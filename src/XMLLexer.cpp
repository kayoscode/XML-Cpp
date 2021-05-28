#include "XMLLexer.h"
#include <map>
#include <iostream>

static std::map<char, int> reserves {
    { '<',  XMLOPENSECTION_CODE },
    { '>', XMLCLOSESECTION_CODE },
    { '=', XMLEQUALS_CODE },
    { '&', XMLAMPERSAND_CODE },
    { '\'', XMLAPOSTROPE_CODE },
    { '"', XMLQUOTE_CODE },
};

static bool isEOF(uint32_t index, uint32_t size) {
    return index >= size;
}

static bool isWhiteSpace(char ch) {
    return ch == ' ' || ch == '\n' || ch == '\r' || ch == 9;
}

static void skipWhiteSpace(char* json, uint32_t& index, uint32_t size) {
    char ch = json[index];

    while(isWhiteSpace(ch) && !isEOF(index, size)) {
        index++;
        ch = json[index];
    }
}

static bool isAlpha(char ch) {
    return (ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z');
}

static bool isNum(char ch) {
    return ch >= '0' && ch <= '9';
}

static bool isAlphaNum(char ch) {
    return isAlpha(ch) || isNum(ch);
}

static bool loadTwoChar(char* xml, uint32_t& index, uint32_t size, XMLLexer::Token* newToken) {
    if(xml[index] == '<' && xml[index + 1] == '/') {
        index += 2;
        newToken->begin = xml + index - 2;
        newToken->end = xml + index;
        newToken->code = XMLCLOSE_TAG_CODE;

        return true;
    }
    else if(xml[index] == '/' && xml[index] == '>') {
        index += 2;
        newToken->begin = xml + index - 2;
        newToken->end = xml + index;
        newToken->code = XMLSELFCLOSE_TAG_CODE;

        return true;
    }

    return false;
}

static bool loadReserve(char* xml, uint32_t& index, uint32_t size, XMLLexer::Token* newToken) {
    int saveIndex = index;
    char ch = xml[index];
    newToken->begin = xml + index;
    index++;
    
    std::map<char, int>::iterator res = reserves.find(ch);

    if(res != reserves.end()) {
        newToken->code = res->second;
        newToken->type = XMLLexer::TokenType::RESERVE;
        newToken->end = xml + index;

        return true;
    }


    index = saveIndex;
    return false;
}

//loads till it finds a space or special character
static bool loadWord(char* xml, uint32_t& index, uint32_t size, XMLLexer::Token* token) {
    uint32_t startIndex = index;
    bool isIdentifier = xml[index] == '_' || isAlpha(xml[index]);

    while(!isWhiteSpace(xml[index]) && reserves.find(xml[index]) == reserves.end()) {
        if(!(isAlphaNum(xml[index]) || xml[index] == '-' || xml[index] == '.' || xml[index] == '_')) {
            isIdentifier = false;
        }

        index++;
    }

    if(index - startIndex >= 3) {
        if(std::toupper(xml[startIndex] == 'X') && std::toupper(xml[startIndex + 1] == 'M') && std::toupper(xml[startIndex + 2] == 'L')) {
            isIdentifier = false;
        }
    }

    if(startIndex != index) {
        token->begin = xml + startIndex;
        token->end = xml + index;

        if(isIdentifier) {
            token->code = XMLIDENTIFIER_CODE;
        }
        else {
            token->code = XMLWORD_CODE;
        }
    }
    else {
        token->begin = nullptr;
        token->end = nullptr;
    }

    return true;
}

static bool loadEscapeSequence(char* xml, uint32_t& index, uint32_t size, XMLLexer::Token* newToken) {
    int saveIndex = index;

    if(xml[index] == '&') {
        index++;
        if(size - index >= 5) {
            //if its a quote escape sequence
            if(xml[index] == 'q' && xml[index + 1] == 'u' && xml[index + 2] == 'o' && xml[index + 3] == 't' && xml[index + 4] == ';') {
                index += 5;
                newToken->begin = xml + saveIndex;
                newToken->end = xml + index;
                newToken->code = XMLESC_QUOTE_CODE;
                return true;
            }
            else if(xml[index] == 'a' && xml[index + 1] == 'p' && xml[index + 2] == 'o' && xml[index + 3] == 's' && xml[index + 4] == ';') {
                index += 5;
                newToken->begin = xml + saveIndex;
                newToken->end = xml + index;
                newToken->code = XMLESC_APOSTROPE_CODE;
                return true;
            }
        }

        if(size - index >= 4) {
            if(xml[index] == 'a' && xml[index + 1] == 'm' && xml[index + 2] == 'p' && xml[index + 3] == ';') {
                index += 4;
                newToken->begin = xml + saveIndex;
                newToken->end = xml + index;
                newToken->code = XMLESC_AMPERSAND_CODE;
                return true;
            }
        }

        if(size - index >= 3) {
            if(xml[index + 1] == 't' && xml[index + 2] == ';') {
                if(xml[index] == 'g') {
                    index += 3;
                    newToken->begin = xml + saveIndex;
                    newToken->end = xml + index;
                    newToken->code = XMLESC_GT_CODE;
                    return true;
                }
                else if(xml[index] == 'l') {
                    index += 3;
                    newToken->begin = xml + saveIndex;
                    newToken->end = xml + index;
                    newToken->code = XMLESC_LT_CODE;
                    return true;
                }
            }
        }
    }

    index = saveIndex;
    return false;
}

bool XMLLexer::getNextToken(XMLLexer::Token& ret) {
    skipWhiteSpace(xml, index, size);
    char ch = xml[index];

    if(index >= size) {
        ret.begin = nullptr;
        ret.end = nullptr;
        return false;
    }

    if(loadEscapeSequence(xml, index, size, &ret)) {
    }
    else if(loadTwoChar(xml, index, size, &ret)) {
    }
    else if(loadReserve(xml, index, size, &ret)) {
    }
    else {
        loadWord(xml, index, size, &ret);
    }

    if(ret.begin == nullptr || ret.end == nullptr) {
        return false;
    }

    return true;
}
