#include "../includes/Bot.hpp"

// Bot sınıfının yapıcı fonksiyonu
Bot::Bot(const string &serv, int port, const string &pass)
	: serv(serv),
	  port(port),
	  botPass(pass),
	  nick("reepNaoBot"),
	  user("IRC00"),
	  realname("recep")
{
	connectServ();
}

// Bot sınıfının yıkıcı fonksiyonu
Bot::~Bot()
{
	close(sock);
}

// Sunucuya bağlanan fonksiyon
void Bot::connectServ()
{
	struct sockaddr_in server_addr;
	struct hostent *host;

	host = gethostbyname(serv.c_str());
	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) == -1)
	{
		write(STDOUT_FILENO, "Error: Bot socket creation failed.\n", 34);
		exit(1);
	}

	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(port);
	server_addr.sin_addr = *((struct in_addr *)host->h_addr);
	memset(&(server_addr.sin_zero), '\0', 8);

	if (connect(sock, (struct sockaddr *)&server_addr, sizeof(struct sockaddr)) == -1)
	{
		write(STDOUT_FILENO, "Error: Bot socket connection failed.\n", 36);
		exit(1);
	}

	if (fcntl(sock, F_SETFL, O_NONBLOCK) == -1)
	{
		write(STDOUT_FILENO, "Error: Bot socket fcntl failed.\n", 32);
		exit(1);
	}

	if (sock != -1)
	{
		sendRegMsg("PASS " + botPass + "\r\n");
		sendRegMsg("NICK " + nick + "\r\n");
		sendRegMsg("USER " + user + " 0 * :" + realname + "\r\n");
	}
}

//sunucudan gelen mesajları dinleyen fonksiyondur, gelen mesajları processMessage fonksiyonuna gönderir
void Bot::listen()
{
	const int BUFFER_SIZE = 1024;//sembolik sabit
	char buffer[BUFFER_SIZE];
	memset(buffer, 0, BUFFER_SIZE);//buffer'ı sıfırla doldurarak başlatır
	string accumulated;//alınan mesajları biriktirir

	int bytes_read = recv(sock, buffer, BUFFER_SIZE - 1, 0);//sock: botun soketi
	if (bytes_read < 0)
	{
		string errrecv = "Error in recv: " + string(strerror(errno)) + "\n";
		write(STDOUT_FILENO, errrecv.c_str(), errrecv.length());
		return;
	}
	else if (bytes_read == 0)
	{
		write(STDOUT_FILENO, "Connection closed by peer.\n", 27);
		return;
	}
	buffer[bytes_read] = '\0';
	accumulated += buffer;

	size_t pos;
	while ((pos = accumulated.find("\r\n")) != string::npos)
	{
		string msg = accumulated.substr(0, pos);
		accumulated.erase(0, pos + 2);
		processMessage(msg);
	}
}

//girilen mesajları işleyen fonksiyon
void Bot::processMessage(const string &msg)
{
	/*
	fonksiyon gelen mesajın "PRIVMSG" komutu ile başladığını kontrol eder. 
	Eğer başlıyorsa, gönderenin nicknamesini ayırır ve mesaj içeriğine göre farklı işlemler yapar:
	*/
	if (msg.find("PRIVMSG") != string::npos)
	/*
	Gelen mesajın "PRIVMSG" komutu ile başladığını kontrol eder. 
	Bu komut, bir kanal veya kullanıcıya özel mesaj göndermek için kullanılır.
	*/
	{
		string senderNick = msg.substr(1, msg.find("!") - 1);
		/*
		Eğer mesaj "PRIVMSG" komutu ile başlıyorsa, gönderenin nicknamesini mesajdan ayırır. 
		İlk karakter ("!") karakterinden önceki kısmı alır.
		*/
		{
			if (msg.find("hello") != string::npos)
				sendMsg(senderNick, "Hello " + senderNick + "!");

			if (msg.find("time") != string::npos || msg.find("date") != string::npos)
			{
				time_t rawtime;
				struct tm *timeinfo;
				time(&rawtime);
				timeinfo = localtime(&rawtime);
				sendMsg(senderNick, asctime(timeinfo));
			}
			//if (msg.find("aq") != string::npos)
				//sendMsg(senderNick, "Söylediklerine dikkat et " + senderNick + "!");
		}
	}
}

//kanala mesaj gönderen fonksiyon
void Bot::sendMsg(const string &channel, const string &message)
{
	string fullMessage = "PRIVMSG " + channel + " :" + message + "\r\n";
	send(sock, fullMessage.c_str(), fullMessage.length(), 0);
}

// Sunucuya kayıt mesajı gönderen fonksiyon
void Bot::sendRegMsg(const string &message)
{
	send(sock, message.c_str(), message.length(), 0);
}
