#pragma once

#include <iostream>

namespace sw
{
	class PrintFieldVisitor
	{
	public:
		template <typename T>
		void visit(const char* name, const T& value)
		{
			_stream << name << "=" << value << ' ';
		}

		explicit PrintFieldVisitor(std::ostream& stream) :
				_stream(stream)
		{}

	private:
		std::ostream& _stream;
	};

}
