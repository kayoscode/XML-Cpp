#include "XMLParser.h"

XMLParser::XMLParser(char* xml, uint32_t size) 
    :lexer(xml, size)
{}

XMLParser::~XMLParser() {

}

static void loadXMLNode(XMLLexer& lexer, XMLLexer::Token* token, XMLNode* node);
static void loadXMLText(XMLLexer& lexer, XMLLexer::Token* token, XMLNode* node, std::string& text);

/**
 * Attribute name followed by an equals
 * */
static void loadXMLAttribute(XMLLexer& lexer, XMLLexer::Token* token, XMLNode* node) {
    //currently we are pointing at the identifier
    std::string identName(token->begin, token->end);
    lexer.getNextToken(*token);

    if(token->code == XMLEQUALS_CODE) {
        lexer.getNextToken(*token);

        if(token->code == XMLQUOTE_CODE) {
            lexer.getNextToken(*token);
            std::string quoteText;
            loadXMLText(lexer, token, node, quoteText);
            node->addAttribute(identName, quoteText);
        }

        if(token->code == XMLQUOTE_CODE) {
            lexer.getNextToken(*token);
        }
        else {
            std::cout << "Expected closing quote\n";
        }
    }
}

static void loadXMLText(XMLLexer& lexer, XMLLexer::Token* token, XMLNode* node, std::string& text) {
    char* indexStart = token->begin, *indexEnd = token->begin;

    while(token->begin != nullptr && token->end != nullptr) {
        if(token->code == XMLOPENSECTION_CODE) {
            //add the text
            if(indexStart != indexEnd) {
                text += std::string(indexStart, indexEnd);
            }

            //create the new node, load the data and add it as a child
            XMLNode* child = new XMLNode();
            loadXMLNode(lexer, token, child);
            node->addSubNode(child);
            indexStart = token->end;
            lexer.getNextToken(*token);
            indexEnd = indexStart;
            continue;
        }
        else if(token->code == XMLESC_QUOTE_CODE) {
            if(indexStart != indexEnd) {
                text += std::string(indexStart, indexEnd);
            }

            text += '"';
            indexStart = token->end;
            lexer.getNextToken(*token);
            indexEnd = token->begin;
            continue;
        }
        else if(token->code == XMLESC_APOSTROPE_CODE) {
            if(indexStart != indexEnd) {
                text += std::string(indexStart, indexEnd);
            }

            text += '\'';
            indexStart = token->end;
            lexer.getNextToken(*token);
            indexEnd = token->begin;
            continue;
        }
        else if(token->code == XMLESC_AMPERSAND_CODE) {
            if(indexStart != indexEnd) {
                text += std::string(indexStart, indexEnd);
            }

            text += '&';
            indexStart = token->end;
            lexer.getNextToken(*token);
            indexEnd = token->begin;
            continue;
        }
        else if(token->code == XMLESC_GT_CODE) {
            if(indexStart != indexEnd) {
                text += std::string(indexStart, indexEnd);
            }

            text += '>';
            indexStart = token->end;
            lexer.getNextToken(*token);
            indexEnd = token->begin;
            continue;
        }
        else if(token->code == XMLESC_LT_CODE) {
            if(indexStart != indexEnd) {
                text += std::string(indexStart, indexEnd);
            }

            text += '<';
            indexStart = token->end;
            lexer.getNextToken(*token);
            indexEnd = token->begin;
            continue;
        }
        else if(token->code == XMLCLOSE_TAG_CODE || token->code == XMLQUOTE_CODE) {
            //return back to node to finish loading the tag
            break;
        }
        else {
            indexEnd = token->end;
        }

        lexer.getNextToken(*token);
    }

    //now that the entire string has been loaded into memory
    if(indexStart != indexEnd) {
        text += std::string(indexStart, indexEnd);
    }
}

/**
 * NODE: <{elementname} ({attribName}={string})* END
 * END: \> || > TEXT </elementname>
 * TEXT: TEXT NODE || NODE TEXT
 * */
static void loadXMLNode(XMLLexer& lexer, XMLLexer::Token* token, XMLNode* node) {
    lexer.getNextToken(*token);

    if(token->code == XMLIDENTIFIER_CODE) {
        std::string elementName(token->begin, token->end);
        lexer.getNextToken(*token);

        while(token->begin != nullptr && token->end != nullptr && token->code == XMLIDENTIFIER_CODE) {
            loadXMLAttribute(lexer, token, node);
        }

        if(token->code == XMLSELFCLOSE_TAG_CODE) {
            //finished loading the tag (obviously no text in this kind of tag)
            lexer.getNextToken(*token);
        }
        else if(token->code == XMLCLOSESECTION_CODE){
            lexer.getNextToken(*token);
            std::string text;
            loadXMLText(lexer, token, node, text);
            node->setText(text);

            if(token->code == XMLCLOSE_TAG_CODE) {
                lexer.getNextToken(*token);
                if(elementName == std::string(token->begin, token->end)) {
                    //valid
                }
                else {
                    //warning
                    std::cout << std::string(token->begin, token->end) << " Does not match " << elementName << "\n";
                }

                lexer.getNextToken(*token);
            }
        }
        else {
            //error
            std::cout << "Expected >\n";
        }
    }
    else {
        //error
        std::cout << "Expected valid element name\n";
    }
}

void XMLParser::parseXML(XMLNode* node) {
    XMLLexer::Token token;
    bool hasAnother = lexer.getNextToken(token);

    if(hasAnother) {
        loadXMLNode(lexer, &token, node);
    }
    else {
        //error
        std::cout << "Syntax error\n";
    }
}