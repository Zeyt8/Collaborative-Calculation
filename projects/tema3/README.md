# Calcule colaborative in sisteme distribuite

Am creat 3 tipuri de proces: Worker, Coordinator si Leader. In main se creeaza o instanta a clasei potrivite in functie de rank si se apeleaza start pe acea instanta.

Inelul parcurs spre dreapta are aceasta ordine: 0 -> 1 -> 2 -> 3 -> 0

## Stabilirea Topologiei

Fiecare process are un array de vectori in care stocheaza topologia.

Coordonatorii isi stabilesc vecinii si isi citeste din fisierul lui workerii pe care ii are si ii adauga la o lista de workeri.

Ideea de la care am pornit este ca fiecare coordonator sa trimita la dreapta lista sa de workeri care sa faca un tur complet de inel.

Coordonatorul isi trimite lista de workeri vecinului din dreapta. Dupa aceea astepta sa cate primeasca o lista de workeri de la fiecare coordonator diferit de el insusi cu tagul egal cu rankul coordonatorului al carui workeri sunt.

Dupa ce primeste lista, isi updateaza topologia si trimite lista de workeri mai departe vecinului din dreapta.

Acest lucru se termina atunci cand un coordonator isi primeste propria lista de workeri.

Dupa ce un coordonator are topologia completa o afiseaza in consola si incepe sa comunice cu workerii sai. Le trimite 3 mesaje fiecaruia:

- rankul coordonatorului

- rankul workerului cu care comunica

- topologia

Workerii afiseaza la consola topologia.

## Realizarea calculelor

Leaderul creeaza vectorul V si calculeaza `chunk` care reprezinta numarul de numere care trebuie calculate de fiecare worker.

Leaderul isi pastreaza `chunk` * numarul de workeri din clusterul lui numere. Restul vectorului il trimite coordonatorului din stanga.

Coordonatorul primeste vectorul si face acelasi lucru. `chunk` este recalculat, dar luand in considerare doar workerii ramasi.

Ultimul coordonator pastreaza tot ce primeste.

Fiecare coordonator trimite workerilor sai cate o bucata din vectorul pe care si l-a pastrat.

Un worker calculeaza vectorul inmultit element cu element cu 5 si il trimite inapoi la coordonator.

Coordonatorul primeste vectorul pe rand de la workeri in aceasi ordine in care l-a trimis.

Dupa ce a primit vectorul de la toti workerii, il trimite la coordonatorul din dreapta, care il primeste si il da mai departe pana la Leader.

Leaderul primeste vectorul in aceasi ordine in care l-a trimis si il afiseaza in consola.

## Tratarea defectelor pe canalul de comuncatie

### Topologie

In cazul existentei unei erori intre 0 si 1 nu mai functioneaza ca fiecare coordonator sa trimita la dreapta.

Ideea de la care am pornit este ca fiecare coordonator sa trimita la dreapta si la stanga un mesaj cu lista sa de workeri care sa ajunga pana in capete.

Coordonatorul trimite lista de workeri si in stanga si in dreapta daca exista vecinul respectiv.

Un coordonator acum asculta si de la vecinul din stanga si de la vecinul din dreapta sa primeasca lista de workeri de la toti coordonatorii care se afla in stanga, respectiv dreapta in noua topologie.

Cand primeste un mesaj, updateaza topologia si trimite mai departe pastrand directia mesajului.

Mesajele se vor opri cand vor ajunge la unul din capete, deoarece nu mai au cum sa trimita mai departe pastrand directia.

Transmiterea topologiei catre workeri functioneaza la fel.

### Calcule

Datorita modului in care se impart calculele, nu necesita nicio modificare. Datele se trimit 0 -> 3 -> 2 -> 1 si dupa inapoi pe traseul invers, asadar eroare de pe legatura 0-1 nu afecteaza calculul.
