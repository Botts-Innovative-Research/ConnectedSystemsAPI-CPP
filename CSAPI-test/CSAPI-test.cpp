#include <iostream>
#include <string>
#include "pch.h"
#include "CppUnitTest.h"
#include "System.h"
#include "ConnectedSystemsAPI.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace std::string_literals;

namespace CSAPItest
{
	TEST_CLASS(CSAPItest)
	{
	public:
		TEST_METHOD(Constructor)
		{
			ConnectedSystemsAPI::ConSysAPI csapi("localhost:8181/sensorhub/api"s, "admin"s, "admin"s);
		}

		TEST_METHOD(GetSystems)
		{
			ConnectedSystemsAPI::ConSysAPI csapi("localhost:8181/sensorhub/api"s, "admin"s, "admin"s);
			auto response = csapi.getSystemsAPI().getSystems();
			std::cout << "Response: " << response.getResponseBody() << std::endl;
			auto items = response.getItems();
			for (const auto& item : items) {
				std::cout << "System: " << item << std::endl;
			}
		}
	};
}
