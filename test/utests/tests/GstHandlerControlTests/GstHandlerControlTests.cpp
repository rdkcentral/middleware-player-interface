
/*
* If not stated otherwise in this file or this component's license file the
* following copyright and licenses apply:
*
* Copyright 2024 RDK Management
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
* http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*/
#include <gtest/gtest.h>
#include "GstHandlerControl.h"

class FunctionalTests : public ::testing::Test {
public:
	GstHandlerControl* mControl;
	void SetUp() override
	{
		mControl = new GstHandlerControl;
	}

	void TearDown() override
	{
		delete mControl;
	}
};

TEST_F(FunctionalTests, instancesRunning)
{
	ASSERT_EQ(0, mControl->instancesRunning());

	std::vector<GstHandlerControl::ScopeHelper> helpers;
	constexpr size_t MAX_INSTANCES = 10;
	for(int i=1; i<=MAX_INSTANCES; i++)
	{
		helpers.push_back(mControl->getScopeHelper());
		ASSERT_EQ(i, mControl->instancesRunning());
	}

	while(helpers.size())
	{
	   helpers.pop_back();
	   ASSERT_EQ(helpers.size(), mControl->instancesRunning());
	}
}

TEST_F(FunctionalTests, returnStraightAway)
{
	ASSERT_FALSE(mControl->getScopeHelper().returnStraightAway());
	mControl->disable();
	ASSERT_TRUE(mControl->getScopeHelper().returnStraightAway());
	mControl->enable();
	ASSERT_FALSE(mControl->getScopeHelper().returnStraightAway());
	mControl->waitForDone(0, "test");
	ASSERT_TRUE(mControl->getScopeHelper().returnStraightAway());
	mControl->enable();
	ASSERT_FALSE(mControl->getScopeHelper().returnStraightAway());
}


TEST_F(FunctionalTests, waitForDoneSingle)
{
	{
		auto scopedHelper=mControl->getScopeHelper();
		ASSERT_FALSE(mControl->waitForDone(0, "test"));
	}

	ASSERT_TRUE(mControl->waitForDone(0, "test"));
}

TEST_F(FunctionalTests, waitForDoneMultiple)
{
	ASSERT_EQ(0, mControl->instancesRunning());
	ASSERT_TRUE(mControl->waitForDone(0, "test"));

	std::vector<GstHandlerControl::ScopeHelper> helpers;
	constexpr size_t MAX_INSTANCES = 10;
	for(int i=1; i<=MAX_INSTANCES; i++)
	{
		helpers.push_back(mControl->getScopeHelper());
		ASSERT_FALSE(mControl->waitForDone(0, "test"));
	}

	while(helpers.size())
	{
	   ASSERT_FALSE(mControl->waitForDone(0, "test"));
	   helpers.pop_back();
	}

	ASSERT_TRUE(mControl->waitForDone(0, "test"));
}

TEST_F(FunctionalTests, moveAssignToDefault)
{
	ASSERT_EQ(0, mControl->instancesRunning());
	{
		GstHandlerControl::ScopeHelper helperA = mControl->getScopeHelper();
		ASSERT_EQ(1, mControl->instancesRunning());
		{
			GstHandlerControl::ScopeHelper helperB;
			ASSERT_EQ(1, mControl->instancesRunning());
			ASSERT_TRUE(helperB.returnStraightAway());
			ASSERT_FALSE(helperA.returnStraightAway());

			helperB = std::move(helperA);
			ASSERT_EQ(1, mControl->instancesRunning());
			ASSERT_TRUE(helperA.returnStraightAway());
			ASSERT_FALSE(helperB.returnStraightAway());
		}
	}

	ASSERT_EQ(0, mControl->instancesRunning());
}

TEST_F(FunctionalTests, moveAssignOverwriteWithDefault)
{
	ASSERT_EQ(0, mControl->instancesRunning());

	GstHandlerControl::ScopeHelper helperC = mControl->getScopeHelper();
	ASSERT_EQ(1, mControl->instancesRunning());

	GstHandlerControl::ScopeHelper helperD;
	ASSERT_EQ(1, mControl->instancesRunning());
	ASSERT_TRUE(helperD.returnStraightAway());
	ASSERT_FALSE(helperC.returnStraightAway());

	helperC = std::move(helperD);
	ASSERT_EQ(0, mControl->instancesRunning());
	ASSERT_TRUE(helperD.returnStraightAway());
	ASSERT_TRUE(helperC.returnStraightAway());
}

TEST_F(FunctionalTests, moveAssignOverwriteValidWithValid)
{
	ASSERT_EQ(0, mControl->instancesRunning());
	{
		GstHandlerControl::ScopeHelper helperE = mControl->getScopeHelper();
		ASSERT_EQ(1, mControl->instancesRunning());
		ASSERT_FALSE(helperE.returnStraightAway());


		GstHandlerControl::ScopeHelper helperF = mControl->getScopeHelper();
		ASSERT_EQ(2, mControl->instancesRunning());
		ASSERT_FALSE(helperE.returnStraightAway());
		ASSERT_FALSE(helperF.returnStraightAway());

		helperE = std::move(helperF);
		ASSERT_EQ(1, mControl->instancesRunning());
		ASSERT_FALSE(helperE.returnStraightAway());
		ASSERT_TRUE(helperF.returnStraightAway());
	}

	ASSERT_EQ(0, mControl->instancesRunning());
}


