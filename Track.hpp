#pragma once
#include "Artists.hpp"
#include "Albums.hpp"
#include "Genres.hpp"
#include "TrackImage.hpp"
#include <string>
#include <odb/core.hxx>
using std::string;

#pragma db object
class Track {
public:
	/// <summary>
	/// Create a Track. This constructor is used when creating a new Track.
	/// </summary>
	/// <param name="title"></param>
	/// <param name="artist_id"></param>
	/// <param name="album_id"></param>
	/// <param name="genre_id"></param>
	/// <param name="lyrics"></param>
	/// <param name="duration"></param>
	/// <param name="year"></param>
	/// <param name="file_location"></param>
	/// <param name="cover_art"></param>
	Track(string title, Artists* artist_id, Albums* album_id, Genres* genre_id, string lyrics,
	string year, double duration, string file_location, TrackImage cover_art): title_(title), artist_id_(artist_id),
	album_id_(album_id), genre_id_(genre_id), lyrics_(lyrics), duration_(duration), year_(year),
		file_location_(file_location) {
		this->cover_art_ = cover_art.Data();
	}

	/// <summary>
	/// Create a Track. This constructor is used when creating a new Track.
	/// </summary>
	/// <param name="title"></param>
	/// <param name="artist_id"></param>
	/// <param name="album_id"></param>
	/// <param name="year"></param>
	/// <param name="file_location"></param>
	Track(string title, Artists* artist_id, Albums* album_id, string year, double duration, string file_location) : title_(title), artist_id_(artist_id), album_id_(album_id),
		year_(year), duration_(duration), file_location_(file_location) {
		this->genre_id_ = NULL;
	}

	// Getters

	/// <summary>
	/// Track ID. This is the primary key of the Track table.
	/// </summary>
	/// <returns></returns>
	const int Id();
	/// <summary>
	/// Track title. This is the name of the track.
	/// </summary>
	/// <returns></returns>
	const string Title();
	/// <summary>
	/// The artist ID. This is a foreign key that references the artist table.
	/// </summary>
	/// <returns></returns>
	const Artists* ArtistId();
	/// <summary>
	/// The album ID. This is a foreign key that references the album table.
	/// </summary>
	/// <returns></returns>
	const Albums* AlbumId();
	/// <summary>
	/// The genre ID. This is a foreign key that references the genre table.
	/// </summary>
	/// <returns></returns>
	const Genres* GenreId();
	/// <summary>
	/// The lyrics of the track. This is the text of the lyrics of the track.
	/// </summary>
	/// <returns></returns>
	const string Lyrics();
	/// <summary>
	/// The year the track was released. This is the year the track was released.
	/// </summary>
	/// <returns></returns>
	const string Year();
	/// <summary>
	/// The duration of the track. This is the duration of the track in seconds.
	/// </summary>
	/// <returns></returns>
	const double Duration();
	/// <summary>
	/// The file name of the track. This is the name of the file that contains the track.
	/// </summary>
	/// <returns></returns>
	const string FileName();
	/// <summary>
	/// The image of the track. This is the image of the track.
	/// </summary>
	/// <returns></returns>
	const TrackImage Image() {
		return TrackImage(cover_art_, sizeof(cover_art_)/sizeof(char)); // Return the image object
		// We calculate the size of the image by dividing the size of the image by the size of a char
	}

	// Setters

	/// <summary>
	/// Sets the title of the track. This is the name of the track.
	/// </summary>
	/// <param name="title"></param>
	void SetTitle(string title);
	/// <summary>
	/// Sets the artist ID. This is a foreign key that references the artist table.
	/// </summary>
	/// <param name="artist_id"></param>
	void SetArtistId(Artists* artist_id);
	/// <summary>
	/// Sets the album ID. This is a foreign key that references the album table.
	/// </summary>
	/// <param name="album_id"></param>
	void SetAlbumId(Albums* album_id);
	/// <summary>
	/// Sets the genre ID. This is a foreign key that references the genre table.
	/// </summary>
	/// <param name="genre_id"></param>
	void SetGenreId(Genres* genre_id);
	/// <summary>
	/// Sets the lyrics of the track. This is the text of the lyrics of the track.
	/// </summary>
	/// <param name="lyrics"></param>
	void SetLyrics(string lyrics);
	/// <summary>
	/// Sets the year the track was released. This is the year the track was released.
	/// </summary>
	/// <param name="year"></param>
	void SetYear(const string year);
	/// <summary>
	/// Sets the duration of the track. This is the duration of the track in seconds.
	/// </summary>
	/// <param name="duration"></param>
	void SetDuration(const double duration);
	/// <summary>
	/// Sets the file name of the track. This is the name of the file that contains the track.
	/// </summary>
	/// <param name="filename"></param>
	void SetFileName(const string filename);
	/// <summary>
	/// Sets the image of the track. This is the image of the track.
	/// </summary>
	/// <param name="image"></param>
	void SetImage(TrackImage image) {
		this->cover_art_ = image.Data(); // Set the image data
	}


private:
	friend class odb::access;
	Track(){}

	#pragma db id auto
	int id_;
	string title_;
	Artists* artist_id_;
	Albums* album_id_;
	Genres* genre_id_;
	string lyrics_;
	double duration_;
	string year_;
	string file_location_;
	char* cover_art_;

};