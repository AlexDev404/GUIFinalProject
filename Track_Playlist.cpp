#include "Track_Playlist.hpp"

// Getters
const Track* Track_Playlist::TrackId() {
	return track_id_;
}

const Playlist* Track_Playlist::PlaylistId() {
	return playlist_id_;
}

// Setters
void Track_Playlist::SetTrackId(Track* track_id) {
	track_id_ = track_id;
}

void Track_Playlist::SetPlaylistId(Playlist* playlist_id) {
	playlist_id_ = playlist_id;
}
