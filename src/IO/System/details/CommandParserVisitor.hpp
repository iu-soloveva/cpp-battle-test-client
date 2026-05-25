#pragma once

#include <iostream>

namespace sw
{
	class CommandParserVisitor
	{
	public:
		template <class TField>
		void visit(const char*, TField& field)
		{
			_stream >> field;
		}

		CommandParserVisitor(std::istream& stream) :
				_stream(stream)
		{}

	private:
		std::istream& _stream;
	};
}
