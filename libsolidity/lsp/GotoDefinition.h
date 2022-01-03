#include <libsolidity/lsp/LanguageServer.h>

namespace solidity::lsp
{

class GotoDefinition: public Handler
{
public:
	explicit GotoDefinition(LanguageServer& _server): Handler(_server) {}

	void operator()(MessageID, Json::Value const&);
};

}
