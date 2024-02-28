#include "../../includes/Commands.hpp"

// PRIVMSG komutunu işleyen fonksiyon
void PrivMsg::privMsg(Client *client, vector<string> commandParts, Server *srv)
{
    if (commandParts.size() < 2)
    {
        client->sendReply(ERR_NEEDMOREPARAMS(client->getNickName(), "PRIVMSG"));
        return;
    }
    string commandString = mergeString(commandParts, " ");//konsoldan alınan komutları birleştir

    //hedefin başlangıç konumunu ve mesajın başlangıç konumunu bul
    size_t targetStart = commandString.find("PRIVMSG") + 8;
    size_t messageStart = commandString.find(" :", targetStart);
    if (messageStart == string::npos)//eğer mesaj yoksa hata döndür
    {
        client->sendReply(ERR_NOTEXTTOSEND(client->getNickName()));
        return;
    }
    string target = commandString.substr(targetStart, messageStart - targetStart);//hedef mesajı ayır
    target = strim(target);//hedef mesajı temizle
    string message = commandString.substr(messageStart + 2);//mesajı al

    if (target.at(0) == '#')//mesajı gönderilecek yer kanalsa, kanal mesajı gönder
    {
        sendChannelMessage(client, target, message, srv);
    }
    else
    {
        sendPrivateMessage(client, target, message, srv);//hedef bir kullanıcıysa mesajı özel gönder
    }
}

void sendChannelMessage(Client *client, string channelName, string message, Server *srv)
{
    Channel *channel = srv->getChannel(channelName);
    if (channel == NULL)
    {
        client->sendReply(ERR_NOSUCHCHANNEL(client->getNickName(), channelName));
        return;
    }
    if (channel->getModerated() && !client->isOperator())//eğer kanal moderasyon altındaysa ve kullanıcı op değilse hata mesajı gönder
    {
        client->sendReply(ERR_NOCANNOTSENDTOCHAN(client->getNickName(), channelName));
        return;
    }
    if (channel->getNoExternalMessages() && !channel->isUserOnChannel(client))//eğer kullanıcı kanalda değilse ve kanal dışından mesaj alınmıyorsa hata mesajı gönder
    {
        client->sendReply(ERR_NOCANNOTSENDTOCHAN(client->getNickName(), channelName));
        return;
    }
    channel->broadcastMessage(":" + client->getPrefix() + " PRIVMSG " + channelName + " :" + message, client);
}

// Özel mesajı gönderen yardımcı fonksiyon
void sendPrivateMessage(Client *client, string target, string message, Server *srv)
{
    Client *targetClient = srv->getClient(target);
    if (targetClient == NULL)
    {
        client->sendReply(ERR_NOSUCHNICK(client->getNickName(), target));//hedef kullanıcı yoksa hata mesajı gönder
        return;
    }
    targetClient->sendMessage(":" + client->getPrefix() + " PRIVMSG " + target + " :" + message);
}
