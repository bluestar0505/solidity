#include <libsolidity/lsp/LanguageServer.h>

namespace solidity::lsp
{

class GotoDefinition
{
public:
	explicit GotoDefinition(LanguageServer& _server);

	void operator()(MessageID, Json::Value const&);

private:
	LanguageServer& m_server;
	langutil::CharStreamProvider const& m_charStreamProvider;
	FileRepository const& m_fileRepository;
	Transport& m_client;
};

}
