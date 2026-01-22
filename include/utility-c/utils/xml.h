// SPDX-License-Identifier: Apache-2.0

#ifndef INCLUDE_UTILITY_C_XML_H_
#define INCLUDE_UTILITY_C_XML_H_

#ifdef S_HAVE_CONFIG_H
  #include "utility-c/config.h"
#endif

#include <stdint.h>
#include <stdio.h>
#ifdef S_POSIX
  #include <stdbool.h>
#endif  // S_POSIX

#include <mxml.h>

#ifdef __cplusplus
extern "C" {
#endif

/// @brief Converts an XML string to an XML tree.
/// @param str The XML string to convert.
/// @return The XML tree, or NULL if an error occurred.
/// @note The caller is responsible for freeing the memory using xml_free().
mxml_node_t* xml_fromString(const char* str);

/// @brief Creates an empty XML file.
/// @param path The path to the directory where the file should be created.
/// @param filename The name of the file to create.
/// @return True if the file was created successfully, false otherwise.
bool xml_createFile(const char* path, const char* filename);

/// @brief Reads an XML file into an XML tree.
/// @param path The path to the directory where the file is located.
/// @param filename The name of the file to read.
/// @return The XML tree, or NULL if an error occurred.
/// @note The caller is responsible for freeing the memory using xml_free().
mxml_node_t* xml_readFile(const char* path, const char* filename);

/// @brief Writes an XML tree to an XML file.
/// @param path The path to the directory where the file should be created.
/// @param filename The name of the file to create.
/// @param tree The XML tree to write to the file.
/// @return True if the file was written successfully, false otherwise.
bool xml_writeFile(const char* path, const char* filename, mxml_node_t* tree);

/// @brief Updates an XML file with an XML tree.
/// @param path The path to the directory where the file is located.
/// @param filename The name of the file to update.
/// @param tree The XML tree to update the file with.
/// @return True if the file was updated successfully, false otherwise.
bool xml_updateFile(const char* path, const char* filename, mxml_node_t* tree);

/// @brief Resets an XML file to an initial state.
/// @param path The path to the directory where the file is located.
/// @param filename The name of the file to reset.
/// @param initial The initial XML tree to reset the file to.
/// @return True if the file was reset successfully, false otherwise.
bool xml_resetFile(const char* path, const char* filename, mxml_node_t* initial);

/// @brief Checks if an XML node with a given identifier exists in an XML tree.
/// @param tree The XML tree to search.
/// @param element The name of the XML element to search for.
/// @param sequence The sequence of XML elements to search for.
/// @param identifier The identifier of the XML node to search for.
/// @return True if the XML node exists, false otherwise.
bool xml_existIdentifier(mxml_node_t* tree,
                         const char* element,
                         const char* sequence,
                         const uint32_t identifier);

/// @brief Finds the identifier of an XML node.
/// @param node The XML node to search.
/// @param sequence The sequence of XML elements to search for.
/// @return The identifier of the XML node, or 0 if the node was not found.
uint32_t xml_findIdentifier(mxml_node_t* node, const char* sequence);

/// @brief Gets an XML node with a given identifier.
/// @param tree The XML tree to search.
/// @param element The name of the XML element to search for.
/// @param sequence The sequence of XML elements to search for.
/// @param identifier The identifier of the XML node to search for.
/// @return The XML node, or NULL if the node was not found.
/// @note The caller is responsible for freeing the memory using xml_free().
mxml_node_t* xml_getIdentifier(mxml_node_t* tree,
                               const char* element,
                               const char* sequence,
                               const uint32_t identifier);

/// @brief Parses an attribute from an XML node.
/// @param node The XML node to parse the attribute from.
/// @param attribute The name of the attribute to parse.
/// @return The value of the attribute, or NULL if the attribute was not found.
/// @note The caller is responsible for freeing the memory using free().
char* xml_parseAttribute(mxml_node_t* node, const char* attribute);

/// @brief Sets an attribute of an XML node.
/// @param tree The XML tree to search.
/// @param element The name of the XML element to search for.
/// @param sequence The sequence of XML elements to search for.
/// @param attribute The name of the attribute to set.
/// @param identifier The identifier of the XML node to set the attribute of.
/// @param str The value to set the attribute to.
/// @return True if the attribute was set successfully, false otherwise.
bool xml_setAttribute(mxml_node_t* tree,
                      const char* element,
                      const char* sequence,
                      const char* attribute,
                      const uint32_t identifier,
                      const char* str);

/// @brief Gets an attribute of an XML node.
/// @param tree The XML tree to search.
/// @param element The name of the XML element to search for.
/// @param sequence The sequence of XML elements to search for.
/// @param attribute The name of the attribute to get.
/// @param identifier The identifier of the XML node to get the attribute of.
/// @return The value of the attribute, or NULL if the attribute was not found.
/// @note The caller is responsible for freeing the memory using free().
char* xml_getAttribute(mxml_node_t* tree,
                       const char* element,
                       const char* sequence,
                       const char* attribute,
                       const uint32_t identifier);

/// @brief Frees an XML tree.
/// @param tree The XML tree to free.
void xml_free(mxml_node_t* tree);

#ifdef __cplusplus
}
#endif

#endif  // INCLUDE_UTILITY_C_XML_H_
