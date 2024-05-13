#pragma once
#include "Track.hpp"
#include "Playlist.hpp"
#include <string>
#include <odb/core.hxx>
//#include <qdebug.h>
using std::string;

#pragma db object
class Track_Playlist {
public:
	Track_Playlist(Track* track, Playlist* playlist) : track_id_(track),
		playlist_id_(playlist) {
		// Update the duration of the playlist
		playlist->SetDuration(playlist->Duration() + track->Duration());
		//qDebug() << "Track_Playlist mapping created\n";
		//qDebug() << "Playlist duration: " << playlist->Duration();
	}

	// Getters

	/// <summary>
	/// The unique identifier of the track_playlist.
	/// </summary>
	/// <returns></returns>
	const int Id();
	///<summary>
	/// The unique identifier of the track.
	/// </summary>
	/// <returns></returns>
	const Track* TrackId();
	/// <summary>
	/// The unique identifier of the playlist.
	/// </summary>
	/// <returns></returns>
	const Playlist* PlaylistId();

	// Setters

	/// <summary>
	/// Sets the track identifier.
	/// </summary>
	/// <param name="track_id"></param>
	void SetTrackId(Track* track_id);
	/// <summary>
	/// Sets the playlist identifier.
	/// </summary>
	/// <param name="playlist_id"></param>
	void SetPlaylistId(Playlist* playlist_id);

private:
	Track_Playlist() {}
	friend class odb::access;
#pragma db id auto
	int id_;
	Track* track_id_;
	Playlist* playlist_id_;
};
