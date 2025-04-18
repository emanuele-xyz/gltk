#pragma once

#include <string>

namespace gltk
{
	class Error
	{
	public:
		Error(const std::string& file, int line, const std::string message);
	public:
		const std::string& What() const noexcept { return m_what; }
	private:
		std::string m_what;
	};
}
