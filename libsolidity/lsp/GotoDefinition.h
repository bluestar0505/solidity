#include <libsolidity/lsp/HandlerBase.h>

namespace solidity::lsp
{

class GotoDefinition: public HandlerBase
{
public:
	explicit GotoDefinition(LanguageServer& _server): HandlerBase(_server) {}

	void operator()(MessageID, Json::Value const&);
};

}
