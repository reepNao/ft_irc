#include "../../includes/Commands.hpp"

// PASS komutunu işleyen fonksiyon
void Pass::pass(Client *client, const vector<string> commandParts, Server *srv)
{
    if (client->isRegistered() || client->getUserAuth() || client->getIsPass())
    {
        client->sendReply(ERR_ALREADYREGISTERED(client->getNickName()));
        return;//kullanıcı zaten kayıtlıysa hata mesajı gönder
    }
    if (commandParts.size() < 2)
    {
        client->sendReply(ERR_NEEDMOREPARAMS(client->getNickName(), "PASS"));
        return;//gerekli parametreler eksikse hata mesajı gönder
    }
    string passw = commandParts.at(1);
    if (!srv->verifySrvPass(passw))//şifre doğrulanmazsa hata mesajı döndür
    {
        client->sendMessage("Access denied!");
        return;
    }
    client->setPass(true);//şifre doğrulandıysa kullanıcıya şifre doğrulandı bilgisi gönder
    client->sendMessage("Password accepted!");//kullanıcıya şifre doğrulandı bilgisi gönder
}
