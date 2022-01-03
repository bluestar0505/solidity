#pragma once

#include <liblangutil/SourceLocation.h>

#include <libsolidity/ast/ASTForward.h>

#include <libsolutil/JSON.h>

#include <optional>
#include <vector>

namespace solidity::langutil
{
class CharStreamProvider;
}

namespace solidity::lsp
{

class FileRepository;

std::optional<langutil::LineColumn> parseLineColumn(Json::Value const& _lineColumn);
Json::Value toJson(langutil::LineColumn _pos);
Json::Value toJsonRange(langutil::LineColumn const& _start, langutil::LineColumn const& _end);

Json::Value toRange(
	langutil::CharStreamProvider const& _charStreamProvider,
	langutil::SourceLocation const& _location
);

Json::Value toJson(
	langutil::CharStreamProvider const& _charStreamProvider,
	FileRepository const& _fileRepository,
	langutil::SourceLocation const& _location
);

std::vector<frontend::Declaration const*> allAnnotatedDeclarations(frontend::Identifier const* _identifier);
std::optional<langutil::SourceLocation> declarationPosition(frontend::Declaration const* _declaration);

std::optional<langutil::SourceLocation> parsePosition(
	FileRepository const& _fileRepository,
	std::string const& _sourceUnitName,
	Json::Value const& _position
);

/// @returns the source location given a source unit name and an LSP Range object,
/// or nullopt on failure.
std::optional<langutil::SourceLocation> parseRange(
	FileRepository const& _fileRepository,
	std::string const& _sourceUnitName,
	Json::Value const& _range
);

}
