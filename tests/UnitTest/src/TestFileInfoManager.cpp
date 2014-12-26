#include "gtest/gtest.h"
#include <fstream>
#include <string>
#include "FileInfoManager.h"

//bool create_new_entry(const std::string& filename_upper, const std::string& filename_lower="", int key=-1, const char * mode="w+b")
class FileManagerTest : public ::testing::Test
{
    protected:
        FileManagerTest():FM(JHPCNDF::FileInfoManager::GetInstance()){}
        virtual void TearDown()
        {
            FM.destroy_all();
        }
     JHPCNDF::FileInfoManager& FM;
};
TEST_F(FileManagerTest, CreateNewNormal)
{
    EXPECT_EQ(100, FM.create_new_entry("hoge"));
}
TEST_F(FileManagerTest, CreateNewEmptyFilename)
{
    EXPECT_EQ(-300, FM.create_new_entry(""));
}
TEST_F(FileManagerTest, CreateNewNormalWithEmptyLowerbitFilename)
{
    EXPECT_EQ(100, FM.create_new_entry("hoge", ""));
}
TEST_F(FileManagerTest, CreateNewSameFilename)
{
    EXPECT_EQ(-400, FM.create_new_entry("hoge", "hoge"));
}
TEST_F(FileManagerTest, CreateNewNormalWithLowerbitFilename)
{
    EXPECT_EQ(100, FM.create_new_entry("hoge", "huga"));
}
TEST_F(FileManagerTest, CreateNewDuplicateFilename)
{
    FM.create_new_entry("hoge");
    EXPECT_EQ(-100, FM.create_new_entry("hoge"));
}
TEST_F(FileManagerTest, CreateNewDuplicatekey)
{
    FM.create_new_entry("hoge");
    EXPECT_EQ(-200, FM.create_new_entry("huga","",100));
}

TEST_F(FileManagerTest, CreateNewInvalidMode)
{
    EXPECT_EQ(-500, FM.create_new_entry("huga","",100,"huga"));
}

TEST_F(FileManagerTest, CreateNewNormalWithKey)
{
    EXPECT_EQ(0, FM.create_new_entry("huga","",0));
}

// モード指定に関するテスト
class FileManagerTestWithSpecificMode: public FileManagerTest, public ::testing::WithParamInterface<const char*> {};
TEST_P(FileManagerTestWithSpecificMode, ModeTests)
{
    EXPECT_EQ(10, FM.create_new_entry("huga","",10, GetParam()));
}
INSTANTIATE_TEST_CASE_P(FileManagerTest,
        FileManagerTestWithSpecificMode,
        ::testing::Values("r","rb", "r+", "r+b", "w", "wb", "w+", "w+b", "a", "ab", "a+", "a+b"));

class FileManagerTest2: public FileManagerTest
{
    protected:
        virtual void SetUp()
        {
            FM.create_new_entry("upper", "lower", 10);
        }
};
TEST_F(FileManagerTest2, destroy)
{
    FM.destroy_entry(10);
    EXPECT_EQ(NULL, FM.get_upper_file_pointer(10));
    EXPECT_EQ(NULL, FM.get_lower_file_pointer(10));
}

TEST_F(FileManagerTest2, get_upper)
{
    FILE* tmp=FM.get_upper_file_pointer(10);
    fwrite("upper\n", 6, 1, tmp);
    FM.destroy_entry(10);
    std::ifstream ifs("upper");
    char test[10];
    ifs.read(test, 6);
    EXPECT_STREQ("upper\n", test);
}
TEST_F(FileManagerTest2, get_lower)
{
    FILE* tmp=FM.get_lower_file_pointer(10);
    fwrite("lower\n", 6, 1, tmp);
    FM.destroy_entry(10);
    std::ifstream ifs("lower");
    char test[10];
    ifs.read(test, 6);
    EXPECT_STREQ("lower\n", test);
}
