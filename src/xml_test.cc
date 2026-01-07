// SPDX-License-Identifier: Apache-2.0

#include "utility-c/xml.h"

#include "gtest/gtest.h"
#include "utility-c/fs.h"

static char *l_str1 = "\
<?xml version=\"1.0\" encoding=\"utf-8\"?>\n\
<UAVariable DataType=\"UInt16\" NodeId=\"ns=3;i=23001\"\n\
BrowseName=\"3:DeviceID\" ParentNodeId=\"ns=3;i=23000\">\n\
  <Value>\n\
    <uax:UInt16>17</uax:UInt16>\n\
  </Value>\n\
</UAVariable>\n\
<UAVariable DataType=\"String\" NodeId=\"ns=3;i=23002\"\n\
BrowseName=\"3:HardwareRevision\" ParentNodeId=\"ns=3;i=23000\">\n\
  <Value>\n\
    <uax:String>HW-Vx.y</uax:String>\n\
  </Value>\n\
</UAVariable>\n\
<UAVariable DataType=\"LocalizedText\" NodeId=\"ns=3;i=23003\"\n\
BrowseName=\"3:Manufacture\" ParentNodeId=\"ns=3;i=23000\">\n\
  <Value>\n\
    <uax:LocalizedText>\n\
      <uax:Locale>en-US</uax:Locale>\n\
      <uax:Text>Sentenz</uax:Text>\n\
    </uax:LocalizedText>\n\
  </Value>\n\
</UAVariable>\n\
<UAVariable DataType=\"LocalizedText\" NodeId=\"ns=3;i=23004\"\n\
BrowseName=\"3:Model\" ParentNodeId=\"ns=3;i=23000\">\n\
  <Value>\n\
    <uax:LocalizedText>\n\
      <uax:Locale>en-US</uax:Locale>\n\
      <uax:Text>EX245-SPNA</uax:Text>\n\
    </uax:LocalizedText>\n\
  </Value>\n\
</UAVariable>\n\
<UAVariable DataType=\"String\" NodeId=\"ns=3;i=23005\"\n\
BrowseName=\"3:SerialNumber\" ParentNodeId=\"ns=3;i=23000\">\n\
  <Value>\n\
    <uax:String>xxxxxxxx</uax:String>\n\
  </Value>\n\
</UAVariable>\n\
<UAVariable DataType=\"String\" NodeId=\"ns=3;i=23007\"\n\
BrowseName=\"3:SoftwareRevision\" ParentNodeId=\"ns=3;i=23000\">\n\
  <Value>\n\
    <uax:String>FW-Vx.y</uax:String>\n\
  </Value>\n\
</UAVariable>\n\
<UAVariable DataType=\"UInt16\" NodeId=\"ns=3;i=23008\"\n\
BrowseName=\"3:VendorID\" ParentNodeId=\"ns=3;i=23000\">\n\
  <Value>\n\
    <uax:UInt16>131</uax:UInt16>\n\
  </Value>\n\
</UAVariable>\n\
<UAVariable DataType=\"String\" NodeId=\"ns=3;i=23009\"\n\
BrowseName=\"3:VendorText\" ParentNodeId=\"ns=3;i=23000\">\n\
  <Value>\n\
    <uax:String>www.sentenz.com</uax:String>\n\
  </Value>\n\
</UAVariable>\n\0";

