#include <exception>
#include <string>
#include <string.h>

#include <iostream>

#include "mpd.h"

int main(int /*argc*/, char **/*argv*/)
{
	tmpc::mpd connection;

	connection.play();

	tmpc::song_ptr_vector v = connection.search_queue() << tmpc::tag_contains(tmpc::tag::comment, "zelda") << tmpc::commit();

	for (auto song : v)
	{
		std::cout << song->id() << " " <<  *song << " --> " << song->tag(tmpc::tag::comment) << std::endl;
		connection.play(*song);
	}

	std::cout << std::endl;

	tmpc::song_ptr cur = connection.current_song();

	std::cout << "Current song : " << *cur << std::endl;

	connection.play(*connection.add("jeux/ogc1-01.mp3"));

	return 0;
}


	/**
	 * OK visualiser la chanson en cours avec ses tags, etc (mpc info)
	 * OK rechercher une chanson dans la queue
	 * OK sélectionner une chanson de la playlist pour la lire
	 * OK lire une chanson particulière de la queue
	 * OK ajouter une chanson de la base de données dans la queue
	 */
