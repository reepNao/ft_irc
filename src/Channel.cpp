#include "../includes/Channel.hpp"

Channel::Channel(const string& channelName, const string& channelKey, Client* channelOwner) : _channelName(channelName), _channelOwner(channelOwner),
	  _channelKey(channelKey),
	  _userLimit(1000),
	  _noExternalMessages(false),
	  _moderated(false)
{
}

Channel::~Channel()
{
}

void Channel::broadcastMessage(const string& message) const //mesaj yayınlama fonksiyonu
{
	string tmp = message;
	for (vector<Client*>::const_iterator it = _clients.begin(); it != _clients.end(); ++it)
	{
		(*it)->sendMessage(message);
	}
}

//belirli bir istemciyi hariç tutarak kanala mesajı yayınlayan fonksiyon
void Channel::broadcastMessage(const string& message, Client* exceptClient) const
{
	for (vector<Client*>::const_iterator it = _clients.begin(); it != _clients.end(); ++it)
	{
		if (*it != exceptClient)
			(*it)->sendMessage(message);
	}
}

void Channel::addClient(Client* client) //kanala client ekleyen fonksiyon
{
	_clients.push_back(client);
}

void Channel::removeUserFromChannel(Client* client) //kanaldan client çıkaran fonksiyon
{
	for (vector<Client*>::iterator it = _clients.begin(); it != _clients.end(); ++it)
	{
		if (*it == client)
		{
			_clients.erase(it);
			break;
		}
	}
	// Kanal sahibi bir operatörse ve çıkarsa, yeni bir operatör belirle.
	if (client->isOperator() == true)
	{
		client->setOperator(false);
		for (vector<Client*>::iterator it = _clients.begin(); it != _clients.end(); ++it)
		{
			if ((*it)->isOperator() == true)
				return;
		}
		if (_clients.size() > 0)//kanalda client varsa, ilk clientı yeni operatör belirle
		{
			Client* newOperator = _clients.front();
			if (newOperator->isOperator() == false)
			{
				newOperator->setOperator(true);
				setChannelOwner(newOperator);
				broadcastMessage("MODE " + _channelName + " +o " + newOperator->getNickName());
			}
		}
	}
}

bool Channel::isUserOnChannel(Client* client) const
{
	for (vector<Client*>::const_iterator it = _clients.begin(); it != _clients.end(); ++it)
	{
		if (*it == client)
			return true;
	}
	return false;//clientın kanalda olup olmadığını kontrol eder
}

// Kanalın modlarını ayarlayan fonksiyon
void Channel::setUpModeChannel(Channel* channel, Client* client, string& mode, string& modeParams)
{
	if (!channel || !client)
	{
		write(2, "Channel or Client is null, cannot set mode.", 43);
		return;
	}
	if (mode == "+k" && (client->isOperator() == true))//kanalın şifresini ayarlar
	{
		channel->setChannelKey(modeParams);
		channel->broadcastMessage("MODE " + channel->getChannelName() + " +k " + client->getNickName());
	}
	else if (mode == "+l" && (client->isOperator() == true))//kanalın kullanıcı limitini ayarlar
	{
		channel->setUserLimit(atoi(modeParams.c_str()));
		channel->broadcastMessage("MODE " + channel->getChannelName() + " +l " + client->getNickName());
	}
	else if (mode == "+o" && (client->isOperator() == true))//kanalda operatör belirler
	{
		Client* targetClient = Server::getInstance()->getClient(modeParams);
		if (!targetClient)
		{
			client->sendMessage(":" + client->getHostName() + " 401 " + client->getNickName() + " " + modeParams + " :No such nick/channel\r\n");
			return;
		}
		targetClient->setOperator(true);
		channel->broadcastMessage("MODE " + channel->getChannelName() + " +o " + targetClient->getNickName());
	}
	else if (mode == "+m" && client->isOperator() == true)//kanalı moderatör yapar
	{
		channel->setModerated(true);
		channel->broadcastMessage("MODE " + channel->getChannelName() + " +m " + client->getNickName());
	}
	else if (mode == "+n" && (client->isOperator() == true))//kanalda dış mesajları engeller
	{
		channel->setNoExternalMessages(true);
		channel->broadcastMessage("MODE " + channel->getChannelName() + " +n " + client->getNickName());
	}
	else
	{
		client->sendMessage(":" + client->getHostName() + " 501 " + client->getNickName() + " :Unknown MODE flag");
	}
}

// Kanalın düşük modlarını ayarlayan fonksiyon
void Channel::setLowModeChannel(Channel* channel, Client* client, string& mode, string& modeParams)
{
	if (!channel || !client)
	{
		write(2, "Channel or Client is null, cannot set mode.", 43);
		return;
	}
	if (mode == "-k" && (client->isOperator() == true))//kanalın şifresini kaldırır
	{
		channel->setChannelKey("");
		channel->broadcastMessage("MODE " + channel->getChannelName() + " -k " + client->getNickName());
	}
	else if (mode == "-l" && (client->isOperator() == true))//kanalın kullanıcı limitini kaldırır
	{
		channel->setUserLimit(1000);
		channel->broadcastMessage("MODE " + channel->getChannelName() + " -l " + client->getNickName());
	}
	else if (mode == "-n" && (client->isOperator() == true))//kanalda dış mesajları engellemeyi kaldırır
	{
		channel->setNoExternalMessages(false);
		channel->broadcastMessage("MODE " + channel->getChannelName() + " -n " + client->getNickName());
	}
	else if (mode == "-o" && (client->isOperator() == true))//kanalda operatörü kaldırır
	{
		Client* targetClient = Server::getInstance()->getClient(modeParams);
		if (!targetClient)
		{
			client->sendMessage(":" + client->getHostName() + " 401 " + client->getNickName() + " " + modeParams + " :No such nick/channel\r\n");
			return;
		}
		if (targetClient->getNickName() != channel->getChannelOwner()->getNickName())
		{
			targetClient->setOperator(false);
			channel->broadcastMessage("MODE " + channel->getChannelName() + " -o " + targetClient->getNickName());
		}
	}
	else if (mode == "-m" && client->isOperator() == true)//kanalı moderatör yapmayı kaldırır
	{
		channel->setModerated(false);
		channel->broadcastMessage("MODE " + channel->getChannelName() + " -m " + client->getNickName());
	}
	else
		client->sendMessage(":" + client->getHostName() + " 501 " + client->getNickName() + " :Unknown MODE flag");
}

std::vector<std::string> Channel::getChannelClientNickNames() const
{
	vector<string> nickNames;
	for (vector<Client*>::const_iterator it = _clients.begin(); it != _clients.end(); ++it)
	{
		nickNames.push_back((*it)->getNickName());
	}
	return nickNames;//kanaldaki clientların nicknamelerini tutan vektörü döndürür
}

string Channel::getExistingUsersNickList() const
{
	string nickList;
	for (std::vector<Client*>::const_iterator it = _clients.begin(); it != _clients.end(); ++it)
	{
		if (it != _clients.begin())
			nickList += " ";

		nickList += (*it)->getNickName();
	}
	return nickList;//kanaldaki clientların nicknamelerini boşlukla ayırarak döndürür
}