static char *l_str2 = "\
<?xml version=\"1.0\" encoding=\"utf-8\"?>\n\
<UAVariable DataType=\"ns=2;i=3005\"\n\
NodeId=\"ns=3;i=21101\" BrowseName=\"3:EnergySavingMode\"\n\
ParentNodeId=\"ns=3;i=21100\">\n\
        <Value>\n\
            <uax:Int32>0</uax:Int32>\n\
        </Value>\n\
    </UAVariable>\n\
<UAVariable\n\
DataType=\"Boolean\" ValueRank=\"1\" NodeId=\"ns=3;i=21102\"\n\
ArrayDimensions=\"16\" BrowseName=\"3:InputFilter\"\n\
ParentNodeId=\"ns=3;i=21100\">\n\
        <Value>\n\
            <uax:ListOfBoolean>\n\
                <uax:Boolean>false</uax:Boolean>\n\
                <uax:Boolean>false</uax:Boolean>\n\
                <uax:Boolean>false</uax:Boolean>\n\
                <uax:Boolean>false</uax:Boolean>\n\
                <uax:Boolean>false</uax:Boolean>\n\
                <uax:Boolean>false</uax:Boolean>\n\
                <uax:Boolean>false</uax:Boolean>\n\
                <uax:Boolean>false</uax:Boolean>\n\
                <uax:Boolean>false</uax:Boolean>\n\
                <uax:Boolean>false</uax:Boolean>\n\
                <uax:Boolean>false</uax:Boolean>\n\
                <uax:Boolean>false</uax:Boolean>\n\
                <uax:Boolean>false</uax:Boolean>\n\
                <uax:Boolean>false</uax:Boolean>\n\
                <uax:Boolean>false</uax:Boolean>\n\
                <uax:Boolean>false</uax:Boolean>\n\
            </uax:ListOfBoolean>\n\
        </Value>\n\
    </UAVariable>\n\
<UAVariable\n\
DataType=\"String\" NodeId=\"ns=3;i=21201\" BrowseName=\"3:MqttTopic\"\n\
ParentNodeId=\"ns=3;i=21200\" UserAccessLevel=\"3\" AccessLevel=\"3\">\n\
        <Value>\n\
            <uax:String>EX245-DX1-Slot2</uax:String>\n\
        </Value>\n\
    </UAVariable>\n\
<UAVariable DataType=\"i=14647\" NodeId=\"ns=3;i=21202\"\n\
BrowseName=\"3:MqttPublisherState\" ParentNodeId=\"ns=3;i=21200\"\n\
UserAccessLevel=\"3\" AccessLevel=\"3\">\n\
        <Value>\n\
            <uax:Int32>0</uax:Int32>\n\
        </Value>\n\
    </UAVariable>\n\
<UAVariable DataType=\"i=290\" NodeId=\"ns=3;i=21203\"\n\
BrowseName=\"3:MqttPublishInterval\" ParentNodeId=\"ns=3;i=21200\"\n\
UserAccessLevel=\"3\" AccessLevel=\"3\">\n\
        <Value>\n\
            <uax:Double>1000</uax:Double>\n\
        </Value>\n\
    </UAVariable>\n\
<UAVariable DataType=\"i=15008\" NodeId=\"ns=3;i=21205\"\n\
BrowseName=\"3:MqttQoS\" ParentNodeId=\"ns=3;i=21200\" UserAccessLevel=\"3\"\n\
AccessLevel=\"3\">\n\
        <Value>\n\
            <uax:Int32>1</uax:Int32>\n\
        </Value>\n\
    </UAVariable>\n\
<UAVariable DataType=\"ns=2;i=3001\" NodeId=\"ns=3;i=22001\"\n\
BrowseName=\"3:ModuleLayout\" ParentNodeId=\"ns=3;i=22000\">\n\
        <Value>\n\
            <uax:Int32>0</uax:Int32>\n\
        </Value>\n\
    </UAVariable>\n\
<UAVariable DataType=\"ns=2;i=3002\" NodeId=\"ns=3;i=22002\"\n\
BrowseName=\"3:ModuleState\" ParentNodeId=\"ns=3;i=22000\">\n\
        <Value>\n\
            <uax:Int32>0</uax:Int32>\n\
        </Value>\n\
    </UAVariable>\n\
<UAVariable DataType=\"ns=2;i=3003\" ValueRank=\"1\" NodeId=\"ns=3;i=22003\"\n\
ArrayDimensions=\"16\" BrowseName=\"3:ShortCircuit\"\n\
ParentNodeId=\"ns=3;i=22000\">\n\
        <Value>\n\
            <uax:ListOfInt32>\n\
                <uax:Int32>0</uax:Int32>\n\
                <uax:Int32>0</uax:Int32>\n\
                <uax:Int32>0</uax:Int32>\n\
                <uax:Int32>0</uax:Int32>\n\
                <uax:Int32>0</uax:Int32>\n\
                <uax:Int32>0</uax:Int32>\n\
                <uax:Int32>0</uax:Int32>\n\
                <uax:Int32>0</uax:Int32>\n\
                <uax:Int32>0</uax:Int32>\n\
                <uax:Int32>0</uax:Int32>\n\
                <uax:Int32>0</uax:Int32>\n\
                <uax:Int32>0</uax:Int32>\n\
                <uax:Int32>0</uax:Int32>\n\
                <uax:Int32>0</uax:Int32>\n\
                <uax:Int32>0</uax:Int32>\n\
                <uax:Int32>0</uax:Int32>\n\
            </uax:ListOfInt32>\n\
        </Value>\n\
    </UAVariable>\n\
<UAVariable DataType=\"UInt16\" NodeId=\"ns=3;i=23001\"\n\
BrowseName=\"3:DeviceID\" ParentNodeId=\"ns=3;i=23000\">\n\
  <Value>\n\
    <uax:UInt16>17</uax:UInt16>\n\
  </Value>\n\
</UAVariable>\n\
<UAVariable DataType=\"LocalizedText\" NodeId=\"ns=3;i=23003\"\n\
BrowseName=\"3:Manufacture\" ParentNodeId=\"ns=3;i=23000\">\n\
        <Value>\n\
            <uax:LocalizedText>\n\
                <uax:Locale>en-US</uax:Locale>\n\
                <uax:Text>Sentenz</uax:Text>\n\
            </uax:LocalizedText>\n\
        </Value>\n\
    </UAVariable>\n\
<UAVariable DataType=\"LocalizedText\" NodeId=\"ns=3;i=23004\"\n\
BrowseName=\"3:Model\" ParentNodeId=\"ns=3;i=23000\">\n\
        <Value>\n\
            <uax:LocalizedText>\n\
                <uax:Locale>en-US</uax:Locale>\n\
                <uax:Text>EX245-DX1</uax:Text>\n\
            </uax:LocalizedText>\n\
        </Value>\n\
    </UAVariable>\n\
<UAVariable DataType=\"Byte\" NodeId=\"ns=3;i=23006\"\n\
BrowseName=\"3:SlotNumber\" ParentNodeId=\"ns=3;i=23000\">\n\
        <Value>\n\
            <uax:Byte>0</uax:Byte>\n\
        </Value>\n\
    </UAVariable>\n\
<UAVariable DataType=\"UInt16\" NodeId=\"ns=3;i=23008\"\n\
BrowseName=\"3:VendorID\" ParentNodeId=\"ns=3;i=23000\">\n\
        <Value>\n\
            <uax:UInt16>131</uax:UInt16>\n\
        </Value>\n\
    </UAVariable>\n\
<UAVariable DataType=\"String\" NodeId=\"ns=3;i=23009\"\n\
BrowseName=\"3:VendorText\" ParentNodeId=\"ns=3;i=23000\">\n\
        <Value>\n\
            <uax:String>www.sentenz.com</uax:String>\n\
        </Value>\n\
    </UAVariable>\n\
<UAVariable DataType=\"Byte\" ValueRank=\"1\" NodeId=\"ns=3;i=25001\"\n\
ArrayDimensions=\"2\" BrowseName=\"3:AsArray\" ParentNodeId=\"ns=3;i=25003\">\n\
        <Value>\n\
            <uax:ListOfByte>\n\
                <uax:Byte>0</uax:Byte>\n\
                <uax:Byte>0</uax:Byte>\n\
            </uax:ListOfByte>\n\
        </Value>\n\
    </UAVariable>\n\
<UAVariable DataType=\"ns=2;i=3000\" ValueRank=\"1\" NodeId=\"ns=3;i=25002\"\n\
ArrayDimensions=\"16\" BrowseName=\"3:AsEnum\" ParentNodeId=\"ns=3;i=25003\">\n\
        <Value>\n\
            <uax:ListOfInt32>\n\
                <uax:Int32>0</uax:Int32>\n\
                <uax:Int32>0</uax:Int32>\n\
                <uax:Int32>0</uax:Int32>\n\
                <uax:Int32>0</uax:Int32>\n\
                <uax:Int32>0</uax:Int32>\n\
                <uax:Int32>0</uax:Int32>\n\
                <uax:Int32>0</uax:Int32>\n\
                <uax:Int32>0</uax:Int32>\n\
                <uax:Int32>0</uax:Int32>\n\
                <uax:Int32>0</uax:Int32>\n\
                <uax:Int32>0</uax:Int32>\n\
                <uax:Int32>0</uax:Int32>\n\
                <uax:Int32>0</uax:Int32>\n\
                <uax:Int32>0</uax:Int32>\n\
                <uax:Int32>0</uax:Int32>\n\
                <uax:Int32>0</uax:Int32>\n\
            </uax:ListOfInt32>\n\
        </Value>\n\
    </UAVariable>\n\
<UAVariable DataType=\"UInt32\" NodeId=\"ns=3;i=25003\"\n\
BrowseName=\"3:ProcessData\" ParentNodeId=\"ns=3;i=25000\">\n\
        <Value>\n\
            <uax:UInt32>0</uax:UInt32>\n\
        </Value>\n\
    </UAVariable>\n\0";

