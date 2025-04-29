# Sosyal Ağ Analizi - C ile Red-Black Tree ve DFS Tabanlı Sistem

Bu proje, C programlama dili kullanılarak geliştirilmiş bir **sosyal ağ analiz** sistemidir. Kullanıcılar ve arkadaşlık ilişkileri üzerinden çeşitli analizler yapılmasını sağlar. Veriler Red-Black Tree veri yapısında tutulur ve DFS (Derinlik Öncelikli Arama) ile analizler yapılır.

## 📌 Özellikler

- Kullanıcılar ve arkadaşlıklar bir dosyadan okunur.
- Kullanıcılar Red-Black Tree ile saklanır.
- DFS ile:
  - Belirli mesafedeki arkadaşlar bulunabilir.
  - Ortak arkadaşlar listelenebilir.
  - Topluluklar (connected components) tespit edilir.
  - Etki alanı (influence area) hesaplanır.

🧪 Fonksiyonlar
findFriendsAtDistance(id, mesafe) - Belirli mesafedeki kullanıcıları bulur.

findCommonFriends(id1, id2) - İki kullanıcı arasındaki ortak arkadaşları bulur.

detectCommunities() - Toplulukları (connected components) tespit eder.

calculateInfluence(id) - Kullanıcının etki alanını (reachability) hesaplar.

rbSearch(id) - Red-Black Tree'den kullanıcıyı arar.

👨‍💻 Geliştirici
Bu proje bir üniversite ödevi kapsamında geliştirilmiştir. Teknik gereksinimlere ve algoritma temelli sosyal ağ analizi konularına odaklanılmıştır.