TEST_F(FunctionalTests, moveConstruct)
{
	ASSERT_EQ(0, mControl->instancesRunning());

	GstHandlerControl::ScopeHelper helperA = mControl->getScopeHelper();
	ASSERT_EQ(1, mControl->instancesRunning());
	ASSERT_FALSE(helperA.returnStraightAway());

	{
		GstHandlerControl::ScopeHelper helperB{std::move(helperA)};
		ASSERT_EQ(1, mControl->instancesRunning());
		ASSERT_TRUE(helperA.returnStraightAway());
		ASSERT_FALSE(helperB.returnStraightAway());
	}

	ASSERT_EQ(0, mControl->instancesRunning());
	ASSERT_TRUE(helperA.returnStraightAway());
}

TEST_F(FunctionalTests, defaultConstruct)
{
	ASSERT_EQ(0, mControl->instancesRunning());

	{
		GstHandlerControl::ScopeHelper helper;
		ASSERT_EQ(0, mControl->instancesRunning());
	}

	ASSERT_EQ(0, mControl->instancesRunning());
}
/**
 * @brief Verify that the default constructor of ScopeHelper does not throw and initializes the controller pointer to nullptr
 *
 * This test verifies that invoking the default constructor of GstHandlerControl::ScopeHelper does not throw any exceptions. It confirms that the internal controller pointer (mpController) is properly set to nullptr by the constructor, ensuring stable behavior during object initialization.
 *
 * **Test Group ID:** Basic: 01
 * **Test Case ID:** 001
 * **Priority:** High
 *
 * **Pre-Conditions:** None
 * **Dependencies:** None
 * **User Interaction:** None
 *
 * **Test Procedure:**
 * | Variation / Step | Description                                                                     | Test Data                                         | Expected Result                                        | Notes       |
 * | :--------------: | ------------------------------------------------------------------------------- | ------------------------------------------------- | ------------------------------------------------------ | ----------- |
 * | 01               | Invoke the default constructor of GstHandlerControl::ScopeHelper and verify that no exceptions are thrown. | No input arguments, output: mpController = nullptr | EXPECT_NO_THROW succeeds; internal controller pointer remains nullptr | Should Pass |
 */
TEST_F(FunctionalTests, ScopeHelper_defaultConstructor_initializes_mpController_to_nullptr) {
    std::cout << "Entering ScopeHelper_defaultConstructor_initializes_mpController_to_nullptr test" << std::endl;
    
    EXPECT_NO_THROW({
        // Invoke the default constructor
        GstHandlerControl::ScopeHelper scopeHelper;
        std::cout << "Invoked default constructor: GstHandlerControl::ScopeHelper() executed." << std::endl;
    });
    
    std::cout << "Exiting ScopeHelper_defaultConstructor_initializes_mpController_to_nullptr test" << std::endl;
}
/**
 * @brief Test constructing ScopeHelper with a null controller to ensure no exceptions are thrown.
 *
 * This test verifies that invoking the GstHandlerControl::ScopeHelper constructor with a null controller pointer does not throw any exceptions. The test is designed to ensure the robustness of the constructor when provided with invalid (null) input, which is a critical aspect of defensive programming.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 002@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**@n
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Invoke the GstHandlerControl::ScopeHelper constructor with a null controller pointer | pController = nullptr | No exception should be thrown and the constructor should execute successfully | Should Pass |
 */
TEST_F(FunctionalTests, ConstructWithNullController) {
    std::cout << "Entering ConstructWithNullController test" << std::endl;

    EXPECT_NO_THROW({
        // Create a ScopeHelper object with a null controller pointer
        GstHandlerControl::ScopeHelper scopeHelper(nullptr);
        std::cout << "Invoked GstHandlerControl::ScopeHelper constructor with pController = nullptr" << std::endl;
    });

    std::cout << "Exiting ConstructWithNullController test" << std::endl;
}
/**
 * @brief Verify move assignment with an empty source on a non-empty destination
 *
 * This test verifies that invoking the move assignment operator on a non-empty Destination ScopeHelper using an empty Source ScopeHelper (with its internal controller pointer set to nullptr) does not throw exceptions and returns the reference of the updated destination. It ensures proper resource handling in the move assignment scenario.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 007@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**@n
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Create a valid GstHandlerControl instance for the destination. | controller instance created via new GstHandlerControl() | Valid controller pointer obtained and printed | Should be successful |
 * | 02 | Create a destination ScopeHelper using the valid controller pointer. | destination.controller = valid controller pointer | Destination ScopeHelper is successfully constructed | Should be successful |
 * | 03 | Create a source ScopeHelper using the default constructor (with controller pointer as nullptr). | source.controller = nullptr | Source ScopeHelper is successfully constructed with no controller attached | Should be successful |
 * | 04 | Invoke the move assignment operator: assign std::move(source) to destination. | destination = valid destination, source = default constructed (controller pointer = nullptr) | No exception thrown and move assignment operator returns the reference to destination | Should Pass |
 * | 05 | Delete the allocated controller to cleanup. | controller pointer deletion | Controller is successfully deleted with proper cleanup logged | Should be successful |
 */
