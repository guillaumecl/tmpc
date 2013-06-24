#include <exception>
#include <string>
#include <string.h>

#include <iostream>

#include "mpd++/mpd.h"

int main(int /*argc*/, char **/*argv*/)
{
	mpdpp::mpd connection;

	connection.play();

	mpdpp::song_ptr_vector v = connection.search_queue() << mpdpp::tag_contains(mpdpp::tag::comment, "zelda") << mpdpp::commit();

	for (auto song : v)
	{
		std::cout << song->id() << " " <<  *song << " --> " << song->tag(mpdpp::tag::comment) << std::endl;
		connection.play(*song);
	}

	std::cout << std::endl;

	mpdpp::song_ptr cur = connection.current_song();

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
