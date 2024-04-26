#pragma once
#include "WindowsAccount.hpp"
#include "Track.hpp"
#include <string>
#include <odb/core.hxx>
using std::string;

#pragma db object
class Track_Playcount {
public:
    Track_Playcount(Windows_Account* user, Track* track, int count) : user_id_(user),
        track_id_(track), count_(count) {}

    // Getters

    /// <summary>
    /// The user ID associated with this track playcount.
    /// </summary>
    /// <returns></returns>
    const Windows_Account* UserId();
    ///<summary>
    /// The unique identifier of the track.
    /// </summary>
    /// <returns></returns>
    const Track* TrackId();
    /// <summary>
    /// Gets the playcount for the track.
    /// </summary>
    /// <returns></returns>
    const int Count();

    // Setters

    /// <summary>
    /// Sets user ID associated with this track playcount.
    /// </summary>
    /// <param name="user_id"></param>
    void SetUserId(Windows_Account* user_id);
    /// <summary>
    /// Sets the track ID associated with this track playcount.
    /// </summary>
    /// <param name="track_id"></param>
    void SetTrackId(Track* track_id);
    /// <summary>
    /// Sets the playcount for this track.
    /// </summary>
    /// <param name="count"></param>
    void SetCount(int count);

private:
    Track_Playcount() {}
    friend class odb::access;
#pragma db id auto
    int id_;
    Windows_Account* user_id_;
    Track* track_id_;
    int count_;
};