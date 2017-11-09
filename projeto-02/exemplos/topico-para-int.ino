/*
	Exemplo utilizando a classe String da biblioteca do Arduino para converter o número
	da vaga contida no tópico para `int`

	https://www.arduino.cc/en/Reference/String
*/
void setup() {
	Serial.begin(9600);
	while(!Serial) {}

	char* topico = "vagas/05";
	// Converter caractere '0' em String "0"
	String vaga = String(topico[6]);
	// Concatenar String "0" com o char '5'
	// a classe String implementa esta conversão
	vaga += topico[7];
	// Utilizar o método String::toInt() para interpretar os caracteres da nova String
	// como inteiro
	int numeroVaga = vaga.toInt();

	Serial.println("Vaga: ");
	Serial.println(numeroVaga);
}
