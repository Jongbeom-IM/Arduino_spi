#include <SPI.h>

volatile byte receivedData = 0;   // SPI로 받은 데이터 저장
volatile bool dataReady = false;  // 수신 플래그

void setup() {
  // 시리얼 초기화
  Serial.begin(115200);
  while (!Serial);   // USB 시리얼 안정화 대기 (필요시)

  // SPI 슬레이브 모드 초기화
  pinMode(MISO, OUTPUT);   // 슬레이브는 반드시 MISO를 출력으로 설정
  SPCR |= _BV(SPE);        // SPI Enable (슬레이브 모드)

  // 수신 인터럽트 허용
  SPI.attachInterrupt();

  Serial.println("SPI Slave Ready");
}

// SPI 수신 인터럽트 서비스 루틴
ISR(SPI_STC_vect) {
  receivedData = SPDR;   // 수신 데이터 레지스터
  dataReady = true;      // 메인루프에서 처리하도록 플래그 세팅
}

void loop() {
  if (dataReady) {
    Serial.print("Received: 0x");
    Serial.println(receivedData, HEX);  // 16진수로 출력
    dataReady = false;
  }
}
