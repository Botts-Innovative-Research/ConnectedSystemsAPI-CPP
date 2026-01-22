#pragma once

#include <string>
#include <vector>
#include <exception>

namespace ConnectedSystemsAPI {
	class ValidationException : public std::exception {
	public:
		struct ValidationError {
			std::string field;
			std::string message;

			ValidationError(const std::string& fieldName, const std::string& errorMessage)
				: field(fieldName), message(errorMessage) {
			}
		};

	private:
		std::string context;
		std::string message;
		std::vector<ValidationError> errors;

	public:
		explicit ValidationException(const std::string& contextName, const std::vector<ValidationError>& validationErrors)
			: context(contextName), errors(validationErrors) {
			message = context + " validation failed with " + std::to_string(errors.size()) + " error(s):";
			for (const auto& error : errors) {
				message += "\n  - " + error.field + ": " + error.message;
			}
		}

		const char* what() const noexcept override {
			return message.c_str();
		}

		const std::vector<ValidationError>& getErrors() const noexcept {
			return errors;
		}

		const std::string& getContext() const noexcept {
			return context;
		}
	};
}
