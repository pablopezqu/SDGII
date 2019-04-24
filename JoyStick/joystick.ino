
int ejex = A0;
int ejey = A1;

#define arriba 22
#define abajo 23
#define izquierda 24
#define derecha 25

int vx;
int vy;

void setup() {
  pinMode(arriba, OUTPUT);  
  pinMode(abajo, OUTPUT);
  pinMode(izquierda, OUTPUT);
  pinMode(derecha, OUTPUT);
}
void loop() {
 vx = analogRead(ejex);
 if(vx > 700){
  digitalWrite(derecha, HIGH);
 }
 else {
  digitalWrite(derecha, LOW);
 }
 vx = analogRead(ejex);
 if(vx < 400){
  digitalWrite(izquierda, HIGH);
 }
  else {
  digitalWrite(izquierda, LOW);
 }

 vy = analogRead(ejey);
 if(vy > 700){
  digitalWrite(abajo, HIGH);
 }
 else {
  digitalWrite(abajo, LOW);
 }
 vy = analogRead(ejey);
 if(vy < 400){
  digitalWrite(arriba, HIGH);
 }
  else {
  digitalWrite(arriba, LOW);
 }
}
