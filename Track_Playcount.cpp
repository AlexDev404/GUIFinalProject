#include "Track_Playcount.hpp"

// Getters
const Windows_Account* Track_Playcount::UserId() {
    return user_id_;
}

const Track* Track_Playcount::TrackId() {
    return track_id_;
}

const int Track_Playcount::Count() {
    return count_;
}

// Setters
void Track_Playcount::SetUserId(Windows_Account* user_id) {
    user_id_ = user_id;
}

void Track_Playcount::SetTrackId(Track* track_id) {
    track_id_ = track_id;
}

void Track_Playcount::SetCount(int count) {
    count_ = count;
}