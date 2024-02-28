import subprocess

--AF_INET (Address Family - Internet) bir adres ailesidir ve genellikle IPv4 adreslerini temsil eder. 
Bu adres ailesi, internet üzerindeki cihazların birbirleriyle iletişim kurmalarını sağlayan TCP/IP protokol takımı ile uyumludur.

--Socket programlamada, AF_INET adres ailesi, IPv4 adreslerini ve bağlantı noktalarını kullanarak soketler oluşturmak ve yönetmek için kullanılır. 
Soket programlamada, soket (socket) nedir sorusuna verilebilecek basit bir yanıt, iki cihaz arasında veri iletişimi kurmak için kullanılan bir arayüz veya uç nokta olarak tanımlanabilir.

--SOCK_STREAM, socket programlamada kullanılan bir soket türüdür. Bu soket türü, güvenilir, bağlantı tabanlı (connection-oriented) bir veri akışı sağlar. SOCK_STREAM soket türü, tipik olarak TCP (Transmission Control Protocol) protokolü tarafından desteklenir.

--socket fonksiyonu şu şekilde tanımlanır:
    int socket(int domain, int type, int protocol);
Burada:
-domain: Adres ailesini (Address Family) belirtir.
-type: Soket türünü belirtir.
-protocol: Kullanılacak protokolü belirtir. Bu genellikle 0 olarak atanır ve socket fonksiyonu tarafından otomatik olarak uygun protokol seçilir.
Bu durumda, socket fonksiyonu tarafından otomatik olarak uygun olan protokol seçilir. Örneğin, SOCK_STREAM türü için genellikle TCP (Transmission Control Protocol) kullanılır.


int bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen);
// bind fonksiyonu, bir soketi belirli bir adres ve port numarasına bağlamak için kullanılır.
// sockfd: Bağlanacak soketin dosya tanımlayıcısını belirtir.
// addr: Bağlanacak adresi ve port numarasını içeren bir sockaddr yapısıdır.
// addrlen: addr yapısının boyutunu belirtir.


Komut	    Açıklama
Cap	        IRC protokolüne göre, bu komut sunucu ve istemci arasında yetenek (capability) müzakeresi yapmak için kullanılır. Sunucu, istemciden desteklediği özellikleri bildirmesini ister ve istemci de bu özellikleri sunucuya bildirir.

Join	    Client'ın verilen kanala katılmak istediğini, her kanalın kendisi için verilen anahtarı kullandığını belirtir.
Kick	    Kullanıcının bir kanaldan zorla çıkarılmasını talep etmek için kullanılabilir.
List	    Her kanal hakkında bazı bilgilerle birlikte kanalların bir listesini almak için kullanılır.
Nick	    Client'a bir takma ad vermek veya bir öncekini değiştirmek için kullanılır.
Notice	    Hem kullanıcılar arasında hem de kanallara bildirim göndermek için kullanılır. "NOTICE" ve "PRIVMSG" arasındaki fark, "NOTICE" mesajına yanıt olarak otomatik yanıtların asla gönderilmemesidir.

Pass	    Bir "bağlantı şifresi" ayarlamak için kullanılır. Ayarlanırsa, bağlantıyı kaydetmeye yönelik herhangi bir girişimde bulunulmadan önce parola ayarlanmalıdır.

Ping	    Uygulama katmanında, bağlantının diğer tarafının hala bağlı olup olmadığını kontrol etmek veya bağlantı gecikmesini kontrol etmek için clientlar veya serverlar tarafından gönderilir.

Pong	    Serverlar arasında bağlantı kontrolü yapar. "PING" mesajı gönderildikten sonra kullanıcı client'ı server'a cevap olarak "PONG" mesajı gönderir. Bu, server ile client arasındaki bağlantının sağlıklı olduğunu gösterir ve iletişimin devam edebileceğini işaret eder.

Privmsg	    Kullanıcılar arasında özel mesaj göndermek ve ayrıca kanallara mesaj göndermek için kullanılır.
Quit	    Bir clientın serverla olan bağlantısını sonlandırmak için kullanılır. Server bunu bir "ERROR" mesajıyla yanıtlayarak ve client bağlantısını kapatarak onaylar.

User	    Yeni bir kullanıcının kullanıcı adını ve gerçek adını belirtmek için bir bağlantının başlangıcında kullanılır.
Mode	    Kanalların veya kullanıcıların modlarını değiştirmek için kullanılır. Modlar, kullanıcının yetkilerini, kanalın özelliklerini ve diğer birçok özelliği belirlemek için kullanılır.
