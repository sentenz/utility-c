// SPDX-License-Identifier: Apache-2.0

/**
 * @file xml.h
 * @brief XML parsing and manipulation utilities.
 *
 * This module provides functions for working with XML documents, including
 * parsing, reading, writing, and manipulating XML trees. It uses the Mini-XML
 * (mxml) library as the underlying implementation.
 *
 * @note This module uses the Mini-XML library (mxml.h).
 * @note XML trees must be freed using xml_free() to prevent memory leaks.
 *
 * @warning XML operations may fail silently on malformed input. Always check
 *          return values.
 *
 * @example
 * @code
 * // Parse XML from string
 * const char *xml_data = "<?xml version=\"1.0\"?><config><id>123</id></config>";
 * mxml_node_t *tree = xml_fromString(xml_data);
 * if (tree) {
 *     // Work with the XML tree
 *     xml_free(tree);
 * }
 *
 * // Read and modify XML file
 * mxml_node_t *config = xml_readFile("/etc/app", "config.xml");
 * if (config) {
 *     xml_setAttribute(config, "setting", "settings", "value", 1, "enabled");
 *     xml_writeFile("/etc/app", "config.xml", config);
 *     xml_free(config);
 * }
 * @endcode
 *
 * @see https://www.msweet.org/mxml/
 */

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

/**
 * @brief Parses an XML string into an XML tree.
 *
 * Converts a null-terminated XML string into an in-memory tree structure
 * that can be traversed and manipulated.
 *
 * @param[in] str The null-terminated XML string to parse.
 *
 * @return Pointer to the root node of the parsed XML tree.
 * @return NULL if parsing failed (e.g., malformed XML, memory error).
 *
 * @pre @p str must be a valid, well-formed XML string.
 *
 * @note The caller is responsible for freeing the returned tree using xml_free().
 *
 * @example
 * @code
 * mxml_node_t *tree = xml_fromString("<root><item>value</item></root>");
 * if (tree) {
 *     // Process tree...
 *     xml_free(tree);
 * }
 * @endcode
 *
 * @see xml_free
 */
mxml_node_t* xml_fromString(const char* str);

/**
 * @brief Creates an empty XML file.
 *
 * Creates a new file with minimal XML structure (XML declaration only).
 *
 * @param[in] path     The directory path where the file should be created.
 * @param[in] filename The name of the XML file to create.
 *
 * @return @c true if the file was created successfully.
 * @return @c false if creation failed.
 *
 * @pre @p path must be a valid, existing directory.
 * @pre @p filename must be a valid filename.
 *
 * @see xml_writeFile, xml_readFile
 */
bool xml_createFile(const char* path, const char* filename);

/**
 * @brief Reads an XML file into an XML tree.
 *
 * Parses the specified XML file and returns the document as an in-memory
 * tree structure.
 *
 * @param[in] path     The directory path containing the file.
 * @param[in] filename The name of the XML file to read.
 *
 * @return Pointer to the root node of the parsed XML tree.
 * @return NULL if the file could not be read or parsed.
 *
 * @pre The file must exist and be readable.
 * @pre The file must contain valid XML.
 *
 * @note The caller is responsible for freeing the returned tree using xml_free().
 *
 * @see xml_writeFile, xml_updateFile, xml_free
 */
mxml_node_t* xml_readFile(const char* path, const char* filename);

/**
 * @brief Writes an XML tree to a file.
 *
 * Serializes the XML tree to the specified file, replacing any existing
 * content.
 *
 * @param[in] path     The directory path for the output file.
 * @param[in] filename The name of the file to write.
 * @param[in] tree     The XML tree to serialize.
 *
 * @return @c true if the file was written successfully.
 * @return @c false if writing failed.
 *
 * @pre @p path must be a valid, writable directory.
 * @pre @p tree must be a valid XML tree.
 *
 * @note Existing file content is completely replaced.
 *
 * @see xml_readFile, xml_updateFile
 */
bool xml_writeFile(const char* path, const char* filename, mxml_node_t* tree);

/**
 * @brief Updates an existing XML file with new content.
 *
 * Writes the XML tree to an existing file, updating its content.
 *
 * @param[in] path     The directory path containing the file.
 * @param[in] filename The name of the file to update.
 * @param[in] tree     The XML tree to write.
 *
 * @return @c true if the file was updated successfully.
 * @return @c false if the update failed.
 *
 * @pre The file should exist.
 * @pre @p tree must be a valid XML tree.
 *
 * @see xml_writeFile, xml_readFile
 */
bool xml_updateFile(const char* path, const char* filename, mxml_node_t* tree);

/**
 * @brief Resets an XML file to an initial state.
 *
 * Replaces the content of an XML file with a specified initial tree,
 * effectively resetting the file to a default configuration.
 *
 * @param[in] path     The directory path containing the file.
 * @param[in] filename The name of the file to reset.
 * @param[in] initial  The initial XML tree to use as the new content.
 *
 * @return @c true if the file was reset successfully.
 * @return @c false if the reset failed.
 *
 * @pre The file should exist.
 * @pre @p initial must be a valid XML tree.
 *
 * @see xml_writeFile
 */
