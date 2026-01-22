// SPDX-License-Identifier: Apache-2.0

#include "utility-c/utils/xml.h"

#include "utility-c/utils/char.h"
#include "utility-c/utils/fs.h"
#include "utility-c/utils/regex.h"

static void internal_print(mxml_node_t* tree) {
  char* content = mxmlSaveAllocString(tree, MXML_NO_CALLBACK);
  printf("\n\n%s\n\n", content);
  free(content);
}

static char* internal_getIndex(mxml_node_t* tree,
                               const char* element,
                               const char* sequence,
                               const uint32_t identifier) {
  if (tree == NULL || element == NULL || sequence == NULL || identifier == 0) {
    return NULL;
  }

  mxml_node_t* node = mxmlFindElement(tree, tree, element, sequence, NULL, MXML_DESCEND);
  if (node == NULL) {
    return NULL;
  }

  char* attribute = xml_parseAttribute(node, sequence);
  if (attribute == NULL) {
    return NULL;
  }

  char* value = regex_find("(?<=ns=).*(?=;)", attribute);
  if (char_isEmpty(value)) {
    return NULL;
  }

  char* numeric = char_fromInteger(identifier);
  if (char_isEmpty(numeric)) {
    char_free(value);

    return NULL;
  }

  char* index = char_concats("ns=", value, ";i=", numeric, NULL);
  if (char_isEmpty(index)) {
    char_free(value);
    char_free(numeric);

    return NULL;
  }

  char_free(value);
  char_free(numeric);

  return index;
}

static mxml_node_t* internal_getItem(mxml_node_t* tree,
                                     const char* element,
                                     const char* sequence,
                                     const uint32_t identifier) {
  if (tree == NULL || element == NULL || sequence == NULL || identifier == 0) {
    return NULL;
  }

  char* value = internal_getIndex(tree, element, sequence, identifier);
  if (char_isEmpty(value)) {
    return NULL;
  }

  mxml_node_t* content = mxmlFindElement(tree, tree, element, sequence, value, MXML_DESCEND);
  if (content == NULL) {
    char_free(value);

    return NULL;
  }

  char_free(value);

  return content;
}

static bool internal_setItem(mxml_node_t* tree,
                             const char* element,
                             const char* sequence,
                             const uint32_t identifier,
                             const char* item) {
  if (tree == NULL || element == NULL || sequence == NULL || identifier == 0) {
    return NULL;
  }

  char* value = internal_getIndex(tree, element, sequence, identifier);
  if (char_isEmpty(value)) {
    return NULL;
  }

  mxml_node_t* content = mxmlFindElement(tree, tree, element, sequence, value, MXML_DESCEND);
  if (content == NULL) {
    char_free(value);

    return NULL;
  }

  char_free(value);

  return content;
}

mxml_node_t* xml_fromString(const char* str) {
  return mxmlLoadString(NULL, str, MXML_OPAQUE_CALLBACK);
}

bool xml_createFile(const char* path, const char* filename) {
  if (fs_existFile(path, filename)) {
    return true;
  }

  if (!fs_writeFile(path, filename, "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n\0")) {
    return false;
  }

  return true;
}

mxml_node_t* xml_readFile(const char* path, const char* filename) {
  FILE* file = fs_openFile(path, filename, "r\0");
  if (file == NULL) {
    return NULL;
  }

  mxml_node_t* tree = mxmlLoadFile(NULL, file, MXML_OPAQUE_CALLBACK);
  if (tree == NULL) {
    fs_closeFile(file);

    return NULL;
  }

  fs_closeFile(file);

  return tree;
}

bool xml_writeFile(const char* path, const char* filename, mxml_node_t* tree) {
  if (tree == NULL) {
    return false;
  }

  FILE* file = fs_openFile(path, filename, "w\0");
  if (file == NULL) {
    return NULL;
  }

  if (mxmlSaveFile(tree, file, MXML_NO_CALLBACK) > 0) {
    fs_closeFile(file);

    return false;
  }

  fs_closeFile(file);

  return true;
}

bool xml_updateFile(const char* path, const char* filename, mxml_node_t* tree) {
  FILE* file = fs_openFile(path, filename, "a+\0");
  if (file == NULL) {
    return NULL;
  }

  if (mxmlSaveFile(tree, file, MXML_NO_CALLBACK) > 0) {
    fs_closeFile(file);

    return false;
  }

  fs_closeFile(file);

  return true;
}

