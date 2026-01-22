#include <gtest/gtest.h>

#include <memory>
#include <string>
#include <vector>

#include <mxml.h>

#include "utility-c/utils/xml.h"

namespace {

struct XmlTreeDeleter
{
  void operator()(mxml_node_t *tree) const
  {
    xml_free(tree);
  }
};

const char kXmlSample[] =
    "<?xml version=\"1.0\"?>\n"
    "<UANodeSet>\n"
    "  <UAVariable NodeId=\"ns=2;i=1234\">\n"
    "    <Value>old</Value>\n"
    "  </UAVariable>\n"
    "  <UAVariable NodeId=\"ns=2;i=5678\">\n"
    "    <Value>other</Value>\n"
    "  </UAVariable>\n"
    "</UANodeSet>\n";

std::unique_ptr<mxml_node_t, XmlTreeDeleter> make_tree()
{
  return std::unique_ptr<mxml_node_t, XmlTreeDeleter>(xml_fromString(kXmlSample));
}

}  // namespace

TEST(XmlTest, FromString)
{
  // In-Got-Want
  struct Tests
  {
    std::string label;
    struct In
    {
      const char *xml;
    } in;
    struct Want
    {
      bool should_parse;
    } want;
  };

  // Table-Driven Testing
  const std::vector<Tests> tests = {
    {"valid-xml", {kXmlSample}, {true}},
  };

  for (const auto &tc : tests)
  {
    SCOPED_TRACE(tc.label);

    // Arrange
    // No setup needed

    // Act
    std::unique_ptr<mxml_node_t, XmlTreeDeleter> got(xml_fromString(tc.in.xml));

    // Assert
    if (tc.want.should_parse)
    {
      EXPECT_NE(got.get(), nullptr);
    }
    else
    {
      EXPECT_EQ(got.get(), nullptr);
    }
  }
}

TEST(XmlTest, ParseAttribute)
{
  // In-Got-Want
  struct Tests
  {
    std::string label;
    struct In
    {
      const char *attribute;
    } in;
    struct Want
    {
      const char *expected;
    } want;
  };

  auto tree = make_tree();
  ASSERT_NE(tree.get(), nullptr);

  mxml_node_t *node = mxmlFindElement(tree.get(), tree.get(), "UAVariable", NULL, NULL, MXML_DESCEND);
  ASSERT_NE(node, nullptr);

  // Table-Driven Testing
  const std::vector<Tests> tests = {
    {"nodeid", {"NodeId"}, {"ns=2;i=1234"}},
    {"missing", {"Missing"}, {NULL}},
    {"null-attribute", {NULL}, {NULL}},
  };

  for (const auto &tc : tests)
  {
    SCOPED_TRACE(tc.label);

    // Arrange
    // No setup needed

    // Act
    char *got = xml_parseAttribute(node, tc.in.attribute);

    // Assert
    if (tc.want.expected == NULL)
    {
      EXPECT_EQ(got, nullptr);
    }
    else
    {
      ASSERT_NE(got, nullptr);
      EXPECT_STREQ(got, tc.want.expected);
    }
  }
}

TEST(XmlTest, FindIdentifier)
{
  // In-Got-Want
  struct Tests
  {
    std::string label;
    struct In
    {
      const char *sequence;
    } in;
    struct Want
    {
      uint32_t expected;
    } want;
  };

  auto tree = make_tree();
  ASSERT_NE(tree.get(), nullptr);

  mxml_node_t *node = mxmlFindElement(tree.get(), tree.get(), "UAVariable", NULL, NULL, MXML_DESCEND);
  ASSERT_NE(node, nullptr);

  // Table-Driven Testing
  const std::vector<Tests> tests = {
    {"nodeid", {"NodeId"}, {1234}},
    {"missing", {"Missing"}, {0}},
    {"null-sequence", {NULL}, {0}},
  };

  for (const auto &tc : tests)
  {
    SCOPED_TRACE(tc.label);

    // Arrange
    // No setup needed

    // Act
    uint32_t got = xml_findIdentifier(node, tc.in.sequence);

    // Assert
    EXPECT_EQ(got, tc.want.expected);
  }
}

TEST(XmlTest, ExistIdentifier)
{
  // In-Got-Want
  struct Tests
  {
    std::string label;
    struct In
    {
      mxml_node_t *tree;
      uint32_t identifier;
    } in;
    struct Want
    {
      bool expected;
    } want;
  };

  auto tree = make_tree();
  ASSERT_NE(tree.get(), nullptr);

  // Table-Driven Testing
  const std::vector<Tests> tests = {
    {"exists", {tree.get(), 1234}, {true}},
    {"missing", {tree.get(), 9999}, {false}},
    {"null-tree", {NULL, 1234}, {false}},
  };

  for (const auto &tc : tests)
  {
    SCOPED_TRACE(tc.label);

    // Arrange
    // No setup needed

    // Act
    bool got = xml_existIdentifier(tc.in.tree, "UAVariable", "NodeId", tc.in.identifier);

    // Assert
    EXPECT_EQ(got, tc.want.expected);
  }
}

TEST(XmlTest, GetIdentifier)
{
  // In-Got-Want
  struct Tests
  {
    std::string label;
    struct In
    {
      uint32_t identifier;
    } in;
    struct Want
    {
      bool should_exist;
    } want;
  };

  auto tree = make_tree();
  ASSERT_NE(tree.get(), nullptr);

  // Table-Driven Testing
  const std::vector<Tests> tests = {
    {"exists", {1234}, {true}},
    {"missing", {9999}, {false}},
  };

  for (const auto &tc : tests)
  {
    SCOPED_TRACE(tc.label);

    // Arrange
    // No setup needed

    // Act
    mxml_node_t *got = xml_getIdentifier(tree.get(), "UAVariable", "NodeId", tc.in.identifier);

    // Assert
    if (tc.want.should_exist)
    {
      EXPECT_NE(got, nullptr);
    }
    else
    {
      EXPECT_EQ(got, nullptr);
    }
  }
}

TEST(XmlTest, GetSetAttribute)
{
  // In-Got-Want
  struct Tests
  {
    std::string label;
    struct In
    {
      uint32_t identifier;
      const char *value;
    } in;
    struct Want
    {
      const char *expected;
    } want;
  };

  // Table-Driven Testing
  const std::vector<Tests> tests = {
    {"set-existing", {1234, "new"}, {"new"}},
  };

  for (const auto &tc : tests)
  {
    SCOPED_TRACE(tc.label);

    // Arrange
    auto tree = make_tree();
    ASSERT_NE(tree.get(), nullptr);

    // Act
    bool set_ok = xml_setAttribute(tree.get(), "UAVariable", "NodeId", "Value", tc.in.identifier,
                                   tc.in.value);
    char *got = xml_getAttribute(tree.get(), "UAVariable", "NodeId", "Value", tc.in.identifier);

    // Assert
    EXPECT_TRUE(set_ok);
    ASSERT_NE(got, nullptr);
    EXPECT_STREQ(got, tc.want.expected);
  }
}
