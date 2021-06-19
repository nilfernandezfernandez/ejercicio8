#include "Jugador.h"

void Jugador::afegeixCarta(const Carta& carta)
{
	//AFEGIR UNA CARTA A LA LLISTA DE CARTES DEL JUGADRO
	m_cartes.push_back(carta);
}

bool Jugador::tiraCarta(const Carta& cartaPila, Carta& cartaATirar)
{
	//SELECCIONAR CARTA DE M_CARTES
	//CARTA PILA ES LA ULTIMA CARTA JUGADA
	//SI LA CARTA ES POT TIRAR LA IGAUALAREM A CARTAATIRAR Y M_HAPOGUTTIRAR = TRUE
	//SI NO M_HAPOGUTTIRAR = FALSE
	//SI CARTES POSIBLES > 1 BUSCAR LA MEJOR

	list<Carta>::iterator aux = m_cartes.begin();
	list<Carta> auxColor;
	Carta carta;
	bool valorIgual = false;
	bool color = false;
	bool robaDos = false;
	bool saltaTorn = false;
	for(; aux != m_cartes.end();aux++){
		if(cartaPila.getColor() == aux->getColor() && cartaPila.getValor() != SALTA_TORN){
			color = true;
			auxColor.push_back(*aux);
			carta = *aux;
			m_haPogutTirar = true;
		}else if(cartaPila.getValor() == aux->getValor() && cartaPila.getValor() != SALTA_TORN && !color){
			m_haPogutTirar = true;
			carta = *aux;
			valorIgual = true;
		}
	}

	if(auxColor.size() > 1){
		list<Carta>::iterator it_auxColor = auxColor.begin();
		for(;it_auxColor != auxColor.end();it_auxColor++){
			if(it_auxColor->getValor() == ROBA_DOS){
				robaDos = true;
				cartaATirar = *it_auxColor;
				//m_cartes.remove(*it_auxColor);
			}else if(it_auxColor->getValor() == CANVI_TORN && !robaDos){
				saltaTorn = true;
				cartaATirar = *it_auxColor;
				//m_cartes.remove(*it_auxColor);

			}else if(it_auxColor->getValor() == SALTA_TORN && !robaDos && !saltaTorn){
				saltaTorn = true;
				cartaATirar = *it_auxColor;
				//m_cartes.remove(*it_auxColor);

			}
		}
	}else if(auxColor.size() == 1){
		//m_cartes.remove(carta);
		cartaATirar = carta;
	}else if(valorIgual){
		//m_cartes.remove(carta);
		cartaATirar = carta;
	}

	return m_haPogutTirar;
}













