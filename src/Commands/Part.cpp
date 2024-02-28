#include "../../includes/Commands.hpp"

void Part::part(Client* client, vector<string> commandParts, Server* srv)
{
    // Eğer komut parametreleri yetersizse
    if (commandParts.size() < 2)
    {
        client->sendMessage( ":" + client->getHostName() + " 461 " + client->getNickName() + " PART : Not enough parameters" );
        return;
    }
    // Hedef kanal adını al
    string target = commandParts.at(1);
    // Eğer hedefte virgül yoksa
    if (target.find(',') == string::npos)
        partChannel(client, target, srv);
    else
    {
        istringstream iss(target);//kanal adlarını virgülle ayır
        string channelName;//kanal adlarını tutacak değişken
        while (getline(iss, channelName, ','))//ayrılmış her kanal adı için partChannel fonksiyonunu çağır
            partChannel(client, channelName, srv);
    }
}

void Part::partChannel(Client* client, string channelName, Server* srv)
{
    //eğer kanal adı yoksa veya kanal adı # ile başlamıyorsa
    if (channelName.empty() || channelName.at(0) != '#')
    {
      	client->sendMessage( ":" + client->getHostName() + " 403 " + client->getNickName() + " " + channelName + " :No such channel" );
        return;
    }
    Channel* channel = srv->getChannel(channelName);//kanalı, adıyla birlikte al
    if (!channel)
    {
     	client->sendMessage( ":" + client->getHostName() + " 403 " + client->getNickName() + " " + channelName + " :No such channel" );
        return;//kanal yoksa hata mesajı gönder
    }
    if (!channel->isUserOnChannel(client))
    {
        client->sendMessage( ":" + client->getHostName() + " 442 " + client->getNickName() + " " + channelName + " :You're not on that channel" );
        return;//kullanıcı kanalda değilse hata mesajı gönder
    }
    channel->removeUserFromChannel(client);//kanaldan kullanıcıyı çıkar
    client->removeChannel(channel);//kanalı kanallar listesinden çıkar

    string message = ":" + client->getPrefix() + " PART " + channelName + "\n";
    channel->broadcastMessage(message, client);//kanalda kalan kullanıcılara ayrıldığı kanalı bildir

   	client->sendMessage( "You left the channel " + channelName );//kullanıcıya ayrıldığı kanalı bildir
    if (channel->getChannelClientCount() == 0 && srv->channelExists(channelName))//kanalda kullanıcı kalmadıysa
    {
   		string channelName = channel->getChannelName();
		string message = "Channel " + channelName + " is empty, deleting.\n";
		write( 1, message.c_str(), message.length() );
		srv->removeChannel(channelName);//kanalın silindiğini bildir ve kanalı sil
    }
}