static void setup(const char *path, const char *filename, const char *str) {
  if (path != NULL) {
    fs_createDirectory(path, ACCESSPERMS);
  }

  if (path != NULL && filename != NULL) {
    xml_createFile(path, filename);
  }

  if (path != NULL && filename != NULL && str != NULL) {
    mxml_node_t *node = xml_fromString(str);
    xml_writeFile(path, filename, node);
    xml_free(node);
  }
}

static void teardown(const char *path, const char *filename, mxml_node_t *node) {
  if (path != NULL && filename != NULL) {
    fs_removeFile(path, filename);
  }

  if (path != NULL) {
    fs_removeDirectory(path);
  }

  xml_free(node);
}

TEST(xml, createFile) {
  typedef struct s_test {
    char *in[2];
    bool want;
    bool got;
  } test_t;

  test_t test[19] = {{.in = {"/tmp/test/xml/a\0", "example.xml\0"}, .want = true},
                     {.in = {"/tmp/test/xml/a\0", "example.xml\0"}, .want = true},
                     {.in = {"/tmp/test/xml/b\0", "example.xml\0"}, .want = true},
                     {.in = {"/tmp/test/xml/c/\0", "example.xml\0"}, .want = true},
                     {.in = {"/tmp/test/xml/d/\0", "example.xml\0"}, .want = true},
                     {.in = {"/tmp/test/xml/e/\0", "example.xml\0"}, .want = true},
                     {.in = {"/tmp/test/xml/f//\0", "example.xml\0"}, .want = true},
                     {.in = {"/tmp/test/xml/g///\0", "example.xml\0"}, .want = true},
                     {.in = {"/tmp/test/xml/h///j\0", "example.xml\0"}, .want = true},
                     {.in = {"/tmp/test/xml/i///k/\0", "example.xml\0"}, .want = true},
                     {.in = {"/tmp/test/xml/l/ /m/\0", "example.xml\0"}, .want = true},
                     {.in = {"/tmp/test/xml/n/o p/q/\0", "example.xml\0"}, .want = true},
                     {.in = {"/tmp/test/xml/r/example.xml\0", "example.xml\0"}, .want = true},
                     {.in = {"/tmp/test/xml/s/example.xml\0", "example.xml\0"}, .want = true},
                     {.in = {"/tmp/test/xml/{}[]()\0", "example.xml\0"}, .want = true},
                     {.in = {"/tmp/test/xml/⌘☹Ж\0", "\0"}, .want = false},
                     {.in = {"/tmp/test/xml\0", "\0"}, .want = false},
                     {.in = {"\0", "\0"}, .want = false},
                     {.in = {NULL, NULL}, .want = false}};

  for (size_t i = 0; i < sizeof(test) / sizeof(test[0]); ++i) {
    setup(test[i].in[0], NULL, NULL);

    test[i].got = xml_createFile(test[i].in[0], test[i].in[1]);
    EXPECT_EQ(test[i].got, test[i].want);

    teardown(test[i].in[0], test[i].in[1], NULL);
  }
}