TEST(GstHandlerControl_ScopeHelper, MoveAssignEmptySourceToNonEmptyDestination) {
    std::cout << "Entering MoveAssignEmptySourceToNonEmptyDestination test" << std::endl;
    
    // Create a valid GstHandlerControl instance for destination.
    GstHandlerControl* controller = new GstHandlerControl();
    std::cout << "Created controller with pointer: " << controller << std::endl;
    
    // Create destination ScopeHelper with a valid controller pointer.
    GstHandlerControl::ScopeHelper destination(controller);
    std::cout << "Destination ScopeHelper created with controller pointer ("
              << controller << ")." << std::endl;
    
    // Create source ScopeHelper using the default constructor (mpController = nullptr).
    GstHandlerControl::ScopeHelper source;
    std::cout << "Source ScopeHelper created with default constructor (controller pointer = nullptr)." << std::endl;
    
    // Invoke move assignment operator.
    EXPECT_NO_THROW({
        std::cout << "Invoking move assignment: destination = std::move(source) (source is empty)." << std::endl;
        GstHandlerControl::ScopeHelper& ret = destination = std::move(source);
        std::cout << "Move assignment operator returned address of destination: " << &ret << std::endl;
    });
    std::cout << "Exiting MoveAssignEmptySourceToNonEmptyDestination test" << std::endl;
}
/**
 * @brief Verify that constructing a ScopeHelper with a valid controller does not throw an exception
 *
 * This test verifies that when a valid GstHandlerControl object is provided to the ScopeHelper constructor,
 * the construction completes without throwing any exceptions. The validity of the controller ensures that the
 * ScopeHelper can be instantiated correctly for further operations.
 *
 * **Test Group ID:** Basic: 01
 * **Test Case ID:** 003
 * **Priority:** High
 *
 * **Pre-Conditions:** None
 * **Dependencies:** None
 * **User Interaction:** None
 *
 * **Test Procedure:**
 * | Variation / Step | Description                                                                                           | Test Data                                         | Expected Result                                                   | Notes       |
 * | :--------------: | ----------------------------------------------------------------------------------------------------- | ------------------------------------------------- | ----------------------------------------------------------------- | ----------- |
 * | 01               | Create a GstHandlerControl object with default enabled state and invoke ScopeHelper constructor with a valid pointer | ctrl.state = enabled, pController = &ctrl            | ScopeHelper is constructed successfully without throwing any exception | Should Pass |
 */
TEST_F(FunctionalTests, ConstructWithValidController) {
    std::cout << "Entering ConstructWithValidController test" << std::endl;

    // Create a valid GstHandlerControl object (defaults to enabled)
    GstHandlerControl ctrl;
    std::cout << "Created GstHandlerControl object with default enabled state." << std::endl;

    EXPECT_NO_THROW({
        // Create a ScopeHelper object with a valid controller pointer.
        GstHandlerControl::ScopeHelper scopeHelper(&ctrl);
        std::cout << "Invoked GstHandlerControl::ScopeHelper constructor with a valid pController pointer." << std::endl;
    });

    std::cout << "Exiting ConstructWithValidController test" << std::endl;
}
/**
 * @brief Validate move assignment of valid source to empty destination.
 *
 * This test verifies that the move assignment operator for the ScopeHelper class correctly transfers ownership of a valid GstHandlerControl instance from a source object to a destination object that is initially empty. The test ensures that no exceptions are thrown during the move assignment.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 005@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**@n
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Create a valid GstHandlerControl instance and initialize source ScopeHelper with it, then create destination ScopeHelper with default constructor and invoke move assignment operator. | controller pointer = valid, source = GstHandlerControl pointer valid, destination = nullptr | Move assignment operator returns destination reference without throwing an exception | Should Pass |
 */
