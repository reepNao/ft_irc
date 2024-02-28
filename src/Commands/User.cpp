#include "../../includes/Commands.hpp"

// USER komutunu işleyen fonksiyon
void User::user(Client *client, vector<string> commandParts, Server *srv)
{
    if (!client || !srv)
    {
        write(2, "Error: client or srv is null\n", 29);
        return;//client veya server boş ise hata mesajı yazdır ve çık
    }
    if (client->isRegistered())
    {
        client->sendReply(ERR_ALREADYREGISTERED(client->getNickName()));
        return;//eğer kullanıcı zaten kayıtlı ise hata mesajı gönder ve çık
    }
    if (commandParts.size() < 3)
    {
        client->sendReply(ERR_NEEDMOREPARAMS(client->getNickName(), "USER"));
        return;
    }
    if (!client->getValidName(commandParts.at(1)) || !client->getValidName(commandParts.at(2)))
    {
        client->sendReply(ERR_ERRONEUSNICKNAME(client->getNickName(), commandParts.at(1)));
        return;//eğer kullanıcı adı veya ad geçerli değilse hata mesajı gönder ve çık
    }
    std::string userName = commandParts.at(1);
    std::string realName = commandParts.at(2);

    const std::map<int, Client *> &clients = srv->getAllClients();//kullanıcı adı kullanılıyorsa hata mesajı gönder ve çık
    for (std::map<int, Client *>::const_iterator it = clients.begin(); it != clients.end(); ++it)
    {
        const Client *regUser = it->second;
        if (!regUser)
        {
            continue;
        }
        if (regUser != client && regUser->getUserName() == userName)
        {
            client->sendMessage("This username is already in use");
            return;
        }
    }

    //kullanıcı bilgilerini ayarla ve kullanıcıyı bağlı olarak işaretle
    client->setUserName(userName);
    client->setRealName(realName);
    client->setStatus(CLIENT_CONNECTED);
    client->setUserAuth(true);

    //kullanıcı doğrulandıysa
    if (client->getUserAuth() == true)
    {
        //sunucudan bot al ve bot varsa hoş geldin mesajını gönder
        Bot *bot = srv->getBot();
        if (bot)
            bot->WelcomeMsg(client->getNickName());
    }
}