TEST(xml, readFile) {
  typedef struct s_test {
    char *in[2];
    mxml_node_t *want;
    mxml_node_t *got;
  } test_t;

  test_t test[19] = {{.in = {"/tmp/test/xml/a\0", "example.xml\0"}, .want = NULL},
                     {.in = {"/tmp/test/xml/a\0", "example.xml\0"}, .want = NULL},
                     {.in = {"/tmp/test/xml/b\0", "example.xml\0"}, .want = NULL},
                     {.in = {"/tmp/test/xml/c/\0", "example.xml\0"}, .want = NULL},
                     {.in = {"/tmp/test/xml/d/\0", "example.xml\0"}, .want = NULL},
                     {.in = {"/tmp/test/xml/e/\0", "example.xml\0"}, .want = NULL},
                     {.in = {"/tmp/test/xml/f//\0", "example.xml\0"}, .want = NULL},
                     {.in = {"/tmp/test/xml/g///\0", "example.xml\0"}, .want = NULL},
                     {.in = {"/tmp/test/xml/h///j\0", "example.xml\0"}, .want = NULL},
                     {.in = {"/tmp/test/xml/i///k/\0", "example.xml\0"}, .want = NULL},
                     {.in = {"/tmp/test/xml/l/ /m/\0", "example.xml\0"}, .want = NULL},
                     {.in = {"/tmp/test/xml/n/o p/q/\0", "example.xml\0"}, .want = NULL},
                     {.in = {"/tmp/test/xml/r/.config\0", "example.xml\0"}, .want = NULL},
                     {.in = {"/tmp/test/xml/s/.config\0", "example.xml\0"}, .want = NULL},
                     {.in = {"/tmp/test/xml/{}[]()\0", "example.xml\0"}, .want = NULL},
                     {.in = {"/tmp/test/xml/⌘☹Ж\0", "\0"}, .want = (mxml_node_t *)1},
                     {.in = {"/tmp/test/xml\0", "\0"}, .want = (mxml_node_t *)1},
                     {.in = {"\0", "\0"}, .want = (mxml_node_t *)1},
                     {.in = {NULL, NULL}, .want = (mxml_node_t *)1}};

  for (size_t i = 0; i < sizeof(test) / sizeof(test[0]); ++i) {
    setup(test[i].in[0], test[i].in[1], l_str1);

    test[i].got = xml_readFile(test[i].in[0], test[i].in[1]);
    EXPECT_NE(test[i].got, test[i].want);

    teardown(test[i].in[0], test[i].in[1], test[i].got);
  }
}

