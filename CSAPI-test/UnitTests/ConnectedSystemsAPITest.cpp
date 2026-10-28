#include "pch.h"
#include "CppUnitTest.h"
#include "ConnectedSystemsAPI.h"

#include <thread>
#include <httplib.h>

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
			Assert::IsTrue(authHeader.find("Basic ") == 0);
			Assert::IsTrue(authHeader.length() > 6);
		}

		TEST_METHOD(Constructor_BasicAuth_WithCStyleStrings_CreatesCorrectAuthHeader) {
			ConSysAPI api("localhost:8181/sensorhub/api", "admin", "password");

			std::string authHeader = api.getAuthHeader();
			Assert::IsTrue(authHeader.find("Basic ") == 0);
		}

		TEST_METHOD(Constructor_BearerToken_CreatesCorrectAuthHeader) {
			ConSysAPI api("localhost:8181/sensorhub/api"s, "mytoken123", false);

			std::string expected = "Bearer mytoken123"s;
			Assert::AreEqual(expected, api.getAuthHeader());
		}

		TEST_METHOD(Constructor_BearerToken_WithEmptyToken_CreatesHeader) {
			ConSysAPI api("localhost:8181/sensorhub/api"s, "", false);

			std::string expected = "Bearer "s;
			Assert::AreEqual(expected, api.getAuthHeader());
		}

		TEST_METHOD(Base64Encoding_SimpleCredentials_EncodesCorrectly) {
			ConSysAPI api("localhost", "admin", "password");

			std::string expected = "Basic YWRtaW46cGFzc3dvcmQ=";
			Assert::AreEqual(expected, api.getAuthHeader());
		}

		TEST_METHOD(Base64Encoding_UsernameOnly_EncodesCorrectly) {
			ConSysAPI api("localhost", "user", "");

			std::string expected = "Basic dXNlcjo=";
			Assert::AreEqual(expected, api.getAuthHeader());
		}

		TEST_METHOD(Base64Encoding_PasswordOnly_EncodesCorrectly) {
			ConSysAPI api("localhost", "", "pass");

			std::string expected = "Basic OnBhc3M=";
			Assert::AreEqual(expected, api.getAuthHeader());
		}

		TEST_METHOD(Base64Encoding_EmptyCredentials_EncodesCorrectly) {
			ConSysAPI api("localhost", "", "");

			std::string expected = "Basic Og==";
			Assert::AreEqual(expected, api.getAuthHeader());
		}

		TEST_METHOD(Base64Encoding_SpecialCharacters_EncodesCorrectly) {
			ConSysAPI api("localhost", "user@domain.com", "p@ssw0rd!");

			std::string authHeader = api.getAuthHeader();
			Assert::IsTrue(authHeader.find("Basic ") == 0);
			Assert::AreEqual("Basic dXNlckBkb21haW4uY29tOnBAc3N3MHJkIQ=="s, authHeader);
		}

		TEST_METHOD(Base64Encoding_LongCredentials_EncodesCorrectly) {
			std::string longUsername = "verylongusername1234567890";
			std::string longPassword = "verylongpassword0987654321";
			ConSysAPI api("localhost", longUsername, longPassword);

			std::string authHeader = api.getAuthHeader();
			Assert::IsTrue(authHeader.find("Basic ") == 0);
			Assert::IsTrue(authHeader.length() > 6);
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

			std::string expected = "Bearer " + longToken;
			Assert::AreEqual(expected, api.getAuthHeader());
		}

		TEST_METHOD(Base64Encoding_Padding_LengthMod3Is0) {
			ConSysAPI api("localhost", "aaa", "bbb");

			std::string authHeader = api.getAuthHeader();
			Assert::IsTrue(authHeader.find("Basic ") == 0);
			std::string base64Part = authHeader.substr(6);
			Assert::IsTrue((base64Part.length() % 4) == 0);
		}

		TEST_METHOD(Base64Encoding_Padding_LengthMod3Is1) {
			ConSysAPI api("localhost", "aa", "bb");

			std::string authHeader = api.getAuthHeader();
			Assert::IsTrue(authHeader.find("Basic ") == 0);
			std::string base64Part = authHeader.substr(6);
			Assert::IsTrue((base64Part.length() % 4) == 0);
		}

		TEST_METHOD(Base64Encoding_Padding_LengthMod3Is2) {
			ConSysAPI api("localhost", "a", "b");

			std::string authHeader = api.getAuthHeader();
			Assert::IsTrue(authHeader.find("Basic ") == 0);
			std::string base64Part = authHeader.substr(6);
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

		TEST_METHOD(Request_BasicAuth_SendsAuthorizationHeaderValue) {
			Assert::AreEqual("Basic YWRtaW46cGFzc3dvcmQ="s, captureAuthorizationHeader([](const std::string& root) {
				return ConSysAPI(root, "admin", "password");
			}));
		}

		TEST_METHOD(Request_BearerToken_SendsAuthorizationHeaderValue) {
			Assert::AreEqual("Bearer mytoken123"s, captureAuthorizationHeader([](const std::string& root) {
				return ConSysAPI(root, "mytoken123"s, false);
			}));
		}

		TEST_METHOD(Request_NoScheme_UsesHttp) {
			auto response = requestSystemsFromLocalServer([](int port) { return "127.0.0.1:" + std::to_string(port) + "/api"; });
			Assert::AreEqual(200, response.getResponseCode());
		}

		TEST_METHOD(Request_UppercaseScheme_IsAccepted) {
			auto response = requestSystemsFromLocalServer([](int port) { return "HTTP://127.0.0.1:" + std::to_string(port) + "/api"; });
			Assert::AreEqual(200, response.getResponseCode());
		}

		TEST_METHOD(Request_Https_WithoutSslSupport_FailsWithoutThrowing) {
#if defined(CPPHTTPLIB_SSL_ENABLED) || defined(CPPHTTPLIB_OPENSSL_SUPPORT)
			Logger::WriteMessage("Skipped: cpp-httplib was built with SSL support");
#else
			ConSysAPI api("https://127.0.0.1:1/api", "admin", "password");
			auto response = api.getSystemsAPI().getSystems();

			Assert::AreEqual(0, response.getResponseCode());
			Assert::IsTrue(response.getResponseMessage().find("HTTPS is not supported") == 0);
#endif
		}

		TEST_METHOD(Request_UnsupportedScheme_FailsWithoutThrowing) {
			ConSysAPI api("ftp://127.0.0.1:1/api", "admin", "password");
			auto response = api.getSystemsAPI().getSystems();

			Assert::AreEqual(0, response.getResponseCode());
			Assert::AreEqual("Unsupported URL scheme: ftp"s, response.getResponseMessage());
		}

		TEST_METHOD(GetSubsystems_WithQuery_ReturnsResponse) {
			httplib::Server server;
			std::string receivedQ;
			server.Get("/api/systems/sys1/subsystems", [&](const httplib::Request& req, httplib::Response& res) {
				receivedQ = req.get_param_value("q");
				res.set_content(R"({"items":[{"type":"Feature","id":"sub1","properties":{"featureType":"http://www.w3.org/ns/sosa/Sensor","uid":"urn:test:sub1","name":"Sub"}}]})", "application/json");
			});

			int port = server.bind_to_any_port("127.0.0.1");
			std::thread serverThread([&] { server.listen_after_bind(); });
			server.wait_until_ready();

			ConSysAPI api("http://127.0.0.1:" + std::to_string(port) + "/api", "admin", "password");
			auto response = api.getSystemsAPI().getSubsystems("sys1", Query::SystemsQuery().setQ("gps"));

			server.stop();
			serverThread.join();

			Assert::AreEqual(200, response.getResponseCode());
			Assert::AreEqual("gps"s, receivedQ);
			Assert::AreEqual(size_t(1), response.getItems().size());
			Assert::AreEqual("sub1"s, response.getItems()[0].getId());
		}

	private:
		/// <summary>
		/// Requests /systems from a local server, using an API root built from the server's port.
		/// </summary>
		template<typename MakeRoot>
		static APIResponse<DataModels::System> requestSystemsFromLocalServer(MakeRoot makeRoot) {
			httplib::Server server;
			server.Get("/api/systems", [](const httplib::Request&, httplib::Response& res) {
				res.set_content(R"({"items":[]})", "application/json");
			});

			int port = server.bind_to_any_port("127.0.0.1");
			std::thread serverThread([&] { server.listen_after_bind(); });
			server.wait_until_ready();

			ConSysAPI api(makeRoot(port), "admin", "password");
			auto response = api.getSystemsAPI().getSystems();

			server.stop();
			serverThread.join();
			return response;
		}

		/// <summary>
		/// Sends a request through a ConSysAPI to a local server and returns the Authorization header it received.
		/// </summary>
		template<typename MakeApi>
		static std::string captureAuthorizationHeader(MakeApi makeApi) {
			httplib::Server server;
			std::string received;
			server.Get("/api/systems", [&](const httplib::Request& req, httplib::Response& res) {
				received = req.get_header_value("Authorization");
				res.set_content(R"({"items":[]})", "application/json");
			});

			int port = server.bind_to_any_port("127.0.0.1");
			std::thread serverThread([&] { server.listen_after_bind(); });
			server.wait_until_ready();

			auto api = makeApi("http://127.0.0.1:" + std::to_string(port) + "/api");
			auto response = api.getSystemsAPI().getSystems();

			server.stop();
			serverThread.join();

			Assert::AreEqual(200, response.getResponseCode());
			return received;
		}
	};
}