bool xml_resetFile(const char* path, const char* filename, mxml_node_t* initial) {
  mxml_node_t* tree = xml_readFile(path, filename);
  if (tree == NULL) {
    return false;
  }

  for (mxml_node_t* node = mxmlFindElement(tree, tree, "UAVariable", NULL, NULL, MXML_DESCEND);
       node != NULL;
       node = mxmlFindElement(node, tree, "UAVariable", NULL, NULL, MXML_DESCEND)) {
    uint32_t identifier = xml_findIdentifier(node, "NodeId");

    if (!xml_existIdentifier(initial, "UAVariable", "NodeId", identifier)) {
      continue;
    }

    char* value = xml_getAttribute(initial, "UAVariable", "NodeId", "Value", identifier);
    if (value == NULL) {
      continue;
    }

    bool ok = xml_setAttribute(tree, "UAVariable", "NodeId", "Value", identifier, value);
    if (!ok) {
      continue;
    }
  }

  if (!xml_writeFile(path, filename, tree)) {
    xml_free(tree);

    return false;
  }

  xml_free(tree);

  return true;
}

bool xml_existIdentifier(mxml_node_t* tree,
                         const char* element,
                         const char* sequence,
                         const uint32_t identifier) {
  mxml_node_t* item = internal_getItem(tree, element, sequence, identifier);
  if (item == NULL) {
    return false;
  }

  return true;
}

uint32_t xml_findIdentifier(mxml_node_t* node, const char* sequence) {
  char* attribute = xml_parseAttribute(node, sequence);
  if (attribute == NULL) {
    return 0;
  }

  /* `attribute` is owned by the mxml library (returned by mxmlElementGetAttr)
     and must NOT be freed by callers. Only free resources we allocate ourselves. */
  char* match = regex_find("([A-Z]*\\d[A-Z]*){4,}", attribute);
  if (match == NULL) {
    return 0;
  }

  uint32_t identifier = (uint32_t)char_toInteger(match);

  char_free(match);

  return identifier;
}

mxml_node_t* xml_getIdentifier(mxml_node_t* tree,
                               const char* element,
                               const char* sequence,
                               const uint32_t identifier) {
  return internal_getItem(tree, element, sequence, identifier);
}

char* xml_parseAttribute(mxml_node_t* node, const char* attribute) {
  if (node == NULL || attribute == NULL) {
    return NULL;
  }

  return (char*)mxmlElementGetAttr(node, attribute);
}
bool xml_setAttribute(mxml_node_t* tree,
                      const char* element,
                      const char* sequence,
                      const char* attribute,
                      const uint32_t identifier,
                      const char* str) {
  if (tree == NULL || element == NULL || sequence == NULL || attribute == NULL || identifier == 0 ||
      str == NULL) {
    return false;
  }

  mxml_node_t* item = internal_getItem(tree, element, sequence, identifier);
  if (item == NULL) {
    return false;
  }

  mxml_node_t* section = mxmlFindElement(item, tree, attribute, NULL, NULL, MXML_DESCEND);
  if (section == NULL) {
    return false;
  }

  mxml_node_t* content = mxmlFindElement(section, tree, NULL, NULL, NULL, MXML_DESCEND);
  if (content == NULL) {
    return false;
  }

  if (mxmlSetOpaque(content, str) > 0) {
    return false;
  }

  return true;
}

char* xml_getAttribute(mxml_node_t* tree,
                       const char* element,
                       const char* sequence,
                       const char* attribute,
                       const uint32_t identifier) {
  if (tree == NULL || element == NULL || sequence == NULL || attribute == NULL || identifier == 0) {
    return NULL;
  }

  mxml_node_t* item = internal_getItem(tree, element, sequence, identifier);
  if (item == NULL) {
    return NULL;
  }

  mxml_node_t* section = mxmlFindElement(item, tree, attribute, NULL, NULL, MXML_DESCEND);
  if (section == NULL) {
    return NULL;
  }

  mxml_node_t* content = mxmlFindElement(section, tree, NULL, NULL, NULL, MXML_DESCEND);
  if (content == NULL) {
    return NULL;
  }

  const char* value = mxmlGetOpaque(content);
  if (value == NULL || value[0] == '\n') {
    return NULL;
  }

  return value;
}

void xml_free(mxml_node_t* tree) {
  if (tree == NULL) {
    return;
  }

  mxmlDelete(tree);
}
