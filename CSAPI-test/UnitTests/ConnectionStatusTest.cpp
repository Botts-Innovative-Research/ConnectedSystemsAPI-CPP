#include "pch.h"
#include "CppUnitTest.h"

#include <chrono>
#include <functional>
#include <string>
#include <thread>
#include <httplib.h>

#include "ConnectedSystemsAPI.h"
#include "ConnectionStatus.h"
#include "DataModels/Conformance.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace ConnectedSystemsAPI;
using namespace std::string_literals;

namespace CSAPItest {
	TEST_CLASS(ConnectionStatusTest)
	{
		// Conformance declaration of an OpenSensorHub node, abbreviated
		static constexpr const char* CS_CONFORMANCE = R"({"conformsTo":[
			"http://www.opengis.net/spec/ogcapi-common-1/1.0/conf/core",
			"http://www.opengis.net/spec/ogcapi-connectedsystems-1/1.0/conf/core",
			"http://www.opengis.net/spec/ogcapi-connectedsystems-1/1.0/conf/system"]})";
		// An OGC API server that is not a Connected Systems API server
		static constexpr const char* FEATURES_CONFORMANCE = R"({"conformsTo":[
			"http://www.opengis.net/spec/ogcapi-common-1/1.0/conf/core",
			"http://www.opengis.net/spec/ogcapi-features-1/1.0/conf/core"]})";
		static constexpr const char* NO_SYSTEMS = R"({"items":[]})";

		/// <summary>
		/// A local HTTP server for the duration of a test. Routes are added before start().
		/// </summary>
		struct LocalServer {
			httplib::Server server;
			std::thread thread;
			int port = 0;

			void start() {
				port = server.bind_to_any_port("127.0.0.1");
				thread = std::thread([this] { server.listen_after_bind(); });
				server.wait_until_ready();
			}

			std::string apiRoot() const { return "http://127.0.0.1:" + std::to_string(port) + "/api"; }

			~LocalServer() {
				server.stop();
				if (thread.joinable()) thread.join();
			}
		};

		static void respond(httplib::Response& res, int status, const std::string& body = "", const char* type = "application/json") {
			res.status = status;
			if (!body.empty()) res.set_content(body, type);
		}

		/// <summary>
		/// A node that serves the given conformance and systems responses.
		/// </summary>
		static ConnectionStatus checkNode(int conformanceStatus, const std::string& conformanceBody,
			int systemsStatus = 200, const std::string& systemsBody = NO_SYSTEMS) {
			LocalServer local;
			local.server.Get("/api/conformance", [&](const httplib::Request&, httplib::Response& res) { respond(res, conformanceStatus, conformanceBody); });
			local.server.Get("/api/systems", [&](const httplib::Request&, httplib::Response& res) { respond(res, systemsStatus, systemsBody); });
			local.start();
			return ConSysAPI(local.apiRoot(), "admin", "password").checkConnection();
		}

	public:
		TEST_METHOD(Conformance_ParsesAndDetectsConnectedSystems) {
			auto conformance = nlohmann::json::parse(CS_CONFORMANCE).get<DataModels::Conformance>();
			Assert::AreEqual(size_t(3), conformance.getConformsTo().size());
			Assert::IsTrue(conformance.isConnectedSystemsAPI());
			Assert::IsTrue(conformance.conformsToClass("http://www.opengis.net/spec/ogcapi-connectedsystems-1/1.0/conf/system"));

			auto features = nlohmann::json::parse(FEATURES_CONFORMANCE).get<DataModels::Conformance>();
			Assert::IsFalse(features.isConnectedSystemsAPI());
		}

		TEST_METHOD(Connected_NodeWithNoSystems_IsConnected) {
			auto status = checkNode(200, CS_CONFORMANCE);

			Assert::IsTrue(status.isConnected());
			Assert::IsTrue(status.getState() == ConnectionState::CONNECTED);
			Assert::AreEqual(200, status.getResponseCode());
		}

		TEST_METHOD(Connected_SendsCredentials) {
			LocalServer local;
			std::string conformanceAuth, systemsAuth, systemsLimit;
			local.server.Get("/api/conformance", [&](const httplib::Request& req, httplib::Response& res) {
				conformanceAuth = req.get_header_value("Authorization");
				respond(res, 200, CS_CONFORMANCE);
			});
			local.server.Get("/api/systems", [&](const httplib::Request& req, httplib::Response& res) {
				systemsAuth = req.get_header_value("Authorization");
				systemsLimit = req.get_param_value("limit");
				respond(res, 200, NO_SYSTEMS);
			});
			local.start();

			Assert::IsTrue(ConSysAPI(local.apiRoot(), "admin", "password").checkConnection().isConnected());
			Assert::AreEqual("Basic YWRtaW46cGFzc3dvcmQ="s, conformanceAuth);
			Assert::AreEqual("Basic YWRtaW46cGFzc3dvcmQ="s, systemsAuth);
			Assert::AreEqual("1"s, systemsLimit);
		}

		TEST_METHOD(Unreachable_NothingListening) {
			auto status = ConSysAPI("http://127.0.0.1:1/api", "admin", "password").checkConnection();

			Assert::IsTrue(status.getState() == ConnectionState::UNREACHABLE);
			Assert::AreEqual(0, status.getResponseCode());
			Assert::IsTrue(status.getMessage().find("Could not reach http://127.0.0.1:1/api") == 0);
		}

		TEST_METHOD(Unreachable_NoAnswer_RespectsTimeout) {
			LocalServer local;
			local.server.Get("/api/conformance", [](const httplib::Request&, httplib::Response& res) {
				std::this_thread::sleep_for(std::chrono::seconds(2));
				respond(res, 200, CS_CONFORMANCE);
			});
			local.start();

			auto start = std::chrono::steady_clock::now();
			auto status = ConSysAPI(local.apiRoot(), "admin", "password").checkConnection(std::chrono::milliseconds(300));
			auto elapsed = std::chrono::steady_clock::now() - start;

			Assert::IsTrue(status.getState() == ConnectionState::UNREACHABLE);
			Assert::IsTrue(elapsed < std::chrono::milliseconds(1500), L"checkConnection did not respect its timeout");
		}

		TEST_METHOD(Unauthorized_OnConformance) {
			auto status = checkNode(401, "");

			Assert::IsTrue(status.getState() == ConnectionState::UNAUTHORIZED);
			Assert::AreEqual(401, status.getResponseCode());
		}

		TEST_METHOD(Unauthorized_OnlyOnSystems) {
			// Conformance is readable without credentials, but the resources are not
			auto status = checkNode(200, CS_CONFORMANCE, 403, "");

			Assert::IsTrue(status.getState() == ConnectionState::UNAUTHORIZED);
			Assert::AreEqual(403, status.getResponseCode());
		}

		TEST_METHOD(NotConnectedSystems_OtherOgcApi) {
			auto status = checkNode(200, FEATURES_CONFORMANCE);

			Assert::IsTrue(status.getState() == ConnectionState::NOT_CONNECTED_SYSTEMS_API);
			Assert::IsTrue(status.getMessage().find("does not implement the Connected Systems API") != std::string::npos);
		}

		TEST_METHOD(NotConnectedSystems_WrongPath) {
			LocalServer local;
			local.start();

			auto status = ConSysAPI(local.apiRoot(), "admin", "password").checkConnection();
			Assert::IsTrue(status.getState() == ConnectionState::NOT_CONNECTED_SYSTEMS_API);
			Assert::AreEqual(404, status.getResponseCode());
		}

		TEST_METHOD(NotConnectedSystems_RedirectWithEmptyBody) {
			// How an OpenSensorHub node answers an unknown path: a redirect to its web UI
			auto status = checkNode(302, "");

			Assert::IsTrue(status.getState() == ConnectionState::NOT_CONNECTED_SYSTEMS_API);
			Assert::AreEqual(302, status.getResponseCode());
		}

		TEST_METHOD(NotConnectedSystems_HtmlPage) {
			LocalServer local;
			local.server.Get("/api/conformance", [](const httplib::Request&, httplib::Response& res) {
				respond(res, 200, "<html><body>Welcome</body></html>", "text/html");
			});
			local.start();

			auto status = ConSysAPI(local.apiRoot(), "admin", "password").checkConnection();
			Assert::IsTrue(status.getState() == ConnectionState::NOT_CONNECTED_SYSTEMS_API);
		}

		TEST_METHOD(ServerError) {
			auto status = checkNode(500, "");

			Assert::IsTrue(status.getState() == ConnectionState::SERVER_ERROR);
			Assert::AreEqual(500, status.getResponseCode());
		}

		TEST_METHOD(StateNames) {
			Assert::AreEqual("CONNECTED"s, std::string(toString(ConnectionState::CONNECTED)));
			Assert::AreEqual("NOT_CONNECTED_SYSTEMS_API"s, std::string(toString(ConnectionState::NOT_CONNECTED_SYSTEMS_API)));
		}
	};
}
