/// <summary>
/// This class is used to store the cover art of a track.
/// </summary>
class TrackImage {
public:
	/// <summary>
	/// Store the cover art of a track.
	/// </summary>
	/// <param name="data"></param>
	/// <param name="size"></param>
	TrackImage(char *data, unsigned int size) : data(data), size(size) {}
	/// <summary>
	/// The data of the image.
	/// </summary>
	/// <returns></returns>
	char* Data();
	/// <summary>
	/// The size of the image.
	/// </summary>
	/// <returns></returns>
	unsigned int Size();
private:
	char* data;
	unsigned int size;
};