TEST(xml, writeFile) {
  typedef struct s_test {
    char *in1[2];
    mxml_node_t *in2;
    bool want;
    bool got;
  } test_t;

  test_t test[19] = {
      {.in1 = {"/tmp/test/xml/a\0", "example.xml\0"}, .in2 = xml_fromString(l_str1), .want = true},
      {.in1 = {"/tmp/test/xml/a\0", "example.xml\0"}, .in2 = xml_fromString(l_str1), .want = true},
      {.in1 = {"/tmp/test/xml/b\0", "example.xml\0"}, .in2 = xml_fromString(l_str1), .want = true},
      {.in1 = {"/tmp/test/xml/c/\0", "example.xml\0"}, .in2 = xml_fromString(l_str1), .want = true},
      {.in1 = {"/tmp/test/xml/d/\0", "example.xml\0"}, .in2 = xml_fromString(l_str1), .want = true},
      {.in1 = {"/tmp/test/xml/e/\0", "example.xml\0"}, .in2 = xml_fromString(l_str1), .want = true},
      {.in1  = {"/tmp/test/xml/f//\0", "example.xml\0"},
       .in2  = xml_fromString(l_str1),
       .want = true},
      {.in1  = {"/tmp/test/xml/g///\0", "example.xml\0"},
       .in2  = xml_fromString(l_str1),
       .want = true},
      {.in1  = {"/tmp/test/xml/h///j\0", "example.xml\0"},
       .in2  = xml_fromString(l_str1),
       .want = true},
      {.in1  = {"/tmp/test/xml/i///k/\0", "example.xml\0"},
       .in2  = xml_fromString(l_str1),
       .want = true},
      {.in1  = {"/tmp/test/xml/l/ /m/\0", "example.xml\0"},
       .in2  = xml_fromString(l_str1),
       .want = true},
      {.in1  = {"/tmp/test/xml/n/o p/q/\0", "example.xml\0"},
       .in2  = xml_fromString(l_str1),
       .want = true},
      {.in1  = {"/tmp/test/xml/r/.config\0", "example.xml\0"},
       .in2  = xml_fromString(l_str1),
       .want = true},
      {.in1  = {"/tmp/test/xml/s/.config\0", "example.xml\0"},
       .in2  = xml_fromString(l_str1),
       .want = true},
      {.in1 = {"/tmp/test/xml/{}[]()\0", "example.xml\0"}, .in2 = NULL, .want = false},
      {.in1 = {"/tmp/test/xml/⌘☹Ж\0", "\0"}, .in2 = xml_fromString(l_str1), .want = false},
      {.in1 = {"/tmp/test/xml\0", "\0"}, .in2 = xml_fromString(l_str1), .want = false},
      {.in1 = {"\0", "\0"}, .in2 = xml_fromString(l_str1), .want = false},
      {.in1 = {NULL, NULL}, .in2 = xml_fromString(l_str1), .want = false}};

  for (size_t i = 0; i < sizeof(test) / sizeof(test[0]); ++i) {
    setup(test[i].in1[0], test[i].in1[1], NULL);

    test[i].got = xml_writeFile(test[i].in1[0], test[i].in1[1], test[i].in2);
    EXPECT_EQ(test[i].got, test[i].want);

    teardown(test[i].in1[0], test[i].in1[1], test[i].in2);
  }
}

