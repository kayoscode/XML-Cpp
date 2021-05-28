#ifndef INCLUDE_XML_NODE_H
#define INCLUDE_XML_NODE_H

#include <map>
#include <vector>
#include <string>

/**
 * Contains data for an XML node
 * @author Bryce Young 5/27/2021
 * */
class XMLNode {
    public:
        XMLNode() 
        {
        }

        ~XMLNode() {
            for(int i = 0; i < subNodes.size(); ++i) {
                delete subNodes[i];
            }
        }

        /**
         * Takes ownership of the XML node
         * @param newNode the new node to add to the list
         * */
        void addSubNode(XMLNode* newNode) {
            subNodes.push_back(newNode);
        }
        
        XMLNode* getChild(int index) {
            return subNodes[index];
        }

        void removeNode(int index) {
            subNodes.erase(subNodes.begin() + index);
        }

        void removeNode(XMLNode* node) {
            for(int i = 0; i < subNodes.size(); ++i) {
                if(subNodes[i] == node) {
                    removeNode(i);
                    break;
                }
            }
        }

        /**
         * Adds an attribute
         * @param name the name of the attribute
         * @param value the value to set in the map
         * */
        void addAttribute(const std::string& name, const std::string& value) {
            //validate name
            attributes[name] = value;
        }

        std::string getAttribute(const std::string& name) {
            std::map<std::string, std::string>::iterator i = attributes.find(name);

            if(i != attributes.end()) {
                return i->second;
            }

            return "";
        }

        /**
         * Removes an attribute from this node
         * @param name the name of the attribute to remove
         * */
        void removeAttribute(const std::string& name) {
            std::map<std::string, std::string>::iterator i = attributes.find(name);

            if(i != attributes.end()) {
                attributes.erase(i);
            }
        }

        /**
         * @return whether the node is a leaf or not
         * It is a leaf if it doesn't include any subnodes
         * */
        bool isLeaf() {
            return subNodes.size() == 0;
        }

        //helper functions for making it easier to get data from the DOM format
        std::string& getText() {
            return text;
        }

        void setText(const std::string& str) {
            this->text = str;
        }

    private:
        std::string text;
        std::vector<XMLNode*> subNodes;
        std::map<std::string, std::string> attributes;
};

#endif