bool xml_resetFile(const char* path, const char* filename, mxml_node_t* initial);

/**
 * @brief Checks if a node with a specific identifier exists.
 *
 * Searches the XML tree for an element matching the specified criteria
 * and identifier value.
 *
 * @param[in] tree       The XML tree to search.
 * @param[in] element    The element tag name to search for.
 * @param[in] sequence   The path sequence to the identifier attribute.
 * @param[in] identifier The identifier value to match.
 *
 * @return @c true if a matching node exists.
 * @return @c false if no matching node is found.
 *
 * @see xml_findIdentifier, xml_getIdentifier
 */
bool xml_existIdentifier(mxml_node_t* tree,
                         const char* element,
                         const char* sequence,
                         const uint32_t identifier);

/**
 * @brief Finds the identifier value of an XML node.
 *
 * Extracts the identifier from the specified node by following the
 * given sequence path.
 *
 * @param[in] node     The XML node to search.
 * @param[in] sequence The path sequence to the identifier.
 *
 * @return The identifier value (as uint32_t).
 * @return 0 if the identifier was not found or is invalid.
 *
 * @see xml_existIdentifier, xml_getIdentifier
 */
uint32_t xml_findIdentifier(mxml_node_t* node, const char* sequence);

/**
 * @brief Gets a node by its identifier.
 *
 * Searches the XML tree and returns the node matching the specified
 * element, sequence, and identifier.
 *
 * @param[in] tree       The XML tree to search.
 * @param[in] element    The element tag name to search for.
 * @param[in] sequence   The path sequence to the identifier attribute.
 * @param[in] identifier The identifier value to match.
 *
 * @return Pointer to the matching XML node.
 * @return NULL if no matching node is found.
 *
 * @note The caller is responsible for freeing the returned node using xml_free().
 *
 * @see xml_existIdentifier, xml_findIdentifier
 */
mxml_node_t* xml_getIdentifier(mxml_node_t* tree,
                               const char* element,
                               const char* sequence,
                               const uint32_t identifier);

/**
 * @brief Parses an attribute value from an XML node.
 *
 * Retrieves the string value of the specified attribute from the node.
 *
 * @param[in] node      The XML node to parse.
 * @param[in] attribute The name of the attribute to retrieve.
 *
 * @return A newly allocated string containing the attribute value.
 * @return NULL if the attribute is not found or memory allocation fails.
 *
 * @note The caller is responsible for freeing the returned string using free().
 *
 * @see xml_getAttribute, xml_setAttribute
 */
char* xml_parseAttribute(mxml_node_t* node, const char* attribute);

/**
 * @brief Sets an attribute value on an XML node.
 *
 * Finds the node matching the specified criteria and sets the attribute
 * to the given value.
 *
 * @param[in] tree       The XML tree to search.
 * @param[in] element    The element tag name to search for.
 * @param[in] sequence   The path sequence to the target node.
 * @param[in] attribute  The name of the attribute to set.
 * @param[in] identifier The identifier of the node to modify.
 * @param[in] str        The value to set the attribute to.
 *
 * @return @c true if the attribute was set successfully.
 * @return @c false if the node was not found or the operation failed.
 *
 * @see xml_getAttribute, xml_parseAttribute
 */
bool xml_setAttribute(mxml_node_t* tree,
                      const char* element,
                      const char* sequence,
                      const char* attribute,
                      const uint32_t identifier,
                      const char* str);

/**
 * @brief Gets an attribute value from an XML node.
 *
 * Finds the node matching the specified criteria and returns the value
 * of the specified attribute.
 *
 * @param[in] tree       The XML tree to search.
 * @param[in] element    The element tag name to search for.
 * @param[in] sequence   The path sequence to the target node.
 * @param[in] attribute  The name of the attribute to retrieve.
 * @param[in] identifier The identifier of the node to query.
 *
 * @return A newly allocated string containing the attribute value.
 * @return NULL if the node or attribute is not found.
 *
 * @note The caller is responsible for freeing the returned string using free().
 *
 * @see xml_setAttribute, xml_parseAttribute
 */
char* xml_getAttribute(mxml_node_t* tree,
                       const char* element,
                       const char* sequence,
                       const char* attribute,
                       const uint32_t identifier);

/**
 * @brief Frees an XML tree and all associated memory.
 *
 * Releases all memory associated with the XML tree, including all
 * child nodes.
 *
 * @param[in] tree The XML tree to free.
 *
 * @post @p tree is no longer valid and must not be used.
 *
 * @note It is safe to pass NULL to this function.
 * @note This function should be called for every tree returned by
 *       xml_fromString(), xml_readFile(), or xml_getIdentifier().
 */
void xml_free(mxml_node_t* tree);

#ifdef __cplusplus
}
#endif

#endif  // INCLUDE_UTILITY_C_XML_H_