TEST(xml, updateFile) {
  typedef struct s_test {
    char *in1[2];
    mxml_node_t *in2;
    bool want;
    bool got;
  } test_t;

  test_t test[19] = {
      {.in1 = {"/tmp/test/xml/a\0", "example.xml\0"}, .in2 = xml_fromString(l_str1), .want = true},
      {.in1 = {"/tmp/test/xml/a\0", "example.xml\0"}, .in2 = xml_fromString(l_str1), .want = true},
      {.in1 = {"/tmp/test/xml/b\0", "example.xml\0"}, .in2 = xml_fromString(l_str1), .want = true},
      {.in1 = {"/tmp/test/xml/c/\0", "example.xml\0"}, .in2 = xml_fromString(l_str1), .want = true},
      {.in1 = {"/tmp/test/xml/d/\0", "example.xml\0"}, .in2 = xml_fromString(l_str1), .want = true},
      {.in1 = {"/tmp/test/xml/e/\0", "example.xml\0"}, .in2 = xml_fromString(l_str1), .want = true},
      {.in1  = {"/tmp/test/xml/f//\0", "example.xml\0"},
       .in2  = xml_fromString(l_str1),
       .want = true},
      {.in1  = {"/tmp/test/xml/g///\0", "example.xml\0"},
       .in2  = xml_fromString(l_str1),
       .want = true},
      {.in1  = {"/tmp/test/xml/h///j\0", "example.xml\0"},
       .in2  = xml_fromString(l_str1),
       .want = true},
      {.in1  = {"/tmp/test/xml/i///k/\0", "example.xml\0"},
       .in2  = xml_fromString(l_str1),
       .want = true},
      {.in1  = {"/tmp/test/xml/l/ /m/\0", "example.xml\0"},
       .in2  = xml_fromString(l_str1),
       .want = true},
      {.in1  = {"/tmp/test/xml/n/o p/q/\0", "example.xml\0"},
       .in2  = xml_fromString(l_str1),
       .want = true},
      {.in1  = {"/tmp/test/xml/r/.config\0", "example.xml\0"},
       .in2  = xml_fromString(l_str1),
       .want = true},
      {.in1  = {"/tmp/test/xml/s/.config\0", "example.xml\0"},
       .in2  = xml_fromString(l_str1),
       .want = true},
      {.in1  = {"/tmp/test/xml/{}[]()\0", "example.xml\0"},
       .in2  = xml_fromString(l_str1),
       .want = true},
      {.in1 = {"/tmp/test/xml/⌘☹Ж\0", "\0"}, .in2 = xml_fromString(l_str1), .want = false},
      {.in1 = {"/tmp/test/xml\0", "\0"}, .in2 = xml_fromString(l_str1), .want = false},
      {.in1 = {"\0", "\0"}, .in2 = xml_fromString(l_str1), .want = false},
      {.in1 = {NULL, NULL}, .in2 = xml_fromString(l_str1), .want = false}};

  for (size_t i = 0; i < sizeof(test) / sizeof(test[0]); ++i) {
    setup(test[i].in1[0], test[i].in1[1], NULL);

    test[i].got = xml_updateFile(test[i].in1[0], test[i].in1[1], test[i].in2);
    EXPECT_EQ(test[i].got, test[i].want);

    teardown(test[i].in1[0], test[i].in1[1], test[i].in2);
  }
}

TEST(xml, resetFile) {
  typedef struct s_test {
    char *in1[2];
    mxml_node_t *in2;
    bool want;
    bool got;
  } test_t;

  test_t test[19] = {
      {.in1 = {"/tmp/test/xml/a\0", "example.xml\0"}, .in2 = xml_fromString(l_str2), .want = true},
      {.in1 = {"/tmp/test/xml/a\0", "example.xml\0"}, .in2 = xml_fromString(l_str2), .want = true},
      {.in1 = {"/tmp/test/xml/b\0", "example.xml\0"}, .in2 = xml_fromString(l_str2), .want = true},
      {.in1 = {"/tmp/test/xml/c/\0", "example.xml\0"}, .in2 = xml_fromString(l_str2), .want = true},
      {.in1 = {"/tmp/test/xml/d/\0", "example.xml\0"}, .in2 = xml_fromString(l_str2), .want = true},
      {.in1 = {"/tmp/test/xml/e/\0", "example.xml\0"}, .in2 = xml_fromString(l_str2), .want = true},
      {.in1  = {"/tmp/test/xml/f//\0", "example.xml\0"},
       .in2  = xml_fromString(l_str2),
       .want = true},
      {.in1  = {"/tmp/test/xml/g///\0", "example.xml\0"},
       .in2  = xml_fromString(l_str2),
       .want = true},
      {.in1  = {"/tmp/test/xml/h///j\0", "example.xml\0"},
       .in2  = xml_fromString(l_str2),
       .want = true},
      {.in1  = {"/tmp/test/xml/i///k/\0", "example.xml\0"},
       .in2  = xml_fromString(l_str2),
       .want = true},
      {.in1  = {"/tmp/test/xml/l/ /m/\0", "example.xml\0"},
       .in2  = xml_fromString(l_str2),
       .want = true},
      {.in1  = {"/tmp/test/xml/n/o p/q/\0", "example.xml\0"},
       .in2  = xml_fromString(l_str2),
       .want = true},
      {.in1  = {"/tmp/test/xml/r/.config\0", "example.xml\0"},
       .in2  = xml_fromString(l_str2),
       .want = true},
      {.in1  = {"/tmp/test/xml/s/.config\0", "example.xml\0"},
       .in2  = xml_fromString(l_str2),
       .want = true},
      {.in1  = {"/tmp/test/xml/{}[]()\0", "example.xml\0"},
       .in2  = xml_fromString(l_str2),
       .want = true},
      {.in1 = {"/tmp/test/xml/⌘☹Ж\0", "\0"}, .in2 = xml_fromString(l_str2), .want = false},
      {.in1 = {"/tmp/test/xml\0", "\0"}, .in2 = xml_fromString(l_str2), .want = false},
      {.in1 = {"\0", "\0"}, .in2 = xml_fromString(l_str2), .want = false},
      {.in1 = {NULL, NULL}, .in2 = xml_fromString(l_str2), .want = false}};

  for (size_t i = 0; i < sizeof(test) / sizeof(test[0]); ++i) {
    setup(test[i].in1[0], test[i].in1[1], l_str1);

    test[i].got = xml_resetFile(test[i].in1[0], test[i].in1[1], test[i].in2);
    EXPECT_EQ(test[i].got, test[i].want);

    teardown(test[i].in1[0], test[i].in1[1], test[i].in2);
  }
}

