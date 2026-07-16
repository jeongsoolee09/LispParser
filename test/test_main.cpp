#include <gmock/gmock.h>
#include <gtest/gtest.h>

class GlobalTestEnvironment : public ::testing::Environment {
public:
  void SetUp() override {}

	void TearDown() override {}
};

int main(int argc, char **argv) {
	::testing::InitGoogleTest(&argc, argv);
	::testing::InitGoogleMock(&argc, argv);

	::testing::AddGlobalTestEnvironment(new GlobalTestEnvironment);

	return RUN_ALL_TESTS();
}
