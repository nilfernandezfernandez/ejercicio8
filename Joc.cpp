#include "Joc.h"
#include <fstream>
using namespace std;

void Joc::inicialitza(const string& nomFitxer, int nJugadors)
{
	ifstream fitxer;
	fitxer.open(nomFitxer);
	if (fitxer.is_open())
	{
		int color, valor;
		fitxer >> color >> valor;
		while (!fitxer.eof())
		{
			Carta carta(color, valor);
			m_cartesBaralla.push(carta);
			fitxer >> color >> valor;
		}
		fitxer.close();
		for (int i = 0; i < nJugadors; i++)
		{
			Jugador jugador;
			for (int j = 0; j < N_CARTES_JUGADOR; j++)
			{	
				Carta carta;
				agafaCarta(carta, false);
				jugador.afegeixCarta(carta);
			}
			m_jugadors.push_back(jugador);
			m_nJugadors++;
		}
		Carta primeraCarta;
		agafaCarta(primeraCarta, false);
		m_cartesJugades.push(primeraCarta);
		m_torn = 0;
	}
}

bool Joc::agafaCarta(Carta& carta, bool guardaMoviment)
{

	//TODO SACAR CARTA DEL STACK DE M_CARTESBARALLA Y PASARLA POR REFERENCIA A CARTA
	//SI GUARDAMOVIMENT ES TRUE, SE HA DE GUARDAR SU INFORMACION (NºJUGADOR, ROBA_CARTA Y VALOR CARTA)


	bool cartaAgafada = false;

	if(!m_cartesBaralla.empty()){
		carta =	m_cartesBaralla.top();
		m_cartesBaralla.pop();
		cartaAgafada = true;

		if(guardaMoviment == true){
			Moviment m_moviment(m_torn, ROBA_CARTA, carta);
			m_moviments.push(m_moviment);
		}
	}

	return cartaAgafada;
}

void Joc::fesMoviment()
{
	Carta carta = m_cartesJugades.top();
	Carta cartaRobar;
	switch (carta.getValor())
	{
	case CANVI_TORN:
		m_sentitTorn = -m_sentitTorn;
		canviTorn();
		canviTorn();
		break;
	case SALTA_TORN:
		canviTorn();
		break;
	case ROBA_DOS:
		if (agafaCarta(cartaRobar, true))
		{
			m_jugadors[m_torn].afegeixCarta(cartaRobar);
			if (agafaCarta(cartaRobar, true))
			{
				m_jugadors[m_torn].afegeixCarta(cartaRobar);
			}
		}
		break;
	}
	tiraCarta();
	canviTorn();
}

void Joc::tiraCarta()
{
	//TODO LLAMAR AL METODO TIRAR CARTA DE M_JUGADORES
	//SI NO TIENE CARTAS, ROBAR CARTAS DE M_CARTESBARALLA (UTILIZAR METODO AGAFA CARTA)
	//VOLVER A LLAMAR AL METODO TIRAR CARTA
	//REPETIR LO ANTERIOR HASTA QUE TIRAR CARTA = TRUE O QUE M_CARTESBARALLA ESTE VACIO
	bool cartaTirada = false;
	bool agafarCarta = false;
	Carta cartaTirar;
	Carta novaCarta;
	do{
		cartaTirada = m_jugadors[m_torn].tiraCarta(m_cartesJugades.top(), cartaTirar);
		if(cartaTirada == false){
			agafarCarta = agafaCarta(novaCarta, true);
			if(agafarCarta == true){
				m_jugadors[m_torn].afegeixCarta(novaCarta);
			}
		}
	}while(agafarCarta && !cartaTirada);

	if(cartaTirada){
		 Moviment m(m_torn,TIRA_CARTA, cartaTirar);
		 m_moviments.push(m);
		 m_cartesJugades.push(cartaTirar);
	}


}

bool Joc::final()
{ 	//TODO DETERMINAR SI HA LLEGADO AL FINAL
	//CASO 1 - SI JUGADOR SE QUEDA SIN CARTAS
	//CASO 2 - SI NO ES PODEN TIRAR CARTES (M_CARTESBARALL = 0)
	bool esFinal = false;
	int i = 0;

	while(!esFinal &&  i < m_nJugadors){
		if(m_jugadors[i].getNCartes() == 0){
			esFinal = true;
		}else{
			i++;
		}
	}
	if(!esFinal){
		if(m_cartesBaralla.empty()){
			esFinal = true;
		}
	}

	return esFinal;

}

void Joc::guarda(const string& nomFitxer)
{
	ofstream guardarFitxer;
	guardarFitxer.open(nomFitxer);
	if(guardarFitxer.is_open()){
		while(!m_moviments.empty()){
			Moviment auxMoviment = m_moviments.front();
			Carta c = auxMoviment.getCarta();
			guardarFitxer << auxMoviment.getJugador()<<" "<< auxMoviment.getAccio() << " " << c.getColor()<< " " <<c.getValor() << endl;
			m_moviments.pop();
		}
		guardarFitxer.close();
	}
}

void Joc::canviTorn()
{
	m_torn += m_sentitTorn;
	if (m_torn < 0)
		m_torn = m_nJugadors - 1;
	if (m_torn >= m_nJugadors)
		m_torn = 0;
}
