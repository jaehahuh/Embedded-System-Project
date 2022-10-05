#define DATAOUT 16 //MOSI
#define DATAIN  14 //MISO 
#define SPICLOCK  15 //sck
#define SLAVESELECT 17 //ss  

//opcodes
#define EXTRA  0
#define WRITE 0
#define READ  1
#define WRSR  1


byte output;
byte input = 0;
byte clr;
int address=0;

char buffer [128];

void fill_buffer()
{
  for (int I=0;I<128;I++)

  {
    buffer[I]=I;
  }
}


char spi_transfer(volatile char data)
{
  SPDR = data;                    // Start the transmission

  while (!(SPSR & (1<<SPIF))) {    // Wait the end of the transmission

  };

  return SPDR;                    // return the received byte
}

void setup()
{

  Serial.begin(9600);

  pinMode(DATAOUT, OUTPUT); // MOSI is toggled output
  pinMode(DATAIN, INPUT); // MISO is toggled input
  pinMode(SPICLOCK,OUTPUT); // clock
  pinMode(SLAVESELECT,OUTPUT); // slaveselect
  digitalWrite(8, HIGH); // disables cs
  digitalWrite(SLAVESELECT,HIGH); //disable device
  

  SPCR = (1<<SPE)|(1<<MSTR); // Enables the SPE line and the Master out 
  
  clr=SPSR;
  clr=SPDR;

  delay(200);


  fill_buffer(); // fills the buffer with random numbers


  digitalWrite(SLAVESELECT,LOW);
  digitalWrite(8, LOW); // turn on the cs pin

  spi_transfer(WRITE); // enables the write

  digitalWrite(SLAVESELECT,HIGH);
  digitalWrite(8, HIGH); // turn off the cs pin

  delay(200);

  digitalWrite(SLAVESELECT,LOW);
  digitalWrite(8, LOW); // turn on the cs pin

  spi_transfer(EXTRA); //write instruction

  address=0;

  spi_transfer((char)(address>>8));   //send MSByte address first

  spi_transfer((char)(address));      //send LSByte address

  //write 128 bytes

  for (int I=0;I<128;I++)
  {
    spi_transfer(buffer[I]); //write data byte
  }

  digitalWrite(SLAVESELECT,HIGH); //release chip
  digitalWrite(8, HIGH); // turn off the cs pin


  delay(2000);



}

byte read_eeprom(int EEPROM_address)
{

  int data; // instantiate an int variable called data

  digitalWrite(SLAVESELECT,LOW); // set the slaveselect line to low
  digitalWrite(8, LOW); // turn on the cs pin


  spi_transfer(READ); //transmit read opcode 
  spi_transfer((char)(EEPROM_address>>8));  //send MSByte address first
  spi_transfer((char)(EEPROM_address));     //send LSByte address
  data = spi_transfer(0xFF); //get data byte


  digitalWrite(SLAVESELECT,HIGH); //release chip, signal end transfer
  digitalWrite(8, HIGH); // turn on the cs pin
  
  return data;
}

void loop()
{
  Serial.println(SPCR, BIN);
  output = read_eeprom(address); // address starts from 0, continuously calls the read function
  Serial.print(output,BIN); 
  Serial.print('\n');

  address++;

  if (address == 128)

    address = 0;

  delay(1000); //pause for readability
}
