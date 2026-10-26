
const int zieloneLewy = 3;
const int czerwoneLewy = 2;

const int zielonePrawy = 4;
const int czerwonePrawy = 7;

const int pirLewy = 13;
const int pirPrawy = 5;

const unsigned long czasZielonego = 5000;

volatile int wykrytoLewy = 0;
volatile int wykrytoPrawy = 0;

int kolejka[2];
int poczatek = 0;
int koniec = 0;
int liczbaWKolejce = 0;

int aktywnyKierunek = -1;
unsigned long czasStartu = 0;

void ustawCzerwone() {
  digitalWrite(zieloneLewy, LOW);
  digitalWrite(czerwoneLewy, HIGH);

  digitalWrite(zielonePrawy, LOW);
  digitalWrite(czerwonePrawy, HIGH);
}

ISR(PCINT0_vect) {
  if (PINB & (1 << PB5)) {
    wykrytoLewy = 1;
  }
}

ISR(PCINT2_vect) {
  if (PIND & (1 << PD5)) {
    wykrytoPrawy = 1;
  }
}

bool jestWKolejceLubAktywny(int kierunek) {
  if (aktywnyKierunek == kierunek) {
    return true;
  }

  for (int i = 0; i < liczbaWKolejce; i++) {
    int indeks = (poczatek + i) % 2;

    if (kolejka[indeks] == kierunek) {
      return true;
    }
  }

  return false;
}

void dodajDoKolejki(int kierunek) {
  if (jestWKolejceLubAktywny(kierunek)) {
    return;
  }

  if (liczbaWKolejce < 2) {
    kolejka[koniec] = kierunek;
    koniec = (koniec + 1) % 2;
    liczbaWKolejce++;

    Serial.print("Dodano kierunek: ");
    Serial.println(kierunek == 0 ? "lewy" : "prawy");
  }
}

void rozpocznijKierunek(int kierunek) {
  ustawCzerwone();

  aktywnyKierunek = kierunek;
  czasStartu = millis();

  if (kierunek == 0) {
    digitalWrite(czerwoneLewy, LOW);
    digitalWrite(zieloneLewy, HIGH);
    Serial.println("Zielone dla lewego kierunku");
  } else {
    digitalWrite(czerwonePrawy, LOW);
    digitalWrite(zielonePrawy, HIGH);
    Serial.println("Zielone dla prawego kierunku");
  }
}

void setup() {
  Serial.begin(9600);

  pinMode(zieloneLewy, OUTPUT);
  pinMode(czerwoneLewy, OUTPUT);
  pinMode(zielonePrawy, OUTPUT);
  pinMode(czerwonePrawy, OUTPUT);

  pinMode(pirLewy, INPUT);
  pinMode(pirPrawy, INPUT);

  ustawCzerwone();

  noInterrupts();

  PCMSK0 |= (1 << PCINT5);
  PCMSK2 |= (1 << PCINT21);

  PCICR |= (1 << PCIE0) | (1 << PCIE2);
  PCIFR |= (1 << PCIF0) | (1 << PCIF2);

  interrupts();

  Serial.println("Uruchomiono sygnalizacje");
}

void loop() {
  int ruchLewy;
  int ruchPrawy;

  noInterrupts();

  ruchLewy = wykrytoLewy;
  ruchPrawy = wykrytoPrawy;

  wykrytoLewy = 0;
  wykrytoPrawy = 0;

  interrupts();

  if (ruchLewy) {
    dodajDoKolejki(0);
  }

  if (ruchPrawy) {
    dodajDoKolejki(1);
  }

  if (aktywnyKierunek != -1 &&
      millis() - czasStartu >= czasZielonego) {

    ustawCzerwone();

    Serial.println("Koniec zielonego swiatla");

    aktywnyKierunek = -1;
  }

  if (aktywnyKierunek == -1 && liczbaWKolejce > 0) {
    int nastepny = kolejka[poczatek];

    poczatek = (poczatek + 1) % 2;
    liczbaWKolejce--;

    rozpocznijKierunek(nastepny);
  }
}