TEST(xml, existIdentifier) {
  typedef struct s_test {
    mxml_node_t *in1;
    char *in2[2];
    uint32_t in3;
    bool want;
    bool got;
  } test_t;

  test_t test[7] = {
      {.in1  = xml_fromString(l_str1),
       .in2  = {"UAVariable\0", "NodeId\0"},
       .in3  = 23001,
       .want = true},
      {.in1  = xml_fromString(l_str1),
       .in2  = {"UAVariable\0", "NodeId\0"},
       .in3  = 23004,
       .want = true},
      {.in1 = NULL, .in2 = {"UAVariable\0", "NodeId\0"}, .in3 = 23001, .want = false},
      {.in1  = xml_fromString(l_str1),
       .in2  = {"UAVariable\0", "NodeId\0"},
       .in3  = 11235,
       .want = false},
      {.in1 = xml_fromString(l_str1), .in2 = {"UAVariable\0", "\0"}, .in3 = 23001, .want = false},
      {.in1 = xml_fromString(l_str1), .in2 = {"\0", "\0"}, .in3 = 23001, .want = false},
      {.in1 = xml_fromString(l_str1), .in2 = {NULL, NULL}, .in3 = 23001, .want = false}};

  for (size_t i = 0; i < sizeof(test) / sizeof(test[0]); ++i) {
    test[i].got = xml_existIdentifier(test[i].in1, test[i].in2[0], test[i].in2[1], test[i].in3);
    EXPECT_EQ(test[i].got, test[i].want);

    teardown(NULL, NULL, test[i].in1);
  }
}

TEST(xml, getIdentifier) {
  typedef struct s_test {
    mxml_node_t *in1;
    char *in2[2];
    uint32_t in3;
    mxml_node_t *want;
    mxml_node_t *got;
  } test_t;

  test_t test[7] = {
      {.in1  = xml_fromString(l_str1),
       .in2  = {"UAVariable\0", "NodeId\0"},
       .in3  = 23001,
       .want = NULL},
      {.in1  = xml_fromString(l_str1),
       .in2  = {"UAVariable\0", "NodeId\0"},
       .in3  = 23004,
       .want = NULL},
      {.in1 = NULL, .in2 = {"UAVariable\0", "NodeId\0"}, .in3 = 23001, .want = (mxml_node_t *)1},
      {.in1  = xml_fromString(l_str1),
       .in2  = {"UAVariable\0", "NodeId\0"},
       .in3  = 11235,
       .want = (mxml_node_t *)1},
      {.in1  = xml_fromString(l_str1),
       .in2  = {"UAVariable\0", "\0"},
       .in3  = 23001,
       .want = (mxml_node_t *)1},
      {.in1 = xml_fromString(l_str1), .in2 = {"\0", "\0"}, .in3 = 23001, .want = (mxml_node_t *)1},
      {.in1 = xml_fromString(l_str1), .in2 = {NULL, NULL}, .in3 = 23001, .want = (mxml_node_t *)1}};

  for (size_t i = 0; i < sizeof(test) / sizeof(test[0]); ++i) {
    test[i].got = xml_getIdentifier(test[i].in1, test[i].in2[0], test[i].in2[1], test[i].in3);
    EXPECT_NE(test[i].got, test[i].want);

    teardown(NULL, NULL, test[i].in1);
  }
}