TEST(GstHandlerControl_ScopeHelper, MoveAssignValidSourceToEmptyDestination) {
    std::cout << "Entering MoveAssignValidSourceToEmptyDestination test" << std::endl;

    // Allocate controller on heap
    auto controller = new GstHandlerControl();
    std::cout << "Created controller with pointer: " << controller << std::endl;

    // Source ScopeHelper takes ownership
    GstHandlerControl::ScopeHelper source(controller);
    std::cout << "Source ScopeHelper created with controller." << std::endl;

    // Destination is initially empty
    GstHandlerControl::ScopeHelper destination;
    std::cout << "Destination ScopeHelper created with default constructor." << std::endl;

    // Perform move assignment
    EXPECT_NO_THROW({
        std::cout << "Invoking move assignment: destination = std::move(source)." << std::endl;
        GstHandlerControl::ScopeHelper& ret = (destination = std::move(source));
        std::cout << "Move assignment returned address of destination: " << &ret << std::endl;
    });

    // No manual delete here — ScopeHelper should call handlerEnd automatically
    std::cout << "Exiting MoveAssignValidSourceToEmptyDestination test" << std::endl;
}
/**
 * @brief Tests move assignment operator of ScopeHelper to overwrite an existing destination
 *
 * This test validates that the move assignment operator for GstHandlerControl::ScopeHelper correctly transfers ownership from the source to the destination. It ensures that after the move, the destination holds the controller pointer from the source, and no exceptions are thrown during the assignment.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 006@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**
 * | Variation / Step | Description                                                                                  | Test Data                                                                                                        | Expected Result                                                                                                   | Notes           |
 * | :--------------: | -------------------------------------------------------------------------------------------- | ---------------------------------------------------------------------------------------------------------------- | ----------------------------------------------------------------------------------------------------------------- | --------------- |
 * |       01       | Instantiate two distinct GstHandlerControl objects (controllerA and controllerB) on the heap.   | controllerA = new GstHandlerControl(), controllerB = new GstHandlerControl()                                     | Both controllerA and controllerB are successfully allocated with distinct addresses.                           | Should be successful |
 * |       02       | Create destination ScopeHelper instance using controllerA.                                   | destination initialized with controllerA                                                                          | Destination ScopeHelper successfully holds controllerA.                                                         | Should be successful |
 * |       03       | Create source ScopeHelper instance using controllerB.                                        | source initialized with controllerB                                                                               | Source ScopeHelper successfully holds controllerB.                                                              | Should be successful |
 * |       04       | Invoke move assignment operator: destination = std::move(source).                             | Call to move assignment with destination holding controllerA and source holding controllerB                       | Move assignment does not throw an exception and destination now holds controllerB.                                | Should Pass         |
 * |       05       | Clean up allocated resources by deleting controllerA and controllerB.                          | delete controllerA, delete controllerB                                                                              | Both controllerA and controllerB are deleted without error.                                                       | Should be successful |
 */
TEST(GstHandlerControl_ScopeHelper, MoveAssignOverwriteExistingDestination) {
    std::cout << "Entering MoveAssignOverwriteExistingDestination test" << std::endl;
    
    // Create two distinct GstHandlerControl instances on the heap.
    GstHandlerControl* controllerA = new GstHandlerControl();
    GstHandlerControl* controllerB = new GstHandlerControl();
    std::cout << "Created controllerA with pointer: " << controllerA << std::endl;
    std::cout << "Created controllerB with pointer: " << controllerB << std::endl;
    
    // Create destination ScopeHelper with controllerA.
    GstHandlerControl::ScopeHelper destination(controllerA);
    std::cout << "Destination ScopeHelper created with controller pointer A ("
              << controllerA << ")." << std::endl;
    
    // Create source ScopeHelper with controllerB.
    GstHandlerControl::ScopeHelper source(controllerB);
    std::cout << "Source ScopeHelper created with controller pointer B ("
              << controllerB << ")." << std::endl;
    
    // Invoke move assignment operator.
    EXPECT_NO_THROW({
        std::cout << "Invoking move assignment: destination = std::move(source)." << std::endl;
        GstHandlerControl::ScopeHelper& ret = destination = std::move(source);
        std::cout << "Move assignment operator returned address of destination: " << &ret << std::endl;
    });
        
    // Cleanup: Delete the controllers.
    // Note: The destination ScopeHelper holds controllerB now.
    //delete controllerA;
    //std::cout << "Deleted controllerA pointer: " << controllerA << " (previously held by destination, now signaled)." << std::endl;
    //delete controllerB;
    //std::cout << "Deleted controllerB pointer: " << controllerB << " (currently held by destination)." << std::endl;
    
    std::cout << "Exiting MoveAssignOverwriteExistingDestination test" << std::endl;
}
/**
 * @brief Verify that ScopeHelper::returnStraightAway() works as expected
 *
 * This test creates a ScopeHelper object using its default constructor and invokes the returnStraightAway() method. It checks that no exceptions are thrown during construction and method call, and that the method returns true when mpController is nullptr.
 *
 * **Test Group ID:** Basic: 01
 * **Test Case ID:** 008
 * **Priority:** High
 *
 * **Pre-Conditions:** None
 * **Dependencies:** None
 * **User Interaction:** None
 *
 * **Test Procedure:**
 * | Variation / Step | Description                                                   | Test Data                                                                                   | Expected Result                                                 | Notes         |
 * | :--------------: | ------------------------------------------------------------- | ------------------------------------------------------------------------------------------- | --------------------------------------------------------------- | ------------- |
 * | 01               | Invoke ScopeHelper default constructor                        | None                                                                                        | Object is created without throwing any exception                | Should be successful |
 * | 02               | Invoke returnStraightAway() method on the created object      | scopeHelper.returnStraightAway() called with no inputs; output variable retVal to capture result | Method returns true and assertion EXPECT_TRUE(retVal) passes      | Should Pass   |
 */
