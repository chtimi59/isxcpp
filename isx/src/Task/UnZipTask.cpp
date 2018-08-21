#include "common.h"
#include "UnZipTask.h"
#include <zip.h>
// std
#include <set>
#include <vector>
#include <cctype>
#include <algorithm>

struct ZipItem {
	ZipItem(int idx, std::string path, zip_uint64_t size) : idx(idx), size((size_t)size)
	{
		if ('/' == path.back()) {
			isFolder = true;
			basename = path;
			basename.pop_back();
			filename = "";
		}
		else
		{
			isFolder = false;
			std::size_t found = path.find_last_of('/');
			if (found != std::string::npos) {
				basename = path.substr(0, found);
				filename = path.substr(found + 1);
			}
			else
			{
				basename = "";
				filename = path;
			}
		}
		std::replace(basename.begin(), basename.end(), '/', '\\');
		std::replace(filename.begin(), filename.end(), '/', '\\');
	}
	int idx;
	std::string filename = "";
	std::string basename = "";
	bool isFolder;
	size_t size;
};

const std::string UnZipTask::main()
{
	setTitle(res::getString(IDS_TASKUNZIP));
	setProgress(0);
	sendUpdate();

	struct zip *za;
	
	/* open archive */
	int err;
	if ((za = zip_open(path.c_str(), 0, &err)) == NULL) {
		return res::getString(IDS_TASKUNZIPERROR, path.c_str());
	}

	/* prepare desttination */
	if (clear) io::DirectoryDelete(dst);
	
	/* get item list */
	std::vector<ZipItem> list;
	struct zip_stat sb;
	for (int i = 0; i < zip_get_num_entries(za, 0); i++) {
		if (zip_stat_index(za, i, 0, &sb) == 0) {
			list.emplace_back(i, sb.name, sb.size);
		}
	}

	/* extracting */
	int count = 1;
	std::string out = SUCCESS;
	char buff[1024 * 20]; // 20K
	std::set<std::string> existingFolders;
	for (auto it = list.begin(); it != list.end(); ++it)
	{
		/* UI update */
		setProgress((100 * count++) / list.size());
		setSubTitle(it->filename);
		sendUpdate();

		/* ensure that folder exist */
		auto folder = dst + "\\" + it->basename;
		if (existingFolders.count(folder) == 0) {
			io::DirectoryCreate(folder);
			existingFolders.insert(folder);
		}

		/* no file to write */
		if (it->isFolder) continue;

		/* open file in archive */
		struct zip_file *zf;
		zf = zip_fopen_index(za, it->idx, 0);
		if (!zf) {
			out = res::getString(IDS_TASKUNZIPERROR, path.c_str());
			break;
		}

		/* open file for write */
		FILE * f = NULL;
		auto fullPath = folder + "\\" + it->filename;
		fopen_s(&f, fullPath.c_str(), "wb");
		if (!f) {
			zip_fclose(zf);
			out = res::getString(IDS_TASKUNZIPERROR, path.c_str());
			break;
		}

		/* write */
		size_t sum = 0;
		bool isWriteError = false;
		while (sum != it->size) {
			size_t len = (size_t)zip_fread(zf, buff, sizeof(buff));
			if (len < 0) {
				isWriteError = true;
				break;
			}
			fwrite(buff, len, 1, f);
			sum += len;
		}
		
		/* close files */
		fclose(f);
		zip_fclose(zf);

		if (isWriteError) {
			out = res::getString(IDS_TASKUNZIPERROR, path.c_str());
			break;
		}
	}

	zip_close(za);
    return out;
}
