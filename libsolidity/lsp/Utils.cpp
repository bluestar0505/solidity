#include <liblangutil/CharStreamProvider.h>
#include <liblangutil/Exceptions.h>
#include <libsolidity/ast/AST.h>
#include <libsolidity/lsp/FileRepository.h>
#include <libsolidity/lsp/Utils.h>

namespace solidity::lsp
{

using langutil::CharStream;
using langutil::LineColumn;
using langutil::SourceLocation;

using namespace frontend;

using std::max;
using std::make_shared;
using std::nullopt;
using std::optional;
using std::string;
using std::vector;

optional<LineColumn> parseLineColumn(Json::Value const& _lineColumn)
{
	if (_lineColumn.isObject() && _lineColumn["line"].isInt() && _lineColumn["character"].isInt())
		return LineColumn{_lineColumn["line"].asInt(), _lineColumn["character"].asInt()};
	else
		return nullopt;
}

Json::Value toJson(LineColumn _pos)
{
	Json::Value json = Json::objectValue;
	json["line"] = max(_pos.line, 0);
	json["character"] = max(_pos.column, 0);

	return json;
}

Json::Value toJsonRange(LineColumn const& _start, LineColumn const& _end)
{
	Json::Value json;
	json["start"] = toJson(_start);
	json["end"] = toJson(_end);
	return json;
}

Json::Value toRange(langutil::CharStreamProvider const& _charStreamProvider, SourceLocation const& _location)
{
	if (!_location.hasText())
		return toJsonRange({}, {});

	solAssert(_location.sourceName, "");
	langutil::CharStream const& stream = _charStreamProvider.charStream(*_location.sourceName);
	LineColumn start = stream.translatePositionToLineColumn(_location.start);
	LineColumn end = stream.translatePositionToLineColumn(_location.end);
	return toJsonRange(start, end);
}

Json::Value toJson(
	langutil::CharStreamProvider const& _charStreamProvider,
	FileRepository const& _fileRepository,
	SourceLocation const& _location
)
{
	solAssert(_location.sourceName);
	Json::Value item = Json::objectValue;
	item["uri"] = _fileRepository.sourceUnitNameToClientPath(*_location.sourceName);
	item["range"] = toRange(_charStreamProvider, _location);
	return item;
}

vector<Declaration const*> allAnnotatedDeclarations(Expression const* _expression)
{
	vector<Declaration const*> output;

	if (auto const* identifier = dynamic_cast<Identifier const*>(_expression))
	{
		output.push_back(identifier->annotation().referencedDeclaration);
		output += identifier->annotation().candidateDeclarations;
	}
	else if (auto const* memberAccess = dynamic_cast<MemberAccess const*>(_expression))
	{
		auto const location = declarationPosition(memberAccess->annotation().referencedDeclaration);
		if (location.has_value())
			output.emplace_back(location.value());
	}

	return output;
}

optional<SourceLocation> declarationPosition(Declaration const* _declaration)
{
	if (!_declaration)
		return nullopt;

	if (_declaration->nameLocation().isValid())
		return _declaration->nameLocation();

	if (_declaration->location().isValid())
		return _declaration->location();

	return nullopt;
}

optional<SourceLocation> parsePosition(
	FileRepository const& _fileRepository,
	string const& _sourceUnitName,
	Json::Value const& _position
)
{
	if (!_fileRepository.sourceUnits().count(_sourceUnitName))
		return nullopt;

	if (optional<LineColumn> lineColumn = parseLineColumn(_position))
		if (optional<int> const offset = CharStream::translateLineColumnToPosition(
			_fileRepository.sourceUnits().at(_sourceUnitName),
			*lineColumn
		))
			return SourceLocation{*offset, *offset, make_shared<string>(_sourceUnitName)};
	return nullopt;
}

optional<SourceLocation> parseRange(
	FileRepository const& _fileRepository,
	string const& _sourceUnitName,
	Json::Value const& _range
)
{
	if (!_range.isObject())
		return nullopt;
	optional<SourceLocation> start = parsePosition(_fileRepository, _sourceUnitName, _range["start"]);
	optional<SourceLocation> end = parsePosition(_fileRepository, _sourceUnitName, _range["end"]);
	if (!start || !end)
		return nullopt;
	solAssert(*start->sourceName == *end->sourceName);
	start->end = end->end;
	return start;
}
}