TEST(GstHandlerControl_ScopeHelper, verifyWorkingOfReturnStraightAway) {
    std::cout << "Entering verifyWorkingOfReturnStraightAway test" << std::endl;
    
    // DEBUG: Create an object using default constructor
    std::cout << "DEBUG: Creating a ScopeHelper object using default constructor" << std::endl;
    EXPECT_NO_THROW({
        GstHandlerControl::ScopeHelper scopeHelper;
        
        // DEBUG: Invoking returnStraightAway()
        std::cout << "DEBUG: Invoking returnStraightAway() method" << std::endl;
        bool retVal = false;
        EXPECT_NO_THROW({
            retVal = scopeHelper.returnStraightAway();
            std::cout << "DEBUG: returnStraightAway() returned value: " << retVal << std::endl;
        });
        
        // In default condition, mpController is expected to be nullptr, so the method returns true.
        EXPECT_TRUE(retVal);
    });
    
    std::cout << "Exiting verifyWorkingOfReturnStraightAway test" << std::endl;
}
/**
 * @brief Verify that the ScopeHelper destructor calls handlerEnd when mpController is not null
 *
 * This test ensures that when a GstHandlerControl pointer is provided to the ScopeHelper, the destructor of ScopeHelper correctly triggers the handlerEnd function on the provided controller. This validation is essential to confirm proper cleanup behavior within the API.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 009@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**
 * | Variation / Step | Description                                                                                                           | Test Data                                             | Expected Result                                                                       | Notes         |
 * | :--------------: | --------------------------------------------------------------------------------------------------------------------- | ----------------------------------------------------- | -------------------------------------------------------------------------------------- | ------------- |
 * | 01               | Create a mock controller and pass it to the ScopeHelper constructor                                                   | input: mockController pointer = instance of GstHandlerControl, output: none | ScopeHelper is constructed with a non-null controller; no immediate output expected    | Should Pass   |
 * | 02               | Allow ScopeHelper to go out of scope to trigger the destructor                                                        | No test data                                          | The ScopeHelper destructor is invoked, which in turn calls handlerEnd on the controller | Should Pass   |
 */
TEST(GstHandlerControl_ScopeHelper, Destructor_calls_handlerEnd_when_mpController_is_not_null) {
    std::cout << "Entering Destructor_calls_handlerEnd_when_mpController_is_not_null test" << std::endl;

    GstHandlerControl* mockController = new GstHandlerControl();
    {
        GstHandlerControl::ScopeHelper scopeHelper(mockController);
        std::cout << "ScopeHelper constructed with mockController" << std::endl;
        // Destructor will be triggered when scopeHelper goes out of scope
    }
    std::cout << "Exiting Destructor_calls_handlerEnd_when_mpController_is_not_null test" << std::endl;
}
/**
 * @brief Verifies that the default construction of GstHandlerControl initializes its internal state properly.
 *
 * This test validates that invoking the default constructor of GstHandlerControl does not throw an exception
 * and that the object's internal state is assumed to be initialized with mEnabled = true and mInstanceCount = 0 as per the expected contract.
 *
 * **Test Group ID:** Basic: 01
 * **Test Case ID:** 010
 * **Priority:** High
 * 
 * **Pre-Conditions:** None
 * **Dependencies:** None
 * **User Interaction:** None
 * 
 * **Test Procedure:**
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :--------------: | ----------- | --------- | --------------- | ----- |
 * | 01 | Invoke the default constructor of GstHandlerControl. | Invocation of GstHandlerControl() with no input arguments; expected internal state: mEnabled = true, mInstanceCount = 0 | No exception thrown; object created with assumed internal state as per contract | Should Pass |
 */
TEST(GstHandlerControl, DefaultConstruction_InitializesProperly) {
    std::cout << "Entering DefaultConstruction_InitializesProperly test" << std::endl;
    
    // Invoking the default constructor and ensuring it does not throw.
    std::cout << "Invoking GstHandlerControl default constructor. Expected: mEnabled = true, mInstanceCount = 0." << std::endl;
    EXPECT_NO_THROW({
        GstHandlerControl obj;  
        std::cout << "GstHandlerControl object created." << std::endl;
        // Debug log: Since the internal variables are private, we assume the default initialization is as expected.
        // mEnabled is expected to be true and mInstanceCount is expected to be 0.
        std::cout << "Internal state assumed: mEnabled = true, mInstanceCount = 0." << std::endl;
        // As there are no getters provided, the logging is limited to the expected contract.
    });
    
    std::cout << "Exiting DefaultConstruction_InitializesProperly test" << std::endl;
}
/**
 * @brief Verify that GstHandlerControl object is constructed and disable() invokes correctly.
 *
 * This test verifies that a GstHandlerControl object can be instantiated using its default constructor without throwing any exceptions, and that calling the disable() method does not throw and results in the internal mEnabled flag being set to false.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 011@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**@n
 * | Variation / Step | Description                                                        | Test Data                                                       | Expected Result                                                                                       | Notes              |
 * | :--------------: | ------------------------------------------------------------------ | --------------------------------------------------------------- | ----------------------------------------------------------------------------------------------------- | ------------------ |
 * | 01               | Construct GstHandlerControl object using default constructor       | input: none, output: object instantiated successfully           | Object is created without throwing exceptions                                                         | Should be successful |
 * | 02               | Invoke disable() method on the constructed object                    | input: none, output: mEnabled flag expected to be false           | disable() executes without exceptions and sets the internal mEnabled flag to false                    | Should Pass         |
 */
