#include "../includes/Server.hpp"

// Kanalı kaldırma fonksiyonu
void Server::removeChannel(const std::string& channelName)
{
	//mapte belirtilen adı ile kanalı bul.
	std::map<std::string, Channel*>::iterator it = _channels.find(channelName);
	if (it != _channels.end())
	{
		Channel* channel = it->second;
		if (channel != NULL)
		{
			// Kanalı sil.
			delete channel;
			_channels.erase(it);
		}
		else
		{
			string message = "Channel " + channelName + " is already null.";
			write(STDOUT_FILENO, message.c_str(), message.size());
		}
	}
	else
	{
		string message = "Channel " + channelName + " does not exist.";
		write(STDOUT_FILENO, message.c_str(), message.size());
	}
}

void Server::removeClientFromAllChannels(Client* client)
{
	//client ve soketin açık olup olmadığını kontrol et.
	if (client == NULL || !client->isSocketOpen())
		return;

	// İstemcinin bulunduğu tüm kanalların bir kopyasını oluşturun ve istemciyi kanallardan kaldırın.
	std::vector<Channel*> channels = client->getChannel();
	while (!channels.empty())
	{
		Channel* channel = channels.back();
		channels.pop_back();
		if (channel != NULL)
		{
			string channelName = channel->getChannelName();
			string clientNick = client->getNickName();
			channel->removeUserFromChannel(client);//kanaldan clientı çıkar
			client->removeChannel(channel);//clientın kanalını kaldır
			// Log mesajı oluştur.
			string leaveMessage = clientNick + " has left the channel " + channelName;
			log(leaveMessage);
			if (channel->getChannelClientCount() == 0 && channelExists(channel->getChannelName()))//kanal boşsa, kanalı sil
			{
				string message = "Channel " + channelName + " is empty, deleting.\n";
				write(STDOUT_FILENO, message.c_str(), message.size());
				removeChannel(channelName);
			}
		}
	}
}

void Server::addChannel(Channel* channel)
{
	_channels.insert(std::make_pair(channel->getChannelName(), channel));//kanalı ekler
}

bool Server::channelExists(const string& channelName)
{
	// Kanallar haritasında belirtilen ad ile kanalı bul.
	for (map<string, Channel*>::iterator it = _channels.begin(); it != _channels.end(); ++it)
	{
		if (it->second->getChannelName() == channelName)
			return true;
	}
	return false;//iteratör ile kanalı var mı diye kontrol eder
}

Client* Server::getClient(string& nickName)
{
	for (map<int, Client*>::iterator it = _clients.begin(); it != _clients.end(); ++it)
	{
		if (it->second->getNickName() == nickName)
			return it->second;
	}
	return NULL;//clientı nicknamine bakarak bulur
}

Channel* Server::getChannel(string& channelName)
{
	for (map<string, Channel*>::iterator it = _channels.begin(); it != _channels.end(); ++it)
	{
		if (it->second->getChannelName() == channelName)
			return it->second;
	}
	return NULL;//kanalı adına bakarak bulur
}
