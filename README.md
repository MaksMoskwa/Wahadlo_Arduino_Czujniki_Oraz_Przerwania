🚦 Arduino – Sygnalizacja świetlna z czujnikami PIR
📌 Opis projektu

Projekt przedstawia system sygnalizacji świetlnej sterowany przez dwa czujniki ruchu PIR: lewy i prawy.

Układ posiada dwie strony sygnalizacji:

Lewa strona – światło czerwone i zielone.
Prawa strona – światło czerwone i zielone.

Czujniki PIR wykrywają ruch po obu stronach układu. Po wykryciu ruchu odpowiedni kierunek zostaje dodany do kolejki oczekujących. Następnie system przyznaje temu kierunkowi zielone światło na określony czas.

Program został napisany w języku C++ dla platformy Arduino.

🎯 Cel projektu

Celem projektu jest stworzenie prostej, automatycznej sygnalizacji świetlnej, która:

reaguje na ruch wykryty przez czujniki PIR,
obsługuje dwa niezależne kierunki,
wykorzystuje kolejkę zgłoszeń,
zapobiega jednoczesnemu wyświetlaniu zielonego światła po obu stronach,
wykorzystuje przerwania sprzętowe do wykrywania zmian stanu czujników,
steruje światłami bez stosowania funkcji delay().
🧰 Wykorzystane elementy
Arduino Uno lub kompatybilna płytka z mikrokontrolerem ATmega328P,
2 czujniki ruchu PIR,
2 czerwone diody LED,
2 zielone diody LED,
4 rezystory ograniczające prąd diod LED,
płytka stykowa,
przewody połączeniowe,
przewód USB do programowania Arduino.
🔌 Podłączenie elementów
Element	Pin Arduino
Zielona dioda – lewa strona	D3
Czerwona dioda – lewa strona	D2
Zielona dioda – prawa strona	D4
Czerwona dioda – prawa strona	D7
Czujnik PIR – lewy	D13
Czujnik PIR – prawy	D5
Zasilanie czujników PIR

Typowy czujnik PIR posiada trzy wyprowadzenia:

VCC – zasilanie zgodne ze specyfikacją czujnika,
GND – masa,
OUT – sygnał wyjściowy podłączony do odpowiedniego pinu Arduino.

Diody LED należy podłączyć przez rezystory ograniczające prąd. Wszystkie elementy muszą mieć wspólną masę.

⚙️ Zasada działania programu
1. Uruchomienie systemu

Po włączeniu Arduino:

inicjalizowany jest port szeregowy z prędkością 9600 bit/s,
ustawiane są piny wejściowe i wyjściowe,
obie strony sygnalizacji otrzymują czerwone światło,
konfigurowane są przerwania do wykrywania ruchu.
2. Wykrywanie ruchu

Czujniki PIR monitorują obie strony układu. Wykrycie sygnału aktywnego powoduje zapisanie informacji o zgłoszeniu.

Program korzysta ze zmiennych:

wykrytoLewy – zgłoszenie z lewego czujnika,
wykrytoPrawy – zgłoszenie z prawego czujnika.
3. Kolejka zgłoszeń

W programie zastosowano kolejkę cykliczną o pojemności dwóch zgłoszeń.

Kierunki są oznaczone następująco:

0 – kierunek lewy,
1 – kierunek prawy.

Jeżeli dany kierunek jest już aktywny lub oczekuje w kolejce, kolejne zgłoszenie tego samego kierunku nie jest dodawane.

4. Włączenie zielonego światła

Po zakończeniu poprzedniego cyklu program pobiera następny kierunek z kolejki.

Najpierw wszystkie światła zostają ustawione na czerwone. Następnie:

dla kierunku lewego zapala się zielone światło po lewej stronie,
dla kierunku prawego zapala się zielone światło po prawej stronie.

Czerwone światło po przeciwnej stronie pozostaje włączone.

5. Zakończenie cyklu

Czas świecenia zielonego światła wynosi 5000 ms, czyli 5 sekund.

Po upływie tego czasu:

zielone światło zostaje wyłączone,
oba kierunki otrzymują czerwone światło,
program przechodzi do obsługi kolejnego zgłoszenia, jeżeli znajduje się ono w kolejce.
⏱️ Najważniejsze parametry programu
Parametr	Wartość	Znaczenie
czasZielonego	5000 ms	Czas zielonego światła
kolejka[2]	2 elementy	Pojemność kolejki
Serial.begin(9600)	9600 bit/s	Prędkość komunikacji szeregowej
aktywnyKierunek	-1	Brak aktywnego kierunku
🖥️ Monitor portu szeregowego

Po otwarciu Monitora portu szeregowego w Arduino IDE i ustawieniu prędkości 9600 bit/s można obserwować komunikaty programu.

Przykładowe komunikaty:

text
Uruchomiono sygnalizacje
Dodano kierunek: lewy
Zielone dla lewego kierunku
Koniec zielonego swiatla
Dodano kierunek: prawy
Zielone dla prawego kierunku
Koniec zielonego swiatla


Komunikaty pojawiają się zależnie od rzeczywistych sygnałów czujników.

🧠 Zastosowane rozwiązania programistyczne
Przerwania sprzętowe – rejestrowanie zmian stanu wejść.
Kolejka cykliczna – przechowywanie zgłoszeń obu kierunków.
millis() – kontrola czasu bez blokowania głównej pętli.
volatile – oznaczenie zmiennych współdzielonych z procedurami przerwań.
Ochrona sekcji krytycznych – krótkotrwałe wyłączenie przerwań podczas odczytu i zerowania flag.
Funkcje pomocnicze – oddzielenie sterowania światłami, obsługi kolejki i uruchamiania kierunku.
⚠️ Ważna uwaga techniczna

Kod wykorzystuje rejestry i wektory przerwań właściwe dla mikrokontrolera ATmega328P, np. PCINT0_vect, PCINT2_vect, PCMSK0 i PCMSK2. Nie jest więc bezpośrednio przenośny na wszystkie modele Arduino.

W przedstawionym kodzie występuje również istotna niezgodność konfiguracji przerwań z pinami:

D13 odpowiada PB5 i jest obsługiwany przez PCINT0_vect.
D5 odpowiada PD5 i jest obsługiwany przez PCINT2_vect, ale jego maska to PCINT21, a nie PCINT5.

Aby oba czujniki działały prawidłowo, należy dodatkowo upewnić się, że ich sygnały wyjściowe są obsługiwane zgodnie z zastosowaną konfiguracją przerwań. W praktyce warto przetestować oba wejścia na docelowej płytce.

Uwaga: Projekt ma charakter edukacyjny. Nie należy używać go jako rzeczywistej sygnalizacji drogowej bez odpowiednich zabezpieczeń, nadzoru i walidacji działania.

🚀 Uruchomienie projektu
Otwórz Arduino IDE.
Utwórz nowy szkic i wklej kod programu.
Podłącz elementy zgodnie z tabelą połączeń.
Wybierz właściwą płytkę Arduino oraz port COM.
Wgraj program do mikrokontrolera.
Otwórz Monitor portu szeregowego i ustaw prędkość 9600 bit/s.
Sprawdź reakcję obu czujników i działanie sygnalizacji.
📄 Licencja

Projekt edukacyjny – do nauki programowania Arduino, obsługi czujników PIR, przerwań i sterowania sygnalizacją świetlną.

Możesz rozwijać program, dodawać funkcje i dostosowywać go do własnych potrzeb.
