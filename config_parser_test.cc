#include "gtest/gtest.h"
#include "config_parser.h"

TEST(NginxConfigParserTest, SimpleConfig) {
  NginxConfigParser parser;
  NginxConfig out_config;

  bool success = parser.Parse("example_config", &out_config);

  EXPECT_TRUE(success);
}

class NginxStringConfigTest: public ::testing::Test
{
protected:
  bool ParseString(const std::string config_string)
  {
    std::stringstream config_stream(config_string);
    return parser_.Parse(&config_stream, &out_config_);
  }
  NginxConfigParser parser_;
  NginxConfig out_config_;
};

TEST_F(NginxStringConfigTest, SimpleConfig_2)
{
  EXPECT_TRUE(ParseString("hello hi;"));
  EXPECT_EQ(1, out_config_.statements_.size());
  EXPECT_EQ("hello", out_config_.statements_[0]->tokens_[0]);
  EXPECT_EQ("hi", out_config_.statements_[0]->tokens_[1]);
}

TEST_F(NginxStringConfigTest, SimpleConfig_3)
{
  EXPECT_TRUE(ParseString("hello {hi hey;}"));
}

TEST_F(NginxStringConfigTest, MultipleTokens)
{
  EXPECT_TRUE(ParseString("hello hi hola bonjour;"));
  EXPECT_EQ(4, out_config_.statements_[0]->tokens_.size());
}

TEST_F(NginxStringConfigTest, MultipleStatements)
{
  EXPECT_TRUE(ParseString("hello hi;\nhola hey;"));
  EXPECT_EQ(2, out_config_.statements_.size());
}

TEST_F(NginxStringConfigTest, WrongCurlyBraces)
{
  EXPECT_FALSE(ParseString("hello }hi hey;{"));
}

TEST_F(NginxStringConfigTest, TooManySemicolons)
{
  EXPECT_FALSE(ParseString("hello hi;;"));
}

TEST_F(NginxStringConfigTest, UnclosedBraces)
{
  EXPECT_FALSE(ParseString("hello {hi hi;"));
}