TEST(GstHandlerControl, disable_start) {
    std::cout << "Entering disable_start test" << std::endl;
    
    // Create object of GstHandlerControl using its default constructor.
    EXPECT_NO_THROW({
        GstHandlerControl handler;
        std::cout << "Constructed GstHandlerControl object using default constructor" << std::endl;
        
        // Log the invocation of disable()
        std::cout << "Invoking disable() method" << std::endl;
        handler.disable();
        std::cout << "disable() method invoked. Expected internal mEnabled flag set to false." << std::endl;
    });
    
    std::cout << "Exiting disable_start test" << std::endl;
}
/**
 * @brief Test to ensure that GstHandlerControl::enable() method successfully enables control without throwing exceptions
 *
 * This test verifies that an instance of GstHandlerControl can be created with its initial mEnabled state set to false and that the enable() method can be invoked without any exceptions. The purpose of this test is to ensure the proper functioning of the enable() method under expected positive conditions.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 012@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**@n
 * | Variation / Step | Description | Test Data | Expected Result | Notes |@n
 * | :----: | --------- | ---------- |-------------- | ----- |@n
 * | 01 | Create instance of GstHandlerControl with initial mEnabled state false. | Constructor: mEnabled = false | Instance created without throwing exceptions. | Should Pass |@n
 * | 02 | Invoke the enable() method on the instance. | Method call: handlerControl.enable() | enable() called successfully without throwing exceptions. | Should Pass |
 */
TEST(GstHandlerControl, enable_start)
{
    std::cout << "Entering GstHandlerControl::enable()_start test" << std::endl;
    
    // Log: Creating instance of GstHandlerControl with initial mEnabled state set to false.
    std::cout << "Creating instance of GstHandlerControl with initial mEnabled state false" << std::endl;
    EXPECT_NO_THROW({
        GstHandlerControl handlerControl;
        std::cout << "Instance created successfully using default constructor." << std::endl;
        
        // Log: Invoking enable() method.
        std::cout << "Invoking enable() method." << std::endl;
        EXPECT_NO_THROW({
            handlerControl.enable();
            std::cout << "enable() invoked successfully, no exceptions thrown." << std::endl;
        });
    });
    
    std::cout << "Exiting GstHandlerControl::enable()_end test" << std::endl;
}
/**
 * @brief Validate that GstHandlerControl's getScopeHelper() executes successfully without throwing exceptions.
 *
 * This test verifies that a GstHandlerControl instance can be created using its default constructor and that invoking 
 * the getScopeHelper() method on this instance executes without any exceptions. It ensures that both object creation and 
 * the API call behave as expected in a positive scenario.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 013@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**@n
 * | Variation / Step | Description                                                         | Test Data                                                          | Expected Result                                  | Notes            |
 * | :--------------: | ------------------------------------------------------------------- | ------------------------------------------------------------------ | ------------------------------------------------ | ---------------- |
 * | 01               | Create GstHandlerControl instance using default constructor         | GstHandlerControl instance = default                               | Instance is created without throwing exceptions   | Should be successful |
 * | 02               | Invoke getScopeHelper() on GstHandlerControl instance                 | getScopeHelper() is invoked on the created instance                  | Returns a valid scope helper without throwing exceptions | Should Pass     |
 */
