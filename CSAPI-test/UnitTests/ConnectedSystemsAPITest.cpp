#include "pch.h"
#include "CppUnitTest.h"
#include "ConnectedSystemsAPI.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace ConnectedSystemsAPI;
using namespace std::string_literals;

namespace CSAPItest {
	TEST_CLASS(ConnectedSystemsAPITest)
	{
	public:
		TEST_METHOD(Constructor_BasicAuth_CreatesCorrectAuthHeader) {
			ConSysAPI api("localhost:8181/sensorhub/api", "admin", "password");

			std::string authHeader = api.getAuthHeader();
			Assert::IsTrue(authHeader.find("Authorization: Basic ") == 0);
			Assert::IsTrue(authHeader.length() > 21);
		}

		TEST_METHOD(Constructor_BasicAuth_WithCStyleStrings_CreatesCorrectAuthHeader) {
			ConSysAPI api("localhost:8181/sensorhub/api", "admin", "password");

			std::string authHeader = api.getAuthHeader();
			Assert::IsTrue(authHeader.find("Authorization: Basic ") == 0);
		}

		TEST_METHOD(Constructor_BearerToken_CreatesCorrectAuthHeader) {
			ConSysAPI api("localhost:8181/sensorhub/api"s, "mytoken123", false);

			std::string expected = "Authorization: Bearer mytoken123"s;
			Assert::AreEqual(expected, api.getAuthHeader());
		}

		TEST_METHOD(Constructor_BearerToken_WithEmptyToken_CreatesHeader) {
			ConSysAPI api("localhost:8181/sensorhub/api"s, "", false);

			std::string expected = "Authorization: Bearer "s;
			Assert::AreEqual(expected, api.getAuthHeader());
		}

		TEST_METHOD(Base64Encoding_SimpleCredentials_EncodesCorrectly) {
			ConSysAPI api("localhost", "admin", "password");

			std::string expected = "Authorization: Basic YWRtaW46cGFzc3dvcmQ=";
			Assert::AreEqual(expected, api.getAuthHeader());
		}

		TEST_METHOD(Base64Encoding_UsernameOnly_EncodesCorrectly) {
			ConSysAPI api("localhost", "user", "");

			std::string expected = "Authorization: Basic dXNlcjo=";
			Assert::AreEqual(expected, api.getAuthHeader());
		}

		TEST_METHOD(Base64Encoding_PasswordOnly_EncodesCorrectly) {
			ConSysAPI api("localhost", "", "pass");

			std::string expected = "Authorization: Basic OnBhc3M=";
			Assert::AreEqual(expected, api.getAuthHeader());
		}

		TEST_METHOD(Base64Encoding_EmptyCredentials_EncodesCorrectly) {
			ConSysAPI api("localhost", "", "");

			std::string expected = "Authorization: Basic Og==";
			Assert::AreEqual(expected, api.getAuthHeader());
		}

		TEST_METHOD(Base64Encoding_SpecialCharacters_EncodesCorrectly) {
			ConSysAPI api("localhost", "user@domain.com", "p@ssw0rd!");

			std::string authHeader = api.getAuthHeader();
			Assert::IsTrue(authHeader.find("Authorization: Basic ") == 0);
			Assert::AreEqual("Authorization: Basic dXNlckBkb21haW4uY29tOnBAc3N3MHJkIQ=="s, authHeader);
		}

		TEST_METHOD(Base64Encoding_LongCredentials_EncodesCorrectly) {
			std::string longUsername = "verylongusername1234567890";
			std::string longPassword = "verylongpassword0987654321";
			ConSysAPI api("localhost", longUsername, longPassword);

			std::string authHeader = api.getAuthHeader();
			Assert::IsTrue(authHeader.find("Authorization: Basic ") == 0);
			Assert::IsTrue(authHeader.length() > 21);
		}

		TEST_METHOD(GetApiRoot_ReturnsCorrectValue) {
			std::string expectedRoot = "localhost:8181/sensorhub/api";
			ConSysAPI api(expectedRoot, "admin", "password");

			Assert::AreEqual(expectedRoot, api.getApiRoot());
		}

		TEST_METHOD(GetApiRoot_WithTrailingSlash_ReturnsAsProvided) {
			std::string expectedRoot = "localhost:8181/sensorhub/api/";
			ConSysAPI api(expectedRoot, "admin", "password");

			Assert::AreEqual(expectedRoot, api.getApiRoot());
		}

		TEST_METHOD(GetApiRoot_WithProtocol_ReturnsAsProvided) {
			std::string expectedRoot = "https://localhost:8181/sensorhub/api";
			ConSysAPI api(expectedRoot, "admin", "password");

			Assert::AreEqual(expectedRoot, api.getApiRoot());
		}

		TEST_METHOD(GetSystemsAPI_ReturnsValidReference) {
			ConSysAPI api("localhost", "admin", "password");

			SystemsAPI systemsAPI = api.getSystemsAPI();
			Assert::IsNotNull(&systemsAPI);
		}

		TEST_METHOD(GetDataStreamsAPI_ReturnsValidReference) {
			ConSysAPI api("localhost", "admin", "password");

			DataStreamsAPI dataStreamsAPI = api.getDataStreamsAPI();
			Assert::IsNotNull(&dataStreamsAPI);
		}

		TEST_METHOD(GetObservationsAPI_ReturnsValidReference) {
			ConSysAPI api("localhost", "admin", "password");

			ObservationsAPI observationsAPI = api.getObservationsAPI();
			Assert::IsNotNull(&observationsAPI);
		}

		TEST_METHOD(GetControlStreamsAPI_ReturnsValidReference) {
			ConSysAPI api("localhost", "admin", "password");

			ControlStreamsAPI controlStreamsAPI = api.getControlStreamsAPI();
			Assert::IsNotNull(&controlStreamsAPI);
		}

		TEST_METHOD(GetAllSubAPIs_ReturnValidReferences) {
			ConSysAPI api("localhost", "admin", "password");

			Assert::IsNotNull(&api.getSystemsAPI());
			Assert::IsNotNull(&api.getDataStreamsAPI());
			Assert::IsNotNull(&api.getObservationsAPI());
			Assert::IsNotNull(&api.getControlStreamsAPI());
		}

		TEST_METHOD(MultipleInstances_AreIndependent) {
			ConSysAPI api1("server1", "user1", "pass1");
			ConSysAPI api2("server2", "user2", "pass2");

			Assert::AreNotEqual(api1.getApiRoot(), api2.getApiRoot());
			Assert::AreNotEqual(api1.getAuthHeader(), api2.getAuthHeader());
		}

		TEST_METHOD(Constructor_BearerToken_WithLongToken_CreatesCorrectAuthHeader) {
			std::string longToken = "eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJzdWIiOiIxMjM0NTY3ODkwIiwibmFtZSI6IkpvaG4gRG9lIiwiaWF0IjoxNTE2MjM5MDIyfQ.SflKxwRJSMeKKF2QT4fwpMeJf36POk6yJV_adQssw5c";
			ConSysAPI api("localhost", longToken, false);

			std::string expected = "Authorization: Bearer " + longToken;
			Assert::AreEqual(expected, api.getAuthHeader());
		}

		TEST_METHOD(Base64Encoding_Padding_LengthMod3Is0) {
			ConSysAPI api("localhost", "aaa", "bbb");

			std::string authHeader = api.getAuthHeader();
			Assert::IsTrue(authHeader.find("Authorization: Basic ") == 0);
			std::string base64Part = authHeader.substr(21);
			Assert::IsTrue((base64Part.length() % 4) == 0);
		}

		TEST_METHOD(Base64Encoding_Padding_LengthMod3Is1) {
			ConSysAPI api("localhost", "aa", "bb");

			std::string authHeader = api.getAuthHeader();
			Assert::IsTrue(authHeader.find("Authorization: Basic ") == 0);
			std::string base64Part = authHeader.substr(21);
			Assert::IsTrue((base64Part.length() % 4) == 0);
		}

		TEST_METHOD(Base64Encoding_Padding_LengthMod3Is2) {
			ConSysAPI api("localhost", "a", "b");

			std::string authHeader = api.getAuthHeader();
			Assert::IsTrue(authHeader.find("Authorization: Basic ") == 0);
			std::string base64Part = authHeader.substr(21);
			Assert::IsTrue((base64Part.length() % 4) == 0);
		}

		TEST_METHOD(Constructor_MinimalApiRoot_Works) {
			ConSysAPI api("api", "admin", "password");

			Assert::AreEqual("api"s, api.getApiRoot());
		}

		TEST_METHOD(Constructor_ComplexUrl_PreservesFormat) {
			std::string complexUrl = "https://api.example.com:8443/v2/sensorhub/api?param=value";
			ConSysAPI api(complexUrl, "admin", "password");

			Assert::AreEqual(complexUrl, api.getApiRoot());
		}
	};
}
