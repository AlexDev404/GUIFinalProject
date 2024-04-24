#include "Track.hpp"

// Getters
const int Track::Id() {
	return id_;
}

const string Track::Title() {
	return title_;
}

const Artists* Track::ArtistId() {
	return artist_id_;
}

const Albums* Track::AlbumId() {
	return album_id_;
}

const Genres* Track::GenreId() {
	return genre_id_;
}

const string Track::Lyrics() {
	return lyrics_;
}

const string Track::Year() {
	return year_;
}

const double Track::Duration() {
	return duration_;
}

const string Track::FileName() {
	return file_location_;
}

// Setters
void Track::SetTitle(string title) {
	this->title_ = title;
}

void Track::SetArtistId(Artists* artist_id) {
	this->artist_id_ = artist_id;
}

void Track::SetAlbumId(Albums* album_id) {
	this->album_id_ = album_id;
}

void Track::SetGenreId(Genres* genre_id) {
	this->genre_id_ = genre_id;
}

void Track::SetLyrics(string lyrics) {
	this->lyrics_ = lyrics;
}

void Track::SetYear(string year) {
	this->year_ = year;
}

void Track::SetDuration(double duration) {
	this->duration_ = duration;
}

void Track::SetFileName(string file_location) {
	this->file_location_ = file_location;
}