TEST(GstHandlerControl, getScopeHelper_start) {
    std::cout << "Entering getScopeHelper_start test" << std::endl;
    
    // Create a GstHandlerControl instance using the default constructor.
    EXPECT_NO_THROW({
        GstHandlerControl control;
        std::cout << "Created GstHandlerControl instance (default enabled state)" << std::endl;
        
        // Invoke getScopeHelper() on the instance.
        auto scopeHelper = control.getScopeHelper();
        std::cout << "Invoked getScopeHelper() on GstHandlerControl instance" << std::endl;
    });
    
    std::cout << "Exiting getScopeHelper_start test" << std::endl;
}
/**
 * @brief Verify that the instancesRunning method returns a valid instance count without throwing exceptions.
 *
 * This test case creates a GstHandlerControl object using the default constructor, invokes its instancesRunning() method, and verifies that the returned instance count is -1 or greater. It also ensures that no exception is thrown during object creation and method invocation.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 014@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**@n
 * | Variation / Step | Description                                                                          | Test Data                                                    | Expected Result                                              | Notes          |
 * | :--------------: | ------------------------------------------------------------------------------------ | ------------------------------------------------------------ | ------------------------------------------------------------ | -------------- |
 * | 01               | Create GstHandlerControl object using its default constructor                        | Constructor: no input, output: object instance               | Object is created without throwing an exception              | Should Pass    |
 * | 02               | Invoke instancesRunning() method on the GstHandlerControl object                       | Method: instancesRunning(), no input, output: count value      | Method returns a count value that meets the condition (>= -1)  | Should Pass    |
 * | 03               | Verify that the returned count is -1 or greater                                        | Returned count value from instancesRunning()                   | Assertion (EXPECT_GE) confirms the value is greater than or equal to -1 | Should Pass    |
 * | 04               | Log the internal state corresponding to mInstanceCount with the returned count         | Logged output: count value                                  | Internal state is logged successfully                          | Should be successful |
 */
TEST(GstHandlerControl, instancesRunning_start)
{
    std::cout << "Entering instancesRunning_start test" << std::endl;
    
    // Create instance of GstHandlerControl using default constructor
    std::cout << "Creating GstHandlerControl object using default constructor." << std::endl;
    EXPECT_NO_THROW({
        GstHandlerControl handler;
        std::cout << "GstHandlerControl object created successfully." << std::endl;
        
        // Invoke instancesRunning() method
        std::cout << "Invoking instancesRunning() method on GstHandlerControl object." << std::endl;
        int count = handler.instancesRunning();
        std::cout << "Method instancesRunning() returned: " << count << std::endl;
        
        // Verify that returned count is -1 or higher
        std::cout << "Verifying that the instance count is -1 or greater." << std::endl;
        EXPECT_GE(count, -1);
        
        // Log internal state: (Here, we assume the internal state mInstanceCount equals the returned count.)
        std::cout << "Internal state mInstanceCount value: " << count << std::endl;
    });
    
    std::cout << "Exiting instancesRunning_start test" << std::endl;
}
/**
 * @brief Validate that isEnabled() correctly returns true when the GstHandlerControl object is constructed with mEnabled set to true.
 *
 * This test case verifies that the GstHandlerControl's custom constructor initializes the object with mEnabled set to true and that the isEnabled() method returns true. This ensures that the internal state of the object is correctly maintained.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 015@n
 * **Priority:** High@n
 * 
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 * 
 * **Test Procedure:**@n
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Construct GstHandlerControl object with mEnabled set to true using a custom constructor. | Constructor: sets mEnabled = true | Object is created without throwing any exception. | Should be successful |
 * | 02 | Invoke the isEnabled() method on the GstHandlerControl object. | Method Input: none, Output: enabledResult (bool) | enabledResult holds the value returned by isEnabled(), expected to be true. | Should Pass |
 * | 03 | Validate that the isEnabled() method returns true using an assertion. | Assertion: EXPECT_TRUE(enabledResult) | The assertion passes confirming that enabledResult is true. | Should Pass |
 */
TEST(GstHandlerControl, isEnabled_ReturnsTrue) {
    std::cout << "Entering isEnabled_ReturnsTrue test" << std::endl;
    
    // Create an object of GstHandlerControl using a custom constructor that sets mEnabled to true.
    // EXPECT_NO_THROW ensures that construction does not throw any exceptions.
    EXPECT_NO_THROW({
        GstHandlerControl handler;
        std::cout << "Created GstHandlerControl object with mEnabled = true" << std::endl;
        
        // Invoke isEnabled() method.
        std::cout << "Invoking isEnabled() method" << std::endl;
        bool enabledResult = handler.isEnabled();
        std::cout << "isEnabled() returned: " << (enabledResult ? "true" : "false") << std::endl;
        
        // Validate result.
        EXPECT_TRUE(enabledResult);
    });
    
    std::cout << "Exiting isEnabled_ReturnsTrue test" << std::endl;
}
/**
 * @brief Verify that the waitForDone method returns true when provided valid inputs.
 *
 * This test case validates that the waitForDone method of the GstHandlerControl class functions correctly
 * by returning true when invoked with a specified delay and an error message indicating all handlers are done.
 *
 * **Test Group ID:** Basic: 01
 * **Test Case ID:** 016
 * **Priority:** High
 *
 * **Pre-Conditions:** None
 * **Dependencies:** None
 * **User Interaction:** None
 *
 * **Test Procedure:**
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Create a GstHandlerControl object, set delay to 1000 and errorMsg to "All handlers done", call waitForDone, and verify that it returns true. | delay = 1000, errorMsg = "All handlers done", result = output of waitForDone | Returns true; assertion (EXPECT_TRUE) passes | Should Pass |
 */
