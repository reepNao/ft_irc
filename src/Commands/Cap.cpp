#include "../../includes/Commands.hpp"

/*
    desteklenen yeteneklere dair bilgi sağlar. "CAP LS" komutu ile desteklenen yeteneklerin listesini döner.
    bu yetenekler /PASS, /USER, /NICK, /JOIN, /PART, /PRIVMSG, /NOTICE, /KICK, /MODE, /QUIT komutlarını içerir.
    client Komutu gerçekleştiren istemciye işaretçi.
    commandParts Komut ve parametrelerini içeren vektör.
*/
void Cap::cap(Client *client, const vector<string> commandParts)
{
    if (commandParts.size() < 2)
    {
        // CAP komutu için yeterli parametre yok.
        client->sendReply(ERR_NEEDMOREPARAMS(client->getNickName(), "CAP"));
        return;
    }

    string cap = commandParts.at(1);
    if (cap == "LS" || cap == "ls" )
    {
        // Desteklenen yetenekleri listele.
        client->sendMessage("/PASS\n/USER\n/NICK\n/JOIN\n/PART\n/PRIVMSG\n/NOTICE\n/KICK\n/MODE\n/QUIT\n");
    }
    else
    {
        // Geçersiz yetenek komutu.
        client->sendReply(ERR_INVALIDCAPCMD(client->getNickName()));
    }
}
