#include <gtest/gtest.h>

#include <string>
#include <vector>

#include "DrmJsonObject.h"

TEST(DrmJsonObjectCoverage, AddGetAndPrintRoundTrip)
{
    DrmJsonObject object;
    ASSERT_TRUE(object.add("name", std::string("AAMP")));
    ASSERT_TRUE(object.add("enabled", true));
    ASSERT_TRUE(object.add("count", 7));
    ASSERT_TRUE(object.add("ratio", 1.5));

    const std::vector<std::string> expectedItems{"one", "two"};
    ASSERT_TRUE(object.add("items", expectedItems));

    DrmJsonObject nested;
    ASSERT_TRUE(nested.add("label", std::string("child")));
    ASSERT_TRUE(object.add("nested", nested));

    EXPECT_TRUE(object.isString("name"));
    EXPECT_TRUE(object.isNumber("count"));
    EXPECT_TRUE(object.isArray("items"));
    EXPECT_TRUE(object.isObject("nested"));
    EXPECT_FALSE(object.isArray("missing"));

    std::string name;
    ASSERT_TRUE(object.get("name", name));
    EXPECT_EQ(name, "AAMP");

    int count = 0;
    ASSERT_TRUE(object.get("count", count));
    EXPECT_EQ(count, 7);

    double ratio = 0.0;
    ASSERT_TRUE(object.get("ratio", ratio));
    EXPECT_DOUBLE_EQ(ratio, 1.5);

    std::vector<std::string> items;
    ASSERT_TRUE(object.get("items", items));
    EXPECT_EQ(items, expectedItems);

    DrmJsonObject nestedResult;
    ASSERT_TRUE(object.get("nested", nestedResult));
    std::string label;
    ASSERT_TRUE(nestedResult.get("label", label));
    EXPECT_EQ(label, "child");

    EXPECT_FALSE(object.get("missing", name));
    EXPECT_FALSE(object.print().empty());
    EXPECT_FALSE(object.print_UnFormatted().empty());

    std::vector<uint8_t> printedBytes;
    object.print(printedBytes);
    EXPECT_EQ(std::string(printedBytes.begin(), printedBytes.end()), object.print());
}