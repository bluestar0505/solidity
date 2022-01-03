#include <libsolidity/lsp/Handler.h>

namespace solidity::lsp
{

class GotoDefinition: public Handler
{
public:
	explicit GotoDefinition(LanguageServer& _server): Handler(_server) {}

	void operator()(MessageID, Json::Value const&);
};

}