TEST(xml, setAttribute) {
  typedef struct s_test {
    mxml_node_t *in1;
    char *in2[3];
    uint32_t in3;
    char *in4;
    bool want;
    bool got;
  } test_t;

  test_t test[11] = {{.in1  = xml_fromString(l_str1),
                      .in2  = {"UAVariable\0", "NodeId\0", "Value\0"},
                      .in3  = 23001,
                      .in4  = "11235\0",
                      .want = true},
                     {.in1  = xml_fromString(l_str1),
                      .in2  = {"UAVariable\0", "NodeId\0", "Value\0"},
                      .in3  = 23004,
                      .in4  = "EX245\0",
                      .want = true},
                     {.in1  = xml_fromString(l_str1),
                      .in2  = {"UAVariable\0", "NodeId\0", "Value\0"},
                      .in3  = 23008,
                      .in4  = "11235\0",
                      .want = true},
                     {.in1  = xml_fromString(l_str1),
                      .in2  = {"UAVariable\0", "NodeId\0", "Value\0"},
                      .in3  = 23008,
                      .in4  = "\0",
                      .want = true},
                     {.in1  = xml_fromString(l_str1),
                      .in2  = {"UAVariable\0", "NodeId\0", "Value\0"},
                      .in3  = 23008,
                      .in4  = NULL,
                      .want = false},
                     {.in1  = xml_fromString(l_str1),
                      .in2  = {"UAVariable\0", "NodeId\0", "Value\0"},
                      .in3  = 1,
                      .in4  = "11235\0",
                      .want = false},
                     {.in1  = xml_fromString(l_str1),
                      .in2  = {"UAVariable\0", "NodeId\0", "\0"},
                      .in3  = 23008,
                      .in4  = "11235\0",
                      .want = false},
                     {.in1  = xml_fromString(l_str1),
                      .in2  = {"UAVariable\0", "\0", "Value\0"},
                      .in3  = 23001,
                      .in4  = "11235\0",
                      .want = false},
                     {.in1  = xml_fromString(l_str1),
                      .in2  = {"\0", "NodeId\0", "Value\0"},
                      .in3  = 23001,
                      .in4  = "11235\0",
                      .want = false},
                     {.in1  = xml_fromString(l_str1),
                      .in2  = {"\0", "\0", "\0"},
                      .in3  = 1,
                      .in4  = "\0",
                      .want = false},
                     {.in1  = xml_fromString(l_str1),
                      .in2  = {NULL, NULL, NULL},
                      .in3  = 23001,
                      .in4  = NULL,
                      .want = false}};

  for (size_t i = 0; i < sizeof(test) / sizeof(test[0]); ++i) {
    test[i].got = xml_setAttribute(test[i].in1,
                                   test[i].in2[0],
                                   test[i].in2[1],
                                   test[i].in2[2],
                                   test[i].in3,
                                   test[i].in4);
    EXPECT_EQ(test[i].got, test[i].want);

    teardown(NULL, NULL, test[i].in1);
  }
}

TEST(xml, getAttribute) {
  typedef struct s_test {
    mxml_node_t *in1;
    char *in2[3];
    uint32_t in3;
    char *want;
    char *got;
  } test_t;

  test_t test[9] = {
      {.in1  = xml_fromString(l_str1),
       .in2  = {"UAVariable\0", "NodeId\0", "Value\0"},
       .in3  = 23001,
       .want = "17\0"},
      {.in1  = xml_fromString(l_str1),
       .in2  = {"UAVariable\0", "NodeId\0", "Value\0"},
       .in3  = 23008,
       .want = "131\0"},
      {.in1 = NULL, .in2 = {"UAVariable\0", "NodeId\0", "Value\0"}, .in3 = 23008, .want = NULL},
      {.in1  = xml_fromString(l_str1),
       .in2  = {"UAVariable\0", "NodeId\0", "Value\0"},
       .in3  = 1,
       .want = NULL},
      {.in1  = xml_fromString(l_str1),
       .in2  = {"UAVariable\0", "NodeId\0", "\0"},
       .in3  = 23008,
       .want = NULL},
      {.in1  = xml_fromString(l_str1),
       .in2  = {"UAVariable\0", "\0", "Value\0"},
       .in3  = 23001,
       .want = NULL},
      {.in1  = xml_fromString(l_str1),
       .in2  = {"\0", "NodeId\0", "Value\0"},
       .in3  = 23001,
       .want = NULL},
      {.in1 = xml_fromString(l_str1), .in2 = {"\0", "\0", "\0"}, .in3 = 1, .want = NULL},
      {.in1 = xml_fromString(l_str1), .in2 = {NULL, NULL, NULL}, .in3 = 23001, .want = NULL}};

  for (size_t i = 0; i < sizeof(test) / sizeof(test[0]); ++i) {
    test[i].got =
        xml_getAttribute(test[i].in1, test[i].in2[0], test[i].in2[1], test[i].in2[2], test[i].in3);
    EXPECT_STREQ(test[i].got, test[i].want);

    teardown(NULL, NULL, test[i].in1);
  }
}

TEST(xml, free) {
  typedef struct s_test {
    mxml_node_t *in;
  } test_t;

  test_t test[2] = {{.in = xml_fromString(l_str1)}, {.in = NULL}};

  for (size_t i = 0; i < sizeof(test) / sizeof(test[0]); ++i) {
    xml_free(test[i].in);
  }
}