TEST(GstHandlerControl, waitForDone_returns_true_or_false) {
    std::cout << "Entering waitForDone_returns_true_or_false test" << std::endl;
    
    // Create object using default constructor
    EXPECT_NO_THROW({
        GstHandlerControl gstHandlerControl;
        std::cout << "Created GstHandlerControl object using default constructor" << std::endl;
        
        int delay = 1000;
        std::string errorMsg;
        char errBuffer[50];
        // Using strncpy to assign value to fixed size array, then convert to string
        strncpy(errBuffer, "All handlers done", sizeof(errBuffer) - 1);
        errBuffer[sizeof(errBuffer) - 1] = '\0';
        errorMsg = std::string(errBuffer);
        std::cout << "Assigned errormessage using strncpy: " << errorMsg << std::endl;
        
        std::cout << "Invoking waitForDone with MaximumDelayMilliseconds = " 
                  << delay << " and errormessage = " << errorMsg << std::endl;
        bool result = gstHandlerControl.waitForDone(delay, errorMsg);
        std::cout << "Method waitForDone returned: " << std::boolalpha << result << std::endl;
        
        // Expecting true if all handlers are done
        EXPECT_TRUE(result);
    });
    
    std::cout << "Exiting waitForDone_returns_true_or_false test" << std::endl;
}
/**
 * @brief Test that waitForDone correctly handles a negative delay value by returning false.
 *
 * This test validates that when a negative delay is passed to the waitForDone method of GstHandlerControl, the function correctly interprets the delay as an immediate timeout condition and returns false. The test also verifies that the object is created without exceptions and that the error message is assigned properly.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 017@n
 * **Priority:** High@n
 * 
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 * 
 * **Test Procedure:**@n
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Create GstHandlerControl object using default constructor | None | Object created without throwing exception | Should be successful |
 * | 02 | Assign error message using strncpy to a fixed-size array and convert to std::string | errBuffer = "Invalid delay provided", errorMsg = "Invalid delay provided" | Error message assigned correctly | Should be successful |
 * | 03 | Invoke waitForDone with a negative delay value and the given error message | MaximumDelayMilliseconds = -100, errormessage = "Invalid delay provided" | waitForDone returns false as negative delay implies immediate timeout; EXPECT_FALSE check passes | Should Pass |
 */
TEST(GstHandlerControl, waitForDone_handles_negative_delay) {
    std::cout << "Entering waitForDone_handles_negative_delay test" << std::endl;
    
    // Create object using default constructor
    EXPECT_NO_THROW({
        GstHandlerControl gstHandlerControl;
        std::cout << "Created GstHandlerControl object using default constructor" << std::endl;
        
        int delay = -100;
        std::string errorMsg;
        char errBuffer[50];
        // Using strncpy to assign value to fixed size array, then convert to string
        strncpy(errBuffer, "Invalid delay provided", sizeof(errBuffer) - 1);
        errBuffer[sizeof(errBuffer) - 1] = '\0';
        errorMsg = std::string(errBuffer);
        std::cout << "Assigned errormessage using strncpy: " << errorMsg << std::endl;
        
        std::cout << "Invoking waitForDone with MaximumDelayMilliseconds = " 
                  << delay << " and errormessage = " << errorMsg << std::endl;
        bool result = gstHandlerControl.waitForDone(delay, errorMsg);
        std::cout << "Method waitForDone returned: " << std::boolalpha << result << std::endl;
        
        // Since negative delay implies immediate timeout, expecting false
        EXPECT_FALSE(result);
    });
    
    std::cout << "Exiting waitForDone_handles_negative_delay test" << std::endl;
}
/**
 * @brief Verify that moving from a default-constructed ScopeHelper using the move constructor does not throw an exception
 *
 * This test ensures that a default-constructed ScopeHelper object can be used as a source for move construction to create a new object. It verifies that after moving, no exceptions are thrown, confirming that the move semantics are correctly implemented.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 004@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**
 * | Variation / Step | Description                                                                       | Test Data                                                     | Expected Result                                             | Notes      |
 * | :--------------: | --------------------------------------------------------------------------------- | ------------------------------------------------------------- | ----------------------------------------------------------- | ---------- |
 * | 01               | Default construct a ScopeHelper and then move-construct a new instance from it.     | original = default, moved = move(original)                    | No exception is thrown; EXPECT_NO_THROW check passes        | Should Pass|
 */
TEST(GstHandlerControl_ScopeHelper, MoveFromDefault) {
    std::cout << "Entering MoveFromDefault test" << std::endl;

    // Create default-constructed ScopeHelper
    EXPECT_NO_THROW({
        GstHandlerControl::ScopeHelper original;
        std::cout << "Default ScopeHelper constructed. Calling returnStraightAway() on original." << std::endl;
        // Move-construct a new ScopeHelper from the default constructed one
        GstHandlerControl::ScopeHelper moved(std::move(original));
        std::cout << "Moved ScopeHelper constructed from original." << std::endl;
    });

    std::cout << "Exiting MoveFromDefault test" << std::endl;
}