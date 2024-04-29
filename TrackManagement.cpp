#include "TrackManagement.hpp"
#include "TrackImage.hpp"
#include "taglib/id3v2tag.h"
#include "taglib/id3v2frame.h"
#include "taglib/mpegfile.h"
#include "taglib/id3v2header.h"
#include "taglib/attachedpictureframe.h"
#include <fstream>
#include <string>

int TrackManagement::addTrack(std::string& fileLocation, Playlist& defaultPlaylist, odb::sqlite::database& database_context) {

    TagLib::MPEG::File mpegFile(fileLocation.c_str());
    TagLib::ID3v2::Tag* id3v2tag = mpegFile.ID3v2Tag();
    TagLib::ID3v2::FrameList frame;
    TagLib::ID3v2::AttachedPictureFrame* coverArt = NULL;

    if (id3v2tag == NULL) {
        return 1;
    }

    // Query to see if the album already exists
    odb::result<Albums> albums = database_context.query<Albums>(odb::query<Albums>::title == id3v2tag->album().toCString());
    Albums album(id3v2tag->album().toCString(), std::to_string(id3v2tag->year()));
    if (albums.begin() == albums.end()) {
        database_context.persist(album);
    }
    else {
        album = *albums.begin();
    }

    // Query to see if the artist already exists
    odb::result<Artists> artists = database_context.query<Artists>(odb::query<Artists>::name == id3v2tag->artist().toCString());
    Artists artist(id3v2tag->artist().toCString());
    if (artists.begin() == artists.end()) {
        database_context.persist(artist);
    }
    else {
        artist = *artists.begin();
    }

    // Query to see if the genre already exists
    odb::result<Genres> genres = database_context.query<Genres>(odb::query<Genres>::title == id3v2tag->genre().toCString());
    Genres genre(id3v2tag->genre().toCString());
    if (genres.begin() == genres.end()) {
        database_context.persist(genre);
    }
    else {
        genre = *genres.begin();
    }

    // Get the duration
    int duration = mpegFile.audioProperties()->lengthInMilliseconds() / 1000;
    TrackImage image(0x0, 0); // Initialize the image object

    // Extract the album art
        // Picture frame                              // 0x03 is the ID for the front cover image (APIC = "Attached PICture Frame")
        frame = id3v2tag->frameListMap()["APIC"]; // "APIC" is the frame ID for attached picture frames in ID3v2 tags
                                                      // There can be multiple "APIC" frames in a tag (e.g. one for the front cover, one for the back cover, etc.)
        if (!frame.isEmpty()) {
            for (TagLib::ID3v2::FrameList::ConstIterator it = frame.begin(); it != frame.end(); ++it) {
				coverArt = static_cast<TagLib::ID3v2::AttachedPictureFrame *>(*it);
                if (coverArt->type() == TagLib::ID3v2::AttachedPictureFrame::FrontCover) {
					// Begin writing the image to a file
                    //std::ofstream file("F:\\Music\\albumArt.jpg", std::ios::binary);
                    void* source_image = malloc(coverArt->picture().size());
                    if (source_image) {
                        memcpy(source_image, coverArt->picture().data(), coverArt->picture().size());
                        image = TrackImage((char*)source_image, coverArt->picture().size()); // Set the image object to the album art
                        /*file.write(image.Data(), image.Size());
                        file.flush();
                        file.close();*/
                    }
                    else {
						qDebug() << "Failed to allocate memory for the image";
                    }
					break;
			}
        }
	}


    // Query to see if the genre already exists
    odb::result<Track> tracks = database_context.query<Track>(odb::query<Track>::file_location == fileLocation);
    // Create a new track object
    Track track(id3v2tag->title().toCString(), &artist, &album, &genre, "", std::to_string(id3v2tag->year()), duration, fileLocation, image);
    if (tracks.begin() == tracks.end()) {
            database_context.persist(track);

        try {
            // Add the track to the default playlist
            Track_Playlist playlist_map(&track, &defaultPlaylist);

            database_context.persist(playlist_map);
        }
        catch (odb::exception& e) {
            qDebug() << e.what();
        }

    }

    //// Cleanup
    //f.tag()->~Tag();
    //f.~FileRef();
    return 0;
}