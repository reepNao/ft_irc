#include "../../includes/Commands.hpp"

// QUIT komutunu işleyen fonksiyon
void Quit::quit(Client *client, vector<string> commandParts, Server *srv)
{
    //parametreler boşsa, varsayılan bir neden kullan
    string reason = commandParts.empty() ? "Leaving from Server" : commandParts.at(0);

    //eğer neden ":" ile başlıyorsa, ilk karakteri kaldır
    if (reason.at(0) == ':')
        reason.substr(1);
    client->sendMessage(RPL_QUIT(client->getPrefix(), reason));//kullanıcıya ayrıldığı bilgisini gönder

    srv->removeClientFromAllChannels(client);
    srv->clientDisconnect(client->getClientSocketFD());//kullanıcıyı tüm kanallardan kaldır ve bağlantısını kopar
